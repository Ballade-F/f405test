/*
 * debug.c
 *
 *  Created on: Nov 28, 2022
 *      Author: Ballade_F
 */


#include <uart_communication.h>

float test_time;

uint8_t test_u8;
Uart_Com_Handle_t uart_communication;

uint8_t uartRec = 0;
uint8_t recNum = 0;
uint8_t CommandRec[SERIAL_DATA_LEN] = {0};

void Communication_Init(void)
{
	uart_communication.uart_ch = &huart1;
	HAL_UART_Receive_IT(uart_communication.uart_ch, (uint8_t *)&uartRec,1);
}


void communication_dataProcess(void)
{

	uint32_t *commandData_ptr = &(CommandRec[1]);
	switch(CommandRec[0])
	{
	case Fly_Start:
		if(*commandData_ptr == ARMED)
		{
			State_Arm();
		}
		else
		{
			State_DisArm();
		}
		break;
	case Param_Thrust:


		break;
	default:
		break;

	}



}


void Communication_Receive(void)
{
	if(recNum > 0 )
	{
		if(recNum == SERIAL_DATA_LEN +1)		//读完帧头和数据
		{
			if(uartRec == 0xA5)
			{
				recNum++;
			}
			else								//帧尾异常
			{
				recNum = 0;
			}
		}
		else									//读数据
		{
			CommandRec[recNum-1] = uartRec;
			recNum++;
		}
	}

	else
	{
		if(uartRec == 0x5A)
		{
			recNum = 1;
		}
		else
		{
			recNum = 0;
		}
	}

	if(recNum == 2+SERIAL_DATA_LEN)				//读完帧头+数据+帧尾
	{
		communication_dataProcess();
		recNum = 0;
	}

	HAL_UART_Receive_IT(uart_communication.uart_ch, (uint8_t *)&uartRec,1);
}


void Debug_VarUpload(float *my_var, uint8_t var_num)
{
	//for ming you ke chuang
	uint8_t data_ptr[256] = {0};
	//head
	data_ptr[0] = 0x55;
	data_ptr[1] = 0xaa;
	data_ptr[2] = 0x11;
	//show graph head
	data_ptr[3] = 0x55;
	data_ptr[4] = 0xaa;
	data_ptr[5] = 0x11;
	data_ptr[6] = 0x55;
	data_ptr[7] = 0xaa;
	data_ptr[8] = 0xff;
	data_ptr[9] = 0x01;

	//dataNum
	data_ptr[10] = var_num;
	//data
	uint8_t* temp_ptr = (uint8_t*)my_var;

	for(uint8_t i = 0;i<var_num;++i)
	{
		data_ptr[11+i*4] = temp_ptr[i*4];
		data_ptr[11+i*4+1] = temp_ptr[i*4+1];
		data_ptr[11+i*4+2] = temp_ptr[i*4+2];
		data_ptr[11+i*4+3] = temp_ptr[i*4+3];

	}

	//end
	data_ptr[11+var_num*4] = 0x01;



	HAL_UART_Transmit(uart_communication.uart_ch, data_ptr,12+var_num*4, 1000);



}


void Communication_Recieve(uint8_t *my_var, uint8_t var_num,UART_HandleTypeDef *huart_ptr)
{
	HAL_UART_Receive(huart_ptr, my_var, var_num, 1);
}
