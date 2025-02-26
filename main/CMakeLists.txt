idf_component_register(SRCS "main.c" INCLUDE_DIRS "." REQUIRES mbedtls nvs_flash app_wifi
			   app_handle_query driver_sleep_mode common app_request_https driver_gpio)
