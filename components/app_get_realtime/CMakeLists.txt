idf_component_register(SRCS "app_get_realtime.c" INCLUDE_DIRS
			    "include" REQUIRES app_request_https esp_netif common_nvs lwip)
