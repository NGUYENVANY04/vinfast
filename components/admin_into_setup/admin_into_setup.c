#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include "freertos/task.h"
#include "admin_into_setup.h"
#include "esp_log.h"
#include "hal/gpio_types.h"
#include "driver/gpio.h"
uint32_t last_time = 0;
bool setup = false;
void handle_into_setup(void *arg)
{
    static uint8_t count = 0;
    uint32_t current_time = xTaskGetTickCountFromISR();

    if (current_time - last_time > (200 / portTICK_PERIOD_MS))
    {
        if (current_time - last_time > (2000 / portTICK_PERIOD_MS))
        {
            last_time = current_time;
            count = 0;
        }
        else
        {
            ESP_EARLY_LOGI("SET UP", "Step :%d\n", count);
            last_time = current_time;
            count++;
            if (count == 3)
            {
                ESP_EARLY_LOGI("SET UP", "Admin start config !!!");
                setup = true;
                gpio_isr_handler_remove(GPIO_NUM_0);
            }
        }
    }
}
