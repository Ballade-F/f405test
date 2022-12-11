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
	uint32_t timeStamp;
	float s_q[4];
	float roll;
	float pitch;
	float yaw;

	//角速度deg/s
	float d_roll;
	float d_pitch;
	float d_yaw;

	//加速度m/s2
	float a_right;
	float a_forward;
	float a_up;

}IMU_STATE_t;

extern IMU_STATE_t F_S;

extern float test_roll;

void Angle_Init(void);
void Angle_Update(float gyrox, float gyroy, float gyroz,
				  float ax,    float ay,    float az,
				  float dt);


#endif /* CTRL_IMU_H_ */
