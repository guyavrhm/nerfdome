#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>

enum CommandType {
  POSITION = 1,
  SHOOT = 2,
  READY = 3,
  HOLD = 4
};

typedef struct {
  uint8_t type;
  uint8_t param1;
  uint8_t param2;
} protocol_command_data;

bool extract_protocol_data(const uint8_t* data, uint16_t data_size, protocol_command_data* result);

#endif // PROTOCOL_H
