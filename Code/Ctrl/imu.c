/*
 * imu.c
 *
 *  Created on: 2022年12月7日
 *      Author: Ballade_F
 */

#include "imu.h"

ANGLE_STATE_t F_S;
float IMU_IIR_kp = 0.6;
float IMU_IIR_ki = 0.005;

void Angle_Init(void)
{




}

void Angle_Update(float gyrox, float gyroy, float gyroz,
				  float ax,    float ay,    float az,
				  float dt)
{
	//1.归一化acc
	float acc_norm_inv = 1.0f/sqrt(ax*ax + ay*ay + az*az);
	ax *= acc_norm_inv;
	ay *= acc_norm_inv;
	az *= acc_norm_inv;

	//2.世界系(0,0,-1)在机体系下坐标,右手系
	float gx_BF = 2*(F_S.s_q[1]*F_S.s_q[3] + F_S.s_q[0]*F_S.s_q[2]);
	float gy_BF = 2*(F_S.s_q[2]*F_S.s_q[3] - F_S.s_q[0]*F_S.s_q[1]);
	float gz_BF = 1 - 2*(F_S.s_q[1]*F_S.s_q[1] + F_S.s_q[2]*F_S.s_q[2]);

	//3.计算误差,右手系gxa
	float ex = gy_BF * az - gz_BF * ay;
	float ey = gz_BF * ax - gx_BF * az;
	float ez = gx_BF * ay - gy_BF * ax;

	//4.二阶互补滤波
	static float iir_i_sum[3] = {0};
	iir_i_sum[0] += IMU_IIR_ki*ex*dt;
	iir_i_sum[1] += IMU_IIR_ki*ey*dt;
	iir_i_sum[2] += IMU_IIR_ki*ez*dt;

	gyrox += IMU_IIR_kp*ex + iir_i_sum[0];
	gyrox += IMU_IIR_kp*ex + iir_i_sum[0];
	gyrox += IMU_IIR_kp*ex + iir_i_sum[0];

	//5.更新q，一阶毕卡近似
	gyrox *= (0.5f*dt);
	gyroy *= (0.5f*dt);
	gyroz *= (0.5f*dt);

	float buffer[4] = {F_S.s_q[0],F_S.s_q[1],F_S.s_q[2],F_S.s_q[3]};
	F_S.s_q[0] += (-buffer[1] * gyrox - buffer[2] * gyroy - buffer[3] * gyroz);
	F_S.s_q[1] += (+buffer[0] * gyrox + buffer[2] * gyroz - buffer[3] * gyroy);
	F_S.s_q[2] += (+buffer[0] * gyroy - buffer[1] * gyroz + buffer[3] * gyrox);
	F_S.s_q[3] += (+buffer[0] * gyroz + buffer[1] * gyroy - buffer[2] * gyrox);

	//6.归一化四元数，计算p,r,y
	float q_norm_inv = 1.0f/sqrt(F_S.s_q[0]*F_S.s_q[0] + F_S.s_q[1]*F_S.s_q[1] + F_S.s_q[2]*F_S.s_q[2] + F_S.s_q[3]*F_S.s_q[3]);
	F_S.s_q[0] *= q_norm_inv;
	F_S.s_q[1] *= q_norm_inv;
	F_S.s_q[2] *= q_norm_inv;
	F_S.s_q[3] *= q_norm_inv;

	F_S.roll = atan2_approx(gx_BF,-gz_BF) * RAD2DEG;
	F_S.pitch = (0.5f * M_PIf) - acos_approx(-gy_BF) * RAD2DEG;
//	F_S.yaw = atan2_approx(rMat[1][0], rMat[0][0]);
}
