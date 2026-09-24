#ifndef GPIO_H
#define GPIO_H

#include <avr/io.h>
#include <stdint.h>

#define PORT_B  0
#define PORT_D  1

#define LOW     0
#define HIGH    1

#define INPUT   0
#define OUTPUT  1

//Board pin mapping 
#define BUZZER_PIN   PD4
#define LED_PIN      PD5
#define ECHO_PIN     PD3
#define TRIGGER_PIN  PD7

//Port B pins 
#define PIN_B0  PB0
#define PIN_B1  PB1
#define PIN_B2  PB2
#define PIN_B3  PB3
#define PIN_B4  PB4
#define PIN_B5  PB5

//Port D pins
#define PIN_D0  PD0
#define PIN_D1  PD1
#define PIN_D2  PD2
#define PIN_D3  PD3
#define PIN_D4  PD4
#define PIN_D5  PD5
#define PIN_D6  PD6
#define PIN_D7  PD7

void GPIO_Init(void);
void Direction_Pins(void);

uint8_t GPIO_ReadPin(uint8_t port, uint8_t pin);
void GPIO_WritePin(uint8_t port, uint8_t pin, uint8_t state);
void GPIO_TogglePin(uint8_t port, uint8_t pin);
void GPIO_SetPinDirection(uint8_t port, uint8_t pin, uint8_t direction);

#endif