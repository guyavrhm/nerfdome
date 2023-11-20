#include "serial.h"

void serial_init(uart_inst_t *uart, uint tx, uint rx, uint baud_rate) {
    uart_init(uart, baud_rate);

    gpio_set_function(tx, GPIO_FUNC_UART);
    gpio_set_function(rx, GPIO_FUNC_UART);

    // uart_set_hw_flow(uart, false, false);
    // uart_set_format(uart, 8, 1, UART_PARITY_NONE);
    // uart_set_fifo_enabled(uart, false);
}

char serial_read_char(uart_inst_t *uart) {
    if (uart_is_readable(uart)) {
        return uart_getc(uart);
    }
    return '\0'; // Return null character if no data available
}

void serial_read_string(uart_inst_t *uart, char *buffer) {
    size_t index = 0;

    while (index < SERIAL_MAX_BUFFER_SIZE - 1) {
        if (uart_is_readable(uart)) {
            char c = uart_getc(uart);

            if (c == '\n' || c == '\0') {
                break;
            } else {
                buffer[index++] = c;
            }
        } else {
            break; // Break out of the loop if UART is not readable
        }
    }

    buffer[index] = '\0';
}
