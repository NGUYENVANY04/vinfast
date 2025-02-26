#include <stdio.h>
#include "common_nvs.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_err.h"
nvs_handle_t wifi;
nvs_handle_t id;

int init_info_wifi(char *pass, char *ssid)
{
    esp_err_t err_open = nvs_open("WIFI", NVS_READWRITE, &wifi);
    if (err_open != ESP_OK)
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err_open));
        esp_err_t err_pass = nvs_set_str(wifi, "PASSWORD", "");
        esp_err_t err_ssid = nvs_set_str(wifi, "SSID", "");
        nvs_commit(wifi);
        nvs_close(wifi);
    }
    else
    {
        esp_err_t err_pass = nvs_set_str(wifi, "PASSWORD", pass);
        esp_err_t err_ssid = nvs_set_str(wifi, "SSID", ssid);
        printf("NVS %s - %s", esp_err_to_name(err_ssid), esp_err_to_name(err_pass));
        esp_err_t err_commit = nvs_commit(wifi);
        printf((err_commit != ESP_OK) ? "Failed!\n" : "Done\n");
        // Close
        nvs_close(wifi);
    }
    return err_open;
}
int init_info_old_id_payload(char *id_old)
{

    esp_err_t err_open = nvs_open("ID", NVS_READWRITE, &id);
    if (err_open != ESP_OK)
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err_open));
        esp_err_t err_id = nvs_set_str(wifi, "ID_OLD", "");
        nvs_commit(id);
        nvs_close(id);
    }
    else
    {
        esp_err_t err_id = nvs_set_str(id, "ID_OLD", id_old);

        printf("NVS %s ", esp_err_to_name(err_id));
        esp_err_t err_commit = nvs_commit(id);
        printf((err_commit != ESP_OK) ? "Failed!\n" : "Done\n");
        // Close
        nvs_close(id);
    }
    return err_open;
}
int get_old_id_payload(void)
{
    esp_err_t err_open = nvs_open("ID", NVS_READWRITE, &id);
    if (err_open != ESP_OK)
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err_open));
        esp_err_t err_id = nvs_set_str(wifi, "ID_OLD", "");
        nvs_commit(id);
        nvs_close(id);
    }
    else
    {
        esp_err_t err_id = nvs_set_str(id, "ID_OLD", id_old);

        printf("NVS %s ", esp_err_to_name(err_id));
        esp_err_t err_commit = nvs_commit(id);
        printf((err_commit != ESP_OK) ? "Failed!\n" : "Done\n");
        // Close
        nvs_close(id);
    }
    return err_open;
}
void save_wifi(void) { }
