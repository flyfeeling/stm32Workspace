#include "../BSP/inc/communicate.h"

/*private includes*/
#include "../BSP/inc/pwm.h" 
#include "../BSP/inc/motion.h"
#include "usart.h"
#include "stdio.h"
/*private macro*/
#define COM1 					&huart1
#define COM1_BUF_SIZE	1024
/*private variable*/
uint8_t com1Value = 0;
uint8_t com1State = 0;
uint8_t com1Buffer[COM1_BUF_SIZE] = {0};
uint32_t com1BufPtr = 0;
/*public variable*/

/*private funtion decaleration*/
FILE __stdin;      
int fgetc(FILE *f){    
	uint8_t ch = com1Buffer[com1BufPtr++];
	return ch;
}
/*public funtion decaleration*/

/*funtion*/
void BSP_COMMUNICATE_RECEIVED(void)
{
	double pwm1=0,pwm2=0;
	scanf("(%lf%lf)\r\n",&pwm1,&pwm2);
	printf("COM1 Received: (%lf %lf)\r\n", pwm1,pwm2);
	BSP_MOTION_SET_SPEED(LEFT_WHEEL, pwm1);
	BSP_MOTION_SET_SPEED(RIGHT_WHEEL, pwm2);
}
void BSP_COMMUNICATE_INIT(void)
{
	HAL_UART_Receive_IT(COM1, &com1Value, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == COM1)
	{  
		switch(com1State)
		{
			case 2:
				if(com1Value=='\r' || com1Value=='\n')break;
				com1State = 0;
				com1BufPtr = 0;
			case 0:
				switch(com1Value)
				{
					case '\n': 
						com1State = 0;
						com1BufPtr = 0;
						break;
					case '\r':	
						com1State = 1;
					default:
						com1Buffer[com1BufPtr] = (com1Value!='\r'?com1Value:'\0');
						com1BufPtr++;
						if(com1BufPtr == COM1_BUF_SIZE-1){
							com1Buffer[com1BufPtr] = '\0';
							com1State = 2;
							com1BufPtr = 0;
							BSP_COMMUNICATE_RECEIVED();
						}
						break;
				}
				break;
			case 1:
				switch(com1Value)
				{
					case '\n': 
						com1State = 2;  
						com1BufPtr = 0;
						BSP_COMMUNICATE_RECEIVED();
						break; 
					default:
						com1State = 0;
						com1BufPtr = 0;
						break;
				}
				break;  
			default:
				break;
		}
		HAL_UART_Receive_IT(COM1, &com1Value, 1);
	}
}