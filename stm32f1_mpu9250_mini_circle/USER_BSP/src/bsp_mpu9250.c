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
/*public variable*/

/*private funtion decaleration*/
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
	BSP_MPU_WRITE_BYTE(MPU9250_ADDR, MPU_PWR_MGMT1_REG, 0x80);
	HAL_Delay(100);
	BSP_MPU_WRITE_BYTE(MPU9250_ADDR, MPU_PWR_MGMT1_REG, 0x00);
}
void BSP_MPU_INIT()
{ 
	BSP_MPU_SOFT_RESET();
	BSP_MPU_WRITE_BYTE(MPU9250_ADDR, MPU_FIFO_EN_REG, 0x00);		//disable fifo
	BSP_MPU_WRITE_BYTE(MPU9250_ADDR, MPU_INTBP_CFG_REG, 0x02);	//int low effective, bypass turn on
	BSP_MPU_WRITE_BYTE(MPU9250_ADDR, MPU_INT_EN_REG, 0x01);			//disable interupt
	BSP_MPU_WRITE_BYTE(MPU9250_ADDR, MPU_USER_CTRL_REG, 0x00);	//i2c master turn off
	BSP_MPU_WRITE_BYTE(MPU9250_ADDR, MPU_GYRO_CFG_REG, 0x00);		//fchoice_b	= 00	fchoic = 11
	BSP_MPU_WRITE_BYTE(MPU9250_ADDR, MPU_SAMPLE_RATE_REG, 1);		//sample = 1k/(1+div)
	BSP_MPU_WRITE_BYTE(MPU9250_ADDR, MPU_GYRO_CFG_REG, BSP_GYRO_FS_SEL<<3);		//+-2000dps
	BSP_MPU_WRITE_BYTE(MPU9250_ADDR, MPU_ACCEL_CFG_REG, BSP_ACCEL_FS_SEL<<3);	//+-16g
	BSP_MPU_WRITE_BYTE(MPU9250_ADDR, MPU_CFG_REG, 0x02);
	BSP_MPU_WRITE_BYTE(MPU9250_ADDR, MPU_ACCEL_CFG2_REG, 0x06);	//lp 5hz
	
	uint8_t devId = BSP_MPU_READ_BYTE(MPU9250_ADDR, MPU_DEVICE_ID_REG);
	uint8_t magId = BSP_MPU_READ_BYTE(AK8963_ADDR, MAG_WIA);
	printf("MPU ID:0x%X MAG ID:0x%X\r\n",devId, magId);

//	if(magId == AK8963_ID)
//	{
//		BSP_MPU_WRITE_BYTE(AK8963_ADDR, MAG_CNTL2, 0x01);
//		HAL_Delay(5);
//		BSP_MPU_WRITE_BYTE(AK8963_ADDR, MAG_CNTL1, 0x11);
////		BSP_MPU_WRITE_BYTE(AK8963_ADDR, MAG_CNTL1, 0x12);
////		BSP_MPU_WRITE_BYTE(AK8963_ADDR, MAG_CNTL1, 0x16);
//	}
	return ;
}

void BSP_MPU_GET_TEMPERATURE(double *temp)
{
	uint8_t buf[2];
	uint16_t raw;
	BSP_MPU_READ(MPU9250_ADDR, MPU_TEMP_OUTH_REG, buf, 2);
	raw=((uint16_t)buf[0]<<8)|buf[1];  
  *temp = 21+((double)raw)/333.87;   
}
void BSP_MPU_GET_ACCELEROMETER(int16_t *ax, int16_t *ay, int16_t *az)
{
	uint8_t buf[6];
	BSP_MPU_READ(MPU9250_ADDR, MPU_ACCEL_XOUTH_REG, buf, 6);
	*ax=((uint16_t)buf[0]<<8)|buf[1];  
	*ay=((uint16_t)buf[2]<<8)|buf[3];  
	*az=((uint16_t)buf[4]<<8)|buf[5];
}
void BSP_MPU_GET_GYROSCOPE(int16_t *gx, int16_t *gy, int16_t *gz)
{
	uint8_t buf[6];
	BSP_MPU_READ(MPU9250_ADDR, MPU_GYRO_XOUTH_REG, buf, 6);
	*gx=((uint16_t)buf[0]<<8)|buf[1];  
	*gy=((uint16_t)buf[2]<<8)|buf[3];  
	*gz=((uint16_t)buf[4]<<8)|buf[5];
}
void BSP_MPU_GET_MAGNETOMETER(int16_t *mx, int16_t *my, int16_t *mz)
{
  uint8_t buf[6]; 
	BSP_MPU_READ(MPU9250_ADDR, MAG_XOUT_L, buf, 6); 
	*mx=((uint16_t)buf[1]<<8)|buf[0];  
	*my=((uint16_t)buf[3]<<8)|buf[2];  
	*mz=((uint16_t)buf[5]<<8)|buf[4]; 	 
	BSP_MPU_WRITE_BYTE(AK8963_ADDR,MAG_CNTL1,0X11); //AK8963每次读完以后都需要重新设置为单次测量模式 
}
void BSP_MPU_SET_GYROSCOPE_OFFSET(int16_t gx, int16_t gy, int16_t gz)
{
	uint8_t buf[6];
	buf[1] = (uint8_t)gx;							//offset gx*4/(2^fs_sel)
	buf[0] = (uint8_t)(gx>>8);
	buf[3] = (uint8_t)gy;
	buf[2] = (uint8_t)(gy>>8);
	buf[5] = (uint8_t)gz;
	buf[4] = (uint8_t)(gz>>8);
	BSP_MPU_WRITE(MPU9250_ADDR, MPU_XG_OFS_H, buf, 6);   
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
	BSP_MPU_WRITE(MPU9250_ADDR, MPU_XA_OFS_H, buf, 6);   
}
void BSP_MPU_TEST()
{
	int16_t x,y,z;
	BSP_MPU_GET_ACCELEROMETER(&x, &y, &z);
	printf("ACC:%d,%d,%d\r\n",x,y,z);
	BSP_MPU_GET_GYROSCOPE(&x, &y, &z);
	printf("GYRO:%d,%d,%d\r\n",x,y,z);
	BSP_MPU_GET_MAGNETOMETER(&x, &y, &z);
	printf("MAG:%d,%d,%d\r\n",x,y,z);
}
uint8_t BSP_MPU_INT_STATUS(void)
{
	return BSP_MPU_READ_BYTE(MPU9250_ADDR, MPU_INT_STA_REG);
}