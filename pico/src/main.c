#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "servo.h"
#include "serial.h"
#include "spp.h"
#include "protocol.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Pin Definitions
#define TX_PIN            0
#define RX_PIN            1
#define SERVO_LOAD_PIN    10
#define SERVO_PITCH_PIN   11
#define SERVO_YAW_PIN     12
#define MOTOR1_PIN        18
#define MOTOR2_PIN        19

// Load Angles and Timing
#define LOAD_ANGLE        30
#define LOAD_INTERVAL     300 // load time difference
#define UNLOAD_INTERVAL   100 // time until unload (from load)
static uint8_t loads_left = 0;

static protocol_command_data command;

void bt_on_receive(uint8_t *data, uint16_t size) {
    if (extract_protocol_data(data, size, &command)) {
      if (command.type == POSITION) {
      // 1     40   120
      // type  yaw  pitch
      printf("Yaw: %hhu Pitch: %hhu\n", command.param1, command.param2);
      servo_set_angle(SERVO_YAW_PIN, command.param1);
      servo_set_angle(SERVO_PITCH_PIN, command.param2);
      }
      else if (command.type == SHOOT) {
        // 2     5
        // type  times
        printf("Shoot %hhu\n", command.param1);
        loads_left = command.param1;
      }
      else if (command.type == READY) {
        printf("DC motors ON\n");
        gpio_put(MOTOR1_PIN, 1);
        gpio_put(MOTOR2_PIN, 1);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);   
      }
      else if (command.type == HOLD) {
        printf("DC motors OFF\n");
        gpio_put(MOTOR1_PIN, 0);
        gpio_put(MOTOR2_PIN, 0);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
      }
      else {
        printf("Unknown Command: %hhu\n", command.type);
      }
    }
}

void initialize() {
  stdio_init_all();

  // Setup Bluetooth
  if (cyw43_arch_init()) {
    printf("cyw43_arch_init() failed.\n");
    return;
  }
  btstack_main(0, NULL);

  // Setup Motors
  gpio_init(MOTOR1_PIN);
  gpio_init(MOTOR2_PIN);
  gpio_set_dir(MOTOR1_PIN, GPIO_OUT);
  gpio_set_dir(MOTOR2_PIN, GPIO_OUT);

  // Setup Servos
  servo_init(SERVO_LOAD_PIN);
  servo_init(SERVO_YAW_PIN);
  servo_init(SERVO_PITCH_PIN);

  // Servo Angles
  servo_set_angle(SERVO_YAW_PIN, 0);
  servo_set_angle(SERVO_PITCH_PIN, 0);
  servo_set_angle(SERVO_LOAD_PIN, 0);
}

void handle_load(bool *is_loaded, int *current_time, int *last_load_time) {
  if (loads_left > 0 && *current_time - *last_load_time >= LOAD_INTERVAL) {
    servo_set_angle(SERVO_LOAD_PIN, LOAD_ANGLE);
    *last_load_time = *current_time;
    *is_loaded = true;
    loads_left--;
  }
  else if (*is_loaded && *current_time - *last_load_time >= UNLOAD_INTERVAL) {
    servo_set_angle(SERVO_LOAD_PIN, 0);
    *is_loaded = false;
  }
}

int main() {
  initialize();
  printf("Activated\n");

  int current_time = 0;
  int last_load_time = 0;
  bool is_loaded = false;

  while (true) {
    current_time = time_us_32() / 1e3;

    handle_load(&is_loaded, &current_time, &last_load_time);
    sleep_ms(50);
  }
  
  return 0;
}
