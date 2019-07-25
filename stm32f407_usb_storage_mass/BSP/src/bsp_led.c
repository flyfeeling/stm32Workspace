#include "bsp_led.h"
#include "main.h"



void BSP_LED_ON(uint8_t led)
{
	switch(led)
	{
		case 0:HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);break;
		case 1:HAL_GPIO_WritePin(LED0_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);break;
		default:break;
	}
}
void BSP_LED_OFF(uint8_t led)
{
	switch(led)
	{
		case 0:HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);break;
		case 1:HAL_GPIO_WritePin(LED0_GPIO_Port, LED1_Pin, GPIO_PIN_SET);break;
		default:break;
	}
}
void BSP_LED_TOGGLE(uint8_t led)
{
	switch(led)
	{
		case 0:HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);break;
		case 1:HAL_GPIO_TogglePin(LED0_GPIO_Port, LED1_Pin);break;
		default:break;
	}
}

