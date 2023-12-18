#ifndef BLUETOOTH_SPP_H
#define BLUETOOTH_SPP_H

#include <inttypes.h>

void bt_on_receive(uint8_t *data, uint16_t size);
void bt_send(uint8_t *data);

int btstack_main(int argc, const char * argv[]);

#endif // BLUETOOTH_SPP_H
