/*
 * state.c
 *
 *  Created on: 2022年12月21日
 *      Author: Ballade_F
 */


#include "state.h"

uint32_t FlightState = 0;
uint32_t FlightState_Last = 0;


//测试wifi解锁
uint32_t state_test = 0;

void StateLast_Update(void)
{
	FlightState_Last = FlightState;
}
