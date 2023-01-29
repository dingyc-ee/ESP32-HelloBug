# _Sample project_

使用的芯片为ESP32-WROOM-32E 4MB模块。下载/调试串口接线如下：

![调试串口接线](https://ding-aliyun.oss-cn-shenzhen.aliyuncs.com/HelloBug/section1_log_uart.jpg)



## 测试代码
```c
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
```

## 测试结果

![测试结果](https://ding-aliyun.oss-cn-shenzhen.aliyuncs.com/HelloBug/section1_log_result.jpg)
