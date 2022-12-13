/*
 * pid.h
 *
 *  Created on: 2022年12月1日
 *      Author: Ballade_F
 */

#ifndef CTRL_PID_H_
#define CTRL_PID_H_

#include "common.h"
#include "mpu6000.h"
#include "ctrl_math.h"

typedef struct
{
	float error;        //< error
	float error_1;    //< previous error

	float kp;           //< proportional gain
	float ki;           //< integral gain
	float kd;           //< derivative gain

	float i_sum;
	float i_limit;       //< integral limit
	float out_limit;  //< total PID output limit, absolute value. '0' means no limit.

	float dt;           //< delta-time dt

}PID_handle_t;

void PID_Init(PID_handle_t* pid_p, float kp, float ki, float kd, float i_limit,float out_limit,float dt);
float PID_Calculate(PID_handle_t* pid_p,float error);

#endif /* CTRL_PID_H_ */
