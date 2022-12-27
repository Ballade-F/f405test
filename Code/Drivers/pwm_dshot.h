/*
 * pwm_dshot.h
 *
 *  Created on: 2022年11月30日
 *      Author: Ballade_F
 */

#ifndef DRIVERS_PWM_DSHOT_H_
#define DRIVERS_PWM_DSHOT_H_

#include "common.h"
#include "string.h"

#define ESC_BIT_0     26
#define ESC_BIT_1     52
#define ESC_CMD_BUF_LEN 18

extern uint32_t PwmDshot_M1[ESC_CMD_BUF_LEN];
extern uint32_t PwmDshot_M2[ESC_CMD_BUF_LEN];
extern uint32_t PwmDshot_M3[ESC_CMD_BUF_LEN];
extern uint32_t PwmDshot_M4[ESC_CMD_BUF_LEN];
extern uint32_t PwmDshot_M[ESC_CMD_BUF_LEN*4];

void pwmWriteDigital(uint32_t *esc_cmd, uint16_t value);
void pwmDataMerge(void);
void Dshot_BurstWrite(uint32_t *esc_cmd,uint16_t value_m1,uint16_t value_m2,uint16_t value_m3,uint16_t value_m4);

#endif /* DRIVERS_PWM_DSHOT_H_ */
