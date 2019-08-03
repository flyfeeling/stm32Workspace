#include "bsp_mpu9250.h"
#include "bsp_mpu9250_def.h"
/*private includes*/
#include "i2c.h"
#include "stdio.h"
/*private macro*/
#define BSP_MPU_I2C_ADDR		0X68
#define BSP_MPU_I2C_TIMEOUT	0xFFFFFF
#define BSP_MPU_I2C_HANDLE	hi2c1
/*private variable*/

/*public variable*/

/*private funtion decaleration*/

/*public funtion decaleration*/

/*funtion*/
void BSP_MPU_INIT()
{ 
	uint8_t pData[2]={0};
	HAL_I2C_Mem_Read(&BSP_MPU_I2C_HANDLE, 0X0C, MAG_WIA, I2C_MEMADD_SIZE_8BIT, pData, 2, BSP_MPU_I2C_TIMEOUT);
	printf("mpu:%x\r\n",pData[0]);
}