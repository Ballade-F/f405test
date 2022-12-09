/*
 * interrupt.c
 *
 *  Created on: Nov 28, 2022
 *      Author: Ballade_F
 */




#include "common.h"
#include "debug.h"
#include "mpu6000.h"
#include "bmp280.h"
#include "pwm_dshot.h"
#include "imu.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim==&htim6)
	{
		//光流测试
//		PMW3901_Update();
//		float test[2] = {(float)PMW3901_Motion.deltaX,(float)PMW3901_Motion.deltaY};


//		uint32_t time_temp;
//	    time_temp = getTime_us();
//	    delay_us(100);
		MPU6000_Data_UpDate();
		Angle_Update(IMU_UserData.gyro_x,IMU_UserData.gyro_y,IMU_UserData.gyro_z,
					 IMU_UserData.acc_x, IMU_UserData.acc_y, IMU_UserData.acc_z, 0.002);
//		time_temp = getTime_us() - time_temp;
//		if(time_temp>2000)
//		{
//			time_temp--;
//		}

		//气压计测试
//		BMP280GetData(&bmp_p,&bmp_t);


		//dshot600测试
//	    uint16_t valuetest = 100;
//	    uint16_t valuetest2 = 128;
//
//	    pwmWriteDigital(ESC_CMD,valuetest);
//	    pwmWriteDigital(ESC_CMD2,valuetest2);
//
//		HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1,(uint32_t*)ESC_CMD,ESC_CMD_BUF_LEN);
//		HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_2,(uint32_t*)ESC_CMD2,ESC_CMD_BUF_LEN);


	}
}



