#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include <stdint.h>

void Gen_Trigger(void);
void INT1_Echo_Init(void);
float Ultrasonic_GetDistance(void);

#endif