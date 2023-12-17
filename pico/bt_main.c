#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

int btstack_main(int argc, const char * argv[]);

void btstack_receive(const uint8_t *data, uint16_t size) {
    // printf("rcv: '");
    // for (uint16_t i = 0; i < size; i++) {
    //     putchar(data[i]);
    // }
    // printf("'\n");

    if (size == 1 && data[0] == '1') {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    } else {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    }
}

int main() {
    stdio_init_all();

    // initialize CYW43 driver
    if (cyw43_arch_init()) {
        printf("cyw43_arch_init() failed.\n");
        return -1;
    }

    btstack_main(0, NULL);

    while (true) {
        sleep_ms(1000);
    }
}