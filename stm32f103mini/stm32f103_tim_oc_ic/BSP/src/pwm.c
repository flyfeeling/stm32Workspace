#include "../BSP/inc/pwm.h"

/*private includes*/
#include "tim.h"
#include "stdio.h"
/*private macro*/
#define PWM_HANDLE	&htim1
/*private variable*/
 
/*public variable*/

/*private funtion decaleration*/

/*public funtion decaleration*/

/*funtion*/
  
void BSP_PWM_START(uint32_t pwm)
{
	HAL_TIM_PWM_Start(PWM_HANDLE, pwm);
}
void BSP_PWM_STOP(uint32_t pwm)
{
	HAL_TIM_PWM_Stop(PWM_HANDLE, pwm);
}
void BSP_PWM_SET_ARR(uint16_t arr)
{
	__HAL_TIM_SET_AUTORELOAD(PWM_HANDLE, arr);
}


