#include "../BSP/inc/encoder.h"

/*private includes*/
#include "../BSP/inc/motion.h"
#include "tim.h"
#include "stdio.h"
/*private macro*/
#define ENCODER1					&htim3
#define ENCODER2					&htim4
#define ENCODER_TIM_BASE	&htim1
#define ENCODER_PERIOD		1600				//pulse
#define ENCODER_TIME			10					//ms
/*private variable*/
int32_t enc1_cnt0=0,enc1_cnt1=0,enc1_diff;
int32_t enc2_cnt0=0,enc2_cnt1=0,enc2_diff;
uint8_t  enc1_dir=0,enc2_dir=0;
double  enc1_freq,enc2_freq;
/*public variable*/

/*private funtion decaleration*/

/*public funtion decaleration*/

/*funtion*/
void BSP_ENCODER_START(void)
{
#ifdef ENCODER1
	HAL_TIM_Encoder_Start_IT(ENCODER1, TIM_CHANNEL_ALL);
#endif
#ifdef ENCODER2
	HAL_TIM_Encoder_Start_IT(ENCODER2, TIM_CHANNEL_ALL);
#endif
	HAL_TIM_Base_Start_IT(ENCODER_TIM_BASE);
}


void BSP_ENCODER_STOP(void)
{
#ifdef ENCODER1
	HAL_TIM_Encoder_Stop_IT(ENCODER1, TIM_CHANNEL_ALL);
#endif
#ifdef ENCODER2
	HAL_TIM_Encoder_Stop_IT(ENCODER2, TIM_CHANNEL_ALL);
#endif
	HAL_TIM_Base_Stop_IT(ENCODER_TIM_BASE);
}


double BSP_ENCODER_GET_FREQ(ENCODER_ENUM enc)
{
	double freq = 0;
	if(enc == RIGHT_ENCODER)
	{
		freq = (2*enc1_dir-1)*enc1_diff/1.0*(1000/ENCODER_TIME)/ENCODER_PERIOD;
	}
	else if(enc == LEFT_ENCODER)
	{
		freq = -(2*enc2_dir-1)*enc2_diff/1.0*(1000/ENCODER_TIME)/ENCODER_PERIOD;
	}
	else
	{
		Error_Handler();
	}
	return freq;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == ENCODER_TIM_BASE)
	{ 
		enc1_dir = (__HAL_TIM_IS_TIM_COUNTING_DOWN(ENCODER1) > 0);
		enc2_dir = (__HAL_TIM_IS_TIM_COUNTING_DOWN(ENCODER2) > 0);
		enc1_cnt0 = enc1_cnt1;
		enc2_cnt0 = enc2_cnt1;
		enc1_cnt1 = __HAL_TIM_GET_COUNTER(ENCODER1);
		enc2_cnt1 = __HAL_TIM_GET_COUNTER(ENCODER2); 
		
		if(enc1_dir){ 
			enc1_diff = enc1_cnt0 - enc1_cnt1 + ENCODER_PERIOD*(enc1_cnt0<enc1_cnt1);
		}else{ 
			enc1_diff = enc1_cnt1 - enc1_cnt0 + ENCODER_PERIOD*(enc1_cnt1<enc1_cnt0);
		}
		
		if(enc2_dir){ 
			enc2_diff = enc2_cnt0 - enc2_cnt1 + ENCODER_PERIOD*(enc2_cnt0<enc2_cnt1);
		}else{ 
			enc2_diff = enc2_cnt1 - enc2_cnt0 + ENCODER_PERIOD*(enc2_cnt1<enc2_cnt0);
		}
		BSP_MOTION_UPDATE();
	}
}
