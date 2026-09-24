#include "uart.h"
#include <avr/io.h>

#define F_CPU 16000000UL
#define BAUD 9600UL

#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

void UART_Init(void)
{
    //Set baud rate 
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)(UBRR_VALUE);

    // Enable transmitter 
    //UCSR0B = (1 << TXEN0);

    // 8-bit data, 1 stop bit, no parity 
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_SendByte(uint8_t data)
{
    //Wait until transmit buffer is empty
    while (!(UCSR0A & (1 << UDRE0)));

    UDR0 = data;
}

void UART_SendString(const char *str)
{
    while (*str)
    {
        UART_SendByte(*str);
        str++;
    }
}

void UART_SendNumber(uint32_t number)
{
    char buffer[11];
    uint8_t i = 0;

    if (number == 0)
    {
        UART_SendByte('0');
        return;
    }

    while (number > 0)
    {
        buffer[i++] = (number % 10) + '0';
        number /= 10;
    }

    while (i > 0)
    {
        UART_SendByte(buffer[--i]);
    }
}

void UART_SendFloat(float number)
{
    uint32_t integer_part;
    uint32_t decimal_part;

    if (number < 0)
    {
        UART_SendByte('-');
        number = -number;
    }

    integer_part = (uint32_t)number;
    decimal_part = (uint32_t)((number - integer_part) * 100);

    UART_SendNumber(integer_part);
    UART_SendByte('.');

    if (decimal_part < 10)
    {
        UART_SendByte('0');
    }

    UART_SendNumber(decimal_part);
}
