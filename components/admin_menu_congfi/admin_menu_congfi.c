#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include <esp_http_server.h>
#include "esp_err.h"
#include "esp_event.h"
#include "http_parser.h"
#include "nvs.h"
#include <esp_wifi.h>
#include <esp_system.h>
#include "common_nvs.h"
#include "driver_sleep_mode.h"
static void handle_result(char *result);
static const char *TAG = "example";
const char success_page[] = "<!DOCTYPE HTML><html><head>\
                     <meta charset=\"UTF-8\">\
                     <title>Cấu hình thành công</title>\
                     <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
                     <style>\
                        * { margin: 0; padding: 0; box-sizing: border-box; }\
                        body { font-family: Arial, sans-serif; background: linear-gradient(135deg, #6e8efb, #a777e3); display: flex; height: 100vh; align-items: center; justify-content: center; text-align: center; }\
                        .container { max-width: 400px; background: white; padding: 25px; border-radius: 12px; box-shadow: 0 6px 15px rgba(0, 0, 0, 0.2); animation: fadeIn 1s ease-in-out; }\
                        @keyframes fadeIn { from { opacity: 0; transform: translateY(-10px); } to { opacity: 1; transform: translateY(0); } }\
                        h1 { color: #333; font-size: 22px; margin-bottom: 10px; }\
                        p { color: #666; font-size: 16px; margin-bottom: 20px; }\
                        button { width: 100%; padding: 12px; border: none; border-radius: 6px; background-color: #6e8efb; color: white; font-size: 18px; cursor: pointer; transition: 0.3s; }\
                        button:hover { background-color: #5a7de9; transform: scale(1.05); }\
                     </style>\
                     </head><body>\
                     <div class=\"container\">\
                     <h1>Cấu hình thành công!</h1>\
                     <p>Thiết lập của bạn đã được lưu.</p>\
                     <button onclick=\"window.location.href='/'\">Quay lại</button>\
                     </div>\
                     </body></html>";
const char ui_config[] = "<!DOCTYPE HTML><html><head>\
  <meta charset=\"UTF-8\">\
  <title>Cấu hình hệ thống trạm sạc</title>\
  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
  <style>\
    * { margin: 0; padding: 0; box-sizing: border-box; }\
    body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background: linear-gradient(135deg, #667eea, #764ba2); display: flex; height: 100vh; align-items: center; justify-content: center; }\
    .container { width: 100%; max-width: 420px; background: #fff; padding: 30px 25px; border-radius: 16px; box-shadow: 0 10px 25px rgba(0, 0, 0, 0.15); animation: fadeIn 1s ease-in-out; }\
    @keyframes fadeIn { from { opacity: 0; transform: translateY(20px); } to { opacity: 1; transform: translateY(0); } }\
    h1 { color: #333; font-size: 24px; margin-bottom: 25px; }\
    label { font-weight: 600; display: block; text-align: left; color: #555; font-size: 15px; margin-top: 15px; }\
    input { width: 100%; padding: 12px 14px; border: 1.5px solid #ccc; border-radius: 8px; margin-top: 8px; font-size: 16px; transition: border-color 0.3s ease, box-shadow 0.3s ease; }\
    input:focus { border-color: #667eea; outline: none; box-shadow: 0 0 8px rgba(102, 126, 234, 0.4); }\
    button { width: 100%; padding: 14px; border: none; border-radius: 8px; background: linear-gradient(135deg, #667eea, #764ba2); color: white; font-size: 18px; font-weight: 600; cursor: pointer; transition: transform 0.2s ease, box-shadow 0.2s ease; margin-top: 25px; }\
    button:hover { transform: scale(1.05); box-shadow: 0 6px 12px rgba(102, 126, 234, 0.4); }\
  </style>\
  </head><body>\
  <div class=\"container\">\
    <h1>Cấu hình trạm sạc</h1>\
    <form action=\"/config\" method=\"post\">\
      <label for=\"ssid\">WiFi SSID:</label>\
      <input type=\"text\" id=\"ssid\" name=\"ssid\" required>\
      <label for=\"pass\">WiFi Password:</label>\
      <input type=\"password\" id=\"pass\" name=\"pass\" required>\
      <label for=\"cost\">Chi phí (VNĐ/kWh):</label>\
      <input type=\"number\" id=\"cost\" name=\"cost\" required>\
      <label for=\"id\">ID Trụ sạc:</label>\
      <input type=\"text\" id=\"id\" name=\"id\" required>\
      <button type=\"submit\">Lưu cấu hình</button>\
    </form>\
  </div>\
  </body></html>";
static esp_err_t post_handler(httpd_req_t *req)
{
    char buf[100];
    int ret, remaining = req->content_len;

    /* Read the data for the request */
    if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)))) <= 0)
    {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT)
        {
            httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)));
            /* Retry receiving if timeout occurred */
        }
        return ESP_FAIL;
    }
    buf[ret] = '\0';

    /* Log data received */
    ESP_LOGI(TAG, "=========== RECEIVED DATA ==========");
    ESP_LOGI(TAG, "%.*s", ret, buf);
    ESP_LOGI(TAG, "====================================");
    httpd_resp_send(req, success_page, HTTPD_RESP_USE_STRLEN);
    esp_err_t err_open_wifi = nvs_open("CONFIG", NVS_READWRITE, &config);
    handle_result(buf);
    if (nvs_commit(config) == ESP_OK)
    {
        nvs_close(config);
        init_gpio_wakeup();
    }
    return ESP_OK;
}
esp_err_t get_handler(httpd_req_t *req)
{
    /* Send a simple response */
    httpd_resp_send(req, ui_config, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}
static const httpd_uri_t get = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = get_handler,
    .user_ctx = NULL,
};
static const httpd_uri_t post = {
    .uri = "/config",
    .method = HTTP_POST,
    .handler = post_handler,
    .user_ctx = NULL,
};

static esp_err_t stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server,
    return httpd_stop(server);
}
httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK)
    {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &get);
        httpd_register_uri_handler(server, &post);
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

static void handle_result(char *result)
{
    char *token;
    int i = 0;
    token = strtok(result, "&");
    while (token != NULL)
    {
        while (token[i++] != '=') { }
        // printf("%s\n", &token[i]);
        save_config(&token[i]);
        i = 0;
        token = strtok(NULL, "&");
    }
    ESP_ERROR_CHECK(nvs_set_i8(config, "FLAG_CONFIG", true));
    ESP_LOGI("NVS", "SAVE Config Flag Is True");
}

