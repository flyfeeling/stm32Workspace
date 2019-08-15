#ifndef __BSP_ENCODE_H__
#define	__BSP_ENCODE_H__
/*public includes*/
#include "stm32f1xx_hal.h"
/*public macro*/
#define	ENCODE1	(TIM_CHANNEL_1)
#define ENCODE2 (TIM_CHANNEL_2)
/*public variable extern*/

/*public  funtion decaleration*/
void BSP_ENCODE_START();
#endif