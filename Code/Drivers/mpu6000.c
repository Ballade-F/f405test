/*
 * mpu6000.c
 *
 *  Created on: Nov 28, 2022
 *      Author: Ballade_F
 */


#include "mpu6000.h"



// Bits
#define BIT_SLEEP                   0x40
#define BIT_H_RESET                 0x80
#define BITS_CLKSEL                 0x07
#define MPU_CLK_SEL_PLLGYROX        0x01
#define MPU_CLK_SEL_PLLGYROZ        0x03
#define MPU_EXT_SYNC_GYROX          0x02
#define BITS_FS_250DPS              0x00
#define BITS_FS_500DPS              0x08
#define BITS_FS_1000DPS             0x10
#define BITS_FS_2000DPS             0x18
#define BITS_FS_2G                  0x00
#define BITS_FS_4G                  0x08
#define BITS_FS_8G                  0x10
#define BITS_FS_16G                 0x18
#define BITS_FS_MASK                0x18
#define BITS_DLPF_CFG_256HZ         0x00
#define BITS_DLPF_CFG_188HZ         0x01
#define BITS_DLPF_CFG_98HZ          0x02
#define BITS_DLPF_CFG_42HZ          0x03
#define BITS_DLPF_CFG_20HZ          0x04
#define BITS_DLPF_CFG_10HZ          0x05
#define BITS_DLPF_CFG_5HZ           0x06
#define BITS_DLPF_CFG_2100HZ_NOLPF  0x07
#define BITS_DLPF_CFG_MASK          0x07
#define BIT_INT_ANYRD_2CLEAR        0x10
#define BIT_RAW_RDY_EN              0x01
#define BIT_I2C_IF_DIS              0x10
#define BIT_INT_STATUS_DATA         0x01
#define BIT_GYRO                    3
#define BIT_ACC                     2
#define BIT_TEMP                    1


#define ACC_GYRO_RAWDATA_LEN	14



MPU6000_Handle_t mpu6000;
MPU6000_RawData_t MPU6000_rawData;
IMU_DATA_t IMU_UserData;



void mpu6000_handle_set(void)
{
	mpu6000.spi_ch = &hspi1;
	mpu6000.spi_cs_port = GPIOC;
	mpu6000.spi_cs_pin = GPIO_PIN_2;

}



void mpu6000SpiWriteRegister(uint8_t reg, uint8_t data)
{
	HAL_GPIO_WritePin(mpu6000.spi_cs_port, mpu6000.spi_cs_pin, GPIO_PIN_RESET);
	delay_ms(1);
	reg &= ~0x80u;
	HAL_SPI_TransmitReceive(mpu6000.spi_ch,&reg, &reg,sizeof(uint8_t),1000);
	HAL_SPI_TransmitReceive(mpu6000.spi_ch,&data, &data,sizeof(uint8_t),1000);

	HAL_GPIO_WritePin(mpu6000.spi_cs_port, mpu6000.spi_cs_pin, GPIO_PIN_SET);
	delay_ms(1);

}

void mpu6000SpiReadRegister(uint8_t reg, uint8_t length, uint8_t *data)
{
	HAL_GPIO_WritePin(mpu6000.spi_cs_port, mpu6000.spi_cs_pin, GPIO_PIN_RESET);
	reg |= 0x80u;
	HAL_SPI_TransmitReceive(mpu6000.spi_ch,&reg, &reg,sizeof(uint8_t),1000);
	HAL_SPI_Receive(mpu6000.spi_ch, data,length*sizeof(uint8_t),1000);
	HAL_GPIO_WritePin(mpu6000.spi_cs_port, mpu6000.spi_cs_pin, GPIO_PIN_SET);

}

void _speedSet10M(SPI_HandleTypeDef *hspi)
{
	__HAL_SPI_DISABLE(hspi);
	hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	WRITE_REG(hspi->Instance->CR1, ((hspi->Init.Mode & (SPI_CR1_MSTR | SPI_CR1_SSI)) |
	                                  (hspi->Init.Direction & (SPI_CR1_RXONLY | SPI_CR1_BIDIMODE)) |
	                                  (hspi->Init.DataSize & SPI_CR1_DFF) |
	                                  (hspi->Init.CLKPolarity & SPI_CR1_CPOL) |
	                                  (hspi->Init.CLKPhase & SPI_CR1_CPHA) |
	                                  (hspi->Init.NSS & SPI_CR1_SSM) |
	                                  (hspi->Init.BaudRatePrescaler & SPI_CR1_BR_Msk) |
	                                  (hspi->Init.FirstBit  & SPI_CR1_LSBFIRST) |
	                                  (hspi->Init.CRCCalculation & SPI_CR1_CRCEN)));
}

void MPU6000_selfTest(void)
{
	delay_ms(1);
	int i;
	//清零
	for(i = 0;i<3;++i)
	{
		MPU6000_rawData.acc_ave[i] = 0;
	}
	for(i = 0;i<3;++i)
	{
		MPU6000_rawData.gyro_zero[i] = 0;
	}

	//等0.5s，防止刚上电时机体不平稳
	delay_ms(500);
	//舍弃前十个数
	i = 10;
	while(i--)
	{
		MPU6000Read();
		delay_ms(1);
	}

	//读1000次取平均，若用卡尔曼滤波则可以再取方差
	i = 1000;
	while(i--)
	{
		MPU6000Read();
		for(int j = 0;j<3;++j)
		{
			MPU6000_rawData.acc_ave[j] += MPU6000_rawData.acc[j];
		}
		for(int j = 0;j<3;++j)
		{
			MPU6000_rawData.gyro_zero[j] += MPU6000_rawData.gyro[j];
		}
		delay_ms(1);
	}

	for(int j = 0;j<3;++j)
	{
		MPU6000_rawData.acc_ave[j] *= 0.001;
	}
	for(int j = 0;j<3;++j)
	{
		MPU6000_rawData.gyro_zero[j] *= 0.001;
	}

}

void MPU6000_Init(void)
{
	mpu6000_handle_set();

	//复位MPU6000
	mpu6000SpiWriteRegister(MPU_RA_PWR_MGMT_1, BIT_H_RESET);
	delay_ms(50);
	mpu6000SpiWriteRegister(MPU_RA_SIGNAL_PATH_RESET, BIT_GYRO | BIT_ACC | BIT_TEMP);
	delay_ms(50);
	mpu6000SpiWriteRegister(MPU_RA_PWR_MGMT_1, BIT_H_RESET);//复位两次增加传感器稳定性
	delay_ms(50);
	mpu6000SpiWriteRegister(MPU_RA_SIGNAL_PATH_RESET, BIT_GYRO | BIT_ACC | BIT_TEMP);
	delay_ms(50);

	//读取ID
	uint8_t id = 0;
	while(MPU6000_WHO_AM_I_CONST != id)
	{
		mpu6000SpiReadRegister(MPU_RA_WHO_AM_I, 1, &id);
		delay_ms(10);
	}

	//读取正常，初始化

	//设置X轴陀螺作为时钟
	mpu6000SpiWriteRegister(MPU_RA_PWR_MGMT_1, MPU_CLK_SEL_PLLGYROX);
	delay_ms(15);

	//禁止I2C接口
	mpu6000SpiWriteRegister(MPU_RA_USER_CTRL, BIT_I2C_IF_DIS);
	delay_ms(15);
	mpu6000SpiWriteRegister(MPU_RA_PWR_MGMT_2, 0x00);
	delay_ms(15);

	// Accel Sample Rate 1kHz
	// Gyroscope Output Rate =  1kHz when the DLPF is enabled
	mpu6000SpiWriteRegister(MPU_RA_SMPLRT_DIV, 0);//设置采样率
	delay_ms(15);

	//设置陀螺仪 +/- 2000 DPS量程
	mpu6000SpiWriteRegister(MPU_RA_GYRO_CONFIG, FSR_2000DPS << 3);
	delay_ms(15);

	//设置加速度 +/- 8 G 量程
	mpu6000SpiWriteRegister(MPU_RA_ACCEL_CONFIG, FSR_8G << 3);
	delay_ms(15);

	//设置中断引脚功能
	mpu6000SpiWriteRegister(MPU_RA_INT_PIN_CFG, 0 << 7 | 0 << 6 | 0 << 5 | 1 << 4 | 0 << 3 | 0 << 2 | 0 << 1 | 0 << 0);//中断引脚配置
	delay_ms(15);

	//设置低通滤波带宽
	mpu6000SpiWriteRegister(MPU_RA_CONFIG, BITS_DLPF_CFG_98HZ);
	delay_ms(1);

	_speedSet10M(mpu6000.spi_ch);

	MPU6000_selfTest();

}



void MPU6000Read(void)
{

	uint8_t buffer[14];
	mpu6000SpiReadRegister(MPU_RA_ACCEL_XOUT_H, 14, buffer);

//	int16_t testarr[6] = {0};
//	testarr[0] = ((uint16_t) buffer[0] << 8 | buffer[1]) ;
//	testarr[1] = ((uint16_t) buffer[2] << 8 | buffer[3]) ;
//	testarr[2] = ((uint16_t) buffer[4] << 8 | buffer[5]) ;
//
//	testarr[3] = ((uint16_t) buffer[8] << 8 | buffer[9]) ;
//	testarr[4] = ((uint16_t) buffer[10] << 8 | buffer[11]) ;
//	testarr[5] = ((uint16_t) buffer[12] << 8 | buffer[13]) ;
//
//		MPU6000_rawData.acc[0] = testarr[0];
//		MPU6000_rawData.acc[1] = testarr[1];
//		MPU6000_rawData.acc[2] = testarr[2];
//
//		MPU6000_rawData.gyro[0] = testarr[3];
//		MPU6000_rawData.gyro[1] = testarr[4];
//		MPU6000_rawData.gyro[2] = testarr[5];


	MPU6000_rawData.acc[0] = (int16_t)((uint16_t) buffer[0] << 8 | buffer[1]) * ACC_UNIT;
	MPU6000_rawData.acc[1] = (int16_t)((uint16_t) buffer[2] << 8 | buffer[3]) * ACC_UNIT;
	MPU6000_rawData.acc[2] = (int16_t)((uint16_t) buffer[4] << 8 | buffer[5]) * ACC_UNIT;

	MPU6000_rawData.gyro[0] = (int16_t)((uint16_t) buffer[8] << 8 | buffer[9]) * GYRO_UNIT;
	MPU6000_rawData.gyro[1] = (int16_t)((uint16_t) buffer[10] << 8 | buffer[11]) * GYRO_UNIT;
	MPU6000_rawData.gyro[2] = (int16_t)((uint16_t) buffer[12] << 8 | buffer[13]) * GYRO_UNIT;
}

void MPU6000_Data_UpDate(void)
{
	MPU6000Read();
	IMU_UserData.acc_x = -MPU6000_rawData.acc[0];
	IMU_UserData.acc_y = -MPU6000_rawData.acc[1];
	IMU_UserData.acc_z = +MPU6000_rawData.acc[2];

	IMU_UserData.gyro_x = -(MPU6000_rawData.gyro[0] - MPU6000_rawData.gyro_zero[0]);
	IMU_UserData.gyro_y = -(MPU6000_rawData.gyro[1] - MPU6000_rawData.gyro_zero[1]);
	IMU_UserData.gyro_z = +(MPU6000_rawData.gyro[2] - MPU6000_rawData.gyro_zero[2]);

}

