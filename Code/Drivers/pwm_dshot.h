/*
 * pwm_dshot.h
 *
 *  Created on: 2022年11月30日
 *      Author: Ballade_F
 */

#ifndef DRIVERS_PWM_DSHOT_H_
#define DRIVERS_PWM_DSHOT_H_

#include "common.h"

#define ESC_BIT_0     26
#define ESC_BIT_1     52
#define ESC_CMD_BUF_LEN 18

extern uint16_t ESC_CMD[ESC_CMD_BUF_LEN];

void pwmWriteDigital(uint16_t *esc_cmd, uint16_t value);



#endif /* DRIVERS_PWM_DSHOT_H_ */
