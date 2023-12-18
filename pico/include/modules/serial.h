#ifndef SERIAL_H
#define SERIAL_H

#include <stdio.h>
#include "pico/stdlib.h"

#define SERIAL_MAX_BUFFER_SIZE  256
#define NULL_CHAR               0
#define NEWLINE_CHAR            10

char serial_read_char();
void serial_read_string(char *buffer);

#endif
