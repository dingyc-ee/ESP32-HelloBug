#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "bsp_led.h"

#define TAG "LED"

void app_main(void)
{
    bool flag = false;
    led_init();

    while (true) {
        flag = !flag;
        if (flag) {
            ESP_LOGI(TAG, "ON");
            led_on();
        }
        else {
            ESP_LOGI(TAG, "OFF");
            led_off();
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
