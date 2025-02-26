#include <stdio.h>
#include "driver_gpio.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"
void init_gpio(void)
{

    gpio_config_t pin = {
        .pin_bit_mask = 1ULL << GPIO_NUM_2,
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&pin);
    gpio_set_level(GPIO_NUM_2, 0);
}

