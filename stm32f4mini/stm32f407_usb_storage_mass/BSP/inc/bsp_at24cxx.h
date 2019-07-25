#ifndef __BSP_AT24CXX_H__
#define __BSP_AT24CXX_H__
/*public includes*/
#include "stm32f4xx_hal.h"
/*public macro*/

/*public variable extern*/

/*public  funtion decaleration*/
void BSP_AT24CXX_WRITE_BYTE(uint8_t addr, uint8_t dat);
uint8_t BSP_AT24CXX_READ_BYTE(uint8_t addr);
void BSP_AT24CXX_WRITE(uint8_t addr, uint8_t* dat, uint32_t len);
void BSP_AT24CXX_READ(uint8_t addr, uint8_t *buf, uint32_t len);
#endif

