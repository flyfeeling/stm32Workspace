/*private includes*/
#include "bsp_enc28j60.h"
#include "spi.h"
#include "stdio.h"
/*private macro*/
#define ENC28J60_SPI_HANDLE		hspi2
#define ENC28J60_SPI_TIMEOUT	0xFFFFFFFF

#define ENC28J60_ADDRESS_MASK	0X1F
#define ENC28J60_BANK_MASK		0X60
#define ENC28J60_PHY_MASK			0X80
/*private variable*/

/*public variable*/

/*private funtion decaleration*/

void BSP_ENC28J60_CS_SELECTED(uint8_t cs)
{
	cs?HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_RESET):HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}
void BSP_ENC28J60_SOFT_RESET(void)
{
	uint8_t cmd=0XFF;
	BSP_ENC28J60_CS_SELECTED(1);
	HAL_SPI_Transmit(&ENC28J60_SPI_HANDLE, &cmd, 1, ENC28J60_SPI_TIMEOUT); 
	BSP_ENC28J60_CS_SELECTED(0);
}
void BSP_ENC28J60_WRITE_REG(uint8_t  reg, uint8_t  dat)
{
	uint8_t rev=0;
	BSP_ENC28J60_CS_SELECTED(1);
	if(reg >= EIE)//write cr
	{
		reg = (reg & ENC28J60_ADDRESS_MASK); 
		reg = (reg | 0X40);
//		HAL_SPI_TransmitReceive(&ENC28J60_SPI_HANDLE, &reg, &rev, 1, ENC28J60_SPI_TIMEOUT);
//		HAL_SPI_TransmitReceive(&ENC28J60_SPI_HANDLE, &dat, &rev, 1, ENC28J60_SPI_TIMEOUT); 
		HAL_SPI_Transmit(&ENC28J60_SPI_HANDLE, &reg, 1, ENC28J60_SPI_TIMEOUT); 
		HAL_SPI_Transmit(&ENC28J60_SPI_HANDLE, &dat, 1, ENC28J60_SPI_TIMEOUT); 
		BSP_ENC28J60_CS_SELECTED(0);
		return ;
	}
	uint8_t bank = ((reg & ENC28J60_BANK_MASK) >> 6);
	reg = (reg & ENC28J60_ADDRESS_MASK); 
	reg = (reg | 0X40);
	rev = BSP_ENC28J60_READ_REG(ECON1);
	rev = rev&~(0x03);
	rev = rev|bank;
	BSP_ENC28J60_WRITE_REG(ECON1, rev);
	HAL_SPI_Transmit(&ENC28J60_SPI_HANDLE, &reg, 1, ENC28J60_SPI_TIMEOUT); 
	HAL_SPI_Transmit(&ENC28J60_SPI_HANDLE, &dat, 1, ENC28J60_SPI_TIMEOUT); 
	BSP_ENC28J60_CS_SELECTED(0);
}
uint8_t BSP_ENC28J60_READ_REG(uint8_t  reg)
{
	uint8_t rev = 0;
	uint8_t dummy = 0XFF;
	BSP_ENC28J60_CS_SELECTED(1);
	if(reg >= EIE)//read cr
	{ 
		reg = (reg & ENC28J60_ADDRESS_MASK); 
		HAL_SPI_TransmitReceive(&ENC28J60_SPI_HANDLE, &reg, &rev, 1, ENC28J60_SPI_TIMEOUT);
		HAL_SPI_TransmitReceive(&ENC28J60_SPI_HANDLE, &dummy, &rev, 1, ENC28J60_SPI_TIMEOUT);
		//HAL_SPI_TransmitReceive(&ENC28J60_SPI_HANDLE, &dummy, &reg, 1, ENC28J60_SPI_TIMEOUT); 
		BSP_ENC28J60_CS_SELECTED(0);
		return rev;
	}
	uint8_t bank = ((reg & ENC28J60_BANK_MASK) >> 6);
	reg = (reg & 0X1F); 
	rev = BSP_ENC28J60_READ_REG(ECON1);
	rev = rev&~(0x03);
	rev = rev|bank;
	BSP_ENC28J60_WRITE_REG(ECON1, rev);
	HAL_SPI_TransmitReceive(&ENC28J60_SPI_HANDLE, &reg, &rev, 1, ENC28J60_SPI_TIMEOUT);
	HAL_SPI_TransmitReceive(&ENC28J60_SPI_HANDLE, &dummy, &rev, 1, ENC28J60_SPI_TIMEOUT);
	BSP_ENC28J60_CS_SELECTED(0);
	return rev;
}
uint8_t BSP_ENC28J60_READ(uint8_t  dat)
{
	uint8_t rev = 0;   
	HAL_SPI_TransmitReceive(&ENC28J60_SPI_HANDLE, &dat, &rev, 1, ENC28J60_SPI_TIMEOUT);  
	return rev;
 
}
/*public funtion decaleration*/

/*funtion*/
