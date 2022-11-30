/*
 * bmp280.h
 *
 *  Created on: 2022年11月29日
 *      Author: Ballade_F
 */

#ifndef DRIVERS_BMP280_H_
#define DRIVERS_BMP280_H_

#include "common.h"
#include "math.h"

#define BMP280_I2C_ADDR					(0x76<<1)
#define BMP280_DEFAULT_CHIP_ID			(0x58)

#define BMP280_CHIP_ID					(0xD0)  /* Chip ID Register */
#define BMP280_RST_REG					(0xE0)  /* Softreset Register */
#define BMP280_STAT_REG					(0xF3)  /* Status Register */
#define BMP280_CTRL_MEAS_REG			(0xF4)  /* Ctrl Measure Register */
#define BMP280_CONFIG_REG				(0xF5)  /* Configuration Register */
#define BMP280_PRESSURE_MSB_REG			(0xF7)  /* Pressure MSB Register */
#define BMP280_PRESSURE_LSB_REG			(0xF8)  /* Pressure LSB Register */
#define BMP280_PRESSURE_XLSB_REG		(0xF9)  /* Pressure XLSB Register */
#define BMP280_TEMPERATURE_MSB_REG		(0xFA)  /* Temperature MSB Reg */
#define BMP280_TEMPERATURE_LSB_REG		(0xFB)  /* Temperature LSB Reg */
#define BMP280_TEMPERATURE_XLSB_REG		(0xFC)  /* Temperature XLSB Reg */


#define BMP280_TEMPERATURE_CALIB_DIG_T1_LSB_REG             (0x88)
#define BMP280_PRESSURE_TEMPERATURE_CALIB_DATA_LENGTH       (24)

#define BMP280_OVERSAMP_SKIPPED         (0x00)
#define BMP280_OVERSAMP_1X              (0x01)
#define BMP280_OVERSAMP_2X              (0x02)
#define BMP280_OVERSAMP_4X              (0x03)
#define BMP280_OVERSAMP_8X              (0x04)
#define BMP280_OVERSAMP_16X             (0x05)

#define BMP280_FILTER_COEFF_OFF         (0x00)
#define BMP280_FILTER_COEFF_2           (0x01)
#define BMP280_FILTER_COEFF_4           (0x02)
#define BMP280_FILTER_COEFF_8           (0x03)
#define BMP280_FILTER_COEFF_16          (0x04)

#define BMP280_FORCED_MODE             	(0x01)
#define BMP280_NORMAL_MODE				(0x03)

typedef struct bmp280_handle
{
	I2C_HandleTypeDef *	iic_ch;

}BMP280_Handle_t;

typedef struct
{
    uint16_t dig_T1;	/* calibration T1 data */
    int16_t dig_T2; /* calibration T2 data */
    int16_t dig_T3; /* calibration T3 data */
    uint16_t dig_P1;	/* calibration P1 data */
    int16_t dig_P2; /* calibration P2 data */
    int16_t dig_P3; /* calibration P3 data */
    int16_t dig_P4; /* calibration P4 data */
    int16_t dig_P5; /* calibration P5 data */
    int16_t dig_P6; /* calibration P6 data */
    int16_t dig_P7; /* calibration P7 data */
    int16_t dig_P8; /* calibration P8 data */
    int16_t dig_P9; /* calibration P9 data */
    int32_t t_fine; /* calibration t_fine data */
} bmp280Calib;

extern float bmp_t;
extern float bmp_p;

void BMP280_Init(void);
void BMP280GetData(float* pressure, float* temperature);
float BMP280PressureToAltitude(float* pressure/*, float* groundPressure, float* groundTemp*/);

#endif /* DRIVERS_BMP280_H_ */
