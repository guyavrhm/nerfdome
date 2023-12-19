#include "protocol.h"
#include <stdio.h>
#include <string.h>

bool extract_protocol_data(const uint8_t* data, uint16_t data_size, protocol_command_data* result) {
    if (data_size != sizeof(protocol_command_data)) {
        printf("Error: Invalid data length\n");
        return false;
    }

    memcpy(result, data, sizeof(protocol_command_data));
    return true;
}
