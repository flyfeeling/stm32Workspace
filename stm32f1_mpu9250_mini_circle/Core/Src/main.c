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
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_w25qxx.h"
#include "bsp_mpu9250.h"
#include "bsp_delay.h" 

#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FIRMWARE_VERSION 0x190731
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

void BSP_MPU_INT_CALLBACK();
void BSP_KEY0_CALLBACK()
{
	HAL_GPIO_TogglePin(BSP_LED0_GPIO_Port, BSP_LED0_Pin); 
	BSP_DELAY(0,20,0);
} 
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{   
	switch(GPIO_Pin)
	{
		case BSP_KEY0_Pin			:	BSP_KEY0_CALLBACK();break;
//		case BSP_MPU_INT_Pin	:	BSP_MPU_INT_CALLBACK();break;
		default:break;
	} 
}

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
  MX_SPI1_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init(); 
  /* USER CODE BEGIN 2 */
	printf("Version:%X\r\n",FIRMWARE_VERSION);
	BSP_W25QXX_INIT(0);
	printf("FLASH:%s\r\n",bsp_w25qxx.name);
	extern uint8_t uart1Value;
	HAL_UART_Receive_IT(&huart1, &uart1Value, 1); 
	HAL_Delay(10);
	BSP_MPU_INIT();  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//BSP_MPU_TEST();
		BSP_DELAY(1,0,0);
//		double temp = 0;
//		MPU_GET_TEMPERATURE(&temp);
//		printf("temp=%.2f\r\n", temp);
//		BSP_DELAY(1,0,0);
//		uint32_t tickStart = HAL_GetTick();
		HAL_GPIO_TogglePin(BSP_LED0_GPIO_Port, BSP_LED0_Pin); 
//		BSP_DELAY(0,100,0);
//		printf("tick:%d\r\n",HAL_GetTick() - tickStart);
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void BSP_MPU_INT_CALLBACK()
{
#if 0
	static uint16_t time =0;
	static uint16_t tickStart = 0, tickEnd = 0, tickDiff;
	//uint8_t status = BSP_MPU_INT_STATUS();
	//	printf("mpu int\r\n");
	time++;
	if(time == 100)
	{
		time=0;
		tickEnd = HAL_GetTick();
		if(tickEnd>=tickStart){
			tickDiff = tickEnd - tickStart;
		}else{
			tickDiff = 65536 - tickStart + tickEnd;
		}
		
		
		printf("Frequence :	%.2fHz	Tick :	%d\r\n", 100*1000/1.0/tickDiff, tickDiff);
		tickStart = tickEnd; 
		
	} 
#endif 
#if 0
	static uint8_t IntTime = 0;
	IntTime++;
	if(IntTime == 100)
	{
		IntTime = 0;
		BSP_MPU_TEST();
	}
#endif
#if 0	 
	uint8_t mag_status = BSP_MPU_MAG_STATUS(0); 
	static uint16_t mag_time=0;
	static uint16_t mag_tickStart = 0, mag_tickEnd = 0, mag_tickDiff;
	if(mag_status & 0x01){
		BSP_MPU_TEST();
		mag_time++;
		if(mag_time == 1000){
			mag_time=0;
			mag_tickEnd = HAL_GetTick();
			if(mag_tickEnd>=mag_tickStart){
				mag_tickDiff = mag_tickEnd - mag_tickStart;
			}else{
				mag_tickDiff = 65536 - mag_tickStart + mag_tickEnd;
			}
			//printf("Frequence :	%.2fHz	Tick :	%d\r\n", 1000*1000/1.0/mag_tickDiff, mag_tickDiff);
			mag_tickStart = mag_tickEnd;
		} 
	}
#endif
}
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
