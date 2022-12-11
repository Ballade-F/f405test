/*
 * state_ctrl.c
 *
 *  Created on: 2022年12月11日
 *      Author: Ballade_F
 */

#include "state_ctrl.h"

PID_handle_t StateCtrl_PID[PID_NUM];
Ctrl_State_t StateCtrl_State;

StateCtrlParam_t StateCtrl_param;

static StateCtrlParam_t stateCtrl_paramDefault =
{
		.pid =
		{
				[RATE_ROLL]   = {85, 900, 18},
				[RATE_PITCH]  = {90, 1000, 18},
				[RATE_YAW]    = {120, 800, 0},
				[ANGLE_ROLL]  = {600, 0, 0},
				[ANGLE_PITCH] = {600, 0, 0},
				[ANGLE_YAW]   = {600, 0, 0},
				[VELOCITY_Z]  = {150, 200, 50},
				[POSHOLD_Z]   = {45, 0, 0},
				[VELOCITY_XY] = {0, 0, 0},
				[POSHOLD_XY]  = {0, 0, 0},
		},
};


void stateCtrl_resetParam(void)
{
	StateCtrl_param = stateCtrl_paramDefault;
}


void StateCtrl_Init(void)
{
	PID_Init(&StateCtrl_PID[RATE_ROLL],StateCtrl_param.pid[RATE_ROLL].kp,StateCtrl_param.pid[RATE_ROLL].ki,StateCtrl_param.pid[RATE_ROLL].kd,
			PID_RATE_I_LIMIT,PID_RATE_OUT_LIMIT,CTRL_dT);
	PID_Init(&StateCtrl_PID[RATE_PITCH],StateCtrl_param.pid[RATE_PITCH].kp,StateCtrl_param.pid[RATE_PITCH].ki,StateCtrl_param.pid[RATE_PITCH].kd,
			PID_RATE_I_LIMIT,PID_RATE_OUT_LIMIT,CTRL_dT);
	PID_Init(&StateCtrl_PID[RATE_YAW],StateCtrl_param.pid[RATE_YAW].kp,StateCtrl_param.pid[RATE_YAW].ki,StateCtrl_param.pid[RATE_YAW].kd,
			PID_RATE_I_LIMIT,PID_RATE_OUT_LIMIT,CTRL_dT);

	PID_Init(&StateCtrl_PID[ANGLE_ROLL],StateCtrl_param.pid[ANGLE_ROLL].kp,StateCtrl_param.pid[ANGLE_ROLL].ki,StateCtrl_param.pid[ANGLE_ROLL].kd,
			PID_ANGLE_I_LIMIT,PID_ANGLE_OUT_LIMIT,CTRL_dT);
	PID_Init(&StateCtrl_PID[ANGLE_PITCH],StateCtrl_param.pid[ANGLE_PITCH].kp,StateCtrl_param.pid[ANGLE_PITCH].ki,StateCtrl_param.pid[ANGLE_PITCH].kd,
			PID_ANGLE_I_LIMIT,PID_ANGLE_OUT_LIMIT,CTRL_dT);
	PID_Init(&StateCtrl_PID[ANGLE_YAW],StateCtrl_param.pid[ANGLE_YAW].kp,StateCtrl_param.pid[ANGLE_YAW].ki,StateCtrl_param.pid[ANGLE_YAW].kd,
			PID_ANGLE_I_LIMIT,PID_ANGLE_OUT_LIMIT,CTRL_dT);

}

void StateCtrl_Update(void)
{
	//1.更新state的时间戳
	//TODO:

	//2.外环，角度环得期望角速度
	StateCtrl_State.rateDesired_roll = PID_Calculate(&StateCtrl_PID[ANGLE_ROLL],0 - F_S.roll);
	StateCtrl_State.rateDesired_pitch = PID_Calculate(&StateCtrl_PID[ANGLE_PITCH],0 - F_S.pitch);

	//3.内环，角速度环得rpy输出分量
	StateCtrl_State.out_roll = PID_Calculate(&StateCtrl_PID[RATE_ROLL],StateCtrl_State.rateDesired_roll - F_S.d_roll);
	StateCtrl_State.out_pitch = PID_Calculate(&StateCtrl_PID[RATE_PITCH],StateCtrl_State.rateDesired_pitch - F_S.d_pitch);

	//

}




