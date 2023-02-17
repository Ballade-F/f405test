/*
 * debug.h
 *
 *  Created on: Nov 28, 2022
 *      Author: Ballade_F
 */

#ifndef CTRL_UART_COMMUNICATION_H_
#define CTRL_UART_COMMUNICATION_H_


#include "common.h"
#include "state.h"

#define SERIAL_DATA_LEN      (5)

typedef struct uart_com_handle
{
	UART_HandleTypeDef *uart_ch;

}Uart_Com_Handle_t;

typedef enum
{
	Fly_Start								= 1,
	Param_Thrust							= 2,
}command_e;


extern float test_time;
extern uint8_t test_u8;
extern uint8_t CommandRec[SERIAL_DATA_LEN];
extern Uart_Com_Handle_t uart_communication;


void Communication_Init(void);
void Debug_VarUpload(float *my_var, uint8_t var_num);
void Communication_Receive(void);


#endif /* CTRL_UART_COMMUNICATION_H_ */
