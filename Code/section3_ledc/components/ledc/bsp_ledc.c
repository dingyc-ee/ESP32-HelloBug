#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "bsp_ledc.h"

#define LEDC_GPIO               GPIO_NUM_4
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_HIGH_SPEED_MODE
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT
#define LEDC_FREQUENCY          5000

#define LEDC_TEST_DUTY          (pow(2, LEDC_DUTY_RES) - 1)
#define LEDC_TEST_FADE_TIME     2000

void ledc_init(void)
{
    // LEDC定时器配置
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .duty_resolution = LEDC_DUTY_RES,
        .timer_num = LEDC_TIMER,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // LEDC通道配置
    ledc_channel_config_t ledc_channel = {
        .gpio_num   = LEDC_GPIO,
        .speed_mode = LEDC_MODE,
        .channel    = LEDC_CHANNEL,
        .intr_type  = LEDC_INTR_DISABLE,
        .timer_sel  = LEDC_TIMER,
        .duty       = 0,
        .hpoint     = 0,
        .flags.output_invert = 0,
    };
    ledc_channel_config(&ledc_channel);

    // 注册LEDC服务，0表示不允许中断
    ledc_fade_func_install(0);

    while (1) {
        printf("DutyCycle: %.0f%% -> %.0f%%\n", 0 * 100 / ((pow(2, LEDC_DUTY_RES) - 1)), LEDC_TEST_DUTY * 100 / ((pow(2, LEDC_DUTY_RES) - 1)));
        ledc_set_fade_with_time(ledc_channel.speed_mode, ledc_channel.channel, LEDC_TEST_DUTY, LEDC_TEST_FADE_TIME);
        ledc_fade_start(ledc_channel.speed_mode, ledc_channel.channel, LEDC_FADE_WAIT_DONE);

        printf("DutyCycle: %.0f%% -> %.0f%%\n", LEDC_TEST_DUTY * 100 / ((pow(2, LEDC_DUTY_RES) - 1)), 0 * 100 / ((pow(2, LEDC_DUTY_RES) - 1)));
        ledc_set_fade_with_time(ledc_channel.speed_mode, ledc_channel.channel, 0, LEDC_TEST_FADE_TIME);
        ledc_fade_start(ledc_channel.speed_mode, ledc_channel.channel, LEDC_FADE_WAIT_DONE);
    }
}
