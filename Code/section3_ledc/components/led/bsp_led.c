#include <stdio.h>
#include "driver/gpio.h"
#include "bsp_led.h"

#define LED_GPIO        GPIO_NUM_4
#define LED_ON_LEVEL    0

void led_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << LED_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);
    gpio_set_level(LED_GPIO, !LED_ON_LEVEL);
}

void led_on(void)
{
    gpio_set_level(LED_GPIO, LED_ON_LEVEL);
}

void led_off(void)
{
    gpio_set_level(LED_GPIO, !LED_ON_LEVEL);
}
