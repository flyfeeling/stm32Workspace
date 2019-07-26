#include "bsp_delay.h"

/*private includes*/
#include "tim.h"
/*private macro*/

/*private variable*/
#define BSP_DELAY_TIM_BASE htim14
/*public variable*/

/*private funtion decaleration*/

/*public funtion decaleration*/

/*funtion*/
void BSP_DELAY_US(uint16_t us)
{ 
	__HAL_TIM_CLEAR_FLAG(&BSP_DELAY_TIM_BASE, TIM_FLAG_UPDATE); 
	__HAL_TIM_SetCounter(&BSP_DELAY_TIM_BASE, (uint16_t)(1000-us));  
	HAL_TIM_Base_Start(&BSP_DELAY_TIM_BASE);
	while(!__HAL_TIM_GET_FLAG(&BSP_DELAY_TIM_BASE, TIM_FLAG_UPDATE));
	HAL_TIM_Base_Stop(&BSP_DELAY_TIM_BASE);
	__HAL_TIM_CLEAR_FLAG(&BSP_DELAY_TIM_BASE, TIM_FLAG_UPDATE); 
}
void BSP_DELAY_MS(uint32_t ms)
{
	for(; ms>0; ms--)
	{
		BSP_DELAY_US(1000);
	}
}
void BSP_DELAY_S(uint32_t s)
{
	for(; s>0; s--)
	{
		BSP_DELAY_MS(1000);
	}
}
void BSP_DELAY(uint32_t s,uint32_t ms, uint32_t us)
{
	BSP_DELAY_MS(s*1000+ms);
	BSP_DELAY_US(us);
}