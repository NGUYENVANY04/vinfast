#include <stdio.h>
#include "driver_sleep_mode.h"
#include "esp_sleep.h"
#include "hal/gpio_types.h"
#include "driver/rtc_io.h"
#include "esp_log.h"
void init_gpio_wakeup(void)
{
    ESP_LOGI("Tag", "Complete");

    rtc_gpio_pullup_en(GPIO_NUM_0);
    rtc_gpio_pulldown_dis(GPIO_NUM_0);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0);
    esp_deep_sleep_start();
}
void deinit_gpio_rtc(void) { 
    rtc_gpio_deinit(GPIO_NUM_0); 
}
