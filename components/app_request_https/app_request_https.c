
#include <stdio.h>
#include "app_request_https.h"
#include "esp_http_client.h"
#include <esp_log.h>
#include "esp_err.h"
#include "esp_tls.h"
#include "cJSON.h"
#include "string.h"
#define MIN(index_1, index_2) (index_1 < index_2 ? index_1 : index_2)
#define HTTP_RECEIVE_BUFFER_SIZE 1024
#define CONFIG_CALE_BEARER_TOKEN "IGFVJTEYH2IJFYZJLO3STQ901Q4UCTXDPBU2SQD9NYZVGMXLXO1NCZDYUJSP4IK7"
#define MAX_HTTP_OUTPUT_BUFFER 2048

const char *TAG = "HTTPS";
char bearerToken[100] = "";

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    static char *output_buffer; // Buffer to store response of http request from event handler
    static int output_len; // Stores number of bytes read
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
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
        ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        ESP_LOGI(TAG, "Received chunk: %.*s", evt->data_len, (char *)evt->data);

        if (!esp_http_client_is_chunked_response(evt->client))
        {

            // Lần đầu nhận dữ liệu, cấp phát bộ nhớ theo Content-Length
            if (output_buffer == NULL)
            {
                int buffer_len = esp_http_client_get_content_length(evt->client);
                if (buffer_len <= 0)
                {
                    buffer_len = MAX_HTTP_OUTPUT_BUFFER; // Dự phòng nếu không có Content-Length
                }
                output_buffer = (char *)malloc(buffer_len);
                output_len = 0;

                if (output_buffer == NULL)
                {
                    ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                    return ESP_FAIL;
                }
            }

            // Sao chép dữ liệu vào buffer
            int copy_len = MIN(evt->data_len, (MAX_HTTP_OUTPUT_BUFFER - output_len));
            if (copy_len > 0)
            {
                memcpy(output_buffer + output_len, evt->data, copy_len);
                output_len += copy_len;
            }
        }
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
        if (output_buffer != NULL)
        {
            // Response is accumulated in output_buffer. Uncomment the below line to print the
            // accumulated response ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
            free(output_buffer);
            output_buffer = NULL;
        }
        output_len = 0;
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
        if (output_buffer != NULL)
        {
            free(output_buffer);
            output_buffer = NULL;
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
void init_https(void)
{
    esp_http_client_config_t config = {
        .url = "https://my.sepay.vn/userapi/transactions/count",
        .event_handler = _http_event_handler,
        .method = HTTP_METHOD_GET,
        .timeout_ms = 9000,
        .buffer_size = HTTP_RECEIVE_BUFFER_SIZE,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    // Thêm Bearer Token
    esp_http_client_set_header(
        client, "Authorization",
        "Bearer IGFVJTEYH2IJFYZJLO3STQ901Q4UCTXDPBU2SQD9NYZVGMXLXO1NCZDYUJSP4IK7");

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %" PRIu64,
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
    }
    else
    {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}

