# _LedC Project_

## 官方文档

LEDC外设有4个可选的高速和低速定时器，分别对应8个高速通道和8个低速通道。1个定时器可以驱动8路通道，所以LEDC PWM最多可以支持16路PWM波形。

![官方文档](https://ding-aliyun.oss-cn-shenzhen.aliyuncs.com/HelloBug/section3_ledc_dialog.jpg)

官方推荐的分频系数和计算方式如下：

![分频系数](https://ding-aliyun.oss-cn-shenzhen.aliyuncs.com/HelloBug/section3_ledc_feq.jpg)


## 官网例程 <font color='red'>*(非常重要)*</font>

### 工程配置

参考例程的PWM频率为5 kHz，占空比为50%，通过GPIO管脚输出。如要修改，通过调节 `LEDC_FREQUENCY`、`LEDC_DUTY`、`LEDC_OUTPUT_IO` 宏定义来完成。

根据所选的 `LEDC_FREQUENCY`，需要更改对应的 `LEDC_DUTY_RES`。

要常规设置占空比和频率，可以使用以下功能：

设置频率为2500Hz:

```c
ledc_set_freq(LEDC_MODE, LEDC_TIMER, 2500);
```

设置占空比到100%：

```c
ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 8191);
ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
```

要更改占空比，需要根据占空比分辨率计算占空比范围。

如果占空比分辨率为13 bits：

占空比范围：`0 to (2 ** 13) - 1 = 8191`，这里0表示占空比0%，8191表示占空比100%。

## Troubleshooting

* Duty Resolution

If you get the following error log `ledc: requested frequency and duty resolution can not be achieved, try reducing freq_hz or duty_resolution.` you need to change the `LEDC_DUTY_RES` to a lower resolution and change the range of the duty.

## 驱动代码

`bsp_ledc.c`
```c
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
```
`bsp_ledc.h`
```c
#ifndef __BSP_LEDC_H__
#define __BSP_LEDC_H__

extern void ledc_init(void);

#endif  /* __BSP_LEDC_H__ */
```

## 测试代码

```c
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
```

## 测试结果

![LEDC测试结果](https://ding-aliyun.oss-cn-shenzhen.aliyuncs.com/HelloBug/section3_test_result.jpg)
