#ifndef __BSP_BSP_W25QXX_H__
#define __BSP_BSP_W25QXX_H__
/*public includes*/
#include "stm32f1xx_hal.h"
/*public macro*/
#define  W25Q16_ID	0xEF3015     //W25X16   2MB
#define  W25Q32_ID	0xEF4015	   //W25Q16   4MB
#define  W25Q64_ID	0XEF4017     //W25Q64   8MB
#define  W25Q128_ID	0XEF4018     //W25Q128  16MB
/*public type*/
typedef struct {
	uint32_t mftr_device_id;
	uint32_t jedec_id;
	uint64_t unique_id;
	
	uint32_t page_size;
	uint32_t page_count;
	uint32_t sector_size;
	uint32_t sector_count;
	uint32_t block_size;
	uint32_t block_count;
	uint32_t capicity;
	
	uint8_t name[10];
} w25qxx_description_strutct;

/*public variable*/
extern w25qxx_description_strutct bsp_w25qxx ;
/*public  funtion decaleration*/
uint8_t BSP_W25QXX_INIT(uint8_t clear);

void BSP_W25QXX_ERASE_BLOCK(uint32_t addr);
void BSP_W25QXX_ERASE_SECTOR(uint32_t addr);
void BSP_W25QXX_ERASE_CHIP(void);
 
uint8_t BSP_W25QXX_ID(void);
uint32_t BSP_W25QXX_JEDEC_ID(void);
uint32_t BSP_W25QXX_MFTR_DEVICE_ID(void);
uint64_t BSP_W25QXX_UNIQUE_ID(void);

void BSP_W25QXX_POWER_DOWN(void);
void BSP_W25QXX_WAKE_UP(void);

void BSP_W25QXX_PROG_PAGE(uint32_t addr, uint8_t *buf, uint32_t len);
void BSP_W25QXX_PROG_SECTOR(uint32_t addr, uint8_t *buf, uint32_t len);
void BSP_W25QXX_READ_DATA(uint32_t addr, uint8_t *buf, uint32_t len);
void BSP_W25QXX_FAST_READ_DATA(uint32_t addr, uint8_t *buf, uint32_t len);

uint8_t BSP_W25QXX_READ_SR(uint8_t sr);
void BSP_W25QXX_WRITE_SR(uint8_t sr,uint8_t dat);
#endif
