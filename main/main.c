#include <freertos/FreeRTOS.h>
#include <stdbool.h>
#include <stdio.h>
#include "app_wifi.h"
#include "driver_gpio.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "driver_sleep_mode.h"
#include "app_handle_wifi_mode_sta.h"
#include "common_nvs.h"
#include "admin_into_setup.h"
#include "driver_enable_output.h"
void task_check_setup(void *pra)
{
    while (1)
    {
        if (setup)
        {
            ESP_LOGI("setup", "GO TO SET UP");
            nvs_open("CONFIG", NVS_READWRITE, &config);
            nvs_set_i8(config, "FLAG_CONFIG", false);
            nvs_commit(config);
            nvs_close(config);
            esp_restart();
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
void app_main(void)
{
    char *ssid = NULL, *pass = NULL;
    deinit_gpio_rtc();
    init_gpio_into_setup();
    init_gpio_device();
    // enable_output(1);
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    xTaskCreate(task_check_setup, "SEPTUP", 2048 * 3, NULL, 10, NULL);
    //  get_key_device();
    bool flag_reset = get_info_last_wifi(&ssid, &pass);
    printf(" FLag reset%d\n", flag_reset);
    if (!flag_reset)
    {
        init_info_wifi();
        init_ap_mode();
    }
    else
    {
        printf(" Log INFO %s - %s \n", ssid, pass);

        init_wifi(ssid, pass);
    }

    
}
