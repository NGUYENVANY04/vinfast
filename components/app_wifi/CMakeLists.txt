idf_component_register(SRCS "app_wifi.c" INCLUDE_DIRS "include" PRIV_REQUIRES esp_wifi common_nvs
			   REQUIRES driver_sleep_mode esp_event app_request_https app_get_realtime)
