#include "bsp_led.h"
#include "main.h"



void BSP_LED_ON(uint8_t led)
{
	switch(led)
	{
		case 0:HAL_GPIO_WritePin(BSP_LED0_GPIO_Port, BSP_LED0_Pin, GPIO_PIN_RESET);break; 
		default:break;
	}
}
void BSP_LED_OFF(uint8_t led)
{
	switch(led)
	{
		case 0:HAL_GPIO_WritePin(BSP_LED0_GPIO_Port, BSP_LED0_Pin, GPIO_PIN_SET);break; 
		default:break;
	}
}
void BSP_LED_TOGGLE(uint8_t led)
{
	switch(led)
	{
		case 0:HAL_GPIO_TogglePin(BSP_LED0_GPIO_Port, BSP_LED0_Pin);break; 
		default:break;
	}
}

