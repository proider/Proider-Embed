//
// Created by proider on 25-11-15.
//

#include "SCCB.h"

#include "UART_Print.h"

void sccb_delay() {
    __HAL_TIM_SET_COUNTER(&htim7,0);
    // 无符号减法可以正确处理计数器回绕（overflow）
    while ((uint32_t)(__HAL_TIM_GET_COUNTER(&htim7)) < 5) {
    }

}

void sccb_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    __HAL_RCC_GPIOB_CLK_ENABLE();//根据GPIO口手动开启对应时钟
    HAL_TIM_Base_Start(&htim7);
    GPIO_InitStructure.Pin = SCCB_CLK_PIN ;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SCCB_CLK_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = SCCB_SDA_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init(SCCB_CLK_PORT, &GPIO_InitStructure);

}

void sccb_start(void) {

    SCCB_SDA(1);
    SCCB_CLK(1);
    sccb_delay();
    SCCB_SDA(0);
    sccb_delay();
    SCCB_CLK(0);
    sccb_delay();
}


void sccb_stop(void) {
    SCCB_SDA(0);
    sccb_delay();
    SCCB_CLK(1);
    sccb_delay();
    SCCB_SDA(1);
    sccb_delay();
}

void sccb_nack(void) {
    SCCB_SDA(1);
    sccb_delay();
    SCCB_CLK(1);
    sccb_delay();
    SCCB_CLK(0);
    sccb_delay();
}

uint8_t sccb_send_byte(uint8_t data) {
    uint8_t res;
    for (uint8_t i = 0; i < 8; i++) {
        SCCB_SDA((data&0x80)>>7);
        sccb_delay();
        SCCB_CLK(1);
        sccb_delay();
        SCCB_CLK(0);
        data <<= 1;
    }

    SCCB_SDA(1);
    sccb_delay();
    SCCB_CLK(1);
    sccb_delay();
    SCCB_CLK(0);

    if (SCCB_READ_SDA) {
        res = ACK_ERR;//发送失败
    }
    else {

        res = ACK_OK;//发送成功
    }
    return res;
}

uint8_t sccb_read_byte(void) {
    uint8_t receive = 0;
    for (int8_t i = 7; i >=0; i--) {
        sccb_delay();
        SCCB_CLK(1);
        receive|=(SCCB_READ_SDA<<i);
        sccb_delay();
        SCCB_CLK(0);
    }

    sccb_delay();
    SCCB_CLK(1);
    sccb_delay();
    SCCB_CLK(0);
    sccb_delay();
    SCCB_SDA(0);
    sccb_delay();

    return receive;
}



uint8_t sccb_write_reg(uint8_t reg, uint8_t data) {
    uint8_t res = 0;
    sccb_start();
    if (sccb_send_byte(SCCB_WRITE_ADDRESS)) {res = 1;}
    if (sccb_send_byte(reg)) {res = 1;}
    if (sccb_send_byte(data)) {res = 1;}
    sccb_stop();
    return res;
}

uint8_t sccb_read_reg(uint8_t reg)
{
    uint8_t dat = 0;
    sccb_start();
    sccb_send_byte(SCCB_WRITE_ADDRESS);
    sccb_send_byte(reg);
    sccb_stop();
    sccb_start();
    sccb_send_byte(SCCB_READ_ADDRESS);
    dat=sccb_read_byte();
    sccb_stop();

    return dat;
}

void sccb_2_phase_write(uint8_t id_addr, uint8_t sub_addr)
{
    sccb_start();
    sccb_send_byte((id_addr << 1) | SCCB_WRITE_ADDRESS);
    sccb_send_byte(sub_addr);
    sccb_stop();
}

void sccb_2_phase_read(uint8_t id_addr, uint8_t *dat)
{
    sccb_start();
    sccb_send_byte((id_addr << 1) | SCCB_READ_ADDRESS);
    *dat=sccb_read_byte();
    sccb_stop();
}


uint8_t SCCB_WriteHandle_16Bit (uint16_t addr)
{
    uint8_t status;		// 状态标志位

    sccb_start();	// 启动IIC通信
    if( sccb_send_byte(OV5640_DEVICE_ADDRESS) == ACK_OK ) //写数据指令
    {
        if( sccb_send_byte((uint8_t)(addr >> 8)) == ACK_OK ) //写入16位地址
        {
            if( sccb_send_byte((uint8_t)(addr)) != ACK_OK )
            {
                status = ERROR;	// 操作失败
            }
        }
    }
    status = SUCCESS;	// 操作成功
    return status;
}

uint8_t SCCB_WriteReg_16Bit(uint16_t addr,uint8_t value)
{
    uint8_t status;

    sccb_start(); //启动IIC通讯

    if( SCCB_WriteHandle_16Bit(addr) == SUCCESS)	//写入要操作的寄存器
    {
        if (sccb_send_byte(value) != ACK_OK) //写数据
        {
            status = ERROR;
        }
    }
    sccb_stop(); // 停止通讯

    status = SUCCESS;	// 写入成功
    return status;
}

uint8_t SCCB_ReadReg_16Bit (uint16_t addr)
{
    uint8_t value = 0;

    sccb_start();		// 启动IIC通信

    if( SCCB_WriteHandle_16Bit(addr) == SUCCESS) //写入要操作的寄存器
    {
        sccb_stop();	// 停止IIC通信
        sccb_start(); //重新启动IIC通讯

        if (sccb_send_byte(OV5640_DEVICE_ADDRESS|0X01) == ACK_OK)	// 发送读命令
        {
            value = sccb_read_byte();	// 读到最后一个数据时发送 非应答信号
        }
        sccb_stop();	// 停止IIC通信

    }

    return value;
}





