#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

#define SERVO_PULSE_MIN_US   500
#define SERVO_PULSE_MAX_US   2400

#define SERVO_ANGLE_MIN      0
#define SERVO_ANGLE_MAX      180

void Servo_Init(void);

void Servo_Angle(uint8_t angle);

void Servo_Set_Range(uint8_t start_angle, uint8_t end_angle);

void Servo_Update(void);

#endif