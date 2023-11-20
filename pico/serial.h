#ifndef SERIAL_H
#define SERIAL_H

#include "pico/stdlib.h"
#include "hardware/uart.h"

#define SERIAL_MAX_BUFFER_SIZE  256

void serial_init(uart_inst_t *uart, uint tx, uint rx, uint baud_rate);
char serial_read_char(uart_inst_t *uart);
void serial_read_string(uart_inst_t *uart, char *buffer);

#endif
