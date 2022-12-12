/*
 * state_ctrl.h
 *
 *  Created on: 2022年12月11日
 *      Author: Ballade_F
 */

#ifndef CTRL_STATE_CTRL_H_
#define CTRL_STATE_CTRL_H_

#include "pid.h"
#include "motor.h"
#include "imu.h"

#define PID_RATE_I_LIMIT 200.0f
#define PID_ANGLE_I_LIMIT 300.0f

#define PID_RATE_OUT_LIMIT 500.0f
#define PID_ANGLE_OUT_LIMIT 300.0f

#define CTRL_dT 0.002f


enum pidIndex
{
	RATE_ROLL = 0,
	RATE_PITCH,
	RATE_YAW,
	ANGLE_ROLL,
	ANGLE_PITCH,
	ANGLE_YAW,
	VELOCITY_Z,
	POSHOLD_Z,
	VELOCITY_XY,
	POSHOLD_XY,
	PID_NUM
};

typedef struct
{
	float kp;
	float ki;
	float kd;
}pidParam_t;

typedef struct
{
	pidParam_t pid[PID_NUM];
}StateCtrlParam_t;

typedef struct
{
	uint32_t timeStamp;

	float rateDesired_roll;
	float rateDesired_pitch;
	float rateDesired_yaw;

	float out_roll;
	float out_pitch;
	float out_yaw;


}Ctrl_State_t;


extern Ctrl_State_t StateCtrl_State;

extern StateCtrlParam_t StateCtrl_param;


#endif /* CTRL_STATE_CTRL_H_ */
