#ifndef SERVO_H
#define SERVO_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

void servo_init(uint servo_pin);
void servo_set_angle(uint servo_pin, float degrees);

#endif
