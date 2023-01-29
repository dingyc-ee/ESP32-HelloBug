#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "driver/gpio.h"

#define LED_GPIO        GPIO_NUM_4
#define LED_ON_LEVEL    0

extern void led_init(void);
extern void led_on(void);
extern void led_off(void);

#endif  /* __BSP_LED_H__ */
