#include <stdint.h>
#include <stdio.h>
#include "driver_enable_output.h"
#include "hal/gpio_types.h"
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include "freertos/task.h"
void enable_output(uint8_t money)
{
    gpio_set_level(GPIO_NUM_33, 1);
    vTaskDelay(pdMS_TO_TICKS(money * 1000));
    gpio_set_level(GPIO_NUM_33, 0);
}
