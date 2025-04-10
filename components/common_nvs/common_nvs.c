#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "common_nvs.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_err.h"
nvs_handle_t config;
nvs_handle_t id;
nvs_handle_t time_hanlde;
nvs_handle_t wifi;
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
    uint32_t last_id = 0;
    esp_err_t err_open = nvs_open("ID", NVS_READONLY, &id);
    esp_err_t err_id = nvs_get_u32(id, "ID_LAST", &last_id);
    nvs_close(id);
    return last_id;
}
void set_last_time_payload(const char *time_last)
{
    printf("Set TIME to nvs %s :", time_last);
    ESP_ERROR_CHECK(nvs_open("TIME", NVS_READWRITE, &time_hanlde));
    ESP_ERROR_CHECK(nvs_set_str(time_hanlde, "TIME_LAST", time_last));
    ESP_ERROR_CHECK(nvs_commit(time_hanlde));
    nvs_close(time_hanlde);
}
void set_last_id_payload(const uint32_t id_last)
{
    printf("Set ID to nvs %d :", (int)id_last);
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

bool get_info_last_wifi(char **ssid, char **pass)
{
    nvs_open("CONFIG", NVS_READONLY, &config);
    char pass_bf[50];
    char ssid_bf[50];
    size_t length_pass, length_ssid;
    length_pass = 100;
    length_ssid = 50;
    nvs_get_str(config, "SSID", ssid_bf, &length_ssid);
    nvs_get_str(config, "PASS", pass_bf, &length_pass);
    printf("%s - %s", pass_bf, ssid_bf);
    nvs_close(wifi);
    return pass_bf != NULL && ssid_bf != NULL;
}
void save_config(const char *result_parse)
{
    static int count = 0;

    switch (count)
    {
    case 0:

        ESP_ERROR_CHECK(nvs_set_str(config, "SSID", result_parse));
        ESP_LOGI("NVS", "SAVE 0 %s", result_parse);
        count++;
        break;
    case 1:
        nvs_set_str(config, "PASS", result_parse);
        ESP_LOGI("NVS", "SAVE 1 %s", result_parse);
        count++;
        break;
    case 2:
        nvs_set_str(config, "COST", result_parse);
        ESP_LOGI("NVS", "SAVE 2 %s", result_parse);
        count++;
        break;
    default:
        nvs_set_str(config, "ID", result_parse);
        ESP_LOGI("NVS", "SAVE 3 %s", result_parse);
        break;
    }
}

