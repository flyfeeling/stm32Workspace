#include "bsp_sccb.h"

/*private includes*/
#include "bsp_delay.h"
#include "main.h"
/*private macro*/

/*private variable*/

/*public variable*/

/*private funtion decaleration*/

/*public funtion decaleration*/

/*funtion*/

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

void SCCB_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct; 
	GPIO_InitStruct.Pin = OV7670_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  
	GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(OV7670_SDA_GPIO_Port, &GPIO_InitStruct); 
}
void SCCB_SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = OV7670_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; 
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(OV7670_SDA_GPIO_Port, &GPIO_InitStruct); 
}
void SCCB_START(void)	//sda falling when scl=1
{
	SCCB_SDA_OP(1);	//idle scl 1 sda 1 
	SCCB_SCL_OP(1);
	SCCB_DELAY(5);//50
	SCCB_SDA_OP(0); 
	SCCB_DELAY(5);
	SCCB_SCL_OP(0);	
	
}
void SCCB_STOP(void)	//sda rising when scl=1
{
	SCCB_SDA_OP(0);  
	SCCB_DELAY(5);
	SCCB_SCL_OP(1);
	SCCB_DELAY(5); 
	SCCB_SDA_OP(1); //restore idle
	SCCB_DELAY(5); 
}
void SCCB_NA(void)	//iic is ack sccb dont care
{
	SCCB_DELAY(5);
	SCCB_SDA_OP(1);
	SCCB_SCL_OP(1); 
	SCCB_DELAY(5);
	SCCB_SCL_OP(0);
	SCCB_DELAY(5);
	SCCB_SDA_OP(0);
	SCCB_DELAY(5);
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
		SCCB_DELAY(0);
		SCCB_SCL_OP(1);	
		SCCB_DELAY(0); 
		SCCB_SCL_OP(0);	
		dat <<= 1;
	}
	SCCB_SDA_IN();
	SCCB_DELAY(0);
	SCCB_SCL_OP(1);	
	SCCB_DELAY(0);	//50
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
		SCCB_DELAY(0);	//50
		SCCB_SCL_OP(1);
		dat <<= 1; 
		if(HAL_GPIO_ReadPin(OV7670_SDA_GPIO_Port, OV7670_SDA_Pin) == GPIO_PIN_SET)
		{
			dat += 1;
		} 
		SCCB_DELAY(0); 
		SCCB_SCL_OP(0);
	}
	SCCB_SDA_OUT();	 
	return dat;
}
