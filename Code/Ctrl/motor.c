/*
 * motor.c
 *
 *  Created on: 2022年12月11日
 *      Author: Ballade_F
 */


#include "motor.h"

Motor_State_t Motor_State;



/*
 *  ->        <-
 *  M3	 y	  M1
 *
 *  	 z	  x
 *
 *  <-		  ->
 *  M2		  M4

 */

void Motor_Update(void)
{
//	motorPWM.m1 = thrust -r+p-y;
//	motorPWM.m2 = thrust +r-p-y;
//	motorPWM.m3 = thrust +r+p+y;
//	motorPWM.m4 = thrust -r-p+y;
}
