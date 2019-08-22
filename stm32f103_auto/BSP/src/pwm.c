#include "../BSP/inc/pwm.h"

/*private includes*/
#include "tim.h" 
/*private macro*/
#define PWM_TIM_BASE	&htim2
#define PWM_PERIOD		1000
/*private variable*/
 
/*public variable*/

/*private funtion decaleration*/

/*public funtion decaleration*/

/*funtion*/
  
void BSP_PWM_START(uint32_t pwm)
{
	HAL_TIM_PWM_Start(PWM_TIM_BASE, pwm);
}
void BSP_PWM_STOP(uint32_t pwm)
{
	HAL_TIM_PWM_Stop(PWM_TIM_BASE, pwm);
}
void BSP_PWM_SET_ARR(uint16_t arr)
{
	__HAL_TIM_SET_AUTORELOAD(PWM_TIM_BASE, arr);
}

void BSP_PWM_SET_RATE(uint32_t pwm, uint8_t rate)
{
	TIM_OC_InitTypeDef sConfigOC = {0};
	uint16_t arr;
	if(rate>100)return;
	arr = PWM_PERIOD/1.0*rate/100;
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = arr;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
	//HAL_TIM_PWM_Stop(PWM_TIM_BASE, pwm);
  HAL_TIM_PWM_ConfigChannel(PWM_TIM_BASE, &sConfigOC, pwm);
  HAL_TIM_PWM_Start(PWM_TIM_BASE, pwm);
}
void BSP_PWM_SET_PULSE(uint32_t pwm, uint32_t pulse)
{
	TIM_OC_InitTypeDef sConfigOC = {0}; 
	if(pulse > PWM_PERIOD)pulse = PWM_PERIOD;
	if(pulse < 120)pulse = 0;
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = pulse;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
	
	
  HAL_TIM_PWM_ConfigChannel(PWM_TIM_BASE, &sConfigOC, pwm);
  HAL_TIM_PWM_Start(PWM_TIM_BASE, pwm);
}