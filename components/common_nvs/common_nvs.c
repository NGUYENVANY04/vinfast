
#include <stdlib.h>
#include <string.h>
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
nvs_handle_t time_handler;
nvs_handle_t key_handler;

void init_info_wifi(void)
{
    esp_err_t err_open_wifi = nvs_open("CONFIG", NVS_READWRITE, &config);
    printf("NVS init info wifi\n");
    esp_err_t err_pass = nvs_set_str(config, "PASS", "");
    esp_err_t err_ssid = nvs_set_str(config, "SSID", "");
    nvs_commit(config);
    nvs_close(config);
}

void init_info_last_id_payload(void)
{
    esp_err_t err_open_id = nvs_open("ID", NVS_READWRITE, &id);
    esp_err_t err_id = nvs_set_u32(id, "ID_LAST", 0);
    printf("NVS init payload %s ", esp_err_to_name(err_id));
    nvs_commit(id);
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
    printf("\nSet TIME to nvs %s :", time_last);
    nvs_open("TIME", NVS_READWRITE, &time_handler);
    nvs_set_str(time_handler, "TIME_LAST", time_last);
    nvs_commit(time_handler);
    nvs_close(time_handler);
}
void get_last_time_payload(char **time)
{
    char timelast[51];
    size_t length_time = 50;
    nvs_open("TIME", NVS_READWRITE, &time_handler);
    nvs_get_str(time_handler, "TIME_LAST", timelast, &length_time);
    nvs_close(time_handler);
    if (time != NULL)
    {
        *time = malloc(length_time);
        if (*time != NULL)
        {
            strcpy(*time, timelast);
        }
    }
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
    esp_err_t err_open_wifi_set = nvs_open("WIFI", NVS_READWRITE, &config);
    printf("Error (%s) opening WIFI SAVE handle!\n", esp_err_to_name(err_open_wifi_set));
    esp_err_t err_pass = nvs_set_str(config, "PASSWORD", pass);
    esp_err_t err_ssid = nvs_set_str(config, "SSID", ssid);
    nvs_commit(config);
    nvs_close(config);
}

bool get_info_last_wifi(char **ssid, char **pass)
{
    ESP_LOGI("NVS", "GET INFO CONFIG");
    char ssid_bf[51] = { 0 };
    char pass_bf[51] = { 0 };
    int8_t flag_config = 0;
    size_t length_ssid = sizeof(ssid_bf);
    size_t length_pass = sizeof(pass_bf);

    if (nvs_open("CONFIG", NVS_READONLY, &config) != ESP_OK)
    {
        ESP_LOGE("NVS", "Failed to open NVS");
        return false;
    }

    esp_err_t err1 = nvs_get_str(config, "SSID", ssid_bf, &length_ssid);
    esp_err_t err2 = nvs_get_str(config, "PASS", pass_bf, &length_pass);
    esp_err_t err3 = nvs_get_i8(config, "FLAG_CONFIG", &flag_config);

    nvs_close(config);

    if (err1 != ESP_OK || err2 != ESP_OK || err3 != ESP_OK)
    {
        ESP_LOGE("NVS", "Failed to read values from NVS");
        return false;
    }

    if (ssid != NULL)
    {
        *ssid = malloc(length_ssid);
        if (*ssid)
            strcpy(*ssid, ssid_bf);
    }

    if (pass != NULL)
    {
        *pass = malloc(length_pass);
        if (*pass)
            strcpy(*pass, pass_bf);
    }

    return flag_config;
}
void save_config(const char *result_parse)
{
    static int count = 0;

    switch (count)
    {
    case 0:

        ESP_ERROR_CHECK(nvs_set_str(config, "SSID", result_parse));
        ESP_LOGI("NVS", "SAVE SSID %s", result_parse);
        count++;
        break;
    case 1:
        nvs_set_str(config, "PASS", result_parse);
        ESP_LOGI("NVS", "SAVE PASS %s", result_parse);
        count++;
        break;
    case 2:
        nvs_set_str(config, "COST", result_parse);
        ESP_LOGI("NVS", "SAVE COST %s", result_parse);
        count++;
        break;
    default:
        nvs_set_str(config, "ID", result_parse);
        ESP_LOGI("NVS", "SAVE ID %s", result_parse);
        break;
    }
}

void get_key_device(char **key)
{
    char key_device[21];
    size_t length_key = 20;
    nvs_open("ID", NVS_READONLY, &key_handler);
    nvs_get_str(key_handler, "KEY_DEVICE", key_device, &length_key);
    nvs_close(key_handler);
    if (key != NULL)
    {
        *key = malloc(21);
        if (*key != NULL)
        {
            strcpy(*key, key_device);
        }
    }
}
void set_key_device(const char *key_device)
{
    printf("\nSet KEY to nvs %s :", key_device);
    ESP_ERROR_CHECK(nvs_open("KEY_DEVICE", NVS_READWRITE, &key_handler));
    ESP_ERROR_CHECK(nvs_set_str(key_handler, "KEY_DEVICE", key_device));
    ESP_ERROR_CHECK(nvs_commit(key_handler));
    nvs_close(key_handler);
}

