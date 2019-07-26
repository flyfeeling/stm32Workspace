#include "bsp_ov7670.h"

/*private includes*/
#include "bsp_ov7670_def.h"
#include "bsp_delay.h"
#include "i2c.h"
#include "dcmi.h"
#include "main.h"
#include "stdio.h"
/*private macro*/
#define BSP_OV7670_TIMEOUT 	10
#define BSP_OV7670_I2C_ADDR 0X42

/*private variable*/
void SCCB_DELAY(uint32_t d)
{
	//HAL_Delay(d);
	BSP_DELAY_US(d);
}
void SCCB_SDA_OP(uint8_t b)
{
	if(b){
		HAL_GPIO_WritePin(OV7670_SDA_GPIO_Port, OV7670_SDA_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(OV7670_SDA_GPIO_Port, OV7670_SDA_Pin, GPIO_PIN_RESET);
	} 
}
void SCCB_SCL_OP(uint8_t b)
{
	if(b){
		HAL_GPIO_WritePin(OV7670_SCL_GPIO_Port, OV7670_SCL_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(OV7670_SCL_GPIO_Port, OV7670_SCL_Pin, GPIO_PIN_RESET);
	} 
}

void SCCB_SDA_IN()
{
	GPIO_InitTypeDef GPIO_InitStruct; 
	GPIO_InitStruct.Pin = OV7670_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  
	GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(OV7670_SDA_GPIO_Port, &GPIO_InitStruct); 
}
void SCCB_SDA_OUT()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = OV7670_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; 
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(OV7670_SDA_GPIO_Port, &GPIO_InitStruct); 
}
void SCCB_START()	//sda falling when scl=1
{
	SCCB_SDA_OP(1);	//idle scl 1 sda 1 
	SCCB_SCL_OP(1);
	SCCB_DELAY(50);
	SCCB_SDA_OP(0); 
	SCCB_DELAY(50);
	SCCB_SCL_OP(0);	
	
}
void SCCB_STOP()	//sda rising when scl=1
{
	SCCB_SDA_OP(0);  
	SCCB_DELAY(50);
	SCCB_SCL_OP(1);
	SCCB_DELAY(50); 
	SCCB_SDA_OP(1); //restore idle
	SCCB_DELAY(50); 
}
void SCCB_NA(void)	//iic is ack sccb dont care
{
	SCCB_DELAY(50);
	SCCB_SDA_OP(1);
	SCCB_SCL_OP(1); 
	SCCB_DELAY(50);
	SCCB_SCL_OP(0);
	SCCB_DELAY(50);
	SCCB_SDA_OP(0);
	SCCB_DELAY(50);
}
uint8_t SCCB_SEND_BYTE(uint8_t dat)	// scl 0 sda change scl rising sda be read
{ 
	uint8_t ret = 0;
	for(uint8_t i=0; i<8; i++)
	{ 
		if(dat&0x80){
			SCCB_SDA_OP(1);
		}else{
			SCCB_SDA_OP(0);
		}
		SCCB_DELAY(50);
		SCCB_SCL_OP(1);	
		SCCB_DELAY(50); 
		SCCB_SCL_OP(0);	
		dat <<= 1;
	}
	SCCB_SDA_IN();
	SCCB_DELAY(50);
	SCCB_SCL_OP(1);	
	SCCB_DELAY(50);
	if(HAL_GPIO_ReadPin(OV7670_SDA_GPIO_Port, OV7670_SDA_Pin) == GPIO_PIN_SET){
		ret = 0xFF;
	}
	SCCB_SCL_OP(0);
	SCCB_SDA_OUT();
	return ret;
}
uint8_t SCCB_READ_BYTE(void)
{ 
	uint8_t dat;
	SCCB_SDA_IN();
	for(uint8_t i=0;i<8;i++)
	{
		SCCB_DELAY(50);
		SCCB_SCL_OP(1);
		dat <<= 1; 
		if(HAL_GPIO_ReadPin(OV7670_SDA_GPIO_Port, OV7670_SDA_Pin) == GPIO_PIN_SET)
		{
			dat += 1;
		} 
		SCCB_DELAY(50); 
		SCCB_SCL_OP(0);
	}
	SCCB_SDA_OUT();	 
	return dat;
}
/*public variable*/

camera_description_strutct bsp_ov7670 = {0};
/*private funtion decaleration*/
//io
void BSP_OV_WRITE_REG(uint8_t reg, uint8_t dat)
{ 
	SCCB_START(); 
	if(SCCB_SEND_BYTE(BSP_OV7670_I2C_ADDR))printf("ov_write_reg1 no ack\r\n");
	SCCB_DELAY(100);
	if(SCCB_SEND_BYTE(reg))printf("ov_write_reg2 no ack\r\n");
	SCCB_DELAY(100);
	if(SCCB_SEND_BYTE(dat))printf("ov_write_reg3 no ack\r\n");
	SCCB_STOP();
	 
}
uint8_t BSP_OV_READ_REG(uint8_t reg)
{
 	uint8_t dat;
	SCCB_START();  
	if(SCCB_SEND_BYTE(BSP_OV7670_I2C_ADDR))printf("ov_read_reg1 no ack\r\n");
	SCCB_DELAY(100); 
	if(SCCB_SEND_BYTE(reg))printf("ov_read_reg2 no ack\r\n");
	SCCB_DELAY(100);
	SCCB_STOP();
	SCCB_DELAY(100);
	SCCB_START(); 
	if(SCCB_SEND_BYTE(BSP_OV7670_I2C_ADDR|0x01))printf("ov_read_reg3 no ack\r\n");
	SCCB_DELAY(100);
	dat = SCCB_READ_BYTE();  
	SCCB_NA(); 
	SCCB_STOP();
	return dat;
 
	return dat;
}
/*public funtion decaleration*/

/*funtion*/
void BSP_OV_INIT(void)
{ 
	uint8_t id[2];
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_4);
	BSP_DELAY_MS(100); 
	id[0] = BSP_OV_READ_REG(OV7670_PID);
	id[1] = BSP_OV_READ_REG(OV7670_VER); 
	bsp_ov7670.id = (uint16_t)(id[0]<<8|id[1]);
	id[0] = BSP_OV_READ_REG(OV7670_MIDH);
	id[1] = BSP_OV_READ_REG(OV7670_MIDL); 
	bsp_ov7670.mftr_id = (uint16_t)(id[0]<<8|id[1]); 
}
void BSP_OV_SOFT_RST(void)
{
	BSP_OV_WRITE_REG(OV7670_COM7, 0x80);
	BSP_DELAY_MS(10);
}

