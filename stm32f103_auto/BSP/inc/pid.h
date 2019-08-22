#ifndef __BSP_PID_H__
#define __BSP_PID_H__
/*public includes*/
#include "stm32f1xx_hal.h"
/*public macro*/
#define PID_DEBUG 1
typedef struct{
	double kp,ki,kd;
	double err;
	double err_1;
	double err_2;
	double current;
	double target;
#if PID_DEBUG
	double delta;
#endif
} pid_struct;
/*public variable extern*/
extern pid_struct pid_left,pid_right;
/*public  funtion decaleration*/
void 		BSP_PID_CONTROLLER_INIT(pid_struct* pid, double kp, double ki, double kd);
void 		BSP_PID_SET_TARGET(pid_struct* pid,double target);
void		BSP_PID_SET_CURRENT(pid_struct* pid,double current);
double 	BSP_PID_GET_CURRENT(pid_struct* pid);
double 	BSP_PID_UPDATE(pid_struct* pid);
#endif