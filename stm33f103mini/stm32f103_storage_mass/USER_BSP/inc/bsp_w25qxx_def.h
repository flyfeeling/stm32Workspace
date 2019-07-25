
#ifndef __BSP_W25QXX_DEF_H__
#define __BSP_W25QXX_DEF_H__
//instruction set
#define W25Q_WRITE_EN								0X06
#define W25Q_VOLATILE_SR_WRITE_EN		0X50
#define	W25Q_WRITE_DEN							0X04
#define	W25Q_READ_REG_1							0X05
#define W25Q_WRITE_REG_1 						0X01
#define	W25Q_READ_REG_2							0X35
#define W25Q_WRITE_REG_2 						0X31
#define	W25Q_READ_REG_3							0X15
#define W25Q_WRITE_REG_3 						0X11
#define W25Q_CHIP_ERASE							0XC7	//0X60
#define	W25Q_ERASE_PROG_SUSPEND			0X75
#define W25Q_ERASE_PROG_RESUME			0X7A
#define	W25Q_POWER_DOWN							0XB9
#define	W25Q_RELASE_POWER_DOWN			0XAB
#define W25Q_MFTR_DEVICE_ID					0X90
#define W25Q_JEDEC_ID								0X9F
#define	W25Q_GLB_BLK_LOCK						0X7E
#define	W25Q_GLB_BLK_UNLOCK					0X98
#define	W25Q_ENTER_QPI_MODE					0X38
#define W25Q_EANBLE_RESET						0X66
#define W25Q_RESET_DEVICE						0X99
#define W25Q_UNIQUE_ID							0X4B
#define W25Q_PAGE_PROG							0X02
#define W25Q_QUAD_PAGE_PROG					0X32
#define W25Q_SECTOR_ERASE						0X20
#define W25Q_BLK32_ERASE						0X52
#define W25Q_BLK64_ERASE						0XD8
#define W25Q_READ_DATA							0X03
#define W25Q_FAST_READ							0X0B
#define W25Q_FAST_READ_DUAL_OUTPUT	0X3B
#define W25Q_FAST_READ_QUAD_OUTPUT	0X6B
#define W25Q_READ_SFDF_REG					0X5A
#define W25Q_ERASE_SECURITY_REG			0X44
#define W25Q_PROG_SECURITY_REG			0X42
#define W25Q_READ_SECURITY_REG			0X48
#define W25Q_INDIVIDUAL_BLK_LOCK		0X36
#define W25Q_INDIVIDUAL_BLK_UNLOCK	0X39
#define W25Q_READ_BLK_LOCK					0X3D
#define W25Q_FAST_READ_DUAL_IO			0XBB
#define W25Q_MFTR_DEVICE_ID_DUAL_IO	0X92
#define W25Q_SET_BURST_WITH_WRAP		0X77
#define W25Q_FAST_READ_QUAD_IO			0XEB
#define W25Q_WORD_READ_QUAD_IO			0XE7
#define W25Q_OCTAL_WORD_READ_QUAD_IO 0XE3
#define W25Q_MFTR_DEVICE_ID_QUAD_IO	0X94
//QPI instruction
//#define W25Q_
//#define W25Q_
//#define W25Q_
//#define W25Q_
//#define W25Q_
//#define W25Q_
//#define W25Q_
//#define W25Q_
//#define W25Q_
//#define W25Q_
//#define W25Q_
//#define W25Q_
//#define W25Q_
//#define W25Q_
//#define W25Q_
//#define W25Q_
//#define W25Q_
//#define W25Q_
//#define W25Q_
//#define W25Q_
//#define W25Q_
#endif

