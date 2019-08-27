#ifndef __BSP_COMM_H__
#define __BSP_COMM_H__
/*public includes*/
#include "stm32f4xx_hal.h"
/*public macro*/

/*public variable extern*/
extern uint8_t com1Buffer[];
extern uint8_t com1State;
/*public  funtion decaleration*/
void BSP_COMMUNICATE_INIT(void);
#endif