/*
 * imu.h
 *
 *  Created on: 2022年12月7日
 *      Author: Ballade_F
 */

#ifndef CTRL_IMU_H_
#define CTRL_IMU_H_

#include "common.h"
#include "mpu6000.h"
#include "math.h"
#include "ctrl_math.h"



typedef struct
{
	float s_q[4];
	float pitch;
	float roll;
	float yaw;
}ANGLE_STATE_t;

#endif /* CTRL_IMU_H_ */
