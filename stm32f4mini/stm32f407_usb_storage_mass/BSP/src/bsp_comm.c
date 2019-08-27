#include "bsp_comm.h"

/*private includes*/
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
#include "stdio.h"
#include "bsp_led.h"
#if __ARMCLIB_VERSION == 5060037
#pragma import(__use_no_semihosting)
typedef struct __FILE { 
    int handle; 
}FILE; 
#else
__asm(".global __use_no_semihosting\n\t");
void _sys_exit(int x){ x = x; }
void _ttywrch(int x){	x = x; }
void _sys_command_string(int x){	x = x; }
#endif

FILE __stdout;          
int fputc(int ch, FILE *f){    
    while((USART1->SR&0X40)==0);
    USART1->DR = (uint8_t) ch;     
		BSP_LED_TOGGLE(0);
    return ch;
}
FILE __stdin;      
int fgetc(FILE *f){    
	uint8_t ch = com1Buffer[com1BufPtr++];
	return ch;
}

uint8_t uart1Value = 0;


/*public funtion decaleration*/

/*funtion*/
void BSP_COMMUNICATE_RECEIVED(void)
{
	double pwm1=0,pwm2=0;
	scanf("(%lf%lf)\r\n",&pwm1,&pwm2);
	printf("COM1 Received: (%lf %lf)\r\n", pwm1,pwm2);
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