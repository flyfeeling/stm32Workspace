#ifndef __BSP_ENC28J60_H__
#define __BSP_ENC28J60_H__
/*public includes*/
#include "stm32f4xx_hal.h" 
#include "bsp_enc28j60_def.h" 
/*public macro*/

/*public variable extern*/

/*public  funtion decaleration*/
void BSP_ENC28J60_CS_SELECTED(uint8_t cs);
void BSP_ENC28J60_WRITE_REG(uint8_t  reg, uint8_t  dat);
uint8_t BSP_ENC28J60_READ_REG(uint8_t  reg);
void BSP_ENC28J60_SOFT_RESET(void);
uint8_t BSP_ENC28J60_READ(uint8_t  dat);
#endif