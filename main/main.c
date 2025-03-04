#include <stdio.h>
#include "driver/gpio.h"
#include "driver/gpio_etm.h"
#include "driver_gpio.h"
#include <freertos/FreeRTOS.h>
#include "freertos/task.h"
#include "app_request_https.h"
#include "common.h"
#include "hal/gpio_types.h"
#include "nvs_flash.h"
#include "app_wifi.h"
#include "esp_log.h"
#include "driver_sleep_mode.h"
#include "app_handle_query.h"
#include "common_nvs.h"

#include "app_get_realtime.h"
void app_main(void)
{
    init_gpio();
    init_gpio_wakeup();
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);
    init_info_last_id_payload();
    // init_info_wifi();

    init_wifi();
}

