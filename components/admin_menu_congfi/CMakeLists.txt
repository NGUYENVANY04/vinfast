idf_component_register(SRCS "admin_menu_congfi.c" INCLUDE_DIRS
			    "include" REQUIRES esp_http_server esp_eth nvs_flash esp_wifi 
			   driver_sleep_mode esp-tls common_nvs)

