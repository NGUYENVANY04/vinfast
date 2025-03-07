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

    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    // Is time set? If not, tm_year will be (1970 - 1900).
    if (timeinfo.tm_year < (2016 - 1900))
    {
        obtain_time();
        // update 'now' variable with current time
        time(&now);
    }
    // Set timezone to China Standard Time
    setenv("TZ", "GMT-7", 1);
    tzset();
    // int status = sntp_get_sync_status();
    // ESP_LOGW("STATUS", "%d", status);
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%Y-%m-%d+%H:%M:%S", &timeinfo);

    init_https(strftime_buf);
    ESP_LOGI(TAG, "The current date/time in Viet Nam is: %s", strftime_buf);
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

