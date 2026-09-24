#include "timer.h"
#include "gpio.h"
#include "Ultrasonic.h"
#include <avr/interrupt.h>
#include <util/atomic.h>

volatile uint32_t OVERFLOW_COUNT = 0;
static volatile uint8_t FLASH_OVF_COUNT = 0;
volatile uint8_t FLASH_ACTIVE = 0;
volatile uint8_t SERVO_STEP = 0;

void T1_Servo_Init(void)
{
    TCCR1A = 0;
    TCCR1B = 0;
    // Fast PWM Mode 14
    TCCR1A &= ~(1 << WGM10);
    TCCR1A |= (1 << WGM11);

    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << WGM13);

    // Non-inverting PWM on OC1A
    TCCR1A |= (1 << COM1A1);
    TCCR1A &= ~(1 << COM1A0);

    // Prescaler = 8
    TCCR1B &= ~(1 << CS10);
    TCCR1B |= (1 << CS11);
    TCCR1B &= ~(1 << CS12);

    // TOP → 50 Hz
    ICR1 = 39999; // 20ms

    // Initial servo position
    OCR1A = 2900;

    // move step every 10ms
    OCR1B = 19999;

    // Enable Interrupt B
    TIMSK1 |= (1 << OCIE1B);
    TIMSK1 |= (1 << TOIE1);
}

// ultrasonic sector
void T2_Ultrasonic_Init(void) // Rev
{
    // Set Operation Mode to Normal
    TCCR2A &= ~(1 << WGM20); // clear WGM20 bit
    TCCR2A &= ~(1 << WGM21); // clear WGM21 bit
    TCCR2B &= ~(1 << WGM22); // clear WGM22 bit

    // Set Prescaler to 8
    TCCR2B &= ~(1 << CS20); // clear CS20 bit
    TCCR2B |= (1 << CS21);  // set CS21 bit
    TCCR2B &= ~(1 << CS22); // clear CS22 bit
}

uint32_t T2_Pulse_duration(void)
{

    uint32_t OVF_SNAPSHOT;
    uint8_t TCNT2_SNAPSHOT;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    { // wait untill finishes reading

        OVF_SNAPSHOT = OVERFLOW_COUNT;
        TCNT2_SNAPSHOT = TCNT2;
    }

    return ((OVF_SNAPSHOT * 128UL) + (TCNT2_SNAPSHOT / 2)); // return duration
}

void T2_Start_Measuring(void)
{
    TCNT2 = 0;
    OVERFLOW_COUNT = 0;

    TIMSK2 |= (1 << TOIE2); // Enable Timer2 Overflow Interrupt
    TCCR2B |= (1 << CS21);
}

void T2_Stop_Measuring(void)
{
    TCCR2B &= ~((1 << CS20) | (1 << CS21) | (1 << CS22)); // clear prescalar
    TIMSK2 &= ~(1 << TOIE2);                              // Disable Timer2 Overflow interrupt
}

void T0_FlashTimer_init(void)
{

    // set Normal Mode
    TCCR0A &= ~(1 << WGM00);
    TCCR0A &= ~(1 << WGM01);
    TCCR0B &= ~(1 << WGM02);

    // set prescalar at 1024
    TCCR0B |= (1 << CS00);
    TCCR0B &= ~(1 << CS01);
    TCCR0B |= (1 << CS02);

    TIMSK0 |= (1 << TOIE0); // enable overflow interrupt
}

ISR(TIMER0_OVF_vect)
{

    FLASH_OVF_COUNT++;

    if (FLASH_OVF_COUNT >= 31)
    { // 0.5 sec

        FLASH_OVF_COUNT = 0;

        if (FLASH_ACTIVE)
        {

            GPIO_TogglePin(PORT_D, LED_PIN);
            GPIO_TogglePin(PORT_D, BUZZER_PIN);
        }
    }
}

ISR(TIMER2_OVF_vect)
{

    OVERFLOW_COUNT++;
}

ISR(TIMER1_COMPB_vect)
{

    SERVO_STEP = 1;
}

ISR(TIMER1_OVF_vect)
{

    SERVO_STEP = 1;
}