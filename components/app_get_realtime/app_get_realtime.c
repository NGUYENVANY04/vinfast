#include "app_get_realtime.h"
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "esp_system.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_netif_sntp.h"
#include "lwip/ip_addr.h"
#include "esp_sntp.h"
#include "common_nvs.h"
#include "app_request_https.h"

static const char *TAG = "TIME";

static void obtain_time(void);

void init_time(void)
{
    char strftime_buf[64];
    char *timelast = NULL;
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    if (timeinfo.tm_year < (2016 - 1900))
    {
        obtain_time();
        time(&now);
    }
    setenv("TZ", "GMT-7", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%Y-%m-%d+%H:%M:%S", &timeinfo);
    if (strftime_buf[0] != '1')
    {
        ESP_LOGI(TAG, "The current date/time in Viet Nam is: %s", strftime_buf);
        init_https(strftime_buf);
    }
    else
    {
        ESP_LOGI(TAG, "The current date/time in default is: %s", strftime_buf);
        get_last_time_payload(&timelast);
        ESP_LOGI(TAG, "The last date/time request is: %s", timelast);
        if (timelast == NULL)
        {
            ESP_LOGE("TIME Error", "TIME Error");
            // Handler
        }
        else
        {
            init_https(timelast);
        }
    }
}
static void obtain_time(void)
{

    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
    sntp_set_sync_mode(SNTP_OPMODE_POLL);
    esp_netif_sntp_init(&config);

    // time_t now = 0;
    // struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 15;
    while (esp_netif_sntp_sync_wait(2000 / portTICK_PERIOD_MS) == ESP_ERR_TIMEOUT
           && ++retry < retry_count)
    {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
    }
    // time(&now);
    // localtime_r(&now, &timeinfo);
}
char *handle_time(char *realtime)
{

    // Tách các thành phần thời gian
    int year, month, day, hour, minute, second;
    sscanf(realtime, "%4d-%2d-%2d+%2d:%2d:%2d", &year, &month, &day, &hour, &minute, &second);

    // Trừ đi 15 phút
    minute -= 15;
    if (minute < 0)
    {
        minute += 60;
        hour--;
        if (hour < 0)
        {
            hour = 23;
            day--;
            if (day < 1)
            {
                // Xử lý số ngày của tháng trước
                month--;
                if (month < 1)
                {
                    month = 12;
                    year--;
                }
                // Số ngày trong tháng trước
                int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
                if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0))
                {
                    days_in_month[1] = 29; // Năm nhuận
                }
                day = days_in_month[month - 1];
            }
        }
    }

    // Cập nhật lại chuỗi thời gian
    snprintf(realtime, 20, "%04d-%02d-%02d+%02d:%02d:%02d", year, month, day, hour, minute, second);

    printf("Thời gian sau khi trừ 5 phút: %s\n", realtime);
    return realtime;
}

