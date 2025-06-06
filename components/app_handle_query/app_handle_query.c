#include <stdint.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdlib.h>
#include "app_handle_query.h"
#include "arch/sys_arch.h"
#include "esp_log.h"
#include <string.h>
#include "esp_sleep.h"
#include "hal/gpio_types.h"
#include "driver/gpio.h"
#include "driver_enable_output.h"
#include "app_request_https.h"
#include "common_nvs.h"
#include "driver_sleep_mode.h"
void handler_json_query(char *buffer_input, char *charger_id)
{
    uint32_t last_transaction_id = get_last_id_payload();
    ESP_LOGI("NVS", "%ld", last_transaction_id);
    cJSON *root = cJSON_Parse(buffer_input);
    if (!root)
    {
        printf("JSON parsing error!\n");
        return;
    }
    cJSON *status = cJSON_GetObjectItem(root, "status");
    if (!cJSON_IsNumber(status) || status->valueint != 200)
    {
        printf("API error!\n");
        cJSON_Delete(root);
        return;
    }

    cJSON *transactions = cJSON_GetObjectItem(root, "transactions");
    if (!cJSON_IsArray(transactions))
    {
        printf("No transactions found!\n");
        cJSON_Delete(root);
        return;
    }

    int found = 0;
    int transaction_count = cJSON_GetArraySize(transactions);
    for (int i = 0; i < transaction_count; i++)
    {
        cJSON *transaction = cJSON_GetArrayItem(transactions, i);
        cJSON *id = cJSON_GetObjectItem(transaction, "id");
        cJSON *content = cJSON_GetObjectItem(transaction, "transaction_content");
        cJSON *ref_number = cJSON_GetObjectItem(transaction, "reference_number");
        cJSON *amount_out = cJSON_GetObjectItem(transaction, "amount_out");
        cJSON *amount_in = cJSON_GetObjectItem(transaction, "amount_in");
        cJSON *time_pay = cJSON_GetObjectItem(transaction, "transaction_date");
        if (!cJSON_IsString(content) || !cJSON_IsString(ref_number) || !cJSON_IsString(id))
        {
            continue;
        }

        // Convert id payload to int
        int transaction_id = atoi(id->valuestring);

        // Only handle if ID > last_transaction_id
        if (transaction_id > last_transaction_id || last_transaction_id == 0)
        {
            // Check key
            if (strstr(content->valuestring, charger_id) != NULL)
            {
                printf("Giao dich hợp lệ:\n");
                printf("ID: %d\n", transaction_id);
                printf("Nội dung: %s\n", content->valuestring);
                printf("Mã tham chiếu: %s\n", ref_number->valuestring);
                printf("Số tiền: %s\n", amount_in->valuestring);
                printf("Thời gian thanh toán: %s\n", time_pay->valuestring);
                found = 1;
                // last_transaction_id = transaction_id; // update new id
                enable_output(amount_in->valueint);
                set_last_id_payload(transaction_id);
                set_last_time_payload(time_pay->valuestring);
                ESP_LOGW("HTTP", "Sleep mode start");
                init_gpio_wakeup();
                break; // get ID the first and break
            }
        }
    }
    cJSON_Delete(root);
    free(buffer_input);
    buffer_input = NULL;
    if (!found)
    {
        ESP_LOGW("HTTP", "Không có giao dịch mới phù hợp.\n");
        ESP_LOGW("HTTP", "Tiến hành kiểm tra lại hoặc sạc thủ công .\n");
        ESP_LOGW("HTTP", "Hệ thống sleep sau 5p .\n");

        vTaskDelay(2000);
        init_gpio_wakeup();
    }
}
