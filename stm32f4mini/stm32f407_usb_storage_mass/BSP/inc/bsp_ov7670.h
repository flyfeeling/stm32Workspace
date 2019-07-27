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
void BSP_OV_CONTINUOUS_START(uint32_t destaddr);
void BSP_OV_SNAPSHOT_START(uint32_t destaddr);
void BSP_OV_SUSPEND(void) ;
void BSP_OV_RESUME(void) ;
uint8_t BSP_OV_STOP(void) ;

void BSP_OV_SET_FRAME(void);
#endif

