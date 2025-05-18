#ifndef _APP_WIFI_H_
#define _APP_WIFI_H_
/**
 * @brief  Initialize WiFi
 *         Allocate resource for WiFi driver, such as WiFi control structure, RX/TX buffer,
 *         WiFi NVS structure
 *         Allocate resource for Wifi
 *
 * @attention 1. This API must be called before all request
 * @attention 2. Always use WIFI_INIT_CONFIG_DEFAULT macro to initialize the configuration to
 * default values, this can guarantee all the fields get correct value when more fields are added
 * into wifi_init_config_t in future release. If you want to set your own initial values, overwrite
 * the default values which are set by WIFI_INIT_CONFIG_DEFAULT. Please be notified that the field
 * 'magic' of wifi_init_config_t should always be WIFI_INIT_CONFIG_MAGIC!
 * @param  void
 *
 * @return void
 **/
void init_wifi(char *ssid, char *pass);
#endif
