/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dcmi.h"
#include "fatfs.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_w25qxx.h"
#include "bsp_sram.h"
#include "bsp_led.h"
#include "bsp_lcd.h"
#include "bsp_ov7670.h"
#include "bsp_at24cxx.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USB_DEVICE_Init();
  MX_SPI1_Init();
  MX_FATFS_Init();
  MX_FSMC_Init();
  MX_DCMI_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	printf("\r\n*****BASIC INFORMATION*****\r\n");
	printf("Hal Version:%X\r\n", HAL_GetHalVersion());
	printf("HCLK Freq: %.2f MHz\r\n", HAL_RCC_GetHCLKFreq()/1.0/1000/1000);
	printf("PCLK1 Freq: %.2f MHz\r\n", HAL_RCC_GetPCLK1Freq()/1.0/1000/1000);
	printf("PCLK2 Freq: %.2f MHz\r\n", HAL_RCC_GetPCLK2Freq()/1.0/1000/1000);
	printf("SysTick Freq: %d Hz\r\n", 1000/(int)HAL_GetTickFreq());
	printf("Device ID:%X\r\n", HAL_GetDEVID());
	printf("Device Revision ID:%X\r\n", HAL_GetREVID());
	printf("Device UID : %X%X%X\r\n", HAL_GetUIDw0(),HAL_GetUIDw1(),HAL_GetUIDw2());
	
	printf("\r\n*****BSP INIT*****\r\n");
	BSP_W25QXX_INIT(0);
	printf("Flash Type:%s \r\n", bsp_w25qxx.name);
	printf("Flash Unique ID:%llX \r\n", bsp_w25qxx.unique_id);
	printf("Flash Capicity:%.2f KB\r\n", bsp_w25qxx.capicity/1024.0); 
	
	BSP_LCD_INIT();
	printf("LCD Type:%s\r\n", bsp_lcd.name);
	printf("LCD ID:%X\r\n", bsp_lcd.id);
	printf("LCD Resolution: %dX%d\r\n", bsp_lcd.height, bsp_lcd.width);
	
	uint16_t id = 0;
	id = BSP_OV_INIT();
	printf("Cemera ID:%X\r\n", id);
	
	uint8_t str[] = "at24c02";
	BSP_AT24CXX_WRITE(0,str,sizeof(str));
	printf("EEPROM Write: %s\r\n",str);
	BSP_AT24CXX_READ(0,str,sizeof(str));
	printf("EEPROM Read: %s\r\n",str);
	
	extern uint8_t uart1Value;
	HAL_UART_Receive_IT(&huart1, &uart1Value, 1);
	
	retUSER = f_mount(&USERFatFS, (const TCHAR*)USERPath, 1) ;
	if(retUSER == FR_OK){
		printf("f_mount() disk 0:/ sucessful!!\r\n");
//		uint8_t mkfsBuffer[8192] = {0};
//		retUSER = f_mkfs(USERPath, FM_FAT, 4096, mkfsBuffer, 8192);
//		if(retUSER == FR_OK){
			retUSER = f_open(&USERFile, "0:/test.txt", FA_OPEN_EXISTING | FA_READ | FA_WRITE);
			if(retUSER == FR_OK){
				TCHAR context[512] = {0};
				UINT bw = 0;
				printf("f_open() test.txt sucessful!!\r\n");
				//retUSER = f_write(&USERFile, (const TCHAR*)context, sizeof(context), &bw);
				retUSER = f_read(&USERFile, (TCHAR*)context, sizeof(context), &bw);
				printf("%s\r\n", context);
				retUSER = f_close(&USERFile);
			}else{
				printf("f_open() test.txt faild!!\r\n");
			}

//		}
	} 


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		BSP_LED_TOGGLE(1);
		HAL_Delay(100);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_5);
}

/* USER CODE BEGIN 4 */
 
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
