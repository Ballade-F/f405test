/*
 * motor.h
 *
 *  Created on: 2022年12月11日
 *      Author: Ballade_F
 */

#ifndef CTRL_MOTOR_H_
#define CTRL_MOTOR_H_

#include <pos_ctrl.h>
#include "common.h"
#include "pwm_dshot.h"
#include "ctrl_math.h"

#define MOTOR_MAX 1000
#define MOTOR_MIN 1

typedef struct
{
	uint16_t m1;
	uint16_t m2;
	uint16_t m3;
	uint16_t m4;

	uint16_t thrust;
}Motor_State_t;

extern Motor_State_t Motor_State;

void Motor_Init(void);
void Motor_Update(void);
void Motor_Stop(void);

#endif /* CTRL_MOTOR_H_ */
