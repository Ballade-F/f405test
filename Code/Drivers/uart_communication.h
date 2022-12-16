/*
 * debug.h
 *
 *  Created on: Nov 28, 2022
 *      Author: Ballade_F
 */

#ifndef DRIVERS_UART_COMMUNICATION_H_
#define DRIVERS_UART_COMMUNICATION_H_


#include "common.h"

extern float test_time;
extern uint8_t test_u8;

void Debug_VarUpload(float *my_var, uint8_t var_num,UART_HandleTypeDef *huart_ptr);


#endif /* DRIVERS_UART_COMMUNICATION_H_ */
