#ifndef SERVO_H
#define SERVO_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define PWM_FREQUENCY   500000.0f
#define SERVO_FREQUENCY 50.0f

void servo_init(uint servo_pin);
void servo_set_angle(uint servo_pin, float degrees);

#endif
