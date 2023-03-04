/*
 * mtf01.h
 *
 *  Created on: 2023年2月28日
 *      Author: Ballade_F
 */

#ifndef DRIVERS_MTF01_H_
#define DRIVERS_MTF01_H_


#include "common.h"
#include "string.h"

#define MICOLINK_MSG_HEAD            0xEF
#define MICOLINK_MSG_DEV_ID          0x0F
#define MICOLINK_MAX_PAYLOAD_LEN     0x14
#define MICOLINK_MAX_LEN             (MICOLINK_MAX_PAYLOAD_LEN + 7)

enum
{
    MICOLINK_MSG_ID_RANGE_SENSOR = 0x51,     //测距传感器
};


//按一字节对齐
#pragma pack(1)

typedef struct
{
    uint8_t head;
    uint8_t dev_id;
    uint8_t sys_id;
    uint8_t msg_id;
    uint8_t seq;
    uint8_t len;
    uint8_t payload[MICOLINK_MAX_PAYLOAD_LEN];
    uint8_t checksum;

    uint8_t status;
    uint8_t payload_cnt;
} MICOLINK_MSG_t;



#pragma pack()
//取消对齐设置，按默认对齐

typedef struct
{
	UART_HandleTypeDef *uart_ch;
}MTF01_Handle_t;


#pragma pack(1)
typedef struct
{
	uint32_t	timestamp;
	uint32_t	distance;
	uint8_t		tof_strength;
	uint8_t		tof_precision;
	uint8_t		tof_status;
	uint8_t		reserved;
	int16_t		flow_x;
	int16_t		flow_y;
	uint8_t		flow_quality;
	uint8_t		flow_status;
	uint16_t	reserved2;

}MTF01_DATA_t;
#pragma pack()

extern MTF01_DATA_t mtf01_data;
extern MTF01_Handle_t mtf01_dev;


uint8_t micolink_parse_char(MICOLINK_MSG_t* msg, uint8_t data);
void mtf01_Init(void);
void mtf01_DataProcess(void);

#endif /* DRIVERS_MTF01_H_ */
