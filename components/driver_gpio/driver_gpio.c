#include <stdio.h>
#include <string.h>
#include "driver_gpio.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "admin_into_setup.h"
void init_gpio_device(void)
{

    gpio_config_t pin = {
        .pin_bit_mask = 1ULL << GPIO_NUM_33,
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&pin);
    gpio_set_level(GPIO_NUM_33, 0);
}
void init_gpio_into_setup(void)
{

    gpio_config_t pin = {
        .pin_bit_mask = 1ULL << GPIO_NUM_0,
        .mode = GPIO_MODE_INPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_NEGEDGE,
    };
    gpio_config(&pin);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_NUM_0, handle_into_setup, NULL);
}
