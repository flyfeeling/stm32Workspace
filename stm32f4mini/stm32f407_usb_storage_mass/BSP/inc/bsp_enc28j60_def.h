#ifndef __BSP_ENC28J60_DEF_H__
#define __BSP_ENC28J60_DEF_H__
/*public includes*/ 
/*public macro*/
#define RXSTART_INIT     0x0 
#define RXSTOP_INIT      (0x1FFF-0x0600-1) 
#define TXSTART_INIT     (0x1FFF-0x0600) 
#define TXSTOP_INIT      0x1FFF
#define MAX_FRAMELEN		 1500

//register cmd
#define RCR	0X00
#define WCR	0X40
#define RBM	0x3A
#define WBM 0x7A
#define BFS	0x80
#define BFC 0xA0
#define SC	0xFF

#define ENC28J60_ADDRESS_MASK	0X1F
#define ENC28J60_BANK0				0x00
#define ENC28J60_BANK1				0x20
#define ENC28J60_BANK2				0x40
#define ENC28J60_BANK3				0x60

#define ENC28J60_BANK_MASK		0X60
#define ENC28J60_MAC_MASK			0X80

#define ENC28J_REG(X)					((X)&ENC28J60_ADDRESS_MASK)
#define ENC28J_BANK(X)				(((X)&ENC28J60_BANK_MASK)>>5)
#define ENC28J_IS_ETH(X)			(!((X)&ENC28J60_MAC_MASK))

//control register
//cr			eth / mac / mii
#define EIE				(0X1B|0x60|0X80)// 0000_0000b
#define EIR				(0X1C|0x60|0X80)	// x000_0000b
#define ESTAT			(0X1D|0x60|0X80) // 0000_x000b
#define ECON2			(0X1E|0x60|0X80) // 100x_0xxxb
#define ECON1			(0X1F|0x60|0X80)	// 0000_0000b

#define ERDPTL		(0X00|0X00)
#define ERDPTH		(0X01|0X00)
#define EWRPTL		(0X02|0X00)
#define EWRPTH		(0X03|0X00)
#define ETXSTL		(0X04|0X00)
#define ETXSTH		(0X05|0X00)
#define ETXNDL		(0X06|0X00)
#define ETXNDH		(0X07|0X00)
#define ERXSTL		(0X08|0X00)
#define ERXSTH		(0X09|0X00)
#define ERXNDL		(0X0A|0X00)
#define ERXNDH		(0X0B|0X00)
#define ERXRDPTL	(0X0C|0X00)
#define ERXRDPTH	(0X0D|0X00)
#define ERXWRPTL	(0X0E|0X00)
#define ERXWRPTH	(0X0F|0X00)
#define EDMASTL		(0X10|0X00)
#define EDMASTH		(0X11|0X00)
#define EDMANDL		(0X12|0X00)
#define EDMANDH		(0X13|0X00)
#define EDMADSTL	(0X14|0X00)
#define EDMADSTH	(0X15|0X00)
#define EDMACSL		(0X16|0X00)
#define EDMACSH		(0X17|0X00)

#define EHT0			(0X00|0X20)
#define EHT1			(0X01|0X20)
#define EHT2			(0X02|0X20)
#define EHT3			(0X03|0X20)
#define EHT4			(0X04|0X20)
#define EHT5			(0X05|0X20)
#define EHT6			(0X06|0X20)
#define EHT7			(0X07|0X20)
#define EPMM0			(0X08|0X20)
#define EPMM1			(0X09|0X20)
#define EPMM2			(0X0A|0X20)
#define EPMM3			(0X0B|0X20)
#define EPMM4			(0X0C|0X20)
#define EPMM5			(0X0D|0X20)
#define EPMM6			(0X0E|0X20)
#define EPMM7			(0X0F|0X20)
#define EPMCSL		(0X10|0X20)
#define EPMCSH		(0X11|0X20) 
#define EPMOL			(0X14|0X20)
#define EPMOH			(0X15|0X20)
#define EWOLIE		(0X16|0X20)
#define EWOLIR		(0X17|0X20)
#define ERXFCON		(0X18|0X20)
#define ERKTCNT		(0X19|0X20)

#define MACON1		(0X00|0X40|0X80)
#define MACON2		(0X01|0X40|0X80)
#define MACON3		(0X02|0X40|0X80)
#define MACON4		(0X03|0X40|0X80)
#define MABBIPG		(0X04|0X40|0X80) 
#define MAIPGL		(0X06|0X40|0X80)
#define MAIPGH		(0X07|0X40|0X80)
#define MACLCON1	(0X08|0X40|0X80)
#define MACLCON2	(0X09|0X40|0X80)
#define MAMXFLL		(0X0A|0X40|0X80)
#define MAMXFLH		(0X0B|0X40|0X80) 
#define MAPHSUP		(0X0D|0X40|0X80) 
#define MICON			(0X11|0X40|0X80) 
#define MICMD			(0X12|0X40|0X80)
#define MIREGADR	(0X14|0X40|0X80) 
#define MIWRL			(0X16|0X40|0X80)
#define MIWRH			(0X17|0X40|0X80)
#define MIRDL			(0X18|0X40|0X80)
#define MIRDH			(0X19|0X40|0X80)

#define MAADR1		(0X00|0x60|0X80)
#define MAADR0		(0X01|0x60|0X80)
#define MAADR3		(0X02|0x60|0X80)
#define MAADR2		(0X03|0x60|0X80)
#define MAADR5		(0X04|0x60|0X80) 
#define MAADR4		(0X05|0x60|0X80)
#define EBSTSD		(0X06|0x60)
#define EBSTCON		(0X07|0x60)
#define EBSTCSL		(0X08|0x60)
#define EBSTCSH		(0X09|0x60)
#define MISTAT		(0X0A|0x60|0X80) 
#define EREVID		(0X12|0x60)
#define ECOCON		(0X15|0x60)  
#define EFLOCON		(0X17|0x60)
#define EPAUSL		(0X18|0x60)
#define EPAUSH		(0X19|0x60)
/*public variable extern*/

/*public  funtion decaleration*/

#endif