#include <stdint.h>
#include <stdio.h>
#include "common_nvs.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_err.h"
nvs_handle_t wifi;
nvs_handle_t id;
uint32_t last_id = 0;

void init_info_wifi(void)
{
    esp_err_t err_open_wifi = nvs_open("WIFI", NVS_READWRITE, &wifi);
    printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err_open_wifi));
    printf("NVS init info wifi\n");
    esp_err_t err_pass = nvs_set_str(wifi, "PASSWORD", "");
    esp_err_t err_ssid = nvs_set_str(wifi, "SSID", "");
    nvs_commit(wifi);
    nvs_close(wifi);
}

void init_info_last_id_payload(void)
{
    esp_err_t err_open_id = nvs_open("ID", NVS_READWRITE, &id);
    if (err_open_id)
    {
        esp_err_t err_id = nvs_set_u32(id, "ID_LAST", 0);
        printf("NVS init payload %s ", esp_err_to_name(err_id));
        esp_err_t err_commit = nvs_commit(id);
    }
    printf((err_open_id != ESP_OK) ? "Failed!\n" : "Done\n");
    nvs_close(id);
}
uint32_t get_last_id_payload(void)
{
    esp_err_t err_open = nvs_open("ID", NVS_READONLY, &id);
    esp_err_t err_id = nvs_get_u32(id, "ID_LAST", &last_id);
    printf("NVS %s - NVS %s - data get = %ld\n", esp_err_to_name(err_id), esp_err_to_name(err_open),
           last_id);
    nvs_close(id);
    return last_id;
}
void set_last_id_payload(uint32_t id_last)
{
    printf("Set ID to nvs %lu\n :", id_last);
    ESP_ERROR_CHECK(nvs_open("ID", NVS_READWRITE, &id));
    ESP_ERROR_CHECK(nvs_set_u32(id, "ID_LAST", id_last));
    ESP_ERROR_CHECK(nvs_commit(id));
    nvs_close(id);
}
void save_wifi(char *pass, char *ssid)
{
    esp_err_t err_open_wifi_set = nvs_open("WIFI", NVS_READWRITE, &wifi);
    printf("Error (%s) opening WIFI SAVE handle!\n", esp_err_to_name(err_open_wifi_set));
    esp_err_t err_pass = nvs_set_str(wifi, "PASSWORD", pass);
    esp_err_t err_ssid = nvs_set_str(wifi, "SSID", ssid);
    nvs_commit(wifi);
    nvs_close(wifi);
}

