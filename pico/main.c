#include <stdio.h>
#include "servo.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/timer.h"

// Pin Definitions
#define SERVO_LOAD_PIN    10
#define SERVO_PITCH_PIN   11
#define SERVO_YAW_PIN     12
#define POT_YAW_PIN       26
#define POT_PITCH_PIN     27

// Load Angles and Timing
#define LOAD_ANGLE        30
#define LOAD_INTERVAL     1000 // min load time difference
#define UNLOAD_INTERVAL   100 // unload time


void initialize() {
    // Setup ADC
    adc_init();
    adc_gpio_init(POT_YAW_PIN);
    adc_gpio_init(POT_PITCH_PIN);

    // Setup Servos
    servo_init(SERVO_LOAD_PIN);
    servo_init(SERVO_YAW_PIN);
    servo_init(SERVO_PITCH_PIN);

    // Servo Angles
    servo_set_angle(SERVO_YAW_PIN, 0);
    servo_set_angle(SERVO_PITCH_PIN, 0);
    servo_set_angle(SERVO_LOAD_PIN, 0);
}

int readPotentiometer(int pin) {
    adc_select_input(pin - 26);
    return (adc_read()) * 180 / 4095;
}

int main() {
    stdio_init_all();
    initialize();

    int currentTime;
    int yawAngle;
    int pitchAngle;

    int lastLoadTime = 0;
    bool loaded = false;
    int tester = 0;

    while (true) {
        currentTime = time_us_32() / 1e3;

        // shoot signal instead of tester
        if (tester >= 0 && currentTime - lastLoadTime >= LOAD_INTERVAL) {
          servo_set_angle(SERVO_LOAD_PIN, LOAD_ANGLE);
          lastLoadTime = currentTime;
          loaded = true;
        }

        if (loaded && currentTime - lastLoadTime >= UNLOAD_INTERVAL) {
          servo_set_angle(SERVO_LOAD_PIN, 0);
          loaded = false;
        }

        yawAngle = readPotentiometer(POT_YAW_PIN);
        pitchAngle = readPotentiometer(POT_PITCH_PIN);

        servo_set_angle(SERVO_YAW_PIN, yawAngle);
        servo_set_angle(SERVO_PITCH_PIN, pitchAngle);

        tester++;
        sleep_ms(50);
    }

    return 0;
}