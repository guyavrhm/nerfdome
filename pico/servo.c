#include "servo.h"

void servo_init(uint servo_pin) {
  gpio_set_function(servo_pin, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(servo_pin);

  pwm_config config = pwm_get_default_config();

  // Clock divider to achieve low-frequency PWM
  // System clock / Desired frequency = clkdiv
  uint32_t clkdiv = clock_get_hz(clk_sys) / PWM_FREQUENCY;
  pwm_config_set_clkdiv(&config, clkdiv);

  // Wrap for servo motor frequency:
  // Servo frequency cycle / New frequency cycle or
  // New frequency / Servo frequency
  pwm_config_set_wrap(&config, PWM_FREQUENCY / SERVO_FREQUENCY);

  pwm_init(slice_num, &config, true);
}

void servo_set_angle(uint servo_pin, float degs) {
  if (degs < 0.0f) {
    degs = 0.0f;
  } else if (degs > 180.0f) {
    degs = 180.0f;
  }

  // Map angle (0-180) to pulse (500us-2500us)
  float pulse_width = 500.0f + (degs / 180.0f) * 2000.0f;
  
  // Level (Duty Cycle) (0-WRAP) = WRAP / ( cycle (s) / pulse (s) )
  pwm_set_gpio_level(servo_pin, (PWM_FREQUENCY / SERVO_FREQUENCY) / ((1.0f / SERVO_FREQUENCY) / (pulse_width / 1e6)));
}
