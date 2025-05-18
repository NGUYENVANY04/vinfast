idf_component_register(SRCS "app_handle_query.c" INCLUDE_DIRS "include" PRIV_REQUIRES json driver
		driver_sleep_mode common_nvs app_request_https driver_enable_output)
