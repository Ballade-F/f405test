/*
 * state.h
 *
 *  Created on: 2022年12月21日
 *      Author: Ballade_F
 */

#ifndef CTRL_STATE_H_
#define CTRL_STATE_H_

#include "common.h"


typedef enum
{
	ARMED								= (1 << 1),

}flightState_e;

extern uint32_t FlightState;
extern uint32_t FlightState_Last;

extern uint8_t CommandRec;

extern uint32_t state_test;

#define State_IsArmed()					(FlightState & (ARMED))
#define State_Arm()						(FlightState |= (ARMED))
#define State_DisArm()					(FlightState &= ~(ARMED))

#define StateLast_IsArmed()				(FlightState_Last & (ARMED))

void StateLast_Update(void);

#endif /* CTRL_STATE_H_ */
