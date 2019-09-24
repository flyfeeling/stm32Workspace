#include "bsp_mpu9250.h"
#include "bsp_mpu9250_def.h"
/*private includes*/
#include "i2c.h"
#include "stdio.h"

/*private macro*/ 
#define BSP_MPU_I2C_TIMEOUT	0xFFFFFF
#define BSP_MPU_I2C_HANDLE	hi2c1
/*private variable*/
#define BSP_GYRO_FS_SEL			3
#define BSP_ACCEL_FS_SEL		3
#define BSP_SAMPLE_DIV			0				//1K Hz
#define	BSP_ACCEL_DLPF			0				//218 Hz	//1.88 ms delay
#define BSP_GYRO_DLPF				2				//184 Hz	//2.9	ms delay
#define BSP_MPU_FIFO_EN			0
#define	BSP_MPU_FIFO_OW			0				//overwrite
#define BSP_MPU_CLKSEL			1				//best clk
/*public variable*/

/*private funtion decaleration*/
/************************************************************/
/************* 				Private IO					*******************/
/************************************************************/
void BSP_MPU_WRITE_BYTE(uint8_t slave, uint8_t reg, uint8_t dat)
{ 
	HAL_I2C_Mem_Write(&BSP_MPU_I2C_HANDLE, slave<<1, reg, I2C_MEMADD_SIZE_8BIT, &dat, 1, BSP_MPU_I2C_TIMEOUT);
}
uint8_t BSP_MPU_READ_BYTE(uint8_t slave, uint8_t reg)
{ 
	uint8_t dat;
	HAL_I2C_Mem_Read(&BSP_MPU_I2C_HANDLE, slave<<1, reg, I2C_MEMADD_SIZE_8BIT, &dat, 1, BSP_MPU_I2C_TIMEOUT);
	return dat;
}
void BSP_MPU_WRITE(uint8_t slave, uint8_t reg, uint8_t dat[], uint16_t len)
{
	HAL_I2C_Mem_Write(&BSP_MPU_I2C_HANDLE, slave<<1, reg, I2C_MEMADD_SIZE_8BIT, dat, len, BSP_MPU_I2C_TIMEOUT);
}

void BSP_MPU_READ(uint8_t slave, uint8_t reg, uint8_t buf[], uint16_t len)
{
	HAL_I2C_Mem_Read(&BSP_MPU_I2C_HANDLE, slave<<1, reg, I2C_MEMADD_SIZE_8BIT, buf, len, BSP_MPU_I2C_TIMEOUT);
}
/*public funtion decaleration*/

/*funtion*/
void BSP_MPU_SOFT_RESET()
{
	BSP_MPU_WRITE_BYTE(MPU9250_IIC_SLAVE_ADDR, MPU_PWR_MGMT1_REG, 0x80);
	HAL_Delay(50);
	BSP_MPU_WRITE_BYTE(MPU9250_IIC_SLAVE_ADDR, MPU_PWR_MGMT1_REG, (BSP_MPU_CLKSEL<<0));	 
	HAL_Delay(50);
}
void BSP_MPU_INIT()
{  
	BSP_MPU_SOFT_RESET();  
	BSP_MPU_WRITE_BYTE(MPU9250_IIC_SLAVE_ADDR, MPU_PWR_MGMT2_REG, 0xFF);		//diable all axis of gyro & accel
	/*
		Disable Fifo
		Set	Interupt	& INT PIn Configure & Set INT level
		clear INT Status by read INT Status
	*/ 
	BSP_MPU_WRITE_BYTE(MPU9250_IIC_SLAVE_ADDR, MPU_FIFO_EN_REG, 	0x00);		// disable fifo
	BSP_MPU_WRITE_BYTE(MPU9250_IIC_SLAVE_ADDR, MPU_INTBP_CFG_REG, 0x82); //int low effective, bypass turn on
	//BSP_MPU_WRITE_BYTE(MPU9250_IIC_SLAVE_ADDR, MPU_INTBP_CFG_REG, (1<<7)|(0<<6)|(0<<5)|(0<<4)|(1<<1)); //int low effective, bypass turn on
	BSP_MPU_WRITE_BYTE(MPU9250_IIC_SLAVE_ADDR, MPU_INT_EN_REG, 		(1<<0));	// enable raw ready int
	BSP_MPU_WRITE_BYTE(MPU9250_IIC_SLAVE_ADDR, MPU_USER_CTRL_REG, 0x00);		// i2c master turn off
	/*
		Set Gyro Fs 2000dps 		& Accel Fs 16g
		Set Gyro fchoice_b = 00	&	accel_fchoice_b = 0
		Set Accel DLPF	
		
	*/
	BSP_MPU_WRITE_BYTE(MPU9250_IIC_SLAVE_ADDR, MPU_GYRO_CFG_REG, (BSP_GYRO_FS_SEL<<3)|(0x00<<0));		//+-2000dps & fchoice_b = 00 -> fchoic = 11
	BSP_MPU_WRITE_BYTE(MPU9250_IIC_SLAVE_ADDR, MPU_ACCEL_CFG_REG, (BSP_ACCEL_FS_SEL<<3));						//+-16g
	BSP_MPU_WRITE_BYTE(MPU9250_IIC_SLAVE_ADDR, MPU_ACCEL_CFG2_REG, (0x00<<3)|(BSP_ACCEL_DLPF<<0));	//lp 
	/*
		Set Gyro	DLPF
		Set Sample rate
	*/
	BSP_MPU_WRITE_BYTE(MPU9250_IIC_SLAVE_ADDR, MPU_CFG_REG, (BSP_MPU_FIFO_OW<<6)|(0x00<<3)|(BSP_GYRO_DLPF<<0));
	BSP_MPU_WRITE_BYTE(MPU9250_IIC_SLAVE_ADDR, MPU_SAMPLE_RATE_REG, BSP_SAMPLE_DIV);								//sample = 1k/(1+div)
	
 	uint8_t devId = BSP_MPU_READ_BYTE(MPU9250_IIC_SLAVE_ADDR, MPU_DEVICE_ID_REG);
	uint8_t magId = BSP_MPU_READ_BYTE(AK8963_IIC_SLAVE_ADDR, MAG_WIA);
	printf("MPU ID:0x%X MAG ID:0x%X\r\n",devId, magId);
	BSP_MPU_WRITE_BYTE(MPU9250_IIC_SLAVE_ADDR, MPU_PWR_MGMT2_REG, 0X00);		// enable all axis of gyro & accel
	
	if(magId == AK8963_ID)
	{
		BSP_MPU_WRITE_BYTE(AK8963_IIC_SLAVE_ADDR, MAG_CNTL2, 0x01);
		while(BSP_MPU_READ_BYTE(AK8963_IIC_SLAVE_ADDR, MAG_CNTL2))HAL_Delay(5);
		
//		BSP_MPU_WRITE_BYTE(AK8963_IIC_SLAVE_ADDR, MAG_CNTL1, 0x11);		//16bit mode 	0.15uT/LSB & signle measurement
//  	BSP_MPU_WRITE_BYTE(AK8963_IIC_SLAVE_ADDR, MAG_CNTL1, 0x12);		//	continuous measure mode1	8Hz
		BSP_MPU_WRITE_BYTE(AK8963_IIC_SLAVE_ADDR, MAG_CNTL1, 0x16); 	//	continuous measure mode2	100Hz
		//连续测量模式每次需要读ST2
	}
	return ;
}

void BSP_MPU_TEST()
{
	int16_t ax,ay,az,gx,gy,gz,mx,my,mz;
	BSP_MPU_GET_ACCELEROMETER(&ax, &ay, &az);
	//printf("ACC:%d,%d,%d\r\n",x,y,z);
	BSP_MPU_GET_GYROSCOPE(&gx, &gy, &gz);
	//printf("GYRO:%d,%d,%d\r\n",x,y,z);
	BSP_MPU_GET_MAGNETOMETER(&mx, &my, &mz);
	//printf("MAG:%d,%d,%d\r\n",x,y,z);
	printf("%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",ax,ay,az,gx,gy,gz,mx,my,mz);
}
uint8_t BSP_MPU_INT_STATUS(void)
{
	return BSP_MPU_READ_BYTE(MPU9250_IIC_SLAVE_ADDR, MPU_INT_STA_REG);
}

uint8_t BSP_MPU_MAG_STATUS(uint8_t st)
{
	return BSP_MPU_READ_BYTE(AK8963_IIC_SLAVE_ADDR, st>0?ST2:ST1);
	// DRDY bit turns to “1” when data is ready in single measurement mode or self-test mode. It returns to “0”
	// when any one of ST2 register or measurement data register (HXL to HZH) is read
}
	
	
/************************************************************/
/************* 				Read Sensor					*******************/
/************************************************************/
void BSP_MPU_GET_TEMPERATURE(double *temp)
{
	uint8_t buf[2];
	uint16_t raw;
	BSP_MPU_READ(MPU9250_IIC_SLAVE_ADDR, MPU_TEMP_OUTH_REG, buf, 2);
	raw=((uint16_t)buf[0]<<8)|buf[1];  
  *temp = 21+((double)raw)/333.87;   
}
void BSP_MPU_GET_ACCELEROMETER(int16_t *ax, int16_t *ay, int16_t *az)
{
	uint8_t buf[6];
	BSP_MPU_READ(MPU9250_IIC_SLAVE_ADDR, MPU_ACCEL_XOUTH_REG, buf, 6);
	*ax=((uint16_t)buf[0]<<8)|buf[1];  
	*ay=((uint16_t)buf[2]<<8)|buf[3];  
	*az=((uint16_t)buf[4]<<8)|buf[5];
}
void BSP_MPU_GET_GYROSCOPE(int16_t *gx, int16_t *gy, int16_t *gz)
{
	uint8_t buf[6];
	BSP_MPU_READ(MPU9250_IIC_SLAVE_ADDR, MPU_GYRO_XOUTH_REG, buf, 6);
	*gx=((uint16_t)buf[0]<<8)|buf[1];  
	*gy=((uint16_t)buf[2]<<8)|buf[3];  
	*gz=((uint16_t)buf[4]<<8)|buf[5];
}
void BSP_MPU_GET_MAGNETOMETER(int16_t *mx, int16_t *my, int16_t *mz)
{
  uint8_t buf[6]; 
	BSP_MPU_READ(AK8963_IIC_SLAVE_ADDR, MAG_XOUT_L, buf, 6); 
	BSP_MPU_MAG_STATUS(1);
	*mx=((uint16_t)buf[1]<<8)|buf[0];  
	*my=((uint16_t)buf[3]<<8)|buf[2];  
	*mz=((uint16_t)buf[5]<<8)|buf[4]; 	 
	//BSP_MPU_WRITE_BYTE(AK8963_IIC_SLAVE_ADDR,MAG_CNTL1,0X11); //AK8963每次读完以后都需要重新设置为单次测量模式 
}
/************************************************************/
/************* 				Set Sensor Offset		*******************/
/************************************************************/
void BSP_MPU_SET_GYROSCOPE_OFFSET(int16_t gx, int16_t gy, int16_t gz)
{
	uint8_t buf[6];
	buf[1] = (uint8_t)gx;							//offset gx*4/(2^fs_sel)
	buf[0] = (uint8_t)(gx>>8);
	buf[3] = (uint8_t)gy;
	buf[2] = (uint8_t)(gy>>8);
	buf[5] = (uint8_t)gz;
	buf[4] = (uint8_t)(gz>>8);
	BSP_MPU_WRITE(MPU9250_IIC_SLAVE_ADDR, MPU_XG_OFS_H, buf, 6);   
}
void BSP_MPU_SET_ACCELEROMETER_OFFSET(int16_t ax, int16_t ay, int16_t az)
{
	uint8_t buf[6];
	ax = (36/65536/98*100000)*ax;ax<<=1;
	ay = (36/65536/98*100000)*ay;ay<<=1;
	az = (36/65536/98*100000)*az;az<<=1;
	buf[1] = (uint8_t)ax;							//15 bit 0.98-mg steps
	buf[0] = (uint8_t)(ax>>8);
	buf[3] = (uint8_t)ay;
	buf[2] = (uint8_t)(ay>>8);
	buf[5] = (uint8_t)az;
	buf[4] = (uint8_t)(az>>8);
	BSP_MPU_WRITE(MPU9250_IIC_SLAVE_ADDR, MPU_XA_OFS_H, buf, 6);   
}

/************************************************************/
/************* 		Fifo Control						*******************/
/************************************************************/
void BSP_MPU_GET_FIFO_CNT(uint16_t *cnt)
{
	uint8_t buf[2]; 
	BSP_MPU_READ(MPU9250_IIC_SLAVE_ADDR, MPU_FIFO_CNTH_REG, buf, 2);
	*cnt = buf[0]*256 + buf[1];
}
inline void BSP_MPU_FIFO_READ_WRITE(uint8_t *dat)
{
	BSP_MPU_READ(MPU9250_IIC_SLAVE_ADDR, MPU_FIFO_CNTH_REG, dat, 1);
}