#include <freertos/FreeRTOS.h>
#include "freertos/task.h"
#include "nvs_flash.h"
#include "driver_sleep_mode.h"
#include "app_handle_wifi_mode_sta.h"
#include "common_nvs.h"
bool flag_reset = true;
void app_main(void)
{
    /* init_gpio_into_setup();
    init_gpio_device();
    // check neu co du lieu thi se cau hinh hoac de xem da nhe hihi
    init_gpio_wakeup();
   */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);
    if (flag_reset)
    {
        init_ap_mode();
    }
    //   init_info_last_id_payload();
    // init_info_wifi();
    get_info_last_wifi(NULL, NULL);
    /*init_wifi();
     */
}

