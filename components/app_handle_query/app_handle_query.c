#include <stdio.h>
#include <cJSON.h>
#include <stdlib.h>
#include "include/app_handle_query.h"
#include "arch/sys_arch.h"
#include "esp_log.h"
#include <string.h>
#include "esp_sleep.h"
#include "hal/gpio_types.h"
#include "driver/gpio.h"
#include "driver_enable_output.h"
#include "app_request_https.h"
#include "common_nvs.h"
void handler_json_query(char *buffer_input, char *charger_id)
{
    static int last_transaction_id = 0;

    if (init_info_old_id_payload(charger_id) == 0)
    {
        last_transaction_id = 0;
    }
else {
last_transaction_id= 
}
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
            if (strstr(content->valuestring, charger_id) != NULL
                && strstr(amount_out->valuestring, "0.00") != NULL)
            {
                printf("Giao dich hợp lệ:\n");
                printf("ID: %d\n", transaction_id);
                printf("Nội dung: %s\n", content->valuestring);
                printf("Mã tham chiếu: %s\n", ref_number->valuestring);
                printf("Số tiền: %s\n", amount_in->valuestring);
                found = 1;
                last_transaction_id = transaction_id; // update new id
                enable_output();
                vTaskDelay(5000);
                ESP_LOGW("HTTP", "Sleep mode start");
                esp_deep_sleep_start();
                break; // get ID the first and break
            }
        }
    }

    if (!found)
    {
        printf("Không có giao dịch mới phù hợp.\n");
        printf("Tiến hành sạc thủ công .\n");
    }
    cJSON_Delete(root);
    free(buffer_input);
}
