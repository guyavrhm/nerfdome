#include <stdio.h>
#include "servo.h"
#include "serial.h"
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Pin Definitions
#define TX_PIN            0
#define RX_PIN            1
#define SERVO_LOAD_PIN    10
#define SERVO_PITCH_PIN   11
#define SERVO_YAW_PIN     12

// Load Angles and Timing
#define LOAD_ANGLE        30
#define LOAD_INTERVAL     300 // load time difference
#define UNLOAD_INTERVAL   100 // time until unload (from load)

// Commands
enum CommandType {
  POSITION = 1,
  SHOOT = 2,
  READY = 3,
  HOLD = 4
};

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

void handle_load(bool *is_loaded, uint8_t *loads_left, int *current_time, int *last_load_time) {
  if (*loads_left > 0 && *current_time - *last_load_time >= LOAD_INTERVAL) {
    servo_set_angle(SERVO_LOAD_PIN, LOAD_ANGLE);
    *last_load_time = *current_time;
    *is_loaded = true;
    (*loads_left)--;
  }
  else if (*is_loaded && *current_time - *last_load_time >= UNLOAD_INTERVAL) {
    servo_set_angle(SERVO_LOAD_PIN, 0);
    *is_loaded = false;
  }
}

int main() {
  stdio_init_all();
  initialize();
  printf("Activated\n");

  int current_time = 0;
  int last_load_time = 0;
  uint8_t loads_left = 0;
  bool is_loaded = false;

  char serial_buffer[SERIAL_MAX_BUFFER_SIZE];
  uint8_t command = 0;
  uint8_t command_param1 = 0;
  uint8_t command_param2 = 0;

  while (true) {
    current_time = time_us_32() / 1e3;

    // Handle ammunition load
    handle_load(&is_loaded, &loads_left, &current_time, &last_load_time);

    serial_read_string(uart0, serial_buffer);
    if (serial_buffer[0] != '\0') {
      sscanf(serial_buffer, "%hhu %hhu %hhu", &command, &command_param1, &command_param2);

      if (command == POSITION) {
        // 1     40   120
        // type  yaw  pitch
        printf("Yaw: %hhu Pitch: %hhu\n", command_param1, command_param2);
        servo_set_angle(SERVO_YAW_PIN, command_param1);
        servo_set_angle(SERVO_PITCH_PIN, command_param2);
      }
      else if (command == SHOOT) {
        // 2     5
        // type  times
        printf("Shoot %hhu\n", command_param1);
        loads_left = command_param1;
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
          printf("Unknown: %hhu\n", command);
      }

      command = 0;
      command_param1 = 0;
      command_param2 = 0;
    }
    sleep_ms(50);
  }

  return 0;
}
