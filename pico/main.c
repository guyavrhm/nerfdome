#include <stdio.h>
#include "servo.h"
#include "serial.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/timer.h"

// Pin Definitions
#define TX_PIN            0
#define RX_PIN            1
#define SERVO_LOAD_PIN    10
#define SERVO_PITCH_PIN   11
#define SERVO_YAW_PIN     12
#define POT_YAW_PIN       26
#define POT_PITCH_PIN     27

// Load Angles and Timing
#define LOAD_ANGLE        30
#define UNLOAD_INTERVAL   100 // unload time

// Commands
#define HOLD 1
#define READY 2
#define SHOOT 3


void initialize() {
  // Setup ADC
  adc_init();
  adc_gpio_init(POT_YAW_PIN);
  adc_gpio_init(POT_PITCH_PIN);

  // UART
  serial_init(uart0, TX_PIN, RX_PIN, 115200);

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
  printf("Activated\n");

  char serial_buffer[SERIAL_MAX_BUFFER_SIZE];

  int currentTime;
  int yawAngle;
  int pitchAngle;
  int command;

  int lastLoadTime = 0;
  bool loaded = false;

  while (true) {
    currentTime = time_us_32() / 1e3;

    if (loaded && currentTime - lastLoadTime >= UNLOAD_INTERVAL) {
      servo_set_angle(SERVO_LOAD_PIN, 0);
      loaded = false;
    }

    serial_read_string(uart0, serial_buffer);
    if (serial_buffer[0] != '\0') {

      if (sscanf(serial_buffer, "%d %d", &yawAngle, &pitchAngle) == 2) {
        printf("Yaw: %d Pitch: %d\n", yawAngle, pitchAngle);
        servo_set_angle(SERVO_YAW_PIN, yawAngle);
        servo_set_angle(SERVO_PITCH_PIN, pitchAngle);
      }
      else {
        command = atoi(serial_buffer);
        if (command == SHOOT && !loaded) {
          printf("Shoot\n");
          servo_set_angle(SERVO_LOAD_PIN, LOAD_ANGLE);
          lastLoadTime = currentTime;
          loaded = true;
        }
        else if (command == READY) {
          printf("DC motors ON\n");
        }
        else if (command == HOLD) {
          printf("DC motors OFF\n");
        }
        else {
          printf("Unknown: %s\n", serial_buffer);
        }
      }
    }

    sleep_ms(50);
  }

  return 0;
}