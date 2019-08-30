#ifndef __BSP_MPU9250_H__
#define __BSP_MPU9250_H__
/*public includes*/
#include "stm32f1xx_hal.h"
/*public macro*/
#define MPU_INT_STATUS_WOM			0X40
#define MPU_INT_STATUS_FIFO_OW	0X10
#define MPU_INT_STATUS_FSYNC		0X08
#define MPU_INT_STATUS_RAW_RD		0X01
/*public variable extern*/

/*public  funtion decaleration*/
void BSP_MPU_INIT(void);
void BSP_MPU_GET_TEMPERATURE(double *temp);
void BSP_MPU_GET_ACCELEROMETER(int16_t *ax, int16_t *ay, int16_t *az);
void BSP_MPU_GET_GYROSCOPE(int16_t *gx, int16_t *gy, int16_t *gz);
void BSP_MPU_GET_MAGNETOMETER(int16_t *mx, int16_t *my, int16_t *mz);

void BSP_MPU_SET_GYROSCOPE_OFFSET(int16_t gx, int16_t gy, int16_t gz);
void BSP_MPU_SET_ACCELEROMETER_OFFSET(int16_t ax, int16_t ay, int16_t az);
void BSP_MPU_TEST();

uint8_t BSP_MPU_INT_STATUS(void);
uint8_t BSP_MPU_MAG_STATUS(uint8_t st);
void BSP_MPU_GET_FIFO_CNT(uint16_t *cnt);
inline void BSP_MPU_FIFO_READ_WRITE(uint8_t *dat);
#endif