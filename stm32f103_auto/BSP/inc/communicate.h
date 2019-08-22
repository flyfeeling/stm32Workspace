#ifndef __BSP_COMMUNICATE_H__
#define __BSP_COMMUNICATE_H__
/*public includes*/
#include "stm32f1xx_hal.h"
/*public macro*/

/*public variable extern*/
extern uint8_t com1Buffer[];
extern uint8_t com1State;
/*public  funtion decaleration*/
void BSP_COMMUNICATE_INIT(void);
#endif