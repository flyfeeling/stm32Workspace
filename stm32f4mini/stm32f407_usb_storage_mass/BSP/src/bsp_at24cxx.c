#include "bsp_at24cxx.h"

/*private includes*/
#include "i2c.h"
#include "bsp_delay.h"
/*private macro*/
#define BSP_AT24CXX_TIMEOUT 		0xFFFFF
#define BSP_AT24C02_I2C_ADDR 		0XA0
#define BSP_AT24C02_I2C_HANDLE 	hi2c1
/*private variable*/

/*public variable*/

/*private funtion decaleration*/
void BSP_AT24CXX_DELAY(uint32_t us)
{ 
	BSP_DELAY_US(us);
}
/*public funtion decaleration*/

/*funtion*/
void BSP_AT24CXX_WRITE_BYTE(uint8_t addr, uint8_t dat)
{
	HAL_I2C_Mem_Write(&BSP_AT24C02_I2C_HANDLE, BSP_AT24C02_I2C_ADDR, addr, I2C_MEMADD_SIZE_8BIT, &dat, 1, BSP_AT24CXX_TIMEOUT);
	//BSP_AT24CXX_DELAY(0);
}
uint8_t BSP_AT24CXX_READ_BYTE(uint8_t addr)
{
	uint8_t dat = 0xFF;
	HAL_I2C_Mem_Read(&BSP_AT24C02_I2C_HANDLE, BSP_AT24C02_I2C_ADDR, addr, I2C_MEMADD_SIZE_8BIT, &dat, 1, BSP_AT24CXX_TIMEOUT);
	return dat;
}
void BSP_AT24CXX_WRITE(uint8_t addr, uint8_t* dat, uint32_t len)
{
	for(; len>0; len--,addr++,dat++)
	{
		BSP_AT24CXX_WRITE_BYTE(addr, *dat);
	}
}
void BSP_AT24CXX_READ(uint8_t addr, uint8_t *buf, uint32_t len)
{
	HAL_I2C_Mem_Read(&BSP_AT24C02_I2C_HANDLE, BSP_AT24C02_I2C_ADDR, addr, I2C_MEMADD_SIZE_8BIT, buf, len, BSP_AT24CXX_TIMEOUT);
}
