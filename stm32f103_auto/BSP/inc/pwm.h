#ifndef __BSP_PWM_H__
#define __BSP_PWM_H__
/*public includes*/
#include "stm32f1xx_hal.h"
/*public macro*/
#define PWM1	TIM_CHANNEL_1
#define PWM2	TIM_CHANNEL_4


/*public variable extern*/

/*public  funtion decaleration*/
void BSP_PWM_START(uint32_t pwm);
void BSP_PWM_STOP(uint32_t pwm);
void BSP_PWM_SET_ARR(uint16_t arr);
#endif