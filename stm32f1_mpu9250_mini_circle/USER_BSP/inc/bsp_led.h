#ifndef __BSP_LED_H__
#define __BSP_LED_H__
#include "stm32f1xx_hal.h"
void BSP_LED_ON(uint8_t led);
void BSP_LED_OFF(uint8_t led);
void BSP_LED_TOGGLE(uint8_t led);
#endif

