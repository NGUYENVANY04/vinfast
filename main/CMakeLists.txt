idf_component_register(SRCS "main.c" 
		       INCLUDE_DIRS "." 
		       REQUIRES mbedtls nvs_flash app_wifi common app_request_https driver_gpio)
