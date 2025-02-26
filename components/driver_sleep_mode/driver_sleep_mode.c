#include <stdio.h>
#include "driver_sleep_mode.h"
#include "esp_sleep.h"
#include "hal/gpio_types.h"
#include "driver/rtc_io.h"

void init_gpio_wakeup(void)
{

    rtc_gpio_deinit(GPIO_NUM_0);
    /// general gpio functions using the pin.

    rtc_gpio_pullup_en(GPIO_NUM_0);
    rtc_gpio_pulldown_dis(GPIO_NUM_0);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0);
    //   printf("going to sleep. woken up %d\n", timesWokenUp++);
    //   esp_deep_sleep_start();
}

