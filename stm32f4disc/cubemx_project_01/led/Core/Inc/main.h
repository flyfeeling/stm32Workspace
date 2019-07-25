/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define LED3_Pin GPIO_PIN_5
#define LED3_GPIO_Port GPIOD
#define LED4_Pin GPIO_PIN_3
#define LED4_GPIO_Port GPIOK
#define LED2_Pin GPIO_PIN_4
#define LED2_GPIO_Port GPIOD
#define LED1_Pin GPIO_PIN_6
#define LED1_GPIO_Port GPIOG


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
