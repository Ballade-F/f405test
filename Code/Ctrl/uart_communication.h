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
#include "motor.h"
#include "string.h"

#define SERIAL_DATA_LEN      (5)
#define COMMAND_MSG_HEAD	 (0X5A)
#define COMMAND_MSG_TAIL	 (0XA5)

typedef struct uart_com_handle
{
	UART_HandleTypeDef *uart_ch;

}Uart_Com_Handle_t;

typedef enum
{
	Fly_Start								= 2,
	Param_Thrust							= 3,
}command_e;

typedef enum
{
	Header									= 0,
	Load									= 1,
	Tail									= 2,
}uart_status_e;

typedef struct
{
	uint8_t payload[SERIAL_DATA_LEN];
	uart_status_e status;
	uint8_t payload_cnt;
}COMMAND_MSG_t;

extern float test_time;
extern uint8_t test_u8;
extern uint8_t CommandRec[SERIAL_DATA_LEN];
extern Uart_Com_Handle_t uart_communication;


void Communication_Init(void);
void Debug_VarUpload(float *my_var, uint8_t var_num);
void Communication_Receive(void);


#endif /* CTRL_UART_COMMUNICATION_H_ */
