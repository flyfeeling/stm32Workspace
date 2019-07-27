#ifndef __BSP_LCD_H__
#define __BSP_LCD_H__
/*public includes*/
#include "stm32f4xx_hal.h"
/*public macro*/
#define BSP_LCD_HEIGHT			(320)
#define BSP_LCD_WIDTH				(240)

#define ILI9325_ID		(0x9325)

#define BSP_DIR_R2L_D2U	0x00
#define BSP_DIR_L2R_D2U	0x10
#define BSP_DIR_R2L_U2D	0x20
#define BSP_DIR_L2R_U2D	0x30
#define BSP_DIR_D2U_L2R	0x08
#define BSP_DIR_D2U_R2L	0x18
#define BSP_DIR_U2D_L2R	0x28
#define BSP_DIR_U2D_R2L	0x38

/*public variable extern*/
typedef struct {
	uint16_t id, height, width;
	uint8_t name[10];
} lcd_description_strutct;
extern lcd_description_strutct bsp_lcd;

/*public  funtion decaleration*/
void 			BSP_LCD_BL(uint8_t turn);
void 			BSP_LCD_INIT(void);

void 			BSP_LCD_SET_CURSOR(uint16_t x, uint16_t y);
void 			BSP_LCD_WRITE_GRAM(uint16_t x, uint16_t y, uint16_t color);
uint16_t 	BSP_LCD_READ_GRAM(uint16_t x, uint16_t y);
void 			BSP_LCD_SET_WINDOW(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye);
void 			BSP_LCD_CLEAR(void);
void 			BSP_LCD_SET_SCAN_DIR(uint16_t dir);
void 			BSP_LCD_WRITE_CTRL_OR_GRAM(uint16_t dat);
#endif
