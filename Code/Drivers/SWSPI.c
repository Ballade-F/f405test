/*
 * @Author: IlleniumDillon 147900130@qq.com
 * @Date: 2022-11-02 15:03:11
 * @LastEditors: IlleniumDillon 147900130@qq.com
 * @LastEditTime: 2022-11-02 17:14:25
 * @FilePath: \CODE\Peripheral\SWSPI\SWSPI.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "SWSPI.H"

static void _delay(uint32_t time);

static void _set_high(GPIO_TypeDef* port, uint16_t pin);
static void _set_low(GPIO_TypeDef* port, uint16_t pin);

static uint8_t _get_input(GPIO_TypeDef* port, uint16_t pin);

static void _sclk_leading(GPIO_TypeDef* port, uint16_t pin, uint32_t time);
static void _sclk_trailing(GPIO_TypeDef* port, uint16_t pin, uint32_t time);





swspi_dev SWSPIInit(GPIO_TypeDef* miso_port, GPIO_TypeDef* mosi_port, GPIO_TypeDef* sclk_port, GPIO_TypeDef* cs_port,
					uint16_t miso_pin	   , uint16_t mosi_pin	    , uint16_t sclk_pin	     , uint16_t cs_pin	    ,
        			uint32_t baudrate	   , unsigned int SystemCoreClock, uint8_t mode)
{
    swspi_dev dev;
    dev.cs_port = cs_port;
	dev.miso_port = miso_port;
	dev.mosi_port = mosi_port;
	dev.sclk_port = sclk_port;

	dev.cs_pin = cs_pin;
	dev.miso_pin = miso_pin;
	dev.mosi_pin = mosi_pin;
	dev.sclk_pin = sclk_pin;

    dev.delaytime = SystemCoreClock/(baudrate*4);

    if(mode==0)
    {
        dev.sclk_active = &_set_high;
        dev.sclk_idle = &_set_low;
        dev.sclk_effectiveedge = &_sclk_leading;
        dev.sclk_ineffectiveedge = &_sclk_trailing;
    }
    else if(mode==1)
    {
        dev.sclk_active = &_set_high;
        dev.sclk_idle = &_set_low;
        dev.sclk_effectiveedge = &_sclk_trailing;
        dev.sclk_ineffectiveedge = &_sclk_leading;
    }
    else if(mode==2)
    {
        dev.sclk_active = &_set_low;
        dev.sclk_idle = &_set_high;
        dev.sclk_effectiveedge = &_sclk_trailing;
        dev.sclk_ineffectiveedge = &_sclk_leading;
    }
    else if(mode==3)
    {
        dev.sclk_active = &_set_low;
        dev.sclk_idle = &_set_high;
        dev.sclk_effectiveedge = &_sclk_leading;
        dev.sclk_ineffectiveedge = &_sclk_trailing;
    }
    dev.sclk_idle(dev.sclk_port,dev.sclk_pin);
    _set_high(dev.mosi_port,dev.mosi_pin);
    _set_high(dev.cs_port,dev.cs_pin);
    return dev;
}

void SWSPIExchange(swspi_dev* dev, uint8_t* ptxdata, uint8_t* prxdata, size_t count)
{
    SWSPI_start(dev);
    for(size_t i = 0; i < count ; i++)
    {
        prxdata[i] = SWSPIexchange_byte(dev,ptxdata[i]);
    }
    SWSPI_stop(dev);
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


static uint8_t _get_input(GPIO_TypeDef* port, uint16_t pin)
{
    return HAL_GPIO_ReadPin(port,pin);
}

static void _sclk_leading(GPIO_TypeDef* port, uint16_t pin, uint32_t time)
{
    _delay(time);
    _set_high(port,pin);
    _delay(time);
}

static void _sclk_trailing(GPIO_TypeDef* port, uint16_t pin, uint32_t time)
{
    _delay(time);
    _set_low(port,pin);
    _delay(time);
}

void SWSPI_start(swspi_dev* dev)
{
    _delay(dev->delaytime);
    _set_low(dev->cs_port,dev->cs_pin);
    _delay(dev->delaytime);
}

void SWSPI_stop(swspi_dev* dev)
{
    _delay(dev->delaytime);
    dev->sclk_idle(dev->sclk_port,dev->sclk_pin);
    _delay(dev->delaytime);
    _set_high(dev->mosi_port,dev->mosi_pin);
    _delay(dev->delaytime);
    _set_high(dev->cs_port,dev->cs_pin);
    _delay(dev->delaytime);
}

uint8_t SWSPIexchange_byte(swspi_dev* dev, uint8_t byte)
{
    uint8_t rx = 0;
    for(uint8_t i = 0 ; i < 8 ; i++)
    {
        rx<<=1;
        dev->sclk_ineffectiveedge(dev->sclk_port,dev->sclk_pin,dev->delaytime);
        if(byte & 0x80) _set_high(dev->mosi_port,dev->mosi_pin);
        else _set_low(dev->mosi_port,dev->mosi_pin);
        byte<<=1;
        dev->sclk_effectiveedge(dev->sclk_port,dev->sclk_pin,dev->delaytime);
        if(_get_input(dev->miso_port,dev->miso_pin)) rx += 1;
    }
    return rx;
}
