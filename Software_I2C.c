//
// Created by proider on 25-11-2.
//

#include "Software_I2C.h"
#include "gpio.h"
#include "tim.h"
#include "ctiic.h"


void I2C_DelayUs(uint16_t us) {
     __HAL_TIM_SET_COUNTER(&htim7, 0);
     while (__HAL_TIM_GET_COUNTER(&htim7) < us);
}


void Software_I2C_Init(void) {
    GPIO_InitTypeDef gpio_init_struct;

    __HAL_RCC_GPIOB_CLK_ENABLE();   /* SCL引脚时钟使能 */
    __HAL_RCC_GPIOF_CLK_ENABLE();   /* SDA引脚时钟使能 */
      HAL_TIM_Base_Start(&htim7);
    gpio_init_struct.Pin = GPIO_PIN_0;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;             /* 开漏输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                     /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;      /* 高速 */
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);  /* 初始化SCL引脚 */
    gpio_init_struct.Pin = GPIO_PIN_11;
    HAL_GPIO_Init(GPIOF, &gpio_init_struct);  /* 初始化SDA引脚 */
    /* SDA引脚模式设置,开漏输出,上拉, 这样就不用再设置IO方向了, 开漏输出的时候(=1), 也可以读取外部信号的高低电平 */

    I2C_STOP();  /* 停止总线上所有设备 */
}

void I2C_START(void)
{
    IIC_SDA(1);
    IIC_SCL(1);
    I2C_DelayUs(2);
    IIC_SDA(0);     /* START信号: 当SCL为高时, SDA从高变成低, 表示起始信号 */
    I2C_DelayUs(2);
    IIC_SCL(0);     /* 钳住I2C总线，准备发送或接收数据 */
    I2C_DelayUs(2);
}

void I2C_STOP(void)
{
    IIC_SDA(0);     /* STOP信号: 当SCL为高时, SDA从低变成高, 表示停止信号 */
    I2C_DelayUs(2);
    IIC_SCL(1);
    I2C_DelayUs(2);
    IIC_SDA(1);     /* 发送I2C总线结束信号 */
    I2C_DelayUs(2);
}

uint8_t iic_wait_ack(void)
{
    uint8_t waittime = 0;
    uint8_t rack = 0;

    IIC_SDA(1);     /* 主机释放SDA线(此时外部器件可以拉低SDA线) */
    I2C_DelayUs(2);
    IIC_SCL(1);     /* SCL=1, 此时从机可以返回ACK */
    I2C_DelayUs(2);

    while (IIC_READ_SDA)    /* 等待应答 */
    {
        waittime++;

        if (waittime > 250)
        {
            I2C_STOP();
            rack = 1;
            break;
        }
        I2C_DelayUs(2);
    }

    IIC_SCL(0);     /* SCL=0, 结束ACK检查 */
    I2C_DelayUs(2);
    return rack;
}

void iic_ack(void)
{
    IIC_SDA(0);     /* SCL 0 -> 1 时 SDA = 0,表示应答 */
    I2C_DelayUs(2);
    IIC_SCL(1);     /* 产生一个时钟 */
    I2C_DelayUs(2);
    IIC_SCL(0);
    I2C_DelayUs(2);
    IIC_SDA(1);     /* 主机释放SDA线 */
    I2C_DelayUs(2);
}

void iic_nack(void)
{
    IIC_SDA(1);     /* SCL 0 -> 1  时 SDA = 1,表示不应答 */
    I2C_DelayUs(2);
    IIC_SCL(1);     /* 产生一个时钟 */
    I2C_DelayUs(2);
    IIC_SCL(0);
    I2C_DelayUs(2);
}

void iic_send_byte(uint8_t data)
{
    uint8_t t;

    for (t = 0; t < 8; t++)
    {
        IIC_SDA((data & 0x80) >> 7); /* 高位先发送 */
        I2C_DelayUs(2);
        IIC_SCL(1);
        I2C_DelayUs(2);
        IIC_SCL(0);
        data <<= 1;     /* 左移1位,用于下一次发送 */
    }

    IIC_SDA(1);      /* 发送完成, 主机释放SDA线 */
}

uint8_t iic_read_byte(uint8_t ack)
{
    uint8_t i, receive = 0;

    for (i = 0; i < 8; i++ )    /* 接收1个字节数据 */
    {
        receive <<= 1;          /* 高位先输出,所以先收到的数据位要左移 */
        IIC_SCL(1);
        I2C_DelayUs(2);

        if (IIC_READ_SDA)
        {
            receive++;
        }

        IIC_SCL(0);
        I2C_DelayUs(2);

    }

    if (!ack)
    {
        iic_nack();  /* 发送nACK */
    }
    else
    {
        iic_ack();   /* 发送ACK */
    }

    return receive;
}


