#include "bsp_lcd.h"

/*private includes*/
#include "main.h"
#include "bsp_lcd_def.h"
#include "stdio.h"
/*private macro*/
#define BSP_LCD_BASE_ADDR ((uint32_t)(0x6C00007E))


/*private variable*/
typedef struct {
	volatile uint16_t RSL;
	volatile uint16_t RSH;
} BSP_LCD_TYPEDEF;
#define BSP_LCD	(*(BSP_LCD_TYPEDEF *)(BSP_LCD_BASE_ADDR))

/*public variable*/
lcd_description_strutct bsp_lcd = {0};

/*private funtion decaleration*/
//IO1
void BSP_LCD_DELAY(uint32_t ms)
{
	HAL_Delay(ms);
}
void BSP_LCD_WRITE_IR(uint16_t ir)
{
	BSP_LCD.RSL = ir;
}
uint16_t BSP_LCD_READ_INTERNAL_STATUS(void)
{ 
	return BSP_LCD.RSL;
}
void BSP_LCD_WRITE_CTRL_OR_GRAM(uint16_t dat)
{
	BSP_LCD.RSH = dat;
}
uint16_t BSP_LCD_READ_CTRL_OR_GRAM()
{
	return BSP_LCD.RSH;
}

void BSP_LCD_WRITE_REG(uint16_t ir, uint16_t dat)
{
	BSP_LCD_WRITE_IR(ir); 
	BSP_LCD_WRITE_CTRL_OR_GRAM(dat);
}
uint16_t BSP_LCD_READ_REG(uint16_t ir)
{
	BSP_LCD_WRITE_IR(ir); 
	return BSP_LCD_READ_CTRL_OR_GRAM();
}


/*public funtion decaleration*/
uint16_t BSP_LCD_ID(void)
{
	return BSP_LCD_READ_REG(DRIVER_CODE_READ);
}

/*funtion*/
void BSP_LCD_BL(uint8_t turn)
{
	if(turn)HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET);
}

void BSP_LCD_INIT(void)
{
	bsp_lcd.id = BSP_LCD_ID();
	bsp_lcd.width = BSP_LCD_WIDTH;
	bsp_lcd.height = BSP_LCD_HEIGHT;
	switch(bsp_lcd.id)
	{
		case ILI9325_ID: sprintf((char*)bsp_lcd.name, "ILI9325");break; 
		default : sprintf((char*)bsp_lcd.name, "NoneType");break;
	}
	if(bsp_lcd.id == ILI9325_ID)
	{
		BSP_LCD_DELAY(10);
		BSP_LCD_WRITE_REG(DRIVER_OUTPUT_CTRL1, 0x0100);	//the shift direction of outputs is from S720 to S1.  / RGB order
		BSP_LCD_WRITE_REG(LCD_DRIVER_CTRL, 0x0500);			//Line inversion
		BSP_LCD_WRITE_REG(ENTRY_MODE, 0x1030);					//1transfer/pixel  									
		BSP_LCD_WRITE_REG(RESIZE_CTRL, 0x0000);					//no resize	
		BSP_LCD_WRITE_REG(DISPLAY_CTRL2, 0x0202);				//FP 2line BP 2line
		BSP_LCD_WRITE_REG(DISPLAY_CTRL3, 0x0000);				//normal scan
		BSP_LCD_WRITE_REG(DISPLAY_CTRL4, 0x0000);				//fmark output interval : 1 frame
		//rgb interface
		BSP_LCD_WRITE_REG(RGB_DISPLAY_CTRL1, 0x0000);
		BSP_LCD_WRITE_REG(FRAME_MAKER_POS, 0x0000);	
		BSP_LCD_WRITE_REG(RGB_DISPLAY_CTRL2, 0x0000);
		//pwr sequence
		BSP_LCD_WRITE_REG(PWR_CTRL1, 0x0000);	
		BSP_LCD_WRITE_REG(PWR_CTRL2, 0x0007);	
		BSP_LCD_WRITE_REG(PWR_CTRL3, 0x0000);	
		BSP_LCD_WRITE_REG(PWR_CTRL4, 0x0000);	
		BSP_LCD_WRITE_REG(DISPLAY_CTRL1, 0x0000);				//turn off display
		BSP_LCD_WRITE_REG(PWR_CTRL1, 0x1690);	
		BSP_LCD_WRITE_REG(PWR_CTRL2, 0x0227);	
		BSP_LCD_WRITE_REG(PWR_CTRL3, 0x009D);	
		BSP_LCD_WRITE_REG(PWR_CTRL4, 0x1900);	
		BSP_LCD_WRITE_REG(PWR_CTRL7, 0x0025);		
		BSP_LCD_WRITE_REG(FRAME_RATE_AND_COLOR_CTRL, 0x000D);		//93fps
		//gamma
		BSP_LCD_WRITE_REG(GAMMA_CTRL1, 0x0007);	
		BSP_LCD_WRITE_REG(GAMMA_CTRL2, 0x0303);	
		BSP_LCD_WRITE_REG(GAMMA_CTRL3, 0x0003);	
		BSP_LCD_WRITE_REG(GAMMA_CTRL4, 0x0206);	
		BSP_LCD_WRITE_REG(GAMMA_CTRL5, 0x0008);	
		BSP_LCD_WRITE_REG(GAMMA_CTRL6, 0x0406);	
		BSP_LCD_WRITE_REG(GAMMA_CTRL7, 0x0304);	
		BSP_LCD_WRITE_REG(GAMMA_CTRL8, 0x0007);	
		BSP_LCD_WRITE_REG(GAMMA_CTRL9, 0x0602);	
		BSP_LCD_WRITE_REG(GAMMA_CTRL10, 0x0008);	 
		//window
		BSP_LCD_SET_WINDOW(0x00, 0xEF, 0X00, 0x13F);
		//gate scan
		BSP_LCD_WRITE_REG(DRIVER_OUTPUT_CTRL2, 0xA700);
		BSP_LCD_WRITE_REG(BASE_IMG_DISPLAY_CTRL, 0x0001);
		BSP_LCD_WRITE_REG(V_SCROLL_CTRL, 0x0000);
		//partial image
		BSP_LCD_WRITE_REG(PARTIAL_IMG1_DISPLAY_POS, 0x0000);	
		BSP_LCD_WRITE_REG(PARTIAL_IMG1_AREA_START, 0x0000);	
		BSP_LCD_WRITE_REG(PARTIAL_IMG1_AREA_END, 0x0000);	
		BSP_LCD_WRITE_REG(PARTIAL_IMG2_DISPLAY_POS, 0x0000);	
		BSP_LCD_WRITE_REG(PARTIAL_IMG2_AREA_START, 0x0000);	
		BSP_LCD_WRITE_REG(PARTIAL_IMG2_AREA_END, 0x0000);	
		
		BSP_LCD_WRITE_REG(PANEL_INTERFACE_CTRL1, 0x0010);	
		BSP_LCD_WRITE_REG(PANEL_INTERFACE_CTRL2, 0x0600);	
		
		BSP_LCD_WRITE_REG(DISPLAY_CTRL1, 0x0133);				//262k color	//turn on display	 //normal display 
		BSP_LCD_SET_SCAN_DIR(BSP_DIR_D2U_L2R);
		BSP_LCD_CLEAR();
		BSP_LCD_BL(1);
	} 
}



void BSP_LCD_SET_CURSOR(uint16_t x, uint16_t y)
{
	BSP_LCD_WRITE_REG(H_GRAM_ADDR_SET, x);	
	BSP_LCD_WRITE_REG(V_GRAM_ADDR_SET, y);
}
void BSP_LCD_WRITE_GRAM(uint16_t x, uint16_t y, uint16_t color)
{
	BSP_LCD_SET_CURSOR(x, y);
	BSP_LCD_WRITE_REG(GRAM_DATA, color);
}
uint16_t BSP_LCD_READ_GRAM(uint16_t x, uint16_t y)
{
	BSP_LCD_SET_CURSOR(x, y);
	return BSP_LCD_READ_REG(GRAM_DATA);
}
void BSP_LCD_SET_WINDOW(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye)
{
	BSP_LCD_WRITE_REG(H_ADDR_START_POS, xs);
	BSP_LCD_WRITE_REG(H_ADDR_END_POS, xe);
	BSP_LCD_WRITE_REG(V_ADDR_START_POS, ys);
	BSP_LCD_WRITE_REG(V_ADDR_END_POS, ye);
}
void BSP_LCD_SET_SCAN_DIR(uint16_t dir)
{
	uint16_t reg = BSP_LCD_READ_REG(ENTRY_MODE);
	reg &= ~(0x00038);
	reg |= dir|0x0080;
	BSP_LCD_WRITE_REG(ENTRY_MODE, reg);
}
void BSP_LCD_CLEAR(void)
{
	BSP_LCD_SET_CURSOR(0, 0);
	BSP_LCD_WRITE_IR(GRAM_DATA);
	for(uint16_t x=0; x<BSP_LCD_WIDTH; x++){
			for(uint16_t y=0; y<BSP_LCD_HEIGHT; y++){
				BSP_LCD_WRITE_CTRL_OR_GRAM(WHITE);
			} 
	}
 
}

void BSP_LCD_DRAW_IMAGE(uint32_t addr)
{
	uint16_t* iamge = (uint16_t*)addr;
	BSP_LCD_SET_CURSOR(0, 0);
	BSP_LCD_WRITE_IR(GRAM_DATA);
	for(uint32_t pix=0; pix<BSP_LCD_WIDTH*BSP_LCD_HEIGHT; pix++){ 
				BSP_LCD_WRITE_CTRL_OR_GRAM((uint16_t)iamge[pix]); 
	}	 
}
