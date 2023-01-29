# _Led project_

蓝色发光LED硬件接线图如下：

![LED低电平点亮](https://ding-aliyun.oss-cn-shenzhen.aliyuncs.com/HelloBug/section2_led_power.jpg)
![ESP32管脚分配](https://ding-aliyun.oss-cn-shenzhen.aliyuncs.com/HelloBug/section2_led_gpio.jpg)


## 驱动组件

1. 新建`component`组件，增加led驱动文件

![新增led组件](https://ding-aliyun.oss-cn-shenzhen.aliyuncs.com/HelloBug/section2_led_component.jpg)

2. led驱动包含3个文件：`bsp_led.c`，`bsp_led.h`，`CMakeLists.txt`，下面是驱动源码。

*`bsp_led.c`*
```c
#include <stdio.h>
#include "bsp_led.h"

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

```
*`bsp_led.h`*
```c
#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "driver/gpio.h"

#define LED_GPIO        GPIO_NUM_4
#define LED_ON_LEVEL    0

extern void led_init(void);
extern void led_on(void);
extern void led_off(void);

#endif  /* __BSP_LED_H__ */
```
*`CMakeLists.txt`*
```c
idf_component_register(SRCS "bsp_led.c"
                    INCLUDE_DIRS ".")
```

## 测试源码

```c
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
```

## 测试结果

![LED驱动测试结果](https://ding-aliyun.oss-cn-shenzhen.aliyuncs.com/HelloBug/section2_led_driver_result.jpg)