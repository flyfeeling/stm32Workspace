/**
  ******************************************************************************
  * 文件名程: bsp_spiflash.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 板载串行Flash底层驱动实现
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F1Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */

/* 包含头文件 ----------------------------------------------------------------*/
#include "w25qxx.h"
#include "spi.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256
#define W25X_WriteEnable		            0x06 
#define W25X_WriteDisable		            0x04 
#define W25X_ReadStatusReg		          0x05 
#define W25X_WriteStatusReg		          0x01 
#define W25X_ReadData			              0x03 
#define W25X_FastReadData		            0x0B 
#define W25X_FastReadDual		            0x3B 
#define W25X_PageProgram		            0x02 
#define W25X_BlockErase			            0xD8 
#define W25X_SectorErase		            0x20 
#define W25X_ChipErase			            0xC7 
#define W25X_PowerDown			            0xB9 
#define W25X_ReleasePowerDown	          0xAB 
#define W25X_DeviceID			              0xAB 
#define W25X_ManufactDeviceID   	      0x90 
#define W25X_JedecDeviceID		          0x9F 

#define WIP_Flag                        0x01  /* Write In Progress (WIP) flag */
#define Dummy_Byte                      0xFF
     
#define FLASH_SPI_CS_PORT                          GPIOA
#define FLASH_SPI_CS_PIN                           GPIO_PIN_4
#define FLASH_SPI_CS_ENABLE()                      HAL_GPIO_WritePin(FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN, GPIO_PIN_RESET)
#define FLASH_SPI_CS_DISABLE()                     HAL_GPIO_WritePin(FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN, GPIO_PIN_SET)

/* 私有变量 ------------------------------------------------------------------*/
// SPI_HandleTypeDef hspiflash;
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

 
void MX_SPIFlash_Init(void)
{

}

void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{ 
  SPI_FLASH_WriteEnable();
  SPI_FLASH_WaitForWriteEnd(); 
  FLASH_SPI_CS_ENABLE(); 
  SPI_FLASH_SendByte(W25X_SectorErase); 
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16); 
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8); 
  SPI_FLASH_SendByte(SectorAddr & 0xFF); 
  FLASH_SPI_CS_DISABLE(); 
  SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_BulkErase(void)
{ 
  SPI_FLASH_WriteEnable(); 
  FLASH_SPI_CS_ENABLE(); 
  SPI_FLASH_SendByte(W25X_ChipErase); 
  FLASH_SPI_CS_DISABLE(); 
  SPI_FLASH_WaitForWriteEnd();
}

 
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{ 
  SPI_FLASH_WriteEnable(); 
  FLASH_SPI_CS_ENABLE(); 
  SPI_FLASH_SendByte(W25X_PageProgram); 
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16); 
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8); 
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize; 
  } 
  while(NumByteToWrite--)
  { 
    SPI_FLASH_SendByte(*pBuffer); 
    pBuffer++;
  } 
  FLASH_SPI_CS_DISABLE(); 
  SPI_FLASH_WaitForWriteEnd();
}

/**
  * 函数功能: 往串行FLASH写入数据，调用本函数写入数据前需要先擦除扇区
  * 输入参数: pBuffer：待写入数据的指针
  *           WriteAddr：写入地址
  *           NumByteToWrite：写入数据长度
  * 返 回 值: 无
  * 说    明：该函数可以设置任意写入数据长度
  */
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % SPI_FLASH_PageSize;
  count = SPI_FLASH_PageSize - Addr;
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

  if (Addr == 0) /* 若地址与 SPI_FLASH_PageSize 对齐  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /* 若地址与 SPI_FLASH_PageSize 不对齐 */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
      {
        temp = NumOfSingle - count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}


void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  FLASH_SPI_CS_ENABLE();
  SPI_FLASH_SendByte(W25X_ReadData);
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  SPI_FLASH_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) 
  { 
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte); 
    pBuffer++;
  }
  FLASH_SPI_CS_DISABLE();
}

 
uint32_t SPI_FLASH_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
 
  FLASH_SPI_CS_ENABLE(); 
  SPI_FLASH_SendByte(W25X_JedecDeviceID); 
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte); 
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte); 
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte); 
  FLASH_SPI_CS_DISABLE();
  
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
  return Temp;
}

 
uint32_t SPI_FLASH_ReadDeviceID(void)
{
  uint32_t Temp = 0;
 
  FLASH_SPI_CS_ENABLE();
  SPI_FLASH_SendByte(W25X_DeviceID);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  Temp = SPI_FLASH_SendByte(Dummy_Byte);
  FLASH_SPI_CS_DISABLE();

  return Temp;
}

 
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{ 
  FLASH_SPI_CS_ENABLE(); 
  SPI_FLASH_SendByte(W25X_ReadData); 
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16); 
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8); 
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
}
 
uint8_t SPI_FLASH_ReadByte(void)
{
  uint8_t d_read,d_send=Dummy_Byte;
  if(HAL_SPI_TransmitReceive(&hspi1,&d_send,&d_read,1,0xFFFFFF)!=HAL_OK)
    d_read=Dummy_Byte;
  
  return d_read;    
}


uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
  uint8_t d_read,d_send=byte;
  if(HAL_SPI_TransmitReceive(&hspi1,&d_send,&d_read,1,0xFFFFFF)!=HAL_OK)
    d_read=Dummy_Byte;
  
  return d_read; 
}
 
void SPI_FLASH_WriteEnable(void)
{ 
  FLASH_SPI_CS_ENABLE(); 
  SPI_FLASH_SendByte(W25X_WriteEnable); 
  FLASH_SPI_CS_DISABLE();
}

 
void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0; 
  FLASH_SPI_CS_ENABLE(); 
  SPI_FLASH_SendByte(W25X_ReadStatusReg); 
  do
  { 
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	 
  }
  while ((FLASH_Status & WIP_Flag) == SET); /* Write in progress */ 
  FLASH_SPI_CS_DISABLE();
}

 
void SPI_Flash_PowerDown(void)   
{  
  FLASH_SPI_CS_ENABLE(); 
  SPI_FLASH_SendByte(W25X_PowerDown); 
  FLASH_SPI_CS_DISABLE();
}   

 
void SPI_Flash_WAKEUP(void)   
{ 
  FLASH_SPI_CS_ENABLE(); 
  SPI_FLASH_SendByte(W25X_ReleasePowerDown); 
  FLASH_SPI_CS_DISABLE(); 
}   
   
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/

