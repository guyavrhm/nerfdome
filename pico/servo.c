#include "servo.h"

#define WRAP 10000.f
#define SERVO_FREQ 50.f
#define CLKDIV clock_get_hz(clk_sys)/(SERVO_FREQ*WRAP)


void servo_init(uint servo_pin) {
    gpio_set_function(servo_pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(servo_pin);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, CLKDIV);
    pwm_config_set_wrap(&config, WRAP);
    
    pwm_init(slice_num, &config, true);
}

void servo_set_angle(uint servo_pin, float degs) {
    if (degs < 0.0f) {
        degs = 0.0f;
    } else if (degs > 180.0f) {
        degs = 180.0f;
    }

    // map angle (0-180) to ms (500-2500)
    float pulse_width = 500.0f + (degs / 180.0f) * 2000.0f;
    
    pwm_set_gpio_level(servo_pin, (pulse_width/((1/SERVO_FREQ)*1e6))*WRAP);
}
