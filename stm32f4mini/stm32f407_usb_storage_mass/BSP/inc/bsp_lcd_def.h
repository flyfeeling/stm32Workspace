#ifndef __BSP_LCD_DEF_H__
#define __BSP_LCD_DEF_H__
/******************************************************************************************/
/**************************************  reg  *********************************************/
/******************************************************************************************/ 
#define DRIVER_CODE_READ				0X00		//only-read	9325
#define DRIVER_OUTPUT_CTRL1			0X01
#define LCD_DRIVER_CTRL					0X02
#define ENTRY_MODE							0X03
#define RESIZE_CTRL							0X04
#define DISPLAY_CTRL1						0X07
#define DISPLAY_CTRL2						0X08
#define DISPLAY_CTRL3						0X09
#define DISPLAY_CTRL4						0X0A
#define RGB_DISPLAY_CTRL1				0X0C
#define FRAME_MAKER_POS					0X0D
#define RGB_DISPLAY_CTRL2				0X0F
#define PWR_CTRL1								0X10
#define PWR_CTRL2								0X11 
#define PWR_CTRL3								0X12 
#define PWR_CTRL4								0X13 
//cur
#define H_GRAM_ADDR_SET					0X20		
#define V_GRAM_ADDR_SET					0X21
#define GRAM_DATA								0X22
#define PWR_CTRL7								0X29
#define	FRAME_RATE_AND_COLOR_CTRL	0X2B
//gamma
#define GAMMA_CTRL1							0X30
#define GAMMA_CTRL2							0X31
#define GAMMA_CTRL3							0X32
#define GAMMA_CTRL4							0X35
#define GAMMA_CTRL5							0X36
#define GAMMA_CTRL6							0X37
#define GAMMA_CTRL7							0X38
#define GAMMA_CTRL8							0X39
#define GAMMA_CTRL9							0X3C
#define GAMMA_CTRL10						0X3D
//win
#define H_ADDR_START_POS				0X50	//8bit
#define H_ADDR_END_POS					0X51 	//8bit	<=efh
#define V_ADDR_START_POS				0X52 	//12bit
#define V_ADDR_END_POS					0X53 	//12bit	<=13fh
//gate scan ctrl
#define DRIVER_OUTPUT_CTRL2			0X60
#define BASE_IMG_DISPLAY_CTRL		0X61
#define V_SCROLL_CTRL						0X6A

#define PARTIAL_IMG1_DISPLAY_POS 0X80
#define PARTIAL_IMG1_AREA_START	0X81
#define PARTIAL_IMG1_AREA_END		0X82
#define PARTIAL_IMG2_DISPLAY_POS 0X83
#define PARTIAL_IMG2_AREA_START	0X84
#define PARTIAL_IMG2_AREA_END		0X85
#define PANEL_INTERFACE_CTRL1		0X90
#define PANEL_INTERFACE_CTRL2		0X92
#define PANEL_INTERFACE_CTRL4		0X95

#define OPT_VCM_PROG_CTRL				0XA1
#define OPT_VCM_STATUS_AND_EN		0XA2
#define OTP_PROG_ID_KEY					0XA5

/******************************************************************************************/
/**************************************  color  *******************************************/
/******************************************************************************************/ 
#define WHITE         	 	0xFFFF
#define BLACK         	 	0x0000	  
#define BLUE         	 		0x001F  
#define BRED             	0XF81F
#define GRED 			 				0XFFE0
#define GBLUE			 				0X07FF
#define RED           	 	0xF800
#define MAGENTA       	 	0xF81F
#define GREEN         	 	0x07E0
#define CYAN          	 	0x7FFF
#define YELLOW        	 	0xFFE0
#define BROWN 			 			0XBC40 	//��ɫ
#define BRRED 			 			0XFC07 	//�غ�ɫ
#define GRAY  			 		 	0X8430 	//��ɫ
#define DARKBLUE      	 	0X01CF	//����ɫ
#define LIGHTBLUE      	 	0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 	0X5458 	//����ɫ
#define LIGHTGREEN     	 	0X841F 	//ǳ��ɫ
#define LGRAY 			 			0XC618 	//ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        	0XA651 	//ǳ����ɫ(�м����ɫ)
#define LBBLUE           	0X2B12 	//ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
	    															  
#endif
