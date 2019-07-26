#ifndef __BSP_SRAM_H__
#define __BSP_SRAM_H__
/*public includes*/
#include "stm32f4xx_hal.h"
/*public macro*/

/*public variable extern*/

/*public  funtion decaleration*/
void BSP_FSMC_SRAM_READ(uint32_t addr, uint8_t* buf, uint32_t len);
void BSP_FSMC_SRAM_WRITE(uint32_t addr, uint8_t* buf, uint32_t len);
#endif
