//
// Created by proider on 25-11-15.
//

#ifndef SCCB_H
#define SCCB_H

#include "gpio.h"
#include "tim.h"


#define SCCB_DEVICE_ADDR 0x21
/* SCCB接口读写通讯地址bit0 */
#define SCCB_WRITE_ADDRESS  0x42
#define SCCB_READ_ADDRESS   0x43



#define SCCB_CLK_PIN GPIO_PIN_6
#define SCCB_SDA_PIN GPIO_PIN_7

#define SCCB_CLK_PORT GPIOB
#define SCCB_SDA_PORT GPIOB

#define SCCB_SDA(x)  do{x?HAL_GPIO_WritePin(SCCB_SDA_PORT,SCCB_SDA_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(SCCB_SDA_PORT,SCCB_SDA_PIN,GPIO_PIN_RESET);}while(0)
#define SCCB_CLK(x)  do{x?HAL_GPIO_WritePin(SCCB_CLK_PORT,SCCB_CLK_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(SCCB_CLK_PORT,SCCB_CLK_PIN,GPIO_PIN_RESET);}while(0)

#define SCCB_READ_SDA HAL_GPIO_ReadPin(SCCB_SDA_PORT,SCCB_SDA_PIN)

void sccb_init(void);
uint8_t sccb_write_reg(uint8_t reg, uint8_t data);
uint8_t sccb_read_reg(uint8_t reg);

#endif //SCCB_H
