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

#define MOTOR_MAX 700
#define MOTOR_MIN 10

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
void Motor_Test(uint16_t value_m1,uint16_t value_m2,uint16_t value_m3,uint16_t value_m4);

#endif /* CTRL_MOTOR_H_ */
