#include "Ultrasonic.h"
#include "timer.h"
#include "gpio.h"
#include "../UART/uart.h"
#include <util/delay.h>
#include <avr/interrupt.h>

static volatile uint32_t Duration = 0;

void Gen_Trigger(void)
{

    GPIO_WritePin(PORT_D, TRIGGER_PIN, HIGH);
    _delay_us(10);
    GPIO_WritePin(PORT_D, TRIGGER_PIN, LOW);
}

void INT1_Echo_Init(void)
{

    // any Logical change
    EICRA &= ~(1 << ISC11);
    EICRA |= (1 << ISC10);

    // Set flage =1
    EIFR |= (1 << INTF1);
    // Enable interrupt
    EIMSK |= (1 << INT1);
}

ISR(INT1_vect)
{

    if (GPIO_ReadPin(PORT_D, ECHO_PIN))
    {

        // Start timer2
        T2_Start_Measuring();
    }
    else
    {
        T2_Stop_Measuring();
        Duration = T2_Pulse_duration();
    }
}

float Ultrasonic_GetDistance(void)
{

    return ((float)Duration / 58);
}