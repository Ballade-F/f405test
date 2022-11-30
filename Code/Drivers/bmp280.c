/*
 * bmp280.c
 *
 *  Created on: 2022年11月29日
 *      Author: Ballade_F
 */



#include "bmp280.h"




/*配置bmp280气压和温度过采样 工作模式*/
#define BMP280_PRESSURE_OSR         (BMP280_OVERSAMP_16X)
#define BMP280_TEMPERATURE_OSR      (BMP280_OVERSAMP_4X)
#define BMP280_MODE                 (BMP280_PRESSURE_OSR << 2 | BMP280_TEMPERATURE_OSR << 5 | BMP280_NORMAL_MODE) //

/*配置bmp280气压IIR滤波器*/
#define BMP280_FILTER               (4 << 2)	// BMP280_FILTER_COEFF_16

#define BMP280_DATA_FRAME_SIZE      (6)




bmp280Calib  bmp280Cal;
BMP280_Handle_t bmp280;

float bmp_t;
float bmp_p;

static int bmp280RawPressure = 0;
static int bmp280RawTemperature = 0;

static void bmp280GetPressure(void);

void bmp280_handle_set(void)
{
	bmp280.iic_ch = &hi2c1;
}

void iicRead(uint16_t devAddress, uint16_t memAddress,uint16_t len, uint8_t *pData)
{
	HAL_I2C_Mem_Read(bmp280.iic_ch,devAddress,memAddress,I2C_MEMADD_SIZE_8BIT ,pData,len*sizeof(uint8_t),1000);
}

void iicWrite(uint16_t devAddress, uint16_t memAddress, uint8_t data)
{
	HAL_I2C_Mem_Write(bmp280.iic_ch,devAddress,memAddress,I2C_MEMADD_SIZE_8BIT ,&data,sizeof(uint8_t),1000);
}

void BMP280_Init(void)
{
	bmp280_handle_set();

	uint8_t id = 0;
	while(BMP280_DEFAULT_CHIP_ID != id)
	{
		iicRead( BMP280_I2C_ADDR, BMP280_CHIP_ID,1, &id);//读取ID
		delay_ms(10);
	}

	iicRead(BMP280_I2C_ADDR, BMP280_TEMPERATURE_CALIB_DIG_T1_LSB_REG, 24, (uint8_t*)&bmp280Cal);//读取校准数据
	iicWrite(BMP280_I2C_ADDR, BMP280_CTRL_MEAS_REG, BMP280_MODE);//设置过采样率和工作模式
	iicWrite(BMP280_I2C_ADDR, BMP280_CONFIG_REG, BMP280_FILTER);//配置IIR滤波

}

static void bmp280GetPressure(void)
{
    uint8_t data[BMP280_DATA_FRAME_SIZE];
//	u8 status;
//	i2cdevReadByte(I2C1_DEV, BMP280_I2C_ADDR, BMP280_STAT_REG, &status);
//	if(status & 0x08)
	{
    	iicRead(BMP280_I2C_ADDR, BMP280_PRESSURE_MSB_REG, BMP280_DATA_FRAME_SIZE, data);
		bmp280RawPressure = (int)((((uint32_t)(data[0])) << 12) | (((uint32_t)(data[1])) << 4) | ((uint32_t)data[2] >> 4));
		bmp280RawTemperature = (int)((((uint32_t)(data[3])) << 12) | (((uint32_t)(data[4])) << 4) | ((uint32_t)data[5] >> 4));
	}
}

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of "5123" equals 51.23 DegC
// t_fine carries fine temperature as global value
uint32_t bmp280CompensateT(int adcT)
{
	int var1, var2, T;

    var1 = ((((adcT >> 3) - ((int)bmp280Cal.dig_T1 << 1))) * ((int)bmp280Cal.dig_T2)) >> 11;
    var2  = (((((adcT >> 4) - ((int)bmp280Cal.dig_T1)) * ((adcT >> 4) - ((int)bmp280Cal.dig_T1))) >> 12) * ((int)bmp280Cal.dig_T3)) >> 14;
    bmp280Cal.t_fine = var1 + var2;
    T = (bmp280Cal.t_fine * 5 + 128) >> 8;

    return T;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// Output value of "24674867" represents 24674867/256 = 96386.2 Pa = 963.862 hPa
uint32_t bmp280CompensateP(int adcP)
{
    int64_t var1, var2, p;
    var1 = ((int64_t)bmp280Cal.t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)bmp280Cal.dig_P6;
    var2 = var2 + ((var1*(int64_t)bmp280Cal.dig_P5) << 17);
    var2 = var2 + (((int64_t)bmp280Cal.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)bmp280Cal.dig_P3) >> 8) + ((var1 * (int64_t)bmp280Cal.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)bmp280Cal.dig_P1) >> 33;
    if (var1 == 0)
        return 0;
    p = 1048576 - adcP;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)bmp280Cal.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)bmp280Cal.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)bmp280Cal.dig_P7) << 4);
    return (uint32_t)p;
}

void BMP280GetData(float* pressure, float* temperature)
{
	bmp280GetPressure();

	*temperature = bmp280CompensateT(bmp280RawTemperature)/100.0f;	/*单位度*/
	*pressure = bmp280CompensateP(bmp280RawPressure)/256.0f;		/*单位Pa*/

//	*asl=bmp280PressureToAltitude(pressure);	/*转换成海拔*/
}

#define CONST_PF 0.1902630958	//(1/5.25588f) Pressure factor
#define FIX_TEMP 25				// Fixed Temperature. ASL is a function of pressure and temperature, but as the temperature changes so much (blow a little towards the flie and watch it drop 5 degrees) it corrupts the ASL estimates.
								// TLDR: Adjusting for temp changes does more harm than good.
/**
 * Converts pressure to altitude above sea level (ASL) in meters
 */
float bmp280PressureToAltitude(float* pressure/*, float* groundPressure, float* groundTemp*/)
{
    if (*pressure > 0)
    {
        return ((pow((1015.7f / *pressure), CONST_PF) - 1.0f) * (FIX_TEMP + 273.15f)) / 0.0065f;
    }
    else
    {
        return 0;
    }
}
