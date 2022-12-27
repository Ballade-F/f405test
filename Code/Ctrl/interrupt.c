/*
 * interrupt.c
 *
 *  Created on: Nov 28, 2022
 *      Author: Ballade_F
 */




#include "common.h"
#include "ctrl_task_list.h"
#include "state.h"

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
//		MPU6000_Data_UpDate();
//		Angle_Update(IMU_UserData.gyro_x,IMU_UserData.gyro_y,IMU_UserData.gyro_z,
//					 IMU_UserData.acc_x, IMU_UserData.acc_y, IMU_UserData.acc_z, 0.002);
//		time_temp = getTime_us() - time_temp;
//		if(time_temp>2000)
//		{
//			time_temp--;
//		}

		//气压计测试
//		BMP280GetData(&bmp_p,&bmp_t);


//		dshot600测试
		//标记时间
		static uint32_t worktime = 0;
		worktime++;

//		//从锁变解锁，重置worktime
//		if(State_IsArmed() && !(StateLast_IsArmed()))
//		{
//			worktime = 0;
//		}
//		//解锁状态，无事发生
//		else if(State_IsArmed() && StateLast_IsArmed())
//		{
//
//		}
//		//刚上锁或已经上锁，设置worktime到停机区间
//		else
//		{
//			worktime = 10000;
//		}
//		//更新上一状态
//		StateLast_Update();
//
//
		if(worktime<2000)
		{
			uint16_t valuetest = 0;
			pwmWriteDigital(PwmDshot_M1,valuetest);
			pwmWriteDigital(PwmDshot_M2,valuetest);
			pwmWriteDigital(PwmDshot_M3,valuetest);
			pwmWriteDigital(PwmDshot_M4,valuetest);


			HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1,(uint32_t*)PwmDshot_M1,ESC_CMD_BUF_LEN);
			HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_2,(uint32_t*)PwmDshot_M2,ESC_CMD_BUF_LEN);
			HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_3,(uint32_t*)PwmDshot_M3,ESC_CMD_BUF_LEN);
			HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_4,(uint32_t*)PwmDshot_M4,ESC_CMD_BUF_LEN);
			state_test = 0;
		}
		else if(worktime<4000)
		{
			uint16_t valuetest = 64;
			pwmWriteDigital(PwmDshot_M1,valuetest);
			pwmWriteDigital(PwmDshot_M2,valuetest);
			pwmWriteDigital(PwmDshot_M3,valuetest);
			pwmWriteDigital(PwmDshot_M4,valuetest);
			HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1,(uint32_t*)PwmDshot_M1,ESC_CMD_BUF_LEN);
			HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_2,(uint32_t*)PwmDshot_M2,ESC_CMD_BUF_LEN);
			HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_3,(uint32_t*)PwmDshot_M3,ESC_CMD_BUF_LEN);
			HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_4,(uint32_t*)PwmDshot_M4,ESC_CMD_BUF_LEN);
			state_test = 1;
		}
		else if(worktime<200000)
		{
			uint16_t valuetest = 0;
			pwmWriteDigital(PwmDshot_M1,valuetest);
			pwmWriteDigital(PwmDshot_M2,valuetest);
			pwmWriteDigital(PwmDshot_M3,valuetest);
			pwmWriteDigital(PwmDshot_M4,valuetest);
			HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1,(uint32_t*)PwmDshot_M1,ESC_CMD_BUF_LEN);
			HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_2,(uint32_t*)PwmDshot_M2,ESC_CMD_BUF_LEN);
			HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_3,(uint32_t*)PwmDshot_M3,ESC_CMD_BUF_LEN);
			HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_4,(uint32_t*)PwmDshot_M4,ESC_CMD_BUF_LEN);
			state_test = 2;
		}
		else
		{
			worktime = 10000;
		}
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	 if(huart==&huart1)//可以在communication里面封装一个句柄
	 {

		 HAL_UART_Receive_IT(&huart1, (uint8_t *)&CommandRec, 1);

		 if(CommandRec & (1<<1))
		 {

			 State_Arm();
		 }
		 else
		 {
			 State_DisArm();
		 }
	 }
}



