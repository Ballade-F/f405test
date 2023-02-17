/*
 * motor.c
 *
 *  Created on: 2022年12月11日
 *      Author: Ballade_F
 */


#include "motor.h"

Motor_State_t Motor_State;



/*
 *  ->        <-
 *  M3	 y	  M1
 *
 *  	 z	  x
 *
 *  <-		  ->
 *  M2		  M4

 */

void Motor_Init(void)
{
	Motor_State.m1 = 0;
	Motor_State.m2 = 0;
	Motor_State.m3 = 0;
	Motor_State.m4 = 0;
	Motor_State.thrust = 800;


}

void Motor_Update(void)
{
//	motorPWM.m1 = thrust -r+p-y;
//	motorPWM.m2 = thrust +r-p-y;
//	motorPWM.m3 = thrust +r+p+y;
//	motorPWM.m4 = thrust -r-p+y;

	//1.确定平均油门thrust
	//TODO:

	//2.计算各个电机，1~2000
	Motor_State.m1 = CTRL_Constrain(Motor_State.thrust -PosCtrl_State.out_roll+PosCtrl_State.out_pitch-PosCtrl_State.out_yaw , MOTOR_MIN ,MOTOR_MAX);
	Motor_State.m2 = CTRL_Constrain(Motor_State.thrust +PosCtrl_State.out_roll-PosCtrl_State.out_pitch-PosCtrl_State.out_yaw , MOTOR_MIN ,MOTOR_MAX);
	Motor_State.m3 = CTRL_Constrain(Motor_State.thrust +PosCtrl_State.out_roll+PosCtrl_State.out_pitch+PosCtrl_State.out_yaw , MOTOR_MIN ,MOTOR_MAX);
	Motor_State.m4 = CTRL_Constrain(Motor_State.thrust -PosCtrl_State.out_roll-PosCtrl_State.out_pitch+PosCtrl_State.out_yaw , MOTOR_MIN ,MOTOR_MAX);

	//3.转为dshot协议值，48~2047
	//TODO:arm保护
	HAL_TIM_DMABurst_WriteStop(&htim3, TIM_DMA_UPDATE);
	Dshot_BurstWrite(PwmDshot_M,47 + Motor_State.m4,47 + Motor_State.m1,47 + Motor_State.m2,47 + Motor_State.m3);
	__HAL_TIM_CLEAR_FLAG(&htim3,TIM_FLAG_UPDATE);
	TIM_CCxChannelCmd(htim3.Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
	TIM_CCxChannelCmd(htim3.Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);
	TIM_CCxChannelCmd(htim3.Instance, TIM_CHANNEL_3, TIM_CCx_ENABLE);
	TIM_CCxChannelCmd(htim3.Instance, TIM_CHANNEL_4, TIM_CCx_ENABLE);
	HAL_TIM_DMABurst_MultiWriteStart(&htim3, TIM_DMABASE_CCR1, TIM_DMA_UPDATE, PwmDshot_M, TIM_DMABURSTLENGTH_4TRANSFERS, (ESC_CMD_BUF_LEN*4));
	TIM3->EGR = TIM_EGR_UG;
	__HAL_TIM_ENABLE(&htim3);
//	pwmWriteDigital(PwmDshot_M1,47 + Motor_State.m1);
//	pwmWriteDigital(PwmDshot_M2,47 + Motor_State.m2);
//	pwmWriteDigital(PwmDshot_M3,47 + Motor_State.m3);
//	pwmWriteDigital(PwmDshot_M4,47 + Motor_State.m4);
//
//	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1,(uint32_t*)PwmDshot_M1,ESC_CMD_BUF_LEN);
//	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_2,(uint32_t*)PwmDshot_M2,ESC_CMD_BUF_LEN);
//	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_3,(uint32_t*)PwmDshot_M3,ESC_CMD_BUF_LEN);
//	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_4,(uint32_t*)PwmDshot_M4,ESC_CMD_BUF_LEN);

}


void Motor_Stop(void)
{
	HAL_TIM_DMABurst_WriteStop(&htim3, TIM_DMA_UPDATE);
	Dshot_BurstWrite(PwmDshot_M,0,0,0,0);
	__HAL_TIM_CLEAR_FLAG(&htim3,TIM_FLAG_UPDATE);
	TIM_CCxChannelCmd(htim3.Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
	TIM_CCxChannelCmd(htim3.Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);
	TIM_CCxChannelCmd(htim3.Instance, TIM_CHANNEL_3, TIM_CCx_ENABLE);
	TIM_CCxChannelCmd(htim3.Instance, TIM_CHANNEL_4, TIM_CCx_ENABLE);
	HAL_TIM_DMABurst_MultiWriteStart(&htim3, TIM_DMABASE_CCR1, TIM_DMA_UPDATE, PwmDshot_M, TIM_DMABURSTLENGTH_4TRANSFERS, (ESC_CMD_BUF_LEN*4));
	TIM3->EGR = TIM_EGR_UG;
	__HAL_TIM_ENABLE(&htim3);
}

void Motor_Test(uint16_t value_m1,uint16_t value_m2,uint16_t value_m3,uint16_t value_m4)
{
	HAL_TIM_DMABurst_WriteStop(&htim3, TIM_DMA_UPDATE);
	Dshot_BurstWrite(PwmDshot_M,value_m1,value_m2,value_m3,value_m4);
	__HAL_TIM_CLEAR_FLAG(&htim3,TIM_FLAG_UPDATE);
	TIM_CCxChannelCmd(htim3.Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
	TIM_CCxChannelCmd(htim3.Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);
	TIM_CCxChannelCmd(htim3.Instance, TIM_CHANNEL_3, TIM_CCx_ENABLE);
	TIM_CCxChannelCmd(htim3.Instance, TIM_CHANNEL_4, TIM_CCx_ENABLE);
	HAL_TIM_DMABurst_MultiWriteStart(&htim3, TIM_DMABASE_CCR1, TIM_DMA_UPDATE, PwmDshot_M, TIM_DMABURSTLENGTH_4TRANSFERS, (ESC_CMD_BUF_LEN*4));
	TIM3->EGR = TIM_EGR_UG;
	__HAL_TIM_ENABLE(&htim3);
}

