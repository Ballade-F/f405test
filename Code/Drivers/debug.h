/*
 * debug.h
 *
 *  Created on: Nov 28, 2022
 *      Author: Ballade_F
 */

#ifndef DRIVERS_DEBUG_H_
#define DRIVERS_DEBUG_H_


#include "common.h"

extern float test_time;

void Debug_VarUpload(float *my_var, uint8_t var_num,UART_HandleTypeDef *huart_ptr);


#endif /* DRIVERS_DEBUG_H_ */
