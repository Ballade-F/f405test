/*
 * imu.c
 *
 *  Created on: 2022年12月7日
 *      Author: Ballade_F
 */

#include "imu.h"

IMU_STATE_t F_S;
float IMU_IIR_kp = 0.6;
float IMU_IIR_ki = 0.005;

float test_roll = 0;


void Angle_Init(void)
{
	MPU6000_Init();

	F_S.s_q[0] = 1;
	F_S.s_q[1] = 0;
	F_S.s_q[2] = 0;
	F_S.s_q[3] = 0;
	F_S.pitch = 0;
	F_S.roll = 0;
	F_S.yaw = 0;
}

void Angle_Update(float gyrox, float gyroy, float gyroz,
				  float ax,    float ay,    float az,
				  float dt)
{
	//存入角速度和加速度,以及时间
	F_S.a_right = ax;
	F_S.a_forward = ay;
	F_S.a_up = az;

	F_S.d_pitch = gyrox;
	F_S.d_roll = gyroy;
	F_S.d_yaw = gyroz;

	//TODO:存入时间

	//对比直接积分
	test_roll += gyroy*dt;

	gyrox *= DEG2RAD;
	gyroy *= DEG2RAD;
	gyroz *= DEG2RAD;

	//1.归一化acc
	float acc_norm_inv = 1.0f/sqrt(ax*ax + ay*ay + az*az);
	ax *= acc_norm_inv;
	ay *= acc_norm_inv;
	az *= acc_norm_inv;

	//2.世界系(0,0,1)在机体系下坐标,右手系;
	//Hamilton表达中的旋转矩阵乘向量，表示将惯性系的坐标转为世界系下，此处是世界系转惯性系，对矩阵求逆，由于是正交矩阵，等于转置
//	float gx_BF = 2*(F_S.s_q[1]*F_S.s_q[3] - F_S.s_q[0]*F_S.s_q[2]);
//	float gy_BF = 2*(F_S.s_q[2]*F_S.s_q[3] + F_S.s_q[0]*F_S.s_q[1]);
//	float gz_BF = 1 - 2*(F_S.s_q[1]*F_S.s_q[1] + F_S.s_q[2]*F_S.s_q[2]);

	//2.
	static float gx_BF = 0;
	static float gy_BF = 0;
	static float gz_BF = 1;

	//3.计算误差,右手系axg
	float ex = gz_BF * ay - gy_BF * az;
	float ey = gx_BF * az - gz_BF * ax;
	float ez = gy_BF * ax - gx_BF * ay;

	//4.二阶互补滤波
	static float iir_i_sum[3] = {0};
	iir_i_sum[0] += IMU_IIR_ki*ex*dt;
	iir_i_sum[1] += IMU_IIR_ki*ey*dt;
	iir_i_sum[2] += IMU_IIR_ki*ez*dt;

	gyrox += IMU_IIR_kp*ex + iir_i_sum[0];
	gyroy += IMU_IIR_kp*ey + iir_i_sum[1];
	gyroz += IMU_IIR_kp*ez + iir_i_sum[2];

	//5.更新q，一阶毕卡近似
	gyrox *= (0.5f*dt);
	gyroy *= (0.5f*dt);
	gyroz *= (0.5f*dt);

	float buffer[4] = {F_S.s_q[0],F_S.s_q[1],F_S.s_q[2],F_S.s_q[3]};
	F_S.s_q[0] += (-buffer[1] * gyrox - buffer[2] * gyroy - buffer[3] * gyroz);
	F_S.s_q[1] += (+buffer[0] * gyrox + buffer[2] * gyroz - buffer[3] * gyroy);
	F_S.s_q[2] += (+buffer[0] * gyroy - buffer[1] * gyroz + buffer[3] * gyrox);
	F_S.s_q[3] += (+buffer[0] * gyroz + buffer[1] * gyroy - buffer[2] * gyrox);

	//6.归一化四元数,计算R中需要用到的
	float q_norm_inv = 1.0f/sqrt(F_S.s_q[0]*F_S.s_q[0] + F_S.s_q[1]*F_S.s_q[1] + F_S.s_q[2]*F_S.s_q[2] + F_S.s_q[3]*F_S.s_q[3]);
	F_S.s_q[0] *= q_norm_inv;
	F_S.s_q[1] *= q_norm_inv;
	F_S.s_q[2] *= q_norm_inv;
	F_S.s_q[3] *= q_norm_inv;

	gx_BF = 2*(F_S.s_q[1]*F_S.s_q[3] - F_S.s_q[0]*F_S.s_q[2]);
	gy_BF = 2*(F_S.s_q[2]*F_S.s_q[3] + F_S.s_q[0]*F_S.s_q[1]);
	gz_BF = 1 - 2*(F_S.s_q[1]*F_S.s_q[1] + F_S.s_q[2]*F_S.s_q[2]);


	//7.计算rpy,右手系

	F_S.roll = -atan2_approx(gx_BF,gz_BF) * RAD2DEG;
	F_S.pitch = -((0.5f * M_PIf) - acos_approx(-gy_BF)) * RAD2DEG;
//	F_S.yaw = atan2_approx(rMat[1][0], rMat[0][0]);
	F_S.yaw = -atan2_approx(2*(F_S.s_q[1]*F_S.s_q[2] - F_S.s_q[0]*F_S.s_q[3]),1 - 2*(F_S.s_q[3]*F_S.s_q[3] + F_S.s_q[2]*F_S.s_q[2]))* RAD2DEG;

	//右手系，四元数得旋转矩阵的逆，因为要求世界系的单位g在惯性系中的坐标
//    rMat[0][0] = 1.0f - 2.0f * q2q2 - 2.0f * q3q3;
//    rMat[0][1] = 2.0f * (q1q2 + -q0q3);
//    rMat[0][2] = 2.0f * (q1q3 - -q0q2);
//
//    rMat[1][0] = 2.0f * (q1q2 - -q0q3);
//    rMat[1][1] = 1.0f - 2.0f * q1q1 - 2.0f * q3q3;
//    rMat[1][2] = 2.0f * (q2q3 + -q0q1);
//
//    rMat[2][0] = 2.0f * (q1q3 + -q0q2);
//    rMat[2][1] = 2.0f * (q2q3 - -q0q1);
//    rMat[2][2] = 1.0f - 2.0f * q1q1 - 2.0f * q2q2;

}
