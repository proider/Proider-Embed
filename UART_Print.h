//
// Created by proider on 25-11-16.
//

#ifndef UART_PRINT_H
#define UART_PRINT_H

#define TX_BUF_LEN  256     /* 发送缓冲区容量，根据需要进行调整 */

#define UASRT huart1
void Print(const char *__format, ...);
#endif //UART_PRINT_H
