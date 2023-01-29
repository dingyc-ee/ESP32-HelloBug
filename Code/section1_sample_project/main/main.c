#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG     "LOOP"

void app_main(void)
{
    int count = 0;

    while (true) {
        ESP_LOGI(TAG, "count = %d", count++);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
