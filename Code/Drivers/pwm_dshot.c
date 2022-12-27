/*
 * pwm_dshot.c
 *
 *  Created on: 2022年11月30日
 *      Author: Ballade_F
 */

#include "pwm_dshot.h"


uint32_t PwmDshot_M1[ESC_CMD_BUF_LEN]={0};
uint32_t PwmDshot_M2[ESC_CMD_BUF_LEN]={0};
uint32_t PwmDshot_M3[ESC_CMD_BUF_LEN]={0};
uint32_t PwmDshot_M4[ESC_CMD_BUF_LEN]={0};
uint32_t PwmDshot_M[ESC_CMD_BUF_LEN*4] = {0};

static uint16_t prepareDshotPacket(const uint16_t value, int8_t requestTelemetry)
{
    // 油门大小为11位  所以这里先左移一位 添加上请求回传标志共12位
    uint16_t packet = (value << 1) | (requestTelemetry ? 1 : 0);

    // 将12位数据分为3组 每组4位, 进行异或
    // compute checksum
    int csum = 0;
    int csum_data = packet;
    for (int i = 0; i < 3; i++) {
        csum ^=  csum_data;   // xor data by nibbles
        csum_data >>= 4;
    }
    //取最后四位 其他的不要
    csum &= 0xf;
    // append checksum 将CRC添加到后四位
    packet = (packet << 4) | csum;
    return packet;
}

void pwmDataMerge(void)
{
	memcpy(PwmDshot_M,PwmDshot_M1,sizeof(PwmDshot_M1));
	memcpy((PwmDshot_M+ESC_CMD_BUF_LEN*1),PwmDshot_M2,sizeof(PwmDshot_M2));
	memcpy((PwmDshot_M+ESC_CMD_BUF_LEN*2),PwmDshot_M3,sizeof(PwmDshot_M3));
	memcpy((PwmDshot_M+ESC_CMD_BUF_LEN*3),PwmDshot_M4,sizeof(PwmDshot_M4));

}

 void pwmWriteDigital(uint32_t *esc_cmd, uint16_t value)
{
	value = ( (value > 2047) ? 2047 : value );
	value = prepareDshotPacket(value, 0);
    esc_cmd[0]  = (value & 0x8000) ? ESC_BIT_1 : ESC_BIT_0;
    esc_cmd[1]  = (value & 0x4000) ? ESC_BIT_1 : ESC_BIT_0;
    esc_cmd[2]  = (value & 0x2000) ? ESC_BIT_1 : ESC_BIT_0;
    esc_cmd[3]  = (value & 0x1000) ? ESC_BIT_1 : ESC_BIT_0;
    esc_cmd[4]  = (value & 0x0800) ? ESC_BIT_1 : ESC_BIT_0;
    esc_cmd[5]  = (value & 0x0400) ? ESC_BIT_1 : ESC_BIT_0;
    esc_cmd[6]  = (value & 0x0200) ? ESC_BIT_1 : ESC_BIT_0;
    esc_cmd[7]  = (value & 0x0100) ? ESC_BIT_1 : ESC_BIT_0;
    esc_cmd[8]  = (value & 0x0080) ? ESC_BIT_1 : ESC_BIT_0;
    esc_cmd[9]  = (value & 0x0040) ? ESC_BIT_1 : ESC_BIT_0;
    esc_cmd[10] = (value & 0x0020) ? ESC_BIT_1 : ESC_BIT_0;
    esc_cmd[11] = (value & 0x0010) ? ESC_BIT_1 : ESC_BIT_0;
    esc_cmd[12] = (value & 0x8) ? ESC_BIT_1 : ESC_BIT_0;
    esc_cmd[13] = (value & 0x4) ? ESC_BIT_1 : ESC_BIT_0;
    esc_cmd[14] = (value & 0x2) ? ESC_BIT_1 : ESC_BIT_0;
    esc_cmd[15] = (value & 0x1) ? ESC_BIT_1 : ESC_BIT_0;

}

void Dshot_BurstWrite(uint32_t *esc_cmd,uint16_t value_m1,uint16_t value_m2,uint16_t value_m3,uint16_t value_m4)
{
	value_m1 = ( (value_m1 > 2047) ? 2047 : value_m1 );
	value_m2 = ( (value_m2 > 2047) ? 2047 : value_m2 );
	value_m3 = ( (value_m3 > 2047) ? 2047 : value_m3 );
	value_m4 = ( (value_m4 > 2047) ? 2047 : value_m4 );
	value_m1 = prepareDshotPacket(value_m1, 0);
	value_m2 = prepareDshotPacket(value_m2, 0);
	value_m3 = prepareDshotPacket(value_m3, 0);
	value_m4 = prepareDshotPacket(value_m4, 0);

	for(uint8_t i = 0;i<16;++i)
	{
		esc_cmd[4*i] = (value_m1 & (0x0001 << (15-i))) ? ESC_BIT_1 : ESC_BIT_0;
		esc_cmd[4*i+1] = (value_m2 & (0x0001 << (15-i))) ? ESC_BIT_1 : ESC_BIT_0;
		esc_cmd[4*i+2] = (value_m3 & (0x0001 << (15-i))) ? ESC_BIT_1 : ESC_BIT_0;
		esc_cmd[4*i+3] = (value_m4 & (0x0001 << (15-i))) ? ESC_BIT_1 : ESC_BIT_0;
	}
}
