#ifndef _COMMON_NVS_H_
#define _COMMON_NVS_H_
#include "nvs.h"
#include <stdbool.h>
#include <stdint.h>
extern nvs_handle_t config;
extern nvs_handle_t id;
extern nvs_handle_t time_handler;
extern nvs_handle_t key_device;

extern bool flag_reset;

void init_info_wifi(void);
void init_info_last_id_payload(void);
uint32_t get_last_id_payload(void);
void set_last_time_payload(const char *time);
void save_wifi(char *pass, char *ssid);
void set_last_id_payload(const uint32_t id);
void save_config(const char *result_parse);
bool get_info_last_wifi(char **ssid, char **pass);
void get_last_time_payload(char **time);
void set_key_device(const char *key_device);
void get_key_device(char **key);
#endif
