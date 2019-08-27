#ifndef __BSP_NRF24LXX_DEF_H__
#define __BSP_NRF24LXX_DEF_H__
//cmd
#define R_REGISTER					0x00			//1-5 LSB first
#define W_REGISTER					0X20			//1-5 LSB first
#define R_RX_PAYLOAD				0X61			//1-32 LSB first
#define W_TX_PAYLOAD				0XA0			//1-32 LSB first Ó¦´ðÐ´
#define FLUSH_TX						0XE1			//0
#define	FLUSH_RX						0XE2			//0
#define REUSE_TX_PL 				0XE3			//0
#define R_RX_PL_WID 				0X60			//1
#define W_ACK_PAYLOAD 			0XA8			//1-32 LSB first
#define W_TX_PAYLOAD_NO_ACK	0XB0			//1-32 LSB first	ÎÞÓ¦´ðÐ´
#define NOP									0XFF			//0




#define CONFIG 			0X00
#define EN_AA 			0X01
#define EN_RXADDR		0X02
#define SETUP_AW 		0X03
#define SETUP_RETR 	0X04
#define RF_CH 			0X05
#define RF_SETUP 		0X06
#define STATUS			0X07
#define OBSERVE_TX	0X08
#define RPD					0X09
#define RX_ADDR_P0 	0X0A
#define RX_ADDR_P1 	0X0B
#define RX_ADDR_P2 	0X0C
#define RX_ADDR_P3 	0X0D
#define RX_ADDR_P4 	0X0E
#define RX_ADDR_P5 	0X0F
#define TX_ADDR			0X10
#define RX_PW_P0 		0X11
#define RX_PW_P1 		0X12
#define RX_PW_P2 		0X13
#define RX_PW_P3 		0X14
#define RX_PW_P4 		0X15
#define RX_PW_P5 		0X16
#define FIFO_STATUS 0X17
#define DYNPD				0X1C
#define FEATURE			0X1D

#endif

