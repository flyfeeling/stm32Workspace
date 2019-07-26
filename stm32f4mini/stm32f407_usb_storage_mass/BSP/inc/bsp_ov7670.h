#ifndef __BSP_OV7670_H__
#define __BSP_OV7670_H__
/*public includes*/
#include "stm32f4xx_hal.h"
/*public macro*/
typedef struct {
	uint32_t id;
	uint32_t mftr_id;  
} camera_description_strutct;
/*public variable extern*/
extern camera_description_strutct bsp_ov7670;
/*public  funtion decaleration*/
void BSP_OV_INIT(void);
#endif

