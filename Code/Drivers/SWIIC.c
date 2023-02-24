/*
 * @Author: IlleniumDillon 147900130@qq.com
 * @Date: 2022-11-02 15:03:25
 * @LastEditors: IlleniumDillon 147900130@qq.com
 * @LastEditTime: 2022-11-02 17:14:41
 * @FilePath: \CODE\Peripheral\SWIIC\SWIIC.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "SWIIC.h"

static void _delay(uint32_t time);

static void _set_high(GPIO_TypeDef* port, uint16_t pin);
static void _set_low(GPIO_TypeDef* port, uint16_t pin);
static void _set_input(GPIO_TypeDef* port, uint16_t pin);
static void _set_output(GPIO_TypeDef* port, uint16_t pin);
static uint8_t _get_input(GPIO_TypeDef* port, uint16_t pin);

static void _start(swiic_dev* dev);
static void _stop(swiic_dev* dev);
static void _send_ack(swiic_dev* dev, swiic_ack ack);
static swiic_ack _get_ack(swiic_dev* dev);

static swiic_ack _send_byte(swiic_dev* dev, uint8_t byte);
static uint8_t _read_byte(swiic_dev* dev, swiic_ack ack);

static swiic_ack _IIC_write(swiic_dev* dev, uint8_t* pdata, size_t size);
static swiic_ack _IIC_read(swiic_dev* dev, uint8_t* pdata, size_t size);

swiic_dev SWIICInit(GPIO_TypeDef* scl_port, uint16_t scl_pin, GPIO_TypeDef* sda_port, uint16_t sda_pin,uint32_t baudrate,unsigned int SystemCoreClock)
{
    swiic_dev dev;
    dev.delaytime = SystemCoreClock/(baudrate*2);
//    dev.delaytime = 1;
    dev.scl_port = scl_port;
    dev.scl_pin = scl_pin;
    dev.sda_port = sda_port;
	dev.sda_pin = sda_pin;

    dev.slaveaddr = 0xfe;
    _set_output(dev.scl_port,dev.scl_pin);
    _set_output(dev.sda_port,dev.sda_pin);
    _set_high(dev.scl_port,dev.scl_pin);
    _set_high(dev.sda_port,dev.sda_pin);
    return dev;
}

uint8_t SWIICWriteReg(swiic_dev* dev, uint8_t slaveaddr, uint16_t regaddr, uint8_t* pdata, size_t size)
{
	dev->slaveaddr = slaveaddr;
    _start(dev);
    if(_send_byte(dev,slaveaddr)==NACK)
    {
        _stop(dev);
        return 0;
    }
    if(_send_byte(dev,regaddr>>8)==NACK)
    {
        _stop(dev);
        return 0;
    }
    if(_send_byte(dev,regaddr & 0x00ff)==NACK)
    {
        _stop(dev);
        return 0;
    }

    for(size_t i = 0; i < size ; ++i)
    {
        _send_byte(dev,pdata[i]);
    }
    _stop(dev);

	return size;
}

uint8_t SWIICReadReg(swiic_dev* dev, uint8_t slaveaddr, uint16_t regaddr, uint8_t* pdata, size_t size)
{
	dev->slaveaddr = slaveaddr;
	_start(dev);
	if(_send_byte(dev,slaveaddr)==NACK)
	{
		_stop(dev);
		return 0;
	}
	if(_send_byte(dev,regaddr>>8)==NACK)
	{
		_stop(dev);
		return 0;
	}
	if(_send_byte(dev,regaddr & 0x00ff)==NACK)
	{
		_stop(dev);
		return 0;
	}

	_start(dev);
	if(_send_byte(dev,(slaveaddr|1))==NACK)
	{
		_stop(dev);
		return 0;
	}
	for(size_t i = 0; i < size-1 ; i++)
	{
		pdata[i] = _read_byte(dev,ACK);
	}
	pdata[size-1] = _read_byte(dev,NACK);
	_stop(dev);
	return size;
}

void SWIICWrite(swiic_dev* dev, uint8_t slaveaddr, uint8_t* pdata, size_t size)
{
    dev->slaveaddr = slaveaddr;
    _IIC_write(dev,pdata,size);
}

void SWIICRead(swiic_dev* dev, uint8_t slaveaddr, uint8_t* pdata, size_t size)
{
    dev->slaveaddr = slaveaddr;
    _IIC_read(dev,pdata,size);
}

static void _delay(uint32_t time)
{
    while(time--);
}

static void _set_high(GPIO_TypeDef* port, uint16_t pin)
{
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}

static void _set_low(GPIO_TypeDef* port, uint16_t pin)
{
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}

static void _set_input(GPIO_TypeDef* port, uint16_t pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
}

static void _set_output(GPIO_TypeDef* port, uint16_t pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
}

static uint8_t _get_input(GPIO_TypeDef* port, uint16_t pin)
{
    return HAL_GPIO_ReadPin(port,pin);
}


static void _start(swiic_dev* dev)
{
    _set_high(dev->scl_port,dev->scl_pin);
    _set_high(dev->sda_port,dev->sda_pin);
    _delay(dev->delaytime);
    _set_low(dev->sda_port,dev->sda_pin);
    _delay(dev->delaytime);
    _set_low(dev->scl_port,dev->scl_pin);
    _delay(dev->delaytime);
}

static void _stop(swiic_dev* dev)
{
    _set_low(dev->scl_port,dev->scl_pin);
    _set_low(dev->sda_port,dev->sda_pin);
    _delay(dev->delaytime);
    _set_high(dev->scl_port,dev->scl_pin);
    _delay(dev->delaytime);
    _set_high(dev->sda_port,dev->sda_pin);
}

static void _send_ack(swiic_dev* dev, swiic_ack ack)
{
    _set_low(dev->scl_port,dev->scl_pin);
    _delay(dev->delaytime);
    if(ack==ACK)_set_low(dev->sda_port,dev->sda_pin);
    else _set_high(dev->sda_port,dev->sda_pin);
    _set_high(dev->scl_port,dev->scl_pin);
    _delay(dev->delaytime);
    _set_low(dev->scl_port,dev->scl_pin);
    _delay(dev->delaytime);
}

static swiic_ack _get_ack(swiic_dev* dev)
{
    //_set_low(dev->scl_port,dev->scl_pin);
    _set_input(dev->sda_port,dev->sda_pin);
    //_delay(dev->delaytime);
    _set_high(dev->scl_port,dev->scl_pin);
    _delay(dev->delaytime);
    if(_get_input(dev->sda_port,dev->sda_pin))
    {
        _set_output(dev->sda_port,dev->sda_pin);
        _set_low(dev->scl_port,dev->scl_pin);
        _delay(dev->delaytime);
        return NACK;
    }
    else
    {
        _set_output(dev->sda_port,dev->sda_pin);
        _set_low(dev->scl_port,dev->scl_pin);
        _delay(dev->delaytime);
        return ACK;
    }
}

static swiic_ack _send_byte(swiic_dev* dev, uint8_t byte)
{
    for(uint8_t i = 0; i < 8 ; i++)
    {
        if(byte & 0x80) _set_high(dev->sda_port,dev->sda_pin);
        else _set_low(dev->sda_port,dev->sda_pin);
        byte <<= 1;
        //_delay(dev->delaytime);
        _set_high(dev->scl_port,dev->scl_pin);
        _delay(dev->delaytime);
        _set_low(dev->scl_port,dev->scl_pin);
        _delay(dev->delaytime);
    }
    return _get_ack(dev);
}

static uint8_t _read_byte(swiic_dev* dev, swiic_ack ack)
{
    uint8_t byte = 0;
    //_set_high(dev->scl_port,dev->scl_pin);
    _set_input(dev->sda_port,dev->sda_pin);
    for(uint8_t i = 0; i < 8 ; i++)
    {
        byte<<=1;
        _set_high(dev->scl_port,dev->scl_pin);
        _delay(dev->delaytime);
        if(_get_input(dev->sda_port,dev->sda_pin))byte+=1;
        if(i==7)break;
        _set_low(dev->scl_port,dev->scl_pin);
        _delay(dev->delaytime);
    }
    _set_output(dev->sda_port,dev->sda_pin);
    _send_ack(dev,ack);
    return byte;
}

static swiic_ack _IIC_write(swiic_dev* dev, uint8_t* pdata, size_t size)
{
    _start(dev);
    if(_send_byte(dev,(dev->slaveaddr<<1)|0x00)==NACK)
    {
        _stop(dev);
        return NACK;
    }
    for(size_t i = 0; i < size ; i++)
    {
        _send_byte(dev,pdata[i]);
    }
    _stop(dev);
    return ACK;
}

static swiic_ack _IIC_read(swiic_dev* dev, uint8_t* pdata, size_t size)
{
    _start(dev);
    if(_send_byte(dev,(dev->slaveaddr<<1)|0x01)==NACK)
    {
        _stop(dev);
        return NACK;
    }
    for(size_t i = 0; i < size-1 ; i++)
    {
        pdata[i] = _read_byte(dev,ACK);
    }
    pdata[size-1] = _read_byte(dev,NACK);
    _stop(dev);
    return ACK;
}
