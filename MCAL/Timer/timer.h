#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void T1_Servo_Init(void);

void T2_Ultrasonic_Init(void);
void T2_Start_Measuring(void);
void T2_Stop_Measuring(void);
uint32_t T2_Pulse_duration(void);
void T0_FlashTimer_init(void);

extern volatile uint32_t OVERFLOW_COUNT;
extern volatile uint8_t FLASH_ACTIVE;
extern volatile uint8_t SERVO_STEP;

#endif