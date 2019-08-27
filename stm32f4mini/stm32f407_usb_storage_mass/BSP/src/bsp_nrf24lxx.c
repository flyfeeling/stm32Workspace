#include "bsp_nrf24lxx.h"
#include "bsp_nrf24lxx_def.h"

/*private includes*/
#include "spi.h"
/*private macro*/
typedef struct{
	uint8_t id;
	SPI_HandleTypeDef *handle;
	GPIO_TypeDef *CS_Port;
	uint16_t CS_Pin;
	GPIO_TypeDef *CE_Port;
	uint16_t CE_Pin;
	
	uint8_t (*init_Seq)[2];
	uint16_t init_Seq_Len;
	
	uint8_t *tx_Addr;
	uint16_t tx_Addr_Len;
	
	uint8_t *rx_Addr0;
	uint16_t rx_Addr0_Len;
	
	uint8_t *rx_Addr1;
	uint16_t rx_Addr1_Len;
	
	uint8_t *rx_AddrEx;
	uint16_t rx_AddrEx_Len;	
} bsp_nrf24lxx_typedef;

#define BSP_NRF24LXX_HANDLE(id) (BSP_NRF24LXX_TABLE[id].handle)
#define BSP_NRF24LXX_CS(id,cs)	HAL_GPIO_WritePin(BSP_NRF24LXX_TABLE[id].CS_Port, BSP_NRF24LXX_TABLE[id].CS_Pin, cs)
#define BSP_NRF24LXX_CE(id,ce)	HAL_GPIO_WritePin(BSP_NRF24LXX_TABLE[id].CE_Port, BSP_NRF24LXX_TABLE[id].CE_Pin, ce)

/*private variable*/

uint8_t Init_Seq0[][2]={  
												{SETUP_AW,0x03}, 		//SET 5 byte address width
												{EN_AA,0x01},				//ENABLE rx pipe 0 auto ack
												{EN_RXADDR, 0x01},	//ENABLE rx pipe 0
												{SETUP_RETR,0xFF}, 	//DISABLE auto retransmission
												{RF_CH,40},				//SET RF ch in 2.4Ghz + 0Mhz
												{RF_SETUP,0x0F}, 		//SET RF data rate 2Mbps 
												//{FEATURE,0x03},			//ENABLE nack transmit 
												{CONFIG,0x0E}, 			//ENABLE tx mode , crc 2 byte , power up 
												}; 

uint8_t Init_Seq1[][2]={
												{SETUP_AW,0x03}, 		//SET 5 byte address width 
												{EN_AA,0x01},				//ENABLE rx pipe 0 auto ack
												{EN_RXADDR, 0x01},	//ENABLE rx pipe 0
												{RF_CH,40},				//SET RF ch in 2.4Ghz + 0Mhz 
												{RX_PW_P0,32}, 
												{RF_SETUP,0x0F}, 		//SET RF data rate 2Mbps  
//												{FEATURE,0x04},
//												{DYNPD,0X01},				//ENABLE dynamic data length
												{CONFIG,0x0F}, 			//ENABLE rx mode , crc 2 byte , power up
												}; 

uint8_t Tx_Addr[5] = {0,0,0,0,1};
uint8_t Rx_Addr0[5] = {0,0,0,0,1};
uint8_t Rx_Addr1[5] = {0,0,0,0,2};
uint8_t Rx_AddrEx[4] = {3,4,5,6};

bsp_nrf24lxx_typedef BSP_NRF24LXX_TABLE[] = {
	{
		0,
		&hspi1,
		NRF_CS_GPIO_Port,
		NRF_CS_Pin,
		NRF_CE_GPIO_Port,
		NRF_CE_Pin, 
		Init_Seq0,
		sizeof(Init_Seq0)/sizeof(Init_Seq0[0]),
		Tx_Addr,
		sizeof(Tx_Addr)/sizeof(Tx_Addr[0]),
		Tx_Addr,
		sizeof(Tx_Addr)/sizeof(Tx_Addr[0]),
		NULL,
		0,
		NULL,
		0,	 
	},
#if 1
	{
		0,
		&hspi2,
		SPI2_RST_GPIO_Port,
		SPI2_RST_Pin,
		SPI2_CS_GPIO_Port,
		SPI2_CS_Pin, 
		Init_Seq1,
		sizeof(Init_Seq1)/sizeof(Init_Seq1[0]),
		NULL,
		0,
		Rx_Addr0,
		sizeof(Rx_Addr0)/sizeof(Rx_Addr0[0]),
//		Rx_Addr1,
//		sizeof(Rx_Addr1)/sizeof(Rx_Addr1[0]),
//		Rx_AddrEx,
//		sizeof(Rx_AddrEx)/sizeof(Rx_AddrEx[0]), 
	},
#endif
};
/*public variable*/

/*private funtion decaleration*/
void BSP_NRF24LXX_SEND_BYTE(uint8_t id, uint8_t dat)
{ 
	HAL_SPI_Transmit(BSP_NRF24LXX_HANDLE(id), &dat, 1, 0xFFFFFFFF);
}
uint8_t BSP_NRF24LXX_READ_BYTE(uint8_t id)
{ 
	uint8_t dummy=NOP,dat; 
	HAL_SPI_TransmitReceive(BSP_NRF24LXX_HANDLE(id), &dummy, &dat, 1, 0xFFFFFFFF);
	return dat;
}

/*public funtion decaleration*/

/*funtion*/ 
uint8_t BSP_NRF24LXX_GET_NUM(void)
{
	return sizeof(BSP_NRF24LXX_TABLE)/sizeof(BSP_NRF24LXX_TABLE[0]);
}
void BSP_NRF24LXX_REGISTER(void)
{
	for(uint8_t n=0;n<BSP_NRF24LXX_GET_NUM();n++)
	{
		BSP_NRF24LXX_TABLE[n].id = n; 
		BSP_NRF24LXX_CE(n, 0);					//standby I
		BSP_NRF24LXX_CS(n, 1); 
		BSP_NRF24LXX_SEND_BYTE(n, 0xFF);  
	} 
}


void BSP_NRF24LXX_READ_REG(uint8_t id, uint8_t reg, uint8_t* dat, uint32_t len)
{
	BSP_NRF24LXX_CS(id, 0);
	BSP_NRF24LXX_SEND_BYTE(id, reg|R_REGISTER); 
	for(int n=0;n<len;n++)
	{
		dat[n] = BSP_NRF24LXX_READ_BYTE(id);
	}
	BSP_NRF24LXX_CS(id, 1); 
}
void BSP_NRF24LXX_WRITE_REG(uint8_t id, uint8_t reg, uint8_t* dat, uint32_t len)
{
	BSP_NRF24LXX_CS(id, 0);
	BSP_NRF24LXX_SEND_BYTE(id, reg|W_REGISTER); 
	for(int n=0;n<len;n++)
	{
		BSP_NRF24LXX_SEND_BYTE(id, dat[n]);
	}	
	BSP_NRF24LXX_CS(id, 1); 
}
 

void BSP_NRF24LXX_INIT(void)
{
	BSP_NRF24LXX_REGISTER();
	for(uint8_t n=0;n<BSP_NRF24LXX_GET_NUM();n++)
	{
		bsp_nrf24lxx_typedef *NRF = &BSP_NRF24LXX_TABLE[n]; 
		BSP_NRF24LXX_CE(n, 0); 	// CE = 0
		for(uint8_t m=0;m<NRF->init_Seq_Len;m++)
		{ 
			 BSP_NRF24LXX_WRITE_REG(n, NRF->init_Seq[m][0], &NRF->init_Seq[m][1], 1); 
		}   
		 
		
		if(NRF->rx_Addr0!=NULL){ 
			BSP_NRF24LXX_WRITE_REG(n, RX_ADDR_P0, NRF->rx_Addr0, NRF->rx_Addr0_Len); 
		}	
		if(NRF->rx_Addr1!=NULL){ 
			BSP_NRF24LXX_WRITE_REG(n, RX_ADDR_P1, NRF->rx_Addr1, NRF->rx_Addr1_Len); 
			for(uint8_t pipe=1; pipe<NRF->rx_AddrEx_Len; pipe++){
				BSP_NRF24LXX_WRITE_REG(n, RX_ADDR_P1+pipe, &NRF->rx_AddrEx[pipe], 1);
			}
		}	
		if(NRF->tx_Addr!=NULL){
			BSP_NRF24LXX_WRITE_REG(n, TX_ADDR, NRF->tx_Addr, NRF->tx_Addr_Len);
		}else{
			BSP_NRF24LXX_CE(n, 1); 	// CE = 1 
		}		
		
	} 

}
uint8_t BSP_NRF24LXX_RX_PIPE_NUM(uint8_t id, uint8_t pipe)
{
	uint8_t dat;
	BSP_NRF24LXX_READ_REG(id, RX_PW_P0+pipe, &dat, 1);
	return dat;
}

uint8_t BSP_NRF24LXX_GET_STATUS(uint8_t id)
{
	uint8_t dat;
	BSP_NRF24LXX_READ_REG(id, STATUS, &dat, 1);
	BSP_NRF24LXX_WRITE_REG(id, STATUS, &dat, 1);//clear
	return dat;
}

uint8_t BSP_NRF24LXX_GET_OBSERVE_TX(uint8_t id)
{
	uint8_t dat;
	BSP_NRF24LXX_READ_REG(id, OBSERVE_TX, &dat, 1);
	return dat;
}

uint8_t BSP_NRF24LXX_POWER_DETECTOR(uint8_t id)
{
	uint8_t dat;
	BSP_NRF24LXX_READ_REG(id, RPD, &dat, 1);
	return dat;
}

void BSP_NRF24LXX_TRANSMIT(uint8_t id, uint8_t *dat, uint8_t len)
{
	BSP_NRF24LXX_CE(id, 0);
	
	BSP_NRF24LXX_CS(id, 0);
	BSP_NRF24LXX_SEND_BYTE(id, W_TX_PAYLOAD); 
	for(int n=0;n<len;n++)
	{
		BSP_NRF24LXX_SEND_BYTE(id, dat[n]); 
	}
	BSP_NRF24LXX_CS(id, 1); 	
	 

	HAL_Delay(10);		//minimum 10us
	BSP_NRF24LXX_CE(id, 1);
	
}

void BSP_NRF24LXX_TRANSMIT_NACK(uint8_t id, uint8_t *dat, uint8_t len)
{
	BSP_NRF24LXX_CE(id, 0);
	
	BSP_NRF24LXX_CS(id, 0);
	BSP_NRF24LXX_SEND_BYTE(id, W_TX_PAYLOAD_NO_ACK); 
	for(int n=0;n<len;n++)
	{
		BSP_NRF24LXX_SEND_BYTE(id, dat[n]); 
	}
	BSP_NRF24LXX_CS(id, 1); 
	
	 
	HAL_Delay(10);		//minimum 10us
	BSP_NRF24LXX_CE(id, 1);
	
	
}

void BSP_NRF24LXX_RECEIVE(uint8_t id, uint8_t *dat, uint8_t len)
{
	//BSP_NRF24LXX_CE(id, 1);
	BSP_NRF24LXX_CS(id, 0);
	BSP_NRF24LXX_SEND_BYTE(id, R_RX_PAYLOAD); 
	for(int n=0;n<len;n++)
	{
		dat[n] = BSP_NRF24LXX_READ_BYTE(id);
	}
	BSP_NRF24LXX_CS(id, 1); 	
}

void BSP_NRF24LXX_FLUSH(uint8_t id, uint8_t tr)
{
	BSP_NRF24LXX_CS(id, 0);
	if(tr == 0)	{ 
		BSP_NRF24LXX_SEND_BYTE(id, FLUSH_TX);  
	}	else {
		BSP_NRF24LXX_SEND_BYTE(id, FLUSH_RX); 
	}
	BSP_NRF24LXX_CS(id, 1); 
}
uint8_t BSP_NRF24LXX_RX_FIFO_WIDTH(uint8_t id)
{
	uint8_t dat;
	BSP_NRF24LXX_CS(id, 0);
	BSP_NRF24LXX_SEND_BYTE(id, R_RX_PL_WID); 
	dat = BSP_NRF24LXX_READ_BYTE(id); 
	BSP_NRF24LXX_CS(id, 1);
	return dat;
}
uint8_t BSP_NRF24LXX_GET_FIFO_STATUS(uint8_t id)
{
	uint8_t dat;
	BSP_NRF24LXX_READ_REG(id, FIFO_STATUS, &dat, 1); 
	return dat;
}

#if 0 
void BSP_NRF24LXX_CALLBACK(void)
{
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
}
#endif
