#ifndef __BSP_PWM_H__
#define __BSP_PWM_H__
/*public includes*/
#include "stm32f1xx_hal.h"
/*public macro*/
#define PWM1	TIM_CHANNEL_1
#define PWM2	TIM_CHANNEL_2
#define PWM3	TIM_CHANNEL_3
#define PWM4	TIM_CHANNEL_4

#define PWM_PERIOD		10000
/*public variable extern*/

/*public  funtion decaleration*/
void BSP_PWM_START(uint32_t pwm);
void BSP_PWM_STOP(uint32_t pwm); 
void BSP_PWM_SET_RATE(uint32_t pwm, uint8_t rate);
void BSP_PWM_SET_PULSE(uint32_t pwm, uint32_t pulse);
#endif