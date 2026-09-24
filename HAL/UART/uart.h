#ifndef UART_H
#define UART_H

#include <stdint.h>

void UART_Init(void);
void UART_SendByte(uint8_t data);
void UART_SendString(const char *str);
void UART_SendNumber(uint32_t number);
void UART_SendFloat(float number);

#endif