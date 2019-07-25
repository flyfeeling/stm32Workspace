#ifndef __W25QXX_H__
#define __W25QXX_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
//#define  SPI_FLASH_ID                       0xEF3015     //W25X16   2MB
//#define  SPI_FLASH_ID                       0xEF4015	   //W25Q16   4MB
//#define  SPI_FLASH_ID                       0XEF4017     //W25Q64   8MB
#define  SPI_FLASH_ID                       0XEF4018     //W25Q128  16MB YS-F1Pro����Ĭ��ʹ��

 

/* ��չ���� ------------------------------------------------------------------*/
 
extern __IO uint32_t DeviceID;
extern __IO uint32_t FlashID;
/* �������� ------------------------------------------------------------------*/
//io
uint8_t SPI_FLASH_ReadByte(void);
uint8_t SPI_FLASH_SendByte(uint8_t byte);
//description
uint32_t SPI_FLASH_ReadID(void);
uint32_t SPI_FLASH_ReadDeviceID(void);
//lpmode
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);
//ctrol
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);
void MX_SPIFlash_Init(void);
//erase
void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_BulkErase(void);
//funtion
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);


#endif
