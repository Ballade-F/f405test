/*
 * state_ctrl.c
 *
 *  Created on: 2022年12月11日
 *      Author: Ballade_F
 */

#include "pos_ctrl.h"

PID_handle_t StateCtrl_PID[PID_NUM];
PosCtrl_State_t PosCtrl_State;

PosCtrlParam_t PosCtrl_param;

static PosCtrlParam_t posCtrl_paramDefault =
{
		.pid =
		{
				[RATE_ROLL]   = {2, 1.5, 0},
				[RATE_PITCH]  = {2, 1.5, 0},
				[RATE_YAW]    = {1.20, 8, 0},
				[ANGLE_ROLL]  = {8, 0, 0},
				[ANGLE_PITCH] = {8, 0, 0},
				[ANGLE_YAW]   = {8, 0, 0},
				[VELOCITY_Z]  = {1.50, 2, 0.05},
				[POSHOLD_Z]   = {0.45, 0, 0},
				[VELOCITY_XY] = {0, 0, 0},
				[POSHOLD_XY]  = {0, 0, 0},
		},
};


void posCtrl_resetParam(void)
{
	PosCtrl_param = posCtrl_paramDefault;
}


void PosCtrl_Init(void)
{
	posCtrl_resetParam();
	PID_Init(&StateCtrl_PID[RATE_ROLL],PosCtrl_param.pid[RATE_ROLL].kp,PosCtrl_param.pid[RATE_ROLL].ki,PosCtrl_param.pid[RATE_ROLL].kd,
			PID_RATE_I_LIMIT,PID_RATE_OUT_LIMIT,CTRL_dT);
	PID_Init(&StateCtrl_PID[RATE_PITCH],PosCtrl_param.pid[RATE_PITCH].kp,PosCtrl_param.pid[RATE_PITCH].ki,PosCtrl_param.pid[RATE_PITCH].kd,
			PID_RATE_I_LIMIT,PID_RATE_OUT_LIMIT,CTRL_dT);
	PID_Init(&StateCtrl_PID[RATE_YAW],PosCtrl_param.pid[RATE_YAW].kp,PosCtrl_param.pid[RATE_YAW].ki,PosCtrl_param.pid[RATE_YAW].kd,
			PID_RATE_I_LIMIT,PID_RATE_OUT_LIMIT,CTRL_dT);

	PID_Init(&StateCtrl_PID[ANGLE_ROLL],PosCtrl_param.pid[ANGLE_ROLL].kp,PosCtrl_param.pid[ANGLE_ROLL].ki,PosCtrl_param.pid[ANGLE_ROLL].kd,
			PID_ANGLE_I_LIMIT,PID_ANGLE_OUT_LIMIT,CTRL_dT);
	PID_Init(&StateCtrl_PID[ANGLE_PITCH],PosCtrl_param.pid[ANGLE_PITCH].kp,PosCtrl_param.pid[ANGLE_PITCH].ki,PosCtrl_param.pid[ANGLE_PITCH].kd,
			PID_ANGLE_I_LIMIT,PID_ANGLE_OUT_LIMIT,CTRL_dT);
	PID_Init(&StateCtrl_PID[ANGLE_YAW],PosCtrl_param.pid[ANGLE_YAW].kp,PosCtrl_param.pid[ANGLE_YAW].ki,PosCtrl_param.pid[ANGLE_YAW].kd,
			PID_ANGLE_I_LIMIT,PID_ANGLE_OUT_LIMIT,CTRL_dT);

}

void PosCtrl_Update(void)
{
	//1.更新state的时间戳
	//TODO:

	//2.外环，角度环得期望角速度
	PosCtrl_State.rateDesired_roll = PID_Calculate(&StateCtrl_PID[ANGLE_ROLL],0 - F_S.roll);
	PosCtrl_State.rateDesired_pitch = PID_Calculate(&StateCtrl_PID[ANGLE_PITCH],0 - F_S.pitch);

	//3.内环，角速度环得rpy输出分量
	PosCtrl_State.out_roll = PID_Calculate(&StateCtrl_PID[RATE_ROLL],PosCtrl_State.rateDesired_roll - F_S.d_roll);
	PosCtrl_State.out_pitch = PID_Calculate(&StateCtrl_PID[RATE_PITCH],PosCtrl_State.rateDesired_pitch - F_S.d_pitch);

	//

}




