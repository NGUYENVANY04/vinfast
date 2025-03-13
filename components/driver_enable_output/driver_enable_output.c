#include <stdio.h>
#include "driver_enable_output.h"
#include "hal/gpio_types.h"
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include "freertos/task.h"
void enable_output(void)
{
    int t = 5;
    while (t--)
    {
        gpio_set_level(GPIO_NUM_2, 1);
        vTaskDelay(1000);
        gpio_set_level(GPIO_NUM_2, 0);
        vTaskDelay(1000);
    }
}
