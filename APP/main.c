#define F_CPU 16000000UL
#include "timer.h"
#include "gpio.h"
#include "servo.h"
#include "Ultrasonic.h"
#include "../UART/uart.h"
#include <avr/interrupt.h>
#include <util/delay.h>

#define OBJECT_MIN_DIST 2.0f
#define OBJECT_MAX_DIST 30.0f

int main(void)
{

    float Distance = 0.0;

    GPIO_Init();
    UART_Init();
    Direction_Pins();

    T2_Ultrasonic_Init();
    T0_FlashTimer_init();
    Servo_Init();

    INT1_Echo_Init();
    Servo_Set_Range(30, 150);
    sei(); // Enable global interrupt

    while (1)
    {

        Servo_Update();
        Gen_Trigger();
        _delay_ms(2); // Wait until echo measurement is complete

        Distance = Ultrasonic_GetDistance();

        // object detected condition
        if (Distance >= OBJECT_MIN_DIST && Distance <= OBJECT_MAX_DIST)
        {

            FLASH_ACTIVE = 1;
        }

        else
        {

            FLASH_ACTIVE = 0;
            GPIO_WritePin(PORT_D, LED_PIN, LOW);
            GPIO_WritePin(PORT_D, BUZZER_PIN, LOW);
        }

        _delay_ms(15);
    }
}