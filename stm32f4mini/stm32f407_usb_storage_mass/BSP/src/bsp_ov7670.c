#include "bsp_ov7670.h"

/*private includes*/
#include "bsp_ov7670_def.h"
#include "i2c.h"
#include "dcmi.h"
#include "main.h"
/*private macro*/
#define BSP_OV7670_TIMEOUT 	0xFFFFF
#define BSP_OV7670_I2C_ADDR 0X42
/*private variable*/
void SCCB_DELAY(uint32_t d)
{
	HAL_Delay(d);
}
void SCCB_SDA_OP(uint8_t b)
{
	if(b)HAL_GPIO_WritePin(OV7670_SDA_GPIO_Port, OV7670_SDA_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(OV7670_SDA_GPIO_Port, OV7670_SDA_Pin, GPIO_PIN_RESET);
}
void SCCB_SCL_OP(uint8_t b)
{
	if(b)HAL_GPIO_WritePin(OV7670_SCL_GPIO_Port, OV7670_SCL_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(OV7670_SCL_GPIO_Port, OV7670_SCL_Pin, GPIO_PIN_RESET);
}

 
void SCCB_START()	//sda falling when scl=1
{
	SCCB_SDA_OP(1);	//idle scl 1 sda 1
	SCCB_DELAY(1);
	SCCB_SCL_OP(1);
	SCCB_DELAY(1);
	SCCB_SDA_OP(0); 
	SCCB_DELAY(1);
	SCCB_SCL_OP(0);	
	
}
void SCCB_STOP()	//sda rising when scl=1
{
	SCCB_SCL_OP(0);
	SCCB_DELAY(1);
	SCCB_SDA_OP(0);  
	SCCB_DELAY(1);
	SCCB_SCL_OP(1);	//restore idle
	SCCB_DELAY(1);
	SCCB_SDA_OP(1); 
}
void SCCB_SCL_ONE_CYCLE(uint8_t na)	//iic is ack sccb dont care
{
	SCCB_SCL_OP(0);
	SCCB_DELAY(1);
	if(na)
		SCCB_SDA_OP(1); 
	else
		SCCB_SDA_OP(0); 
	SCCB_DELAY(1);
	SCCB_SCL_OP(1);
	SCCB_DELAY(1);
	SCCB_SCL_OP(0);
}
void SCCB_SEND_BYTE(uint8_t dat)	// scl 0 sda change scl rising sda be read
{
	for(uint8_t i=0; i<8; i++)
	{
		SCCB_SCL_OP(0);	
		SCCB_DELAY(1);
		if(dat&0x80)SCCB_SDA_OP(1);
		else SCCB_SDA_OP(0);
		SCCB_DELAY(1);
		SCCB_SCL_OP(1);	
		SCCB_DELAY(1); 
		dat <<= 1;
	}
	SCCB_SCL_OP(0);	
}
uint8_t SCCB_READ_BYTE(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_PinState PinState; 
	GPIO_InitStruct.Pin = OV7670_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT; 
	GPIO_InitStruct.Pull = GPIO_PULLUP;//GPIO_NOPULL;
  HAL_GPIO_Init(OV7670_SDA_GPIO_Port, &GPIO_InitStruct);
	 
	uint8_t dat;
	for(uint8_t i=0;i<8;i++)
	{
		SCCB_SCL_OP(0);
		SCCB_DELAY(1);
		SCCB_SCL_OP(1);
		SCCB_DELAY(1); 
		PinState = HAL_GPIO_ReadPin(OV7670_SDA_GPIO_Port, OV7670_SDA_Pin);
		dat <<= 1; 
		dat |= (uint8_t)PinState; 
	}
	SCCB_SCL_OP(0);	
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; 
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(OV7670_SDA_GPIO_Port, &GPIO_InitStruct);
	return dat;
}
/*public variable*/

/*private funtion decaleration*/
//io
void BSP_OV_WRITE_REG(uint8_t reg, uint8_t dat)
{ 
	SCCB_START();
	SCCB_SEND_BYTE(BSP_OV7670_I2C_ADDR);
	SCCB_SCL_ONE_CYCLE(0);
	SCCB_SEND_BYTE(reg);
	SCCB_SCL_ONE_CYCLE(0);
	SCCB_SEND_BYTE(dat);
	SCCB_SCL_ONE_CYCLE(0);	
	SCCB_STOP();
}
uint8_t BSP_OV_READ_REG(uint8_t reg)
{
	uint8_t dat;
	SCCB_START();
	SCCB_SEND_BYTE(BSP_OV7670_I2C_ADDR);
	SCCB_SCL_ONE_CYCLE(0);	
	SCCB_SEND_BYTE(reg);
	SCCB_SCL_ONE_CYCLE(0);	
	SCCB_START();
	SCCB_SEND_BYTE(BSP_OV7670_I2C_ADDR + 1);
	SCCB_SCL_ONE_CYCLE(0);	
	dat = SCCB_READ_BYTE();
	SCCB_SCL_ONE_CYCLE(1);	
	return dat;
}
/*public funtion decaleration*/

/*funtion*/
uint16_t BSP_OV_INIT(void)
{ 
	uint8_t pid, ver;
	pid = BSP_OV_READ_REG(OV7670_PID);
	ver = BSP_OV_READ_REG(OV7670_VER); 
	return (uint16_t)(pid<<8|ver);
}
