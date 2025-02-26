#ifndef _COMMON_NVS_H_
#define _COMMON_NVS_H_
#include "nvs.h"
extern nvs_handle_t id;
extern nvs_handle_t wifi;
int init_info_wifi(char *pass, char *ssid);
int init_info_old_id_payload(char *id);
void get_old_id_payload(void);
void save_wifi(void);
#endif
