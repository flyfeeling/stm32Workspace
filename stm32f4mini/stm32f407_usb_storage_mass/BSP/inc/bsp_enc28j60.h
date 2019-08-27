#ifndef __BSP_ENC28J60_H__
#define __BSP_ENC28J60_H__
/*public includes*/
#include "stm32f4xx_hal.h" 
#include "bsp_enc28j60_def.h" 
/*public macro*/

/*public variable extern*/

/*public  funtion decaleration*/
void BSP_ENC28J60_CS_SELECTED(uint8_t cs);
void BSP_ENC28J60_RST(uint8_t rst);


uint8_t BSP_ENC28J60_GET_REV(void);
void BSP_ENC28J60_INIT(uint8_t* macaddr);


#endif