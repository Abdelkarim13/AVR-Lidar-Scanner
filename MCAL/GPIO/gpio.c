#include "gpio.h"
#include <avr/io.h>
#include <stdint.h>


void GPIO_Init(void)
{
    DDRB = 0x00; // Set all PORTB pins as input
    DDRD = 0x00; // Set all PORTD pins as input

    PORTB = 0x00; // all pull-up resistors disabled
    PORTD = 0x00; // pull-up resistors disabled
}

uint8_t GPIO_ReadPin(uint8_t port, uint8_t pin)
{
    if (port == PORT_B)
    return (PINB & (1 << pin)) ? HIGH : LOW; 
    
    else if (port == PORT_D)
    return (PIND & (1 << pin)) ? HIGH : LOW;
    
    else
    return LOW;
}

void GPIO_WritePin(uint8_t port, uint8_t pin, uint8_t state)
{
    // Write values to PORTB and PORTD
    if (port == PORT_B)
    {
        if (state == HIGH)
            PORTB |= (1 << pin);
        else
            PORTB &= ~(1 << pin);
    }
    else if (port == PORT_D)
    {
        if (state == HIGH)
            PORTD |= (1 << pin);
        else
            PORTD &= ~(1 << pin);
    }
}

void GPIO_TogglePin(uint8_t port, uint8_t pin)
{
    // Toggle specified pin on PORTB or PORTD
    if (port == PORT_B)
    PORTB ^= (1 << pin);  
    else if (port == PORT_D)
    PORTD ^= (1 << pin); 
}

void GPIO_SetPinDirection(uint8_t port, uint8_t pin, uint8_t direction)
{
    // Set the direction of a specific pin on PORTB or PORTD
    if (port == PORT_B)
    {
        if (direction == OUTPUT)
        DDRB |= (1 << pin);
        else
        DDRB &= ~(1 << pin);  
    }
    else if (port == PORT_D)
    {
        if (direction == OUTPUT)
        DDRD |= (1 << pin);
        else
            DDRD &= ~(1 << pin);
        }
}

void Direction_Pins(void){
                                                                                                                                                                                                                                                                                                  
    GPIO_SetPinDirection(PORT_D, PD5, OUTPUT); //LED
    GPIO_SetPinDirection(PORT_D, PD4, OUTPUT); //BUZZER
    GPIO_SetPinDirection(PORT_D, PD3, INPUT); //Echo
    GPIO_SetPinDirection(PORT_D, PD7, OUTPUT); //Trigger
    GPIO_SetPinDirection(PORT_B, PB1, OUTPUT); //Servo
}