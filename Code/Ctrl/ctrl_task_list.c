/*
 * ctrl_task_list.c
 *
 *  Created on: 2022年12月13日
 *      Author: Ballade_F
 */


#include "ctrl_task_list.h"

void Ctrl_Init(void)
{
	Common_Init();
	Angle_Init();
	//BMP280_Init();

	PosCtrl_Init();
	Motor_Init();
}

void Ctrl_Task(void)
{
	if(State_IsArmed())
	{
		//1.陀螺仪加速度计获取数据
		MPU6000_Data_UpDate();

		//2.姿态解算
		Angle_Update(IMU_UserData.gyro_x,IMU_UserData.gyro_y,IMU_UserData.gyro_z,
					 IMU_UserData.acc_x, IMU_UserData.acc_y, IMU_UserData.acc_z, 0.002);

		//3.PID计算
		PosCtrl_Update();

		//4.motor输出
		Motor_Update();
	}
	else
	{
		Motor_Stop();
	}
}
