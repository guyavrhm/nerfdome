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

// Load Angles and Timing
#define LOAD_ANGLE        30
#define UNLOAD_INTERVAL   100 // unload time

// Commands
#define POSITION 1
#define SHOOT 2
#define READY 3
#define HOLD 4


void initialize() {
  // Setup Serial
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

int main() {
  stdio_init_all();
  initialize();
  printf("Activated\n");

  int currentTime;
  int lastLoadTime = 0;
  bool loaded = false;

  char serial_buffer[SERIAL_MAX_BUFFER_SIZE];
  uint8_t command;
  uint8_t commandParam1;
  uint8_t commandParam2;

  while (true) {
    currentTime = time_us_32() / 1e3;

    if (loaded && currentTime - lastLoadTime >= UNLOAD_INTERVAL) {
      servo_set_angle(SERVO_LOAD_PIN, 0);
      loaded = false;
    }

    serial_read_string(uart0, serial_buffer);
    if (serial_buffer[0] != '\0') {
      sscanf(serial_buffer, "%d %d %d", &command, &commandParam1, &commandParam2);

      if (command == POSITION) {
        // 1     40   120
        // type  yaw  pitch
        printf("Yaw: %d Pitch: %d\n", commandParam1, commandParam2);
        servo_set_angle(SERVO_YAW_PIN, commandParam1);
        servo_set_angle(SERVO_PITCH_PIN, commandParam2);
      }
      else if (command == SHOOT && !loaded) {
        // 2     5
        // type  times
        printf("Shoot %d\n", commandParam1);
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
        if (strlen(serial_buffer) != 0)
          printf("Unknown: %s\n", serial_buffer);
        else
          printf("Unknown: %d\n", command);
      }

      commandParam1 = 0;
      commandParam2 = 0;
    }
    sleep_ms(50);
  }

  return 0;
}