#include "bsp_button.h"

/*private includes*/

/*private macro*/

/*private variable*/

/*public variable*/

/*private funtion decaleration*/

/*public funtion decaleration*/

/*funtion*/

uint32_t keyValue = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{ 
	keyValue = 3;
}
uint8_t GET_KEY()
{
	uint8_t f = keyValue;
	keyValue = 0;
	return f;
}