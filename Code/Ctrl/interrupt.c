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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim==&htim6)
	{
//		PMW3901_Update();
//		float test[2] = {(float)PMW3901_Motion.deltaX,(float)PMW3901_Motion.deltaY};


//		BMP280GetData(&bmp_p,&bmp_t);

//	    uint16_t valuetest = 100;
//	    uint16_t valuetest2 = 128;

//	    pwmWriteDigital(ESC_CMD,valuetest);
//	    pwmWriteDigital(ESC_CMD2,valuetest2);
//
//	//	delay_us(1000);
//		HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1,(uint32_t*)ESC_CMD,ESC_CMD_BUF_LEN);
//		HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_2,(uint32_t*)ESC_CMD2,ESC_CMD_BUF_LEN);

		//INV_DataUpdate();
	}
}

//void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
//{
//	if(htim==&htim3)
//	{
//		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_1)
//		{
//			HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
//		}else if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_2)
//		{
//			HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_2);
//		}
//		else if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_3)
//		{
//			HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_3);
//		}
//		else if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_4)
//		{
//			HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_4);
//		}
//	}
//
//}

