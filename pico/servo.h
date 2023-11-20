#ifndef SERVO_H
#define SERVO_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define WRAP 10000.f
#define SERVO_FREQ 50.f

void servo_init(uint servo_pin);
void servo_set_angle(uint servo_pin, float degrees);

#endif
