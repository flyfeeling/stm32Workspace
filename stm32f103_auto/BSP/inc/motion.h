#ifndef __BSP_MOTION_H__
#define __BSP_MOTION_H__
/*public includes*/
#include "stm32f1xx_hal.h"
/*public macro*/

/*public variable extern*/
typedef enum
{
  LEFT_WHEEL	= 0U, 
	RIGHT_WHEEL	= 1U, 
} WHEEl_ENUM;

typedef struct{
	double left_speed;
	double right_speed;
	double speed;
	double angluar_speed;
	//double move_radius;
} wheel_speed_struct;

//extern double delta_left;
//extern double delta_right;
extern wheel_speed_struct wheel;
/*public  funtion decaleration*/ 
void BSP_MOTION_UPDATE(void);
void BSP_MOTION_SET_SPEED(WHEEl_ENUM wheel, double speed);

#endif