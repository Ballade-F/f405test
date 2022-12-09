/*
 * common.h
 *
 *  Created on: 2022年11月28日
 *      Author: Ballade_F
 */

#ifndef DRIVERS_COMMON_H_
#define DRIVERS_COMMON_H_
#include "gpio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "i2c.h"

void Common_Init(void);
void delay_init(void);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);
uint32_t getTime_us(void);

#endif /* DRIVERS_COMMON_H_ */
