//
// Created by proider on 25-11-2.
//

#ifndef SOFTWARE_I2C_H
#define SOFTWARE_I2C_H




#define SDA_GPIO_PORT GPIOF
#define SCL_GPIO_PORT GPIOB

#define SDA_PIN GPIO_PIN_11
#define SCL_PIN GPIO_PIN_0


/* IO操作 */
#define IIC_SCL(x)        do{ x ? \
HAL_GPIO_WritePin(SCL_GPIO_PORT, SCL_PIN, GPIO_PIN_SET) : \
HAL_GPIO_WritePin(SCL_GPIO_PORT, SCL_PIN, GPIO_PIN_RESET); \
}while(0)       /* SCL */

#define IIC_SDA(x)        do{ x ? \
HAL_GPIO_WritePin(SDA_GPIO_PORT, SDA_PIN, GPIO_PIN_SET) : \
HAL_GPIO_WritePin(SDA_GPIO_PORT, SDA_PIN, GPIO_PIN_RESET); \
}while(0)       /* SDA */

#define IIC_READ_SDA     HAL_GPIO_ReadPin(SDA_GPIO_PORT, SDA_PIN) /* 读取SDA */
#include <stdint.h>


void Software_I2C_Init(void);

void I2C_START(void);

void I2C_STOP(void);

void I2C_DelayUs(uint16_t us);

uint8_t iic_wait_ack(void);

void iic_ack(void);

void iic_nack(void);

void iic_send_byte(uint8_t data);

uint8_t iic_read_byte(uint8_t ack);


#endif //SOFTWARE_I2C_H


