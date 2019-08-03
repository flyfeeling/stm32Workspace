#ifndef __BSP_DELAY_H__
#define __BSP_DELAY_H__
/*public includes*/
#include "stm32f1xx_hal.h"
/*public macro*/

/*public variable extern*/
void BSP_DELAY(uint32_t s,uint32_t ms, uint32_t us);
void BSP_DELAY_MS(uint32_t ms);
void BSP_DELAY_US(uint16_t us);
void BSP_DELAY_S(uint32_t s);
/*public  funtion decaleration*/

#endif

