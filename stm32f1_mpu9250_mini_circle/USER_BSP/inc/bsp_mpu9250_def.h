#ifndef __BSP_MPU9250_DEF_H__
#define __BSP_MPU9250_DEF_H__
/*public includes*/

/*public macro*/
//如果AD0脚(9脚)接地,IIC地址为0X68(不包含最低位).
//如果接V3.3,则IIC地址为0X69(不包含最低位).
#define MPU9250_IIC_SLAVE_ADDR	0X68    //MPU6500的器件IIC地址
#define MPU6500_ID1				0X71  	//MPU6500的器件ID1
#define MPU6500_ID2				0X73  	//MPU6500的器件ID2

//MPU9250内部封装了一个AK8963磁力计,地址和ID如下:
#define AK8963_IIC_SLAVE_ADDR	0X0C	//AK8963的I2C地址
#define AK8963_ID					0X48	//AK8963的器件ID


//AK8963的内部寄存器
#define MAG_WIA						0x00	//AK8963的器件ID寄存器地址
#define MAG_CNTL1     		0X0A    
#define MAG_CNTL2     		0X0B

#define MAG_XOUT_L				0X03	
#define MAG_XOUT_H				0X04
#define MAG_YOUT_L				0X05
#define MAG_YOUT_H				0X06
#define MAG_ZOUT_L				0X07
#define MAG_ZOUT_H				0X08

//MPU6500的内部寄存器
#define MPU_SELF_TESTGX_REG		0X00	//自检寄存器X
#define MPU_SELF_TESTGY_REG		0X01	//自检寄存器Y
#define MPU_SELF_TESTGZ_REG		0X02	//自检寄存器Z

#define MPU_SELF_TESTAX_REG		0X0D	//自检寄存器X
#define MPU_SELF_TESTAY_REG		0X0E	//自检寄存器Y
#define MPU_SELF_TESTAZ_REG		0X0F	//自检寄存器Z

#define MPU_XG_OFS_H	0X13
#define MPU_XG_OFS_L	0X14
#define MPU_YG_OFS_H	0X15
#define MPU_YG_OFS_L	0X16
#define MPU_ZG_OFS_H	0X17	
#define MPU_ZG_OFS_L	0X18

#define MPU_XA_OFS_H	0X77
#define MPU_XA_OFS_L	0X78
#define MPU_YA_OFS_H	0X7A
#define MPU_YA_OFS_L	0X7B
#define MPU_ZA_OFS_H	0X7D
#define MPU_ZA_OFS_L	0X7E

#define MPU_SAMPLE_RATE_REG		0X19	//采样频率分频器
#define MPU_CFG_REG						0X1A	//配置寄存器
#define MPU_GYRO_CFG_REG			0X1B	//陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG			0X1C	//加速度计配置寄存器
#define MPU_ACCEL_CFG2_REG		0X1D	//加速度计配置寄存器2
#define MPU_MOTION_DET_REG		0X1F	//运动检测阀值设置寄存器
#define MPU_FIFO_EN_REG				0X23	//FIFO使能寄存器
#define MPU_I2CMST_CTRL_REG		0X24	//IIC主机控制寄存器
#define MPU_I2CSLV0_ADDR_REG	0X25	//IIC从机0器件地址寄存器
#define MPU_I2CSLV0_REG				0X26	//IIC从机0数据地址寄存器
#define MPU_I2CSLV0_CTRL_REG	0X27	//IIC从机0控制寄存器
#define MPU_I2CSLV1_ADDR_REG	0X28	//IIC从机1器件地址寄存器
#define MPU_I2CSLV1_REG				0X29	//IIC从机1数据地址寄存器
#define MPU_I2CSLV1_CTRL_REG	0X2A	//IIC从机1控制寄存器
#define MPU_I2CSLV2_ADDR_REG	0X2B	//IIC从机2器件地址寄存器
#define MPU_I2CSLV2_REG				0X2C	//IIC从机2数据地址寄存器
#define MPU_I2CSLV2_CTRL_REG	0X2D	//IIC从机2控制寄存器
#define MPU_I2CSLV3_ADDR_REG	0X2E	//IIC从机3器件地址寄存器
#define MPU_I2CSLV3_REG				0X2F	//IIC从机3数据地址寄存器
#define MPU_I2CSLV3_CTRL_REG	0X30	//IIC从机3控制寄存器
#define MPU_I2CSLV4_ADDR_REG	0X31	//IIC从机4器件地址寄存器
#define MPU_I2CSLV4_REG				0X32	//IIC从机4数据地址寄存器
#define MPU_I2CSLV4_DO_REG		0X33	//IIC从机4写数据寄存器
#define MPU_I2CSLV4_CTRL_REG	0X34	//IIC从机4控制寄存器
#define MPU_I2CSLV4_DI_REG		0X35	//IIC从机4读数据寄存器

#define MPU_I2CMST_STA_REG		0X36	//IIC主机状态寄存器
#define MPU_INTBP_CFG_REG			0X37	//中断/旁路设置寄存器
#define MPU_INT_EN_REG				0X38	//中断使能寄存器
#define MPU_INT_STA_REG				0X3A	//中断状态寄存器

#define MPU_ACCEL_XOUTH_REG		0X3B	//加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG		0X3C	//加速度值,X轴低8位寄存器
#define MPU_ACCEL_YOUTH_REG		0X3D	//加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG		0X3E	//加速度值,Y轴低8位寄存器
#define MPU_ACCEL_ZOUTH_REG		0X3F	//加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG		0X40	//加速度值,Z轴低8位寄存器

#define MPU_TEMP_OUTH_REG		0X41	//温度值高八位寄存器
#define MPU_TEMP_OUTL_REG		0X42	//温度值低8位寄存器

#define MPU_GYRO_XOUTH_REG		0X43	//陀螺仪值,X轴高8位寄存器
#define MPU_GYRO_XOUTL_REG		0X44	//陀螺仪值,X轴低8位寄存器
#define MPU_GYRO_YOUTH_REG		0X45	//陀螺仪值,Y轴高8位寄存器
#define MPU_GYRO_YOUTL_REG		0X46	//陀螺仪值,Y轴低8位寄存器
#define MPU_GYRO_ZOUTH_REG		0X47	//陀螺仪值,Z轴高8位寄存器
#define MPU_GYRO_ZOUTL_REG		0X48	//陀螺仪值,Z轴低8位寄存器

#define MPU_I2CSLV0_DO_REG		0X63	//IIC从机0数据寄存器
#define MPU_I2CSLV1_DO_REG		0X64	//IIC从机1数据寄存器
#define MPU_I2CSLV2_DO_REG		0X65	//IIC从机2数据寄存器
#define MPU_I2CSLV3_DO_REG		0X66	//IIC从机3数据寄存器

#define MPU_I2CMST_DELAY_REG	0X67	//IIC主机延时管理寄存器
#define MPU_SIGPATH_RST_REG		0X68	//信号通道复位寄存器
#define MPU_MDETECT_CTRL_REG	0X69	//运动检测控制寄存器
#define MPU_USER_CTRL_REG		0X6A	//用户控制寄存器
#define MPU_PWR_MGMT1_REG		0X6B	//电源管理寄存器1
#define MPU_PWR_MGMT2_REG		0X6C	//电源管理寄存器2 
#define MPU_FIFO_CNTH_REG		0X72	//FIFO计数寄存器高八位
#define MPU_FIFO_CNTL_REG		0X73	//FIFO计数寄存器低八位
#define MPU_FIFO_RW_REG			0X74	//FIFO读写寄存器
#define MPU_DEVICE_ID_REG		0X75	//器件ID寄存器


//

//Read-only Register
#define WIA 	0x00
#define INFO 	0x01
#define ST1 	0x02
#define HXL		0x03
#define HXH		0X04
#define	HYL		0X05
#define HYH		0X06
#define HZL		0X07
#define	HZH		0X08
#define ST2		0X09
#define CNTL1	0X0A
#define CNTL2	0X0B
#define ASTC	0X0C
#define TS1		0X0D
#define TS2		0X0E
#define I2CDIS	0X0F
#define ASAX	0x10
#define ASAY	0x11
#define ASAZ	0x12
 

/*public variable extern*/

/*public  funtion decaleration*/

#endif