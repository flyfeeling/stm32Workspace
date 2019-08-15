#include "../BSP/inc/encoder.h"

/*private includes*/
#include "tim.h"
#include "stdio.h"
/*private macro*/
#define ENCODER1	&htim3
#define ENCODER2	&htim4
#define ENCODER_TIM_BASE 		&htim2
#define ENCODER_ONE_CIRCLE 	1600				//pulse
#define ENCODER_TIM_UPDATE	10					//ms
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
double BSP_ENCODER_GET_FREQ(uint8_t enc)
{
	double freq = 0;
	if(enc == 1)
	{
		freq = (2*enc1_dir-1)*enc1_diff/1.0*(1000/ENCODER_TIM_UPDATE)/ENCODER_ONE_CIRCLE;
	}
	else if(enc == 2)
	{
		freq = (2*enc2_dir-1)*enc2_diff/1.0*(1000/ENCODER_TIM_UPDATE)/ENCODER_ONE_CIRCLE;
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
			enc1_diff = enc1_cnt0 - enc1_cnt1 + ENCODER_ONE_CIRCLE*(enc1_cnt0<enc1_cnt1);
		}else{ 
			enc1_diff = enc1_cnt1 - enc1_cnt0 + ENCODER_ONE_CIRCLE*(enc1_cnt1<enc1_cnt0);
		}
		
		if(enc2_dir){ 
			enc2_diff = enc2_cnt0 - enc2_cnt1 + ENCODER_ONE_CIRCLE*(enc2_cnt0<enc2_cnt1);
		}else{ 
			enc2_diff = enc2_cnt1 - enc2_cnt0 + ENCODER_ONE_CIRCLE*(enc2_cnt1<enc2_cnt0);
		}
		 
		//printf("%d  %d\r\n", enc2_diff,enc2_dir);
	}
}
