#include "bsp_button.h"

/*private includes*/
#include "main.h"
#include "stdio.h"
#include "bsp_nrf24lxx.h"
/*private macro*/

/*private variable*/

/*public variable*/

/*private funtion decaleration*/

/*public funtion decaleration*/

/*funtion*/

uint32_t keyValue = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{ 
	if(GPIO_Pin == GPIO_PIN_0)
	{
		keyValue = 3;
	} 
	#if 0
	if(GPIO_Pin == NRF_INT_Pin)
	{
		uint8_t status = BSP_NRF24LXX_GET_STATUS(0);
		if(status & BSP_NRF24LXX_FLAG_TX_DS){
			printf("NRF 1 TX Finished!\r\n");
		}
		if(status & BSP_NRF24LXX_FLAG_TX_FULL){
			BSP_NRF24LXX_FLUSH(0, 0);
			printf("NRF 1 TX FULL!\r\n");
		}
		if(status & BSP_NRF24LXX_FLAG_MAX_RT){ 
			printf("NRF 1 TX MAX RT!\r\n"); 
		} 
		
		uint8_t fifo_status = BSP_NRF24LXX_GET_FIFO_STATUS(0);
	} 
	if(GPIO_Pin == NRF2_INT_Pin)
	{
		uint8_t status = BSP_NRF24LXX_GET_STATUS(1); 
		if(status & BSP_NRF24LXX_FLAG_RX_DR){ 
			uint8_t num = BSP_NRF24LXX_RX_FIFO_WIDTH(1);
			uint8_t buf[33];
			buf[num]=0;
			BSP_NRF24LXX_RECEIVE(1, buf, num);
			printf("NRF 2 Received %d byte in pipe %d : %s\r\n",num,(status >> 1)&0x7,buf);  
			//BSP_NRF24LXX_FLUSH(1, 1);
		}
	 
		uint8_t fifo_status = BSP_NRF24LXX_GET_FIFO_STATUS(1);
	}
	#endif
}
uint8_t GET_KEY()
{
	uint8_t f = keyValue;
	keyValue = 0;
	return f;
}