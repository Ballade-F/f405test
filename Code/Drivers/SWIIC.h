/*
 * @Author: IlleniumDillon 147900130@qq.com
 * @Date: 2022-11-02 15:03:20
 * @LastEditors: IlleniumDillon 147900130@qq.com
 * @LastEditTime: 2022-11-02 15:36:05
 * @FilePath: \CODE\Peripheral\SWIIC\SWIIC.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _SWIIC_H_
#define _SWIIC_H_

#include "common.h"

typedef enum
{
    ACK,
    NACK,
}swiic_ack;

typedef struct
{
	GPIO_TypeDef* 		scl_port;
	uint16_t			scl_pin;
	GPIO_TypeDef* 		sda_port;
	uint16_t			sda_pin;

    uint32_t delaytime;
    uint8_t slaveaddr;
}swiic_dev;

/** \brief IIC初始化
 * \param dev IIC设备配置结构体指针
 * \param scl 时钟引脚
 * \param sda 数据引脚
 * \param baudrate 波特率
 * \return 返回HSIC_OK表明初始化成功
 *
 * Coding example:
 * \code
 *    Hsic_swIICInit(&dev,IfxPort_P02_5,IfxPort_P02_4,1000);
 * \endcode
 *
 */
swiic_dev SWIICInit(GPIO_TypeDef* scl_port, uint16_t scl_pin, GPIO_TypeDef* sda_port, uint16_t sda_pin,uint32_t baudrate,unsigned int SystemCoreClock);

/** \brief IIC写
 * \param dev IIC设备配置结构体指针
 * \param slaveaddr 设备地址
 * \param pdata 数据指针
 * \para size 数据量
 * \return void
 *
 * Coding example:
 * \code
 *    Hsic_swIICWrite(&dev,0x7e,&i,1);
 * \endcode
 *
 */
void SWIICWrite(swiic_dev* dev, uint8_t slaveaddr, uint8_t* pdata, size_t size);

/** \brief IIC读
 * \param dev IIC设备配置结构体指针
 * \param slaveaddr 设备地址
 * \param pdata 数据指针
 * \param size 数据量
 * \return void
 *
 * Coding example:
 * \code
 *    Hsic_swIICRead(&dev,0x7e,&i,1);
 * \endcode
 *
 */
void SWIICRead(swiic_dev* dev, uint8_t slaveaddr, uint8_t* pdata, size_t size);

uint8_t SWIICWriteReg(swiic_dev* dev, uint8_t slaveaddr, uint16_t regaddr, uint8_t* pdata, size_t size);

uint8_t SWIICReadReg(swiic_dev* dev, uint8_t slaveaddr, uint16_t regaddr, uint8_t* pdata, size_t size);

#endif
