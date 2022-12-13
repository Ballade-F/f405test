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
	Motor_State.thrust = 0;
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
	Motor_State.m1 = CTRL_Constrain(Motor_State.thrust -StateCtrl_State.out_roll+StateCtrl_State.out_pitch-StateCtrl_State.out_yaw , MOTOR_MIN ,MOTOR_MAX);
	Motor_State.m2 = CTRL_Constrain(Motor_State.thrust +StateCtrl_State.out_roll-StateCtrl_State.out_pitch-StateCtrl_State.out_yaw , MOTOR_MIN ,MOTOR_MAX);
	Motor_State.m3 = CTRL_Constrain(Motor_State.thrust +StateCtrl_State.out_roll+StateCtrl_State.out_pitch+StateCtrl_State.out_yaw , MOTOR_MIN ,MOTOR_MAX);
	Motor_State.m4 = CTRL_Constrain(Motor_State.thrust -StateCtrl_State.out_roll-StateCtrl_State.out_pitch+StateCtrl_State.out_yaw , MOTOR_MIN ,MOTOR_MAX);

	//3.转为dshot协议值，48~2047
	//TODO:arm保护
	pwmWriteDigital(PwmDshot_M1,47 + Motor_State.m1);
	pwmWriteDigital(PwmDshot_M2,47 + Motor_State.m2);
	pwmWriteDigital(PwmDshot_M3,47 + Motor_State.m3);
	pwmWriteDigital(PwmDshot_M4,47 + Motor_State.m4);

	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1,(uint32_t*)PwmDshot_M1,ESC_CMD_BUF_LEN);
	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_2,(uint32_t*)PwmDshot_M2,ESC_CMD_BUF_LEN);
	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_3,(uint32_t*)PwmDshot_M3,ESC_CMD_BUF_LEN);
	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_4,(uint32_t*)PwmDshot_M4,ESC_CMD_BUF_LEN);


}
