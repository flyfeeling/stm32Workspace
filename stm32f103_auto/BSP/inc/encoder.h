#ifndef __BSP_ENCODE_H__
#define	__BSP_ENCODE_H__
/*public includes*/
#include "stm32f1xx_hal.h"
/*public macro*/

/*public variable extern*/

/*public  funtion decaleration*/
void BSP_ENCODER_START(void);
void BSP_ENCODER_STOP(void);
double BSP_ENCODER_GET_FREQ(uint8_t enc);
#endif