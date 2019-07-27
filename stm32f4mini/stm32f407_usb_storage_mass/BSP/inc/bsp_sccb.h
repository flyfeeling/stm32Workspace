#ifndef __BSP_SCCB_H__
#define __BSP_SCCB_H__
/*public includes*/
#include "stm32f4xx_hal.h"
/*public macro*/

/*public variable extern*/

/*public  funtion decaleration*/ 
void SCCB_SDA_OP(uint8_t b);
void SCCB_SCL_OP(uint8_t b);
void SCCB_SDA_IN(void);
void SCCB_SDA_OUT(void);
void SCCB_START(void);
void SCCB_STOP(void);
void SCCB_NA(void);
uint8_t SCCB_SEND_BYTE(uint8_t dat);
uint8_t SCCB_READ_BYTE(void);
void SCCB_DELAY(uint32_t d);


#endif
