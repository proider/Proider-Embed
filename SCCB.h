//
// Created by proider on 25-11-15.
//

#ifndef SCCB_H
#define SCCB_H

#include "gpio.h"
#include "tim.h"

#define OV2640_DEVICE_ADDRESS     0x60    // OV2640地址
#define OV5640_DEVICE_ADDRESS     0X78		// OV5640地址

#define ACK_OK  	1  			// 响应正常
#define ACK_ERR 	0				// 响应错误

#define SCCB_DEVICE_ADDR 0x21
/* SCCB接口读写通讯地址bit0 */
#define SCCB_WRITE_ADDRESS  0x42
#define SCCB_READ_ADDRESS   0x43



#define SCCB_CLK_PIN GPIO_PIN_8
#define SCCB_SDA_PIN GPIO_PIN_9

#define SCCB_CLK_PORT GPIOI
#define SCCB_SDA_PORT GPIOI

#define SCCB_SDA(x)  do{x?HAL_GPIO_WritePin(SCCB_SDA_PORT,SCCB_SDA_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(SCCB_SDA_PORT,SCCB_SDA_PIN,GPIO_PIN_RESET);}while(0)
#define SCCB_CLK(x)  do{x?HAL_GPIO_WritePin(SCCB_CLK_PORT,SCCB_CLK_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(SCCB_CLK_PORT,SCCB_CLK_PIN,GPIO_PIN_RESET);}while(0)

#define SCCB_READ_SDA HAL_GPIO_ReadPin(SCCB_SDA_PORT,SCCB_SDA_PIN)

void sccb_init(void);
uint8_t sccb_write_reg(uint8_t reg, uint8_t data);
uint8_t sccb_read_reg(uint8_t reg);
uint8_t SCCB_WriteReg_16Bit(uint16_t addr,uint8_t value);
uint8_t SCCB_ReadReg_16Bit (uint16_t addr);
uint8_t SCCB_WriteBuffer_16Bit(uint16_t addr,uint8_t *pData, uint32_t size);

#endif //SCCB_H


