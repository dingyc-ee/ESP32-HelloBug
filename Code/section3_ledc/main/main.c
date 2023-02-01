#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "bsp_ledc.h"

#define TAG "LEDC"

void app_main(void)
{
    ledc_init();

    while (true) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
