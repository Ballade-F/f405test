/*
 * @Author: IlleniumDillon 147900130@qq.com
 * @Date: 2022-11-02 15:03:04
 * @LastEditors: IlleniumDillon 147900130@qq.com
 * @LastEditTime: 2022-11-02 16:47:40
 * @FilePath: \CODE\Peripheral\SWSPI\SWSPI.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _SWSPI_H_
#define _SWSPI_H_

#include "common.h"

/*
 * \brief 确定极性与相位的函数指针
 */
typedef void (*sclk_active_t)(GPIO_TypeDef* port, uint16_t pin);
typedef void (*sclk_idle_t)(GPIO_TypeDef* port, uint16_t pin);
typedef void (*sclk_effectiveedge_t)(GPIO_TypeDef* port, uint16_t pin, uint32_t delaytime);
typedef void (*sclk_ineffectiveedge_t)(GPIO_TypeDef* port, uint16_t pin, uint32_t delaytime);

/*
 * \brief SPI设备结构体
 */
typedef struct
{
	GPIO_TypeDef* 		mosi_port;
	uint16_t			mosi_pin;
	GPIO_TypeDef* 		miso_port;
	uint16_t			miso_pin;
	GPIO_TypeDef* 		sclk_port;
	uint16_t			sclk_pin;
	GPIO_TypeDef* 		cs_port;
	uint16_t			cs_pin;

    uint32_t delaytime;
    sclk_active_t sclk_active;
    sclk_idle_t sclk_idle;
    sclk_effectiveedge_t sclk_effectiveedge;
    sclk_ineffectiveedge_t sclk_ineffectiveedge;
}swspi_dev;

/** \brief SPI设备初始化
 * \param miso 主机输入从机输出
 * \param mosi 主机输出从机输入
 * \param sclk 主机时钟输出
 * \param cs 主机片选输出
 * \param baudrate 波特率
 * \param mode 极性与相位
 * \return hsic_swspi_dev_t SPI设备结构体
 *
 * Coding example:
 * \code
 *    hsic_swspi_dev_t dev = Hsic_swSPIInit(IfxPort_P20_12,IfxPort_P20_14,IfxPort_P20_11,IfxPort_P20_8,1000000,0);
 * \endcode
 *
 */
swspi_dev SWSPIInit(GPIO_TypeDef* miso_port, GPIO_TypeDef* mosi_port, GPIO_TypeDef* sclk_port, GPIO_TypeDef* cs_port,
					uint16_t miso_pin	   , uint16_t mosi_pin	    , uint16_t sclk_pin	     , uint16_t cs_pin	    ,
        			uint32_t baudrate	   , unsigned int SystemCoreClock, uint8_t mode);

/** \brief SPI数据传输
 * \param dev SPI设备配置结构体指针
 * \param ptxdata 发送的数据
 * \param prxdata 接收的数据
 * \param count 数据量
 * \return void
 *
 * Coding example:
 * \code
 *    Hsic_swSPIExchange(&dev,&i[0],&j[0],5);
 * \endcode
 *
 */
void SWSPIExchange(swspi_dev* dev, uint8_t* ptxdata, uint8_t* prxdata, size_t count);
uint8_t SWSPIexchange_byte(swspi_dev* dev, uint8_t byte);
void SWSPI_start(swspi_dev* dev);
void SWSPI_stop(swspi_dev* dev);
#endif
