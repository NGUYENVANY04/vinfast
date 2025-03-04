idf_component_register(SRCS "main.c" INCLUDE_DIRS "." REQUIRES mbedtls nvs_flash app_wifi
			   app_handle_query driver_sleep_mode app_get_realtime common_nvs common
			       app_request_https driver_gpio app_get_realtime)
