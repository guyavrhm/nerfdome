#include "serial.h"

char serial_read_char() {
  signed char c = getchar_timeout_us(0);

  if (c != PICO_ERROR_TIMEOUT) {
    return c;
  }
  return NULL_CHAR; // Return null character if no data available
}

void serial_read_string(char *buffer) {
  signed char c;
  size_t index = 0;

  while (index < SERIAL_MAX_BUFFER_SIZE - 1) {
    c = getchar_timeout_us(0);

    if (c == PICO_ERROR_TIMEOUT || c == NEWLINE_CHAR || c == NULL_CHAR) {
      break;
    } else {
      buffer[index++] = c;
    }
  }

  buffer[index] = NULL_CHAR;
}
