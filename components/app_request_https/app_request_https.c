#include <stdbool.h>
#include <stdio.h>
#include "app_request_https.h"
#include "arch/sys_arch.h"
#include "esp_http_client.h"
#include <esp_log.h>
#include <stdlib.h>
#include <string.h>
#include "esp_err.h"
#include "esp_system.h"
#include "esp_tls.h"
#include "app_handle_query.h"
#include "string.h"
#include "common_nvs.h"
#include "driver_sleep_mode.h"
#include "app_get_realtime.h"
#define MIN(index_1, index_2) (index_1 < index_2 ? index_1 : index_2)
#define HTTP_RECEIVE_BUFFER_SIZE 1024
#define CONFIG_CALE_BEARER_TOKEN "IGFVJTEYH2IJFYZJLO3STQ901Q4UCTXDPBU2SQD9NYZVGMXLXO1NCZDYUJSP4IK7"
#define MAX_HTTP_OUTPUT_BUFFER 2048

const char *TAG = "HTTPS";
char *output_buffer = NULL; // Buffer to store response of http request from event handler
bool flag = false;
static int output_len; // Stores number of bytes read
esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{

    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
        init_gpio_wakeup();
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d data= %s", evt->data_len, (char *)evt->data);
        if (flag)
        {
            output_buffer = realloc(output_buffer, output_len + evt->data_len + 1);
            memcpy(output_buffer + output_len, evt->data, evt->data_len);
            output_len += evt->data_len;
            output_buffer[output_len] = '\0';
        }
        else
        {
            flag = true;
            output_buffer = malloc(evt->data_len);
            memcpy(output_buffer, evt->data, evt->data_len);
            output_len += evt->data_len;
            if (output_buffer == NULL)
            {
                ESP_LOGE(TAG, "Failed to allocate memory!");
                free(output_buffer);
                output_buffer = NULL;
                output_len = 0;
            }
        }
        break;

    case HTTP_EVENT_ON_FINISH:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
        if (output_buffer != NULL)
        {
            handler_json_query(output_buffer, "TRUSAC1");
            ESP_LOGI("HTTPS", "%s", output_buffer);
            // TRUSAC1 is key in response
        }
        output_len = 0;
        flag = false;
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
        int mbedtls_err = 0;
        esp_err_t err =
            esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
        if (err != 0)
        {
            ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
            ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
        }

        output_len = 0;
        break;
    case HTTP_EVENT_REDIRECT:
        ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
        esp_http_client_set_header(evt->client, "From", "user@example.com");
        esp_http_client_set_header(evt->client, "Accept", "text/html");
        esp_http_client_set_redirection(evt->client);
        break;
    }
    return ESP_OK;
}
void init_https(char *time)
{

    char buffer[87];
    sprintf(buffer, "%s%s", "https://my.sepay.vn/userapi/transactions/list?transaction_date_min=",
            handle_time(time));
    ESP_LOGW("Test", "%s", buffer);

    esp_http_client_config_t config = {
        .url = buffer,
        .event_handler = _http_event_handler,
        .method = HTTP_METHOD_GET,
        .timeout_ms = 30000,
        .buffer_size = DEFAULT_HTTP_BUF_SIZE,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(
        client, "Authorization",
        "Bearer IGFVJTEYH2IJFYZJLO3STQ901Q4UCTXDPBU2SQD9NYZVGMXLXO1NCZDYUJSP4IK7");
    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}
