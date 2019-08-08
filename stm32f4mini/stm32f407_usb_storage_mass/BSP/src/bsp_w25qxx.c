/*private includes*/
#include "bsp_w25qxx.h"
#include "bsp_w25qxx_def.h"
#include "spi.h"
#include "stdio.h"
/*private macro*/ 
#define W25QXX_SPI_HANDLE	hspi1
#define WIP_Flag                      0x01  /* Write In Progress (WIP) flag */
//register
//decription
#define CAPICITY			(16*1024*1024)
#define PAGE_SIZE			(256)											//byte 
#define PAGE_COUNT		(CAPICITY/PAGE_SIZE)			//65536
#define SECTOR_SIZE		(4096)
#define SECTOR_COUNT 	(CAPICITY/SECTOR_SIZE)		//4096
#define BLOCK_SIZE		(64*1024)
#define BLOCK_COUNT		(CAPICITY/BLOCK_SIZE)			//256
#define W25Q_SPI_TIMEOUT (0xFFFFFF)
/*private variable*/
__IO uint8_t BSP_W25QXX_SECTOR_CACHE[SECTOR_SIZE] = {0};
__IO uint8_t *cache_ptr;
/*private funtion decaleration*/
 
/*public variable*/
w25qxx_description_strutct bsp_w25qxx = {0};

/*funtion*/
/*IO*/
uint8_t BSP_W25QXX_READ_BYTE(void)
{
  uint8_t d_read,d_send=0xFF;
  if(HAL_SPI_TransmitReceive(&W25QXX_SPI_HANDLE,&d_send,&d_read,1,0xFFFFFF)!=HAL_OK)d_read=0xFF;
  return d_read;    
}


uint8_t BSP_W25QXX_SEND_BYTE(uint8_t byte)
{
  uint8_t d_read,d_send=byte;
  if(HAL_SPI_TransmitReceive(&W25QXX_SPI_HANDLE,&d_send,&d_read,1,0xFFFFFF)!=HAL_OK)d_read=0xFF;
  return d_read; 
}

void BSP_W25QXX_CS_SELECT(uint8_t cs)
{
	cs?HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET):HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
}

void BSP_W25QXX_WRITE_ENABLE(void)
{ 
  BSP_W25QXX_CS_SELECT(0);  
	BSP_W25QXX_SEND_BYTE(W25Q_WRITE_EN);
  BSP_W25QXX_CS_SELECT(1); 
}

#if 0
void BSP_W25QXX_WRITE_DISABLE(void)
{
  BSP_W25QXX_CS_SELECT(0);  
	BSP_W25QXX_SEND_BYTE(W25Q_WRITE_DEN);
  BSP_W25QXX_CS_SELECT(1); 
}
#endif
void BSP_W25QXX_IS_BUSY(void)
{   
  while ((BSP_W25QXX_READ_SR(1)&WIP_Flag)==SET){
	
	} /* Write in progress */ 
}

//funtion
void BSP_W25QXX_ERASE_SECTOR(uint32_t addr)
{
	addr = addr/SECTOR_SIZE;
	addr = addr*SECTOR_SIZE;
	
	BSP_W25QXX_WRITE_ENABLE();
  BSP_W25QXX_IS_BUSY(); 
  BSP_W25QXX_CS_SELECT(0);  
	BSP_W25QXX_SEND_BYTE(W25Q_SECTOR_ERASE);
	BSP_W25QXX_SEND_BYTE((uint8_t)(addr>>16));
	BSP_W25QXX_SEND_BYTE((uint8_t)(addr>>8));
	BSP_W25QXX_SEND_BYTE((uint8_t)(addr>>0)); 
  BSP_W25QXX_CS_SELECT(1); 
  BSP_W25QXX_IS_BUSY();
}
void BSP_W25QXX_ERASE_BLOCK(uint32_t addr)
{
	addr = addr/BLOCK_SIZE;
	addr = addr*BLOCK_SIZE;
	
	BSP_W25QXX_WRITE_ENABLE();
  BSP_W25QXX_IS_BUSY(); 
  BSP_W25QXX_CS_SELECT(0);  
	BSP_W25QXX_SEND_BYTE(W25Q_BLK64_ERASE);
	BSP_W25QXX_SEND_BYTE((uint8_t)(addr>>16));
	BSP_W25QXX_SEND_BYTE((uint8_t)(addr>>8));
	BSP_W25QXX_SEND_BYTE((uint8_t)(addr>>0)); 
  BSP_W25QXX_CS_SELECT(1); 
  BSP_W25QXX_IS_BUSY();
}
void BSP_W25QXX_ERASE_CHIP(void)
{ 
	BSP_W25QXX_WRITE_ENABLE(); 
  BSP_W25QXX_IS_BUSY(); 
  BSP_W25QXX_CS_SELECT(0);  
	BSP_W25QXX_SEND_BYTE(W25Q_CHIP_ERASE);
  BSP_W25QXX_CS_SELECT(1); 
  BSP_W25QXX_IS_BUSY();
} 

void BSP_W25QXX_PROG_PAGE(uint32_t addr, uint8_t *buf, uint32_t len)
{
	if(len>PAGE_SIZE)return;
	addr = addr/PAGE_SIZE;
	addr = addr*PAGE_SIZE;
	
	BSP_W25QXX_WRITE_ENABLE(); 
  BSP_W25QXX_CS_SELECT(0);  
	BSP_W25QXX_SEND_BYTE(W25Q_PAGE_PROG);
	BSP_W25QXX_SEND_BYTE((uint8_t)(addr>>16));
	BSP_W25QXX_SEND_BYTE((uint8_t)(addr>>8));
	BSP_W25QXX_SEND_BYTE((uint8_t)(addr>>0));  
	for(; len>0; len--,buf++){
		BSP_W25QXX_SEND_BYTE(*buf); 
	}
  BSP_W25QXX_CS_SELECT(1); 
  BSP_W25QXX_IS_BUSY();
}
void BSP_W25QXX_PROG_SECTOR(uint32_t addr, uint8_t *buf, uint32_t len)
{
	if(len>SECTOR_SIZE)return;
	addr = addr/SECTOR_SIZE;
	addr = addr*SECTOR_SIZE;
	 
	BSP_W25QXX_READ_DATA(addr, (uint8_t *)BSP_W25QXX_SECTOR_CACHE, SECTOR_SIZE);
	BSP_W25QXX_ERASE_SECTOR(addr);
	for(uint32_t i=0; i<len; i++,buf++)
	{
		BSP_W25QXX_SECTOR_CACHE[i] = *buf;
	}
	cache_ptr = BSP_W25QXX_SECTOR_CACHE;
	for(uint32_t i=0; i<(SECTOR_SIZE/PAGE_SIZE); i++)
	{  
		BSP_W25QXX_PROG_PAGE(addr, (uint8_t *)cache_ptr, PAGE_SIZE); 
		cache_ptr+=PAGE_SIZE;
		addr+=PAGE_SIZE;
	}  
}

void BSP_W25QXX_READ_DATA(uint32_t addr, uint8_t *buf, uint32_t len)
{  
  BSP_W25QXX_CS_SELECT(0);  
	BSP_W25QXX_SEND_BYTE(W25Q_READ_DATA);
	BSP_W25QXX_SEND_BYTE((uint8_t)(addr>>16));
	BSP_W25QXX_SEND_BYTE((uint8_t)(addr>>8));
	BSP_W25QXX_SEND_BYTE((uint8_t)(addr>>0)); 
	for(; len>0; len--,buf++){
		*buf = BSP_W25QXX_READ_BYTE(); 
	}
  BSP_W25QXX_CS_SELECT(1);  
}
void BSP_W25QXX_FAST_READ_DATA(uint32_t addr, uint8_t *buf, uint32_t len)
{ 
	BSP_W25QXX_IS_BUSY(); 
  BSP_W25QXX_CS_SELECT(0);  
	BSP_W25QXX_SEND_BYTE(W25Q_FAST_READ);
	BSP_W25QXX_SEND_BYTE((uint8_t)(addr>>16));
	BSP_W25QXX_SEND_BYTE((uint8_t)(addr>>8));
	BSP_W25QXX_SEND_BYTE((uint8_t)(addr>>0)); 
	BSP_W25QXX_SEND_BYTE(0xFF); 
	for(; len>0; len--,buf++){
		*buf = BSP_W25QXX_READ_BYTE(); 
	}
  BSP_W25QXX_CS_SELECT(1); 
  BSP_W25QXX_IS_BUSY();
}
uint8_t BSP_W25QXX_ID()
{
	BSP_W25QXX_IS_BUSY(); 
  BSP_W25QXX_CS_SELECT(0);  
	uint8_t id=0;
	BSP_W25QXX_SEND_BYTE(W25Q_RELASE_POWER_DOWN);
	id = BSP_W25QXX_SEND_BYTE(0xFF);	 
	id = BSP_W25QXX_SEND_BYTE(0xFF);	 
	id = BSP_W25QXX_SEND_BYTE(0xFF);	 
	id = BSP_W25QXX_SEND_BYTE(0xFF);	//id7_0 
  BSP_W25QXX_CS_SELECT(1); 
  BSP_W25QXX_IS_BUSY();
		
	return id;
}
uint32_t BSP_W25QXX_JEDEC_ID()
{ 
	BSP_W25QXX_IS_BUSY(); 
  BSP_W25QXX_CS_SELECT(0);  
	uint8_t id[3] = {0}; 
	BSP_W25QXX_SEND_BYTE(W25Q_JEDEC_ID);
	id[0] = BSP_W25QXX_SEND_BYTE(0xFF);	//mf7_0
	id[1] = BSP_W25QXX_SEND_BYTE(0xFF);	//id15_8
	id[2] = BSP_W25QXX_SEND_BYTE(0xFF);	//id7_0 
  BSP_W25QXX_CS_SELECT(1); 
  BSP_W25QXX_IS_BUSY();
		
	return (uint32_t)((id[0]<<16)|(id[1]<<8)|(id[2]<<0));
}

uint32_t BSP_W25QXX_MFTR_DEVICE_ID()
{  
	uint8_t id[2] = {0};
 
  BSP_W25QXX_CS_SELECT(0); 
  BSP_W25QXX_SEND_BYTE(W25Q_MFTR_DEVICE_ID);
  BSP_W25QXX_SEND_BYTE(0xFF);
  BSP_W25QXX_SEND_BYTE(0xFF);
  BSP_W25QXX_SEND_BYTE(0x00);
  id[0] = BSP_W25QXX_SEND_BYTE(0xFF);	//mf7_0
	id[1] = BSP_W25QXX_SEND_BYTE(0xFF);	//id7_0
  BSP_W25QXX_CS_SELECT(1); 

  return (id[0]<<8|id[1]);
}

uint64_t BSP_W25QXX_UNIQUE_ID()
{ 
	BSP_W25QXX_IS_BUSY(); 
  BSP_W25QXX_CS_SELECT(0);  
 
	uint8_t data = 0;
	uint64_t unqiue_id = 0;
 
	BSP_W25QXX_SEND_BYTE(W25Q_UNIQUE_ID);
	BSP_W25QXX_SEND_BYTE(0xFF);
	BSP_W25QXX_SEND_BYTE(0xFF);
	BSP_W25QXX_SEND_BYTE(0xFF);
	BSP_W25QXX_SEND_BYTE(0xFF);
	for(int i=0; i<8; i++){
		unqiue_id = unqiue_id << 8;
		data = BSP_W25QXX_READ_BYTE(); 
		unqiue_id = unqiue_id | data;
	}
  BSP_W25QXX_CS_SELECT(1); 
  BSP_W25QXX_IS_BUSY();
	
	return unqiue_id;
} 

void BSP_W25QXX_POWER_DOWN(void)   
{   
  BSP_W25QXX_CS_SELECT(0);  
	BSP_W25QXX_SEND_BYTE(W25Q_POWER_DOWN);
  BSP_W25QXX_CS_SELECT(1);  
}   

 
void BSP_W25QXX_WAKE_UP(void)   
{  
  BSP_W25QXX_CS_SELECT(0);  
	BSP_W25QXX_SEND_BYTE(W25Q_RELASE_POWER_DOWN);
  BSP_W25QXX_CS_SELECT(1);  
}  

#if 0
void BSP_W25QXX_RESET_DEVICE(void)   
{  
  BSP_W25QXX_CS_SELECT(0);  
	BSP_W25QXX_SEND_BYTE(W25Q_RESET_DEVICE);
  BSP_W25QXX_CS_SELECT(1);  
}  
void BSP_W25QXX_ENABLE_RESET(void)   
{  
  BSP_W25QXX_CS_SELECT(0);  
	BSP_W25QXX_SEND_BYTE(W25Q_EANBLE_RESET);
  BSP_W25QXX_CS_SELECT(1);  
}  
void BSP_W25QXX_GLOBAL_BLOCK_LOCK()
{  
  BSP_W25QXX_CS_SELECT(0);  
	BSP_W25QXX_SEND_BYTE(W25Q_GLB_BLK_LOCK);
  BSP_W25QXX_CS_SELECT(1);  
}  
void BSP_W25QXX_GLOBAL_BLOCK_UNLOCK()
{  
  BSP_W25QXX_CS_SELECT(0);  
	BSP_W25QXX_SEND_BYTE(W25Q_GLB_BLK_UNLOCK);
  BSP_W25QXX_CS_SELECT(1);  
} 
void BSP_W25QXX_ERASE_PROGRAM_SUSPEND()
{  
  BSP_W25QXX_CS_SELECT(0);  
	BSP_W25QXX_SEND_BYTE(W25Q_ERASE_PROG_SUSPEND);
  BSP_W25QXX_CS_SELECT(1);  
} 
void BSP_W25QXX_ERASE_PROGRAM_RESUME()
{  
  BSP_W25QXX_CS_SELECT(0);  
	BSP_W25QXX_SEND_BYTE(W25Q_ERASE_PROG_RESUME);
  BSP_W25QXX_CS_SELECT(1);  
}
#endif
void BSP_W25QXX_WRITE_SR(uint8_t sr,uint8_t dat)
{
  BSP_W25QXX_CS_SELECT(0);  
	switch(sr){
		case 1:BSP_W25QXX_SEND_BYTE(W25Q_WRITE_REG_1);break;
		case 2:BSP_W25QXX_SEND_BYTE(W25Q_WRITE_REG_2);break;
		case 3:BSP_W25QXX_SEND_BYTE(W25Q_WRITE_REG_3);break;
		default:BSP_W25QXX_CS_SELECT(1);return;
	} 
	BSP_W25QXX_SEND_BYTE(dat);
  BSP_W25QXX_CS_SELECT(1); 
}
uint8_t BSP_W25QXX_READ_SR(uint8_t sr)
{
	uint8_t dat=0;
  BSP_W25QXX_CS_SELECT(0);  
	switch(sr){
		case 1:BSP_W25QXX_SEND_BYTE(W25Q_READ_REG_1);break;
		case 2:BSP_W25QXX_SEND_BYTE(W25Q_READ_REG_2);break;
		case 3:BSP_W25QXX_SEND_BYTE(W25Q_READ_REG_3);break;
		default:BSP_W25QXX_CS_SELECT(1);return 0;
	} 
	dat = BSP_W25QXX_READ_BYTE();
  BSP_W25QXX_CS_SELECT(1); 
	return dat;
}

uint8_t BSP_W25QXX_INIT(uint8_t clear)
{
	if(clear)BSP_W25QXX_ERASE_CHIP();
	
	
	bsp_w25qxx.page_size = PAGE_SIZE;
	bsp_w25qxx.page_count = PAGE_COUNT;
	bsp_w25qxx.block_count = BLOCK_COUNT;
	bsp_w25qxx.block_size = BLOCK_SIZE;
	bsp_w25qxx.sector_count = SECTOR_COUNT;
	bsp_w25qxx.sector_size = SECTOR_SIZE;
	bsp_w25qxx.capicity = CAPICITY;
	
	bsp_w25qxx.unique_id = BSP_W25QXX_UNIQUE_ID();
	bsp_w25qxx.mftr_device_id = BSP_W25QXX_MFTR_DEVICE_ID();
	bsp_w25qxx.jedec_id = BSP_W25QXX_JEDEC_ID();
	
	  
	switch(bsp_w25qxx.jedec_id)
	{
		case W25Q128_ID: sprintf((char*)bsp_w25qxx.name, "W25Q128");break;
		case W25Q64_ID: sprintf((char*)bsp_w25qxx.name, "W25Q64");break;
		case W25Q32_ID: sprintf((char*)bsp_w25qxx.name, "W25Q32");break;
		case W25Q16_ID: sprintf((char*)bsp_w25qxx.name, "W25Q16");break;
		default : sprintf((char*)bsp_w25qxx.name, "NoneType");break;
	}
	BSP_W25QXX_SEND_BYTE(0xFF);
	return 0;
}
