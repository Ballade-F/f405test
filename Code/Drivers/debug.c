/*
 * debug.c
 *
 *  Created on: Nov 28, 2022
 *      Author: Ballade_F
 */


#include "debug.h"

float test_time;




void Debug_VarUpload(float *my_var, uint8_t var_num,UART_HandleTypeDef *huart_ptr)
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



	HAL_UART_Transmit(huart_ptr, data_ptr,12+var_num*4, 1000);



}
