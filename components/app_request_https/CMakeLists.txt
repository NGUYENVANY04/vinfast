idf_component_register(SRCS "app_request_https.c" INCLUDE_DIRS "include" PRIV_REQUIRES esp-tls 
driver_sleep_mode app_get_realtime common_nvs esp_http_client json app_handle_query)
