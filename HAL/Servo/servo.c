#include "servo.h"
#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define PULSE_TO_TICK(us) ((us) * 2) // Convert microseconds to ticks

// Servo sweep state
static uint8_t sweep_angle = 20;
static int8_t sweep_dir = 1;
static uint8_t sweep_min = 20;
static uint8_t sweep_max = 180;

void Servo_Init(void)
{
    T1_Servo_Init(); 
}

void Servo_Angle(uint8_t angle)
{
    // Make sure the angle is within the valid range
    if (angle > SERVO_ANGLE_MAX)
        angle = SERVO_ANGLE_MAX;

    else if (angle < SERVO_ANGLE_MIN)
        angle = SERVO_ANGLE_MIN;

    // Linear mapping between angle and pulse width in microseconds
    uint16_t pulse_us = SERVO_PULSE_MIN_US +
                        ((uint32_t)(angle - SERVO_ANGLE_MIN) *
                         (SERVO_PULSE_MAX_US - SERVO_PULSE_MIN_US)) /
                            (SERVO_ANGLE_MAX - SERVO_ANGLE_MIN);

    // Convert to timer ticks and set OCR1A
    OCR1A = PULSE_TO_TICK(pulse_us);
}

void Servo_Set_Range(uint8_t start_angle, uint8_t end_angle)
{

    // Ensure both angles are within the valid servo range
    if (start_angle < SERVO_ANGLE_MIN)
        start_angle = SERVO_ANGLE_MIN;
    if (start_angle > SERVO_ANGLE_MAX)
        start_angle = SERVO_ANGLE_MAX;
    if (end_angle > SERVO_ANGLE_MAX)
        end_angle = SERVO_ANGLE_MAX;
    if (end_angle < SERVO_ANGLE_MIN)
        end_angle = SERVO_ANGLE_MIN;

    if (start_angle <= end_angle)
    {

        sweep_min = start_angle;
        sweep_max = end_angle;
        sweep_dir = 1;
    }
    else
    {

        sweep_min = end_angle;
        sweep_max = start_angle;
        sweep_dir = -1;
    }

    sweep_angle = start_angle;
    Servo_Angle(sweep_angle);
}

void Servo_Update(void)
{

    if (!SERVO_STEP)
        return;
    SERVO_STEP = 0;

    // move servo to next angle
    Servo_Angle(sweep_angle);

    // change direction at the limits
    if (sweep_angle == sweep_max)
        sweep_dir = -1;

    else if (sweep_angle == sweep_min)
        sweep_dir = 1;

    sweep_angle += sweep_dir;
}