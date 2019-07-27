#include "bsp_ov7670.h"

/*private includes*/
#include "bsp_ov7670_def.h"
#include "bsp_sccb.h"
#include "bsp_delay.h" 
#include "dcmi.h"
#include "main.h" 
/*private macro*/
#define BSP_OV7670_TIMEOUT 	10
#define BSP_OV7670_I2C_ADDR 0X42

/*private variable*/
const uint8_t BSP_OV7670_INIT_SEQUENCE[][2] = 
{
	{0x3a, 0x0C}, 
	{0x40, 0xd0}, 
	{0x12, 0x14}, //COM7 0x14
	{0x32, 0x80}, 
	{0x17, 0x16}, 
	{0x18, 0x04}, 
	{0x19, 0x02}, 
	{0x1a, 0x7a},//0x7a, 
	{0x03, 0x05},//0x0a, 		
	{0x0c, 0x00}, 
	{0x3e, 0x00},// 
	{0x70, 0x00}, 
	{0x71, 0x01}, 
	{0x72, 0x11}, 
	{0x73, 0x00},// 
	{0xa2, 0x02}, 
	{0x11, 0x81}, 
	{0x7a, 0x20}, 
	{0x7b, 0x1c}, 
	{0x7c, 0x28}, 
	{0x7d, 0x3c}, 
	{0x7e, 0x55}, 
	{0x7f, 0x68}, 
	{0x80, 0x76}, 
	{0x81, 0x80}, 
	{0x82, 0x88}, 
	{0x83, 0x8f}, 
	{0x84, 0x96}, 
	{0x85, 0xa3}, 
	{0x86, 0xaf}, 
	{0x87, 0xc4}, 
	{0x88, 0xd7}, 
	{0x89, 0xe8}, 
	{0x13, 0xe0}, 
	{0x00, 0x00},//AGC 
	{0x10, 0x00}, 
	{0x0d, 0x00}, 
	{0x14, 0x28},//0x38, limit the max gain 
	{0xa5, 0x05}, 
	{0xab, 0x07}, 
	{0x24, 0x75}, 
	{0x25, 0x63}, 
	{0x26, 0xA5}, 
	{0x9f, 0x78}, 
	{0xa0, 0x68}, 
	{0xa1, 0x03},//0x0b, 
	{0xa6, 0xdf},//0xd8, 
	{0xa7, 0xdf},//0xd8, 
	{0xa8, 0xf0}, 
	{0xa9, 0x90}, 
	{0xaa, 0x94}, 
	{0x13, 0xe5}, 
	{0x0e, 0x61}, 
	{0x0f, 0x4b}, 
	{0x16, 0x02}, 
	{0x1e, 0x07},//0x07, 
	{0x21, 0x02}, 
	{0x22, 0x91}, 
	{0x29, 0x07}, 
	{0x33, 0x0b}, 
	{0x35, 0x0b}, 
	{0x37, 0x1d}, 
	{0x38, 0x71}, 
	{0x39, 0x2a}, 
	{0x3c, 0x78}, 
	{0x4d, 0x40}, 
	{0x4e, 0x20}, 
	{0x69, 0x55}, 
	{0x6b, 0x0A},//PLL 重要参数 0x0A
	{0x74, 0x19}, 
	{0x8d, 0x4f}, 
	{0x8e, 0x00}, 
	{0x8f, 0x00}, 
	{0x90, 0x00}, 
	{0x91, 0x00}, 
	{0x92, 0x00},//0x19,//0x66 
	{0x96, 0x00}, 
	{0x9a, 0x80}, 
	{0xb0, 0x84}, 
	{0xb1, 0x0c}, 
	{0xb2, 0x0e}, 
	{0xb3, 0x82}, 
	{0xb8, 0x0a}, 
	{0x43, 0x14}, 
	{0x44, 0xf0}, 
	{0x45, 0x34}, 
	{0x46, 0x58}, 
	{0x47, 0x28}, 
	{0x48, 0x3a}, 
	{0x59, 0x88}, 
	{0x5a, 0x88}, 
	{0x5b, 0x44}, 
	{0x5c, 0x67}, 
	{0x5d, 0x49}, 
	{0x5e, 0x0e}, 
	{0x64, 0x04}, 
	{0x65, 0x20}, 
	{0x66, 0x05}, 
	{0x94, 0x04}, 
	{0x95, 0x08}, 
	{0x6c, 0x0a}, 
	{0x6d, 0x55}, 
	{0x6e, 0x11}, 
	{0x6f, 0x9f},//0x9e for advance AWB 
	{0x6a, 0x40}, 
	{0x01, 0x40}, 
	{0x02, 0x40}, 
	{0x13, 0xe7}, 
	{0x15, 0x08}, //重要参数 
	{0x4f, 0x80}, 
	{0x50, 0x80}, 
	{0x51, 0x00}, 
	{0x52, 0x22}, 
	{0x53, 0x5e}, 
	{0x54, 0x80}, 
	{0x55, 0x0A},//亮度 
	{0x56, 0x4f},//对比度 
	{0x58, 0x9e},	
	{0x41, 0x08}, 
	{0x3f, 0x03},//边缘增强调整 0x05
	{0x75, 0x05}, 
	{0x76, 0xe1}, 
	{0x4c, 0x0F},//噪声抑制强度 
	{0x77, 0x0a}, 
	{0x3d, 0xc2},//0xc0, 
	{0x4b, 0x09}, 
	{0xc9, 0x60}, 
	{0x41, 0x38}, 
	{0x34, 0x11}, 
	{0x3b, 0x02},//0x00,//0x02, 
	{0xa4, 0x89},//0x88, 
	{0x96, 0x00}, 
	{0x97, 0x30}, 
	{0x98, 0x20}, 
	{0x99, 0x30}, 
	{0x9a, 0x84}, 
	{0x9b, 0x29}, 
	{0x9c, 0x03}, 
	{0x9d, 0x4c}, 
	{0x9e, 0x3f}, 
	{0x78, 0x04},	
	{0x79, 0x01}, 
	{0xc8, 0xf0}, 
	{0x79, 0x0f}, 
	{0xc8, 0x00}, 
	{0x79, 0x10}, 
	{0xc8, 0x7e}, 
	{0x79, 0x0a}, 
	{0xc8, 0x80}, 
	{0x79, 0x0b}, 
	{0xc8, 0x01}, 
	{0x79, 0x0c}, 
	{0xc8, 0x0f}, 
	{0x79, 0x0d}, 
	{0xc8, 0x20}, 
	{0x79, 0x09}, 
	{0xc8, 0x80}, 
	{0x79, 0x02}, 
	{0xc8, 0xc0}, 
	{0x79, 0x03}, 
	{0xc8, 0x40}, 
	{0x79, 0x05}, 
	{0xc8, 0x30}, 
	{0x79, 0x26}, 
	{0x09, 0x02}, 
	{0x3b, 0xc2},//0x82,//0xc0,//0xc2,	//night mode 0x42
};


/*public variable*/

camera_description_strutct bsp_ov7670 = {0};
/*private funtion decaleration*/
//io
void BSP_OV_WRITE_REG(uint8_t reg, uint8_t dat)
{ 
	SCCB_START(); 
	if(SCCB_SEND_BYTE(BSP_OV7670_I2C_ADDR)){}
	SCCB_DELAY(1);	//100
	if(SCCB_SEND_BYTE(reg)){}
	SCCB_DELAY(1);
	if(SCCB_SEND_BYTE(dat)){}
	SCCB_STOP(); 
}
uint8_t BSP_OV_READ_REG(uint8_t reg)
{
 	uint8_t dat;
	SCCB_START();  
	if(SCCB_SEND_BYTE(BSP_OV7670_I2C_ADDR)){}
	SCCB_DELAY(1); 
	if(SCCB_SEND_BYTE(reg)){}
	SCCB_DELAY(1);
	SCCB_STOP();
	SCCB_DELAY(1);
	SCCB_START(); 
	if(SCCB_SEND_BYTE(BSP_OV7670_I2C_ADDR|0x01)){}
	SCCB_DELAY(1);
	dat = SCCB_READ_BYTE();  
	SCCB_NA(); 
	SCCB_STOP(); 
	return dat;
}
/*public funtion decaleration*/
void BSP_OV_SOFT_RST(void)
{
	BSP_OV_WRITE_REG(OV7670_COM7, 0x80);
	BSP_DELAY_MS(10);
}

/*funtion*/
void BSP_OV_INIT(void)
{ 
	uint8_t id[2];
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_4);
	//BSP_DELAY(0, 0, 200);
	BSP_OV_SOFT_RST();
	id[0] = BSP_OV_READ_REG(OV7670_PID);
	id[1] = BSP_OV_READ_REG(OV7670_VER); 
	bsp_ov7670.id = (uint16_t)(id[0]<<8|id[1]);
	id[0] = BSP_OV_READ_REG(OV7670_MIDH);
	id[1] = BSP_OV_READ_REG(OV7670_MIDL); 
	bsp_ov7670.mftr_id = (uint16_t)(id[0]<<8|id[1]);  
	
	for(uint16_t i=0; i<sizeof(BSP_OV7670_INIT_SEQUENCE)/2; i++)
	{ 
		BSP_OV_WRITE_REG(BSP_OV7670_INIT_SEQUENCE[i][0], BSP_OV7670_INIT_SEQUENCE[i][1]);
		//BSP_DELAY(0, 0, 200);
	}
}
void BSP_OV_CONTINUOUS_START(uint32_t destaddr)
{    
  HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, (uint32_t)destaddr, 320*240);  
}
void BSP_OV_SNAPSHOT_START(uint32_t destaddr)
{    
  HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT, (uint32_t)destaddr, 320*240);  
}
void BSP_OV_SUSPEND(void) 
{ 
  HAL_DCMI_Suspend(&hdcmi);
}
void BSP_OV_RESUME(void) 
{ 
  HAL_DCMI_Resume(&hdcmi);
}
uint8_t BSP_OV_STOP(void) 
{
  DCMI_HandleTypeDef *phdcmi; 
  uint8_t ret = 0xFF;  
  phdcmi = &hdcmi;   
  if(HAL_DCMI_Stop(phdcmi) == HAL_OK)
  {
     ret = 0;
  } 
  return ret;
}

void BSP_OV_SET_FRAME(void)
{
	//24Mhz input clock
	//30 fps, PCLK = 24Mhz
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0x80);
	BSP_OV_WRITE_REG(OV7670_DBLV, 0x0a);
	BSP_OV_WRITE_REG(OV7670_EXHCH, 0x00);
	BSP_OV_WRITE_REG(OV7670_EXHCL, 0x00);
	BSP_OV_WRITE_REG(OV7670_DM_LNL, 0x00);
	BSP_OV_WRITE_REG(OV7670_DM_LNH, 0x00);
	BSP_OV_WRITE_REG(OV7670_COM11, 0x0a);
	//15 fps, PCLK = 12Mhz
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0X00);
	BSP_OV_WRITE_REG(OV7670_DBLV, 0x0a);
	BSP_OV_WRITE_REG(OV7670_EXHCH, 0x00);
	BSP_OV_WRITE_REG(OV7670_EXHCL, 0x00);
	BSP_OV_WRITE_REG(OV7670_DM_LNL, 0x00);
	BSP_OV_WRITE_REG(OV7670_DM_LNH, 0x00);
	BSP_OV_WRITE_REG(OV7670_COM11, 0x0a);	
	//25fps, PCLK = 24Mhz
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0x80);
	BSP_OV_WRITE_REG(OV7670_DBLV, 0x0a);
	BSP_OV_WRITE_REG(OV7670_EXHCH, 0x00);
	BSP_OV_WRITE_REG(OV7670_EXHCL, 0x00);
	BSP_OV_WRITE_REG(OV7670_DM_LNL, 0X66);
	BSP_OV_WRITE_REG(OV7670_DM_LNH, 0x00);
	BSP_OV_WRITE_REG(OV7670_COM11, 0x0a);		
	//14.3fps, PCLK = 12Mhz
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0X00);
	BSP_OV_WRITE_REG(OV7670_DBLV, 0x0a);
	BSP_OV_WRITE_REG(OV7670_EXHCH, 0x00);
	BSP_OV_WRITE_REG(OV7670_EXHCL, 0x00);
	BSP_OV_WRITE_REG(OV7670_DM_LNL, 0X1a);
	BSP_OV_WRITE_REG(OV7670_DM_LNH, 0x00);
	BSP_OV_WRITE_REG(OV7670_COM11, 0x0a);		
	
	//26 Mhz input clock
	//30 fps, PCLK = 26Mhz
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0x80);
	BSP_OV_WRITE_REG(OV7670_DBLV, 0x0a);
	BSP_OV_WRITE_REG(OV7670_EXHCH, 0x00);
	BSP_OV_WRITE_REG(OV7670_EXHCL, 0x00);
	BSP_OV_WRITE_REG(OV7670_DM_LNL, 0X2b);
	BSP_OV_WRITE_REG(OV7670_DM_LNH, 0x00);
	BSP_OV_WRITE_REG(OV7670_COM11, 0x0a);	
	//15 fps, PCLK = 13Mhz
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0x00);
	BSP_OV_WRITE_REG(OV7670_DBLV, 0x0a);
	BSP_OV_WRITE_REG(OV7670_EXHCH, 0x00);
	BSP_OV_WRITE_REG(OV7670_EXHCL, 0x00);
	BSP_OV_WRITE_REG(OV7670_DM_LNL, 0X2b);
	BSP_OV_WRITE_REG(OV7670_DM_LNH, 0x00);
	BSP_OV_WRITE_REG(OV7670_COM11, 0x0a);	
	//25fps, PCLK = 26Mhz
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0x80);
	BSP_OV_WRITE_REG(OV7670_DBLV, 0x0a);
	BSP_OV_WRITE_REG(OV7670_EXHCH, 0x00);
	BSP_OV_WRITE_REG(OV7670_EXHCL, 0x00);
	BSP_OV_WRITE_REG(OV7670_DM_LNL, 0x99);
	BSP_OV_WRITE_REG(OV7670_DM_LNH, 0x00);
	BSP_OV_WRITE_REG(OV7670_COM11, 0x0a);		
	//14.3fps, PCLK = 13Mhz
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0x00);
	BSP_OV_WRITE_REG(OV7670_DBLV, 0x0a);
	BSP_OV_WRITE_REG(OV7670_EXHCH, 0x00);
	BSP_OV_WRITE_REG(OV7670_EXHCL, 0x00);
	BSP_OV_WRITE_REG(OV7670_DM_LNL, 0x46);
	BSP_OV_WRITE_REG(OV7670_DM_LNH, 0x00);
	BSP_OV_WRITE_REG(OV7670_COM11, 0x0a);			
	
	//13 Mhz input clock
	//30 fps, PCLK = 26Mhz
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0x00);
	BSP_OV_WRITE_REG(OV7670_DBLV, 0x4a);
	BSP_OV_WRITE_REG(OV7670_EXHCH, 0x00);
	BSP_OV_WRITE_REG(OV7670_EXHCL, 0x00);
	BSP_OV_WRITE_REG(OV7670_DM_LNL, 0x2b);
	BSP_OV_WRITE_REG(OV7670_DM_LNH, 0x00);
	BSP_OV_WRITE_REG(OV7670_COM11, 0x0a);			
	//15 fps, PCLK = 13Mhz
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0x01);
	BSP_OV_WRITE_REG(OV7670_DBLV, 0x4a);
	BSP_OV_WRITE_REG(OV7670_EXHCH, 0x00);
	BSP_OV_WRITE_REG(OV7670_EXHCL, 0x00);
	BSP_OV_WRITE_REG(OV7670_DM_LNL, 0x2b);
	BSP_OV_WRITE_REG(OV7670_DM_LNH, 0x00);
	BSP_OV_WRITE_REG(OV7670_COM11, 0x0a);		
	//25fps, PCLK = 26Mhz
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0x00);
	BSP_OV_WRITE_REG(OV7670_DBLV, 0x4a);
	BSP_OV_WRITE_REG(OV7670_EXHCH, 0x00);
	BSP_OV_WRITE_REG(OV7670_EXHCL, 0x00);
	BSP_OV_WRITE_REG(OV7670_DM_LNL, 0x99);
	BSP_OV_WRITE_REG(OV7670_DM_LNH, 0x00);
	BSP_OV_WRITE_REG(OV7670_COM11, 0x0a);
	//14.3fps, PCLK = 13Mhz
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0x01);
	BSP_OV_WRITE_REG(OV7670_DBLV, 0x4a);
	BSP_OV_WRITE_REG(OV7670_EXHCH, 0x00);
	BSP_OV_WRITE_REG(OV7670_EXHCL, 0x00);
	BSP_OV_WRITE_REG(OV7670_DM_LNL, 0x46);
	BSP_OV_WRITE_REG(OV7670_DM_LNH, 0x00);
	BSP_OV_WRITE_REG(OV7670_COM11, 0x0a);
}
void BSP_OV_NIGHT_MODE(void)
{
	//Fixed Frame Rate
	//24Mhz/26Mhz Clock Input
	//3.75fps night mode for 60Hz & 50Hz light environment
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0x03);
	BSP_OV_WRITE_REG(OV7670_COM11, 0x0a);	 
	
	//13Mhz Clock Input
	//3.75fps night mode for 60Hz & 50Hz light environment
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0x07);
	BSP_OV_WRITE_REG(OV7670_COM11, 0x0a);
	
	//Auto Frame Rate
	//24Mhz/26Mhz Clock Input
	//30fps ~ 3.75fps night mode for 60Hz
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0x80);
	BSP_OV_WRITE_REG(OV7670_COM11, 0xea);
	//15fps ~ 3.75fps night mode for 60Hz
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0xca);
	BSP_OV_WRITE_REG(OV7670_COM11, 0x00);	
	//25fps ~ 3.125fps night mode for 50Hz light environment
	BSP_OV_WRITE_REG(OV7670_COM11, 0xea);
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0x80);
	//14.3fps ~ 3.6fps night mode for 50Hz light environment
	BSP_OV_WRITE_REG(OV7670_COM11, 0xca);
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0x00);
	
	//13Mhz Clock Input
	//30fps ~ 3.75fps night mode for 60Hz light environment
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0x00);
	BSP_OV_WRITE_REG(OV7670_COM11, 0xea);
	//15fps ~ 3.75fps night mode for 60Hz light environment
	BSP_OV_WRITE_REG(OV7670_COM11, 0xca);
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0x01);
	//25fps ~ 3.125fps night mode for 50Hz light environment
	BSP_OV_WRITE_REG(OV7670_COM11, 0xea);
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0x00);
	//14.3fps ~ 3.6fps night mode for 50Hz light environment
	BSP_OV_WRITE_REG(OV7670_COM11, 0xca);
	BSP_OV_WRITE_REG(OV7670_CLKRC, 0x01);
}
void BSP_OV_SET_BANDING_FILTER(void)
{
	//24Mhz Input Clock
	//30fps for 60Hz light frequency
	BSP_OV_WRITE_REG(OV7670_COM8, 0xe7); //banding filter enable
	BSP_OV_WRITE_REG(OV7670_BD50ST, 0x98); //50Hz banding filter
	BSP_OV_WRITE_REG(OV7670_BD60ST, 0x7f); //60Hz banding filter
	BSP_OV_WRITE_REG(OV7670_BD50MAX, 0x02); //3 step for 50hz
	BSP_OV_WRITE_REG(OV7670_BD60MAX, 0x03); //4 step for 60hz
	BSP_OV_WRITE_REG(OV7670_COM11, 0x02); //Select 60Hz banding filter
	//15fps for 60Hz light frequency
	BSP_OV_WRITE_REG(OV7670_COM8, 0xe7); //banding filter enable
	BSP_OV_WRITE_REG(OV7670_BD50ST, 0x4c); //50Hz banding filter
	BSP_OV_WRITE_REG(OV7670_BD60ST, 0x3f); //60Hz banding filter
	BSP_OV_WRITE_REG(OV7670_BD50MAX, 0x05); //6 step for 50hz
	BSP_OV_WRITE_REG(OV7670_BD60MAX, 0x07); //8 step for 60hz
	BSP_OV_WRITE_REG(OV7670_COM11, 0x02); //Select 60Hz banding filter
	//25fps for 50Hz light frequency
	BSP_OV_WRITE_REG(OV7670_COM8, 0xe7); //banding filter enable
	BSP_OV_WRITE_REG(OV7670_BD50ST, 0x98); //50Hz banding filter
	BSP_OV_WRITE_REG(OV7670_BD60ST, 0x7f); //60Hz banding filter
	BSP_OV_WRITE_REG(OV7670_BD50MAX, 0x03); //4 step for 50hz
	BSP_OV_WRITE_REG(OV7670_BD60MAX, 0x03); //4 step for 60hz
	BSP_OV_WRITE_REG(OV7670_COM11, 0x0a); //Select 50Hz banding filter
	//14.3fps for 50Hz light frequency 
	BSP_OV_WRITE_REG(OV7670_COM8, 0xe7); //banding filter enable
	BSP_OV_WRITE_REG(OV7670_BD50ST, 0x4c); //50Hz banding filter
	BSP_OV_WRITE_REG(OV7670_BD60ST, 0x3f); //60Hz banding filter
	BSP_OV_WRITE_REG(OV7670_BD50MAX, 0x06); //7 step for 50hz
	BSP_OV_WRITE_REG(OV7670_BD60MAX, 0x07); //8 step for 60hz
	BSP_OV_WRITE_REG(OV7670_COM11, 0x0a); //Select 50Hz banding filter
	
	//13Mhz/26Mhz Input Clock
	//30fps for 60Hz light frequency
	BSP_OV_WRITE_REG(OV7670_COM8, 0xe7); //banding filter enable
	BSP_OV_WRITE_REG(OV7670_BD50ST, 0xa5); //50Hz banding filter
	BSP_OV_WRITE_REG(OV7670_BD60ST, 0x89); //60Hz banding filter
	BSP_OV_WRITE_REG(OV7670_BD50MAX, 0x02); //3 step for 50hz
	BSP_OV_WRITE_REG(OV7670_BD60MAX, 0x03); //4 step for 60hz
	BSP_OV_WRITE_REG(OV7670_COM11, 0x02); //Select 60Hz banding filter
	//15fps for 60Hz light frequency
	BSP_OV_WRITE_REG(OV7670_COM8, 0xe7); //banding filter enable
	BSP_OV_WRITE_REG(OV7670_BD50ST, 0x52); //50Hz banding filter
	BSP_OV_WRITE_REG(OV7670_BD60ST, 0x44); //60Hz banding filter
	BSP_OV_WRITE_REG(OV7670_BD50MAX, 0x06); //7 step for 50hz
	BSP_OV_WRITE_REG(OV7670_BD60MAX, 0x07); //8 step for 60hz
	BSP_OV_WRITE_REG(OV7670_COM11, 0x02); //Select 60Hz banding filter
	//25fps for 50Hz light frequency
	BSP_OV_WRITE_REG(OV7670_COM8, 0xe7); //banding filter enable
	BSP_OV_WRITE_REG(OV7670_BD50ST, 0xa5); //50Hz banding filter
	BSP_OV_WRITE_REG(OV7670_BD60ST, 0x89); //60Hz banding filter
	BSP_OV_WRITE_REG(OV7670_BD50MAX, 0x02); //3 step for 50hz
	BSP_OV_WRITE_REG(OV7670_BD60MAX, 0x03); //4 step for 60hz
	BSP_OV_WRITE_REG(OV7670_COM11, 0x0a); //Select 50Hz banding filter
	//14.3fps for 50Hz light frequency
	BSP_OV_WRITE_REG(OV7670_COM8, 0xe7); //banding filter enable
	BSP_OV_WRITE_REG(OV7670_BD50ST, 0x52); //50Hz banding filter
	BSP_OV_WRITE_REG(OV7670_BD60ST, 0x44); //60Hz banding filter
	BSP_OV_WRITE_REG(OV7670_BD50MAX, 0x06); //7 step for 50hz
	BSP_OV_WRITE_REG(OV7670_BD60MAX, 0x07); //8 step for 60hz
	BSP_OV_WRITE_REG(OV7670_COM11, 0x0a); //Select 50Hz banding filter
}
void BSP_OV_VIDEO_MODE(void)
{
	//LightMode
	//Auto 
	BSP_OV_WRITE_REG(0x13, 0xe7); //AWB on
	//Sunny 
	BSP_OV_WRITE_REG(0x13, 0xe5); //AWB off
	BSP_OV_WRITE_REG(0x01, 0x5a);
	BSP_OV_WRITE_REG(0x02, 0x5c);
	//Cloudy 
	BSP_OV_WRITE_REG(0x13, 0xe5); //AWB off
	BSP_OV_WRITE_REG(0x01, 0x58);
	BSP_OV_WRITE_REG(0x02, 0x60);
	//Office 
	BSP_OV_WRITE_REG(0x13, 0xe5); //AWB off
	BSP_OV_WRITE_REG(0x01, 0x84);
	BSP_OV_WRITE_REG(0x02, 0x4c);
	//Home 
	BSP_OV_WRITE_REG(0x13, 0xe5); //AWB off
	BSP_OV_WRITE_REG(0x01, 0x96);
	BSP_OV_WRITE_REG(0x02, 0x40);
	
	//Color Saturation
	//Saturation + 2 
	BSP_OV_WRITE_REG(0x4f, 0xc0);
	BSP_OV_WRITE_REG(0x50, 0xc0);
	BSP_OV_WRITE_REG(0x51, 0x00);
	BSP_OV_WRITE_REG(0x52, 0x33);
	BSP_OV_WRITE_REG(0x53, 0x8d);
	BSP_OV_WRITE_REG(0x54, 0xc0);
	BSP_OV_WRITE_REG(0x58, 0x9e);
	//Saturation + 1 
	BSP_OV_WRITE_REG(0x4f, 0x99);
	BSP_OV_WRITE_REG(0x50, 0x99);
	BSP_OV_WRITE_REG(0x51, 0x00);
	BSP_OV_WRITE_REG(0x52, 0x28);
	BSP_OV_WRITE_REG(0x53, 0x71);
	BSP_OV_WRITE_REG(0x54, 0x99);
	BSP_OV_WRITE_REG(0x58, 0x9e);
	//Saturation 0 
	BSP_OV_WRITE_REG(0x4f, 0x80);
	BSP_OV_WRITE_REG(0x50, 0x80);
	BSP_OV_WRITE_REG(0x51, 0x00);
	BSP_OV_WRITE_REG(0x52, 0x22);
	BSP_OV_WRITE_REG(0x53, 0x5e);
	BSP_OV_WRITE_REG(0x54, 0x80);
	BSP_OV_WRITE_REG(0x58, 0x9e);
	//Saturation -1 
	BSP_OV_WRITE_REG(0x4f, 0x66);
	BSP_OV_WRITE_REG(0x50, 0x66);
	BSP_OV_WRITE_REG(0x51, 0x00);
	BSP_OV_WRITE_REG(0x52, 0x1b);
	BSP_OV_WRITE_REG(0x53, 0x4b);
	BSP_OV_WRITE_REG(0x54, 0x66);
	BSP_OV_WRITE_REG(0x58, 0x9e);
	//Saturation - 2 
	BSP_OV_WRITE_REG(0x4f, 0x40);
	BSP_OV_WRITE_REG(0x50, 0x40);
	BSP_OV_WRITE_REG(0x51, 0x00);
	BSP_OV_WRITE_REG(0x52, 0x11);
	BSP_OV_WRITE_REG(0x53, 0x2f);
	BSP_OV_WRITE_REG(0x54, 0x40);
	BSP_OV_WRITE_REG(0x58, 0x9e);
	
	//Brightness
	//Brightness +2 
	BSP_OV_WRITE_REG(0x55, 0x30);
	//Brightness +1 
	BSP_OV_WRITE_REG(0x55, 0x18);
	//Brightness 0 
	BSP_OV_WRITE_REG(0x55, 0x00);
	//Brightness -1 
	BSP_OV_WRITE_REG(0x55, 0x98);
	//Brightness -2 
	BSP_OV_WRITE_REG(0x55, 0xb0);
	
	//Contrast
	//Contrast +2 
	BSP_OV_WRITE_REG(0x56, 0x60);
	//Contrast +1 
	BSP_OV_WRITE_REG(0x56, 0x50);
	//Contrast 0 
	BSP_OV_WRITE_REG(0x56, 0x40);
	//Contrast -1 
	BSP_OV_WRITE_REG(0x56, 0x38);
	//Contrast -2 
	BSP_OV_WRITE_REG(0x56, 0x30);
	
	//Special effects
	//Antique 
	BSP_OV_WRITE_REG(0x3a, 0x14);
	BSP_OV_WRITE_REG(0x67, 0xa0);
	BSP_OV_WRITE_REG(0x68, 0x40);
	//Bluish 
	BSP_OV_WRITE_REG(0x3a, 0x14);
	BSP_OV_WRITE_REG(0x67, 0x80);
	BSP_OV_WRITE_REG(0x68, 0xc0);
	//Greenish 
	BSP_OV_WRITE_REG(0x3a, 0x14);
	BSP_OV_WRITE_REG(0x67, 0x40);
	BSP_OV_WRITE_REG(0x68, 0x40);
	//Redish 
	BSP_OV_WRITE_REG(0x3a, 0x14);
	BSP_OV_WRITE_REG(0x67, 0xc0);
	BSP_OV_WRITE_REG(0x68, 0x80);
	//B&W 
	BSP_OV_WRITE_REG(0x3a, 0x14);
	BSP_OV_WRITE_REG(0x67, 0x80);
	BSP_OV_WRITE_REG(0x68, 0x80);
	//Negative 
	BSP_OV_WRITE_REG(0x3a, 0x24);
	BSP_OV_WRITE_REG(0x67, 0x80);
	BSP_OV_WRITE_REG(0x68, 0x80);
	//B&W negative 
	BSP_OV_WRITE_REG(0x3a, 0x34);
	BSP_OV_WRITE_REG(0x67, 0x80);
	BSP_OV_WRITE_REG(0x68, 0x80);
	//Normal 
	BSP_OV_WRITE_REG(0x3a, 0x04);
	BSP_OV_WRITE_REG(0x67, 0xc0);
	BSP_OV_WRITE_REG(0x68, 0x80);
}
