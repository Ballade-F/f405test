/*
 * ctrl_task_list.h
 *
 *  Created on: 2022年12月13日
 *      Author: Ballade_F
 */

#ifndef CTRL_CTRL_TASK_LIST_H_
#define CTRL_CTRL_TASK_LIST_H_

#include <pos_ctrl.h>
#include <uart_communication.h>
#include "common.h"
#include "mpu6000.h"
#include "imu.h"
#include "motor.h"
#include "state.h"
#include "position.h"

void Ctrl_Init(void);
void Ctrl_Task(void);

#endif /* CTRL_CTRL_TASK_LIST_H_ */
