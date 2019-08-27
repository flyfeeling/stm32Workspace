#ifndef __BSP_NRF24LXX_H__
#define __BSP_NRF24LXX_H__
/*public includes*/
#include "stm32f4xx_hal.h"
/*public macro*/

//FLAG
//STATUS
#define	BSP_NRF24LXX_FLAG_RX_DR					0X40
#define BSP_NRF24LXX_FLAG_TX_DS					0X20
#define BSP_NRF24LXX_FLAG_MAX_RT				0X10
#define BSP_NRF24LXX_FLAG_TX_FULL				0X01
#define BSP_NRF24LXX_FLAG_RX_P_NO				0X0E	//Data pipe number for the payload available for reading from RX_FIFO
//FIFO STATUS
#define BSP_NRF24LXX_FLAG_FIFO_TX_REUSE	0X40
#define BSP_NRF24LXX_FLAG_FIFO_TX_FULL	0X20
#define BSP_NRF24LXX_FLAG_FIFO_TX_EMPTY	0X10
#define BSP_NRF24LXX_FLAG_FIFO_RX_FULL	0X02
#define BSP_NRF24LXX_FLAG_FIFO_RX_EMPTY	0X01


/*public variable extern*/

/*public  funtion decaleration*/
uint8_t BSP_NRF24LXX_GET_NUM(void);
void 		BSP_NRF24LXX_REGISTER(void); 

void 		BSP_NRF24LXX_READ_REG(uint8_t id, uint8_t reg, uint8_t* dat, uint32_t len);
void 		BSP_NRF24LXX_WRITE_REG(uint8_t id, uint8_t reg, uint8_t* dat, uint32_t len); 

uint8_t BSP_NRF24LXX_RX_PIPE_NUM(uint8_t id, uint8_t pipe);
uint8_t BSP_NRF24LXX_GET_STATUS(uint8_t id);
uint8_t BSP_NRF24LXX_GET_OBSERVE_TX(uint8_t id);
uint8_t BSP_NRF24LXX_RX_FIFO_WIDTH(uint8_t id);
uint8_t BSP_NRF24LXX_GET_FIFO_STATUS(uint8_t id);
uint8_t BSP_NRF24LXX_POWER_DETECTOR(uint8_t id);

void 		BSP_NRF24LXX_INIT(void);
void 		BSP_NRF24LXX_FLUSH(uint8_t id, uint8_t tr);
void 		BSP_NRF24LXX_TRANSMIT(uint8_t id, uint8_t *dat, uint8_t len);
void 		BSP_NRF24LXX_TRANSMIT_NACK(uint8_t id, uint8_t *dat, uint8_t len);
void 		BSP_NRF24LXX_RECEIVE(uint8_t id, uint8_t *dat, uint8_t len);


#endif