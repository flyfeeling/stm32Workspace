#include "bsp_sram.h"

/*private includes*/
#include "main.h"
#include "fsmc.h"
/*private macro*/
#define BSP_FSMC_BANK1_SRAM1_ADDR_START	(0x60000000)
#define BSP_FSMC_BANK1_SRAM1_ADDR_END		(0x63FFFFFF)
#define BSP_FSMC_BANK1_SRAM2_ADDR_START	(0x64000000)
#define BSP_FSMC_BANK1_SRAM2_ADDR_END		(0x67FFFFFF)
#define BSP_FSMC_BANK1_SRAM3_ADDR_START	(0x68000000)
#define BSP_FSMC_BANK1_SRAM3_ADDR_END		(0x6BFFFFFF)
#define BSP_FSMC_BANK1_SRAM4_ADDR_START	(0x6C000000)
#define BSP_FSMC_BANK1_SRAM4_ADDR_END		(0x6FFFFFFF)

#define BSP_SRAM1_SIZE	0
#define BSP_SRAM2_SIZE	0
#define BSP_SRAM3_SIZE	(1024*1024)
#define BSP_SRAM4_SIZE	0

/*private variable*/

/*public variable*/

/*private funtion decaleration*/

/*public funtion decaleration*/

/*funtion*/

void BSP_FSMC_SRAM_WRITE(uint32_t addr, uint8_t* buf, uint32_t len)
{
	if(addr<BSP_FSMC_BANK1_SRAM3_ADDR_START || addr>BSP_FSMC_BANK1_SRAM3_ADDR_END || addr>BSP_FSMC_BANK1_SRAM3_ADDR_START+BSP_SRAM3_SIZE)
	{
		Error_Handler();
	}
	for(uint32_t i=0; i<len; i++)
	{
		*((volatile uint8_t*)(BSP_FSMC_BANK1_SRAM3_ADDR_START+addr+i)) = *(buf+i);
	}
	//HAL_SRAM_Write_8b(&hsram1, (uint32_t*)addr, buf, len);
}
void BSP_FSMC_SRAM_READ(uint32_t addr, uint8_t* buf, uint32_t len)
{
	if(addr<BSP_FSMC_BANK1_SRAM3_ADDR_START || addr>BSP_FSMC_BANK1_SRAM3_ADDR_END || addr>BSP_FSMC_BANK1_SRAM3_ADDR_START+BSP_SRAM3_SIZE)
	{
		Error_Handler();
	}
	for(uint32_t i=0; i<len; i++)
	{
		*(buf+i) = *((volatile uint8_t*)(BSP_FSMC_BANK1_SRAM3_ADDR_START+addr+i));
	}
}