#include "../BSP/inc/encode.h"

/*private includes*/
#include "tim.h"
#include "stdio.h"
/*private macro*/ 
#define ENCODE_HANDE	&htim4
#define MOTOR_FACTOR	100
/*private variable*/
uint32_t ic1Value1=0,ic1Value2=0,ic1Diff=0,ic1Index=0;
double	ic1Freq=0;
uint32_t ic2Value1=0,ic2Value2=0,ic2Diff=0,ic2Index=0;
double	ic2Freq=0;

/*public variable*/

/*private funtion decaleration*/

/*public funtion decaleration*/
void BSP_ENCODE_START()
{
	HAL_TIM_IC_Start_IT(ENCODE_HANDE, ENCODE1);
	HAL_TIM_IC_Start_IT(ENCODE_HANDE, ENCODE2);
} 
void BSP_ENCODE_STOP()
{
	HAL_TIM_IC_Stop_IT(ENCODE_HANDE, ENCODE1); 
	HAL_TIM_IC_Stop_IT(ENCODE_HANDE, ENCODE2); 
} 
/*funtion*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim == ENCODE_HANDE)
	{	
		switch((uint8_t)htim->Channel)
		{
			case HAL_TIM_ACTIVE_CHANNEL_2:{
					if(ic2Index == 0){ 
							ic2Value1 = HAL_TIM_ReadCapturedValue(htim, ENCODE2);
							ic2Index = 1;
					}
					else if(ic2Index == 1){ 
							ic2Value2 = HAL_TIM_ReadCapturedValue(htim, ENCODE2);  
							if (ic2Value2 > ic2Value1) {
								ic2Diff = (ic2Value2 - ic2Value1); 
							}else if (ic2Value2 < ic2Value1){
								ic2Diff = ((10000 - ic2Value1) + ic2Value2) + 1;
							}else{
								Error_Handler();
							}      
							ic2Freq = 1000000/1.0/ic2Diff/MOTOR_FACTOR;
							ic2Index = 0;
							printf("IC2:%.3f\r\n", ic2Freq);
					}				
					break;
			}
			case HAL_TIM_ACTIVE_CHANNEL_1:{ 
					if(ic1Index == 0){ 
							ic1Value1 = HAL_TIM_ReadCapturedValue(htim, ENCODE1);
							ic1Index = 1;
					}
					else if(ic1Index == 1){ 
							ic1Value2 = HAL_TIM_ReadCapturedValue(htim, ENCODE1);  
							if (ic1Value2 > ic1Value1) {
								ic1Diff = (ic1Value2 - ic1Value1); 
							}else if (ic1Value2 < ic1Value1){
								ic1Diff = ((10000 - ic1Value1) + ic1Value2) + 1;
							}else{
								Error_Handler();
							}      
							ic1Freq = 1000000/1.0/ic1Diff/MOTOR_FACTOR;
							ic1Index = 0;
							printf("IC1:%.3f\r\n", ic1Freq);
					}				
					break;
			}
			default:break; 
		} 
	}
	if(htim == &htim2)
	{	
		//printf("IC1:%d\r\n", __HAL_TIM_GET_COUNTER(htim));
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	printf("HAL_TIM_PeriodElapsedCallback\r\n");
}