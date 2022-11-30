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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim==&htim6)
	{
//		PMW3901_Update();
//		float test[2] = {(float)PMW3901_Motion.deltaX,(float)PMW3901_Motion.deltaY};
//		MPU6000Read();
		BMP280GetData(&bmp_p,&bmp_t);

		//INV_DataUpdate();
	}
}
