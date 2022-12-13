/*
 * pid.c
 *
 *  Created on: 2022年12月1日
 *      Author: Ballade_F
 */

#include "pid.h"

void PID_Init(PID_handle_t* pid_p, float kp, float ki, float kd, float i_limit,float out_limit,float dt)
{
	pid_p->kp = kp;
	pid_p->kd = kd;
	pid_p->ki = ki;

	pid_p->error = 0;
	pid_p->error_1 = 0;

	pid_p->i_sum = 0;
	pid_p->i_limit = i_limit;
	pid_p->out_limit = out_limit;

	pid_p->dt = dt;
}



float PID_Calculate(PID_handle_t* pid_p,float error)
{
	float output = 0.0f;
	pid_p->error = error;
	pid_p->i_sum += error * pid_p->dt;

	pid_p->i_sum = CTRL_Constrain(pid_p->i_sum,-pid_p->i_limit,pid_p->i_limit);

	output = (pid_p->kp * error) + (pid_p->kd * (pid_p->error - pid_p->error_1)/pid_p->dt) + (pid_p->ki * pid_p->i_sum);

	output = CTRL_Constrain(output,-pid_p->out_limit,pid_p->out_limit);

	pid_p->error_1 = error;

	return output;
}
