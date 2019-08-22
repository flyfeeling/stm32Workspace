#include "../BSP/inc/pid.h"

/*private includes*/
#include "stdio.h"
/*private macro*/

/*private variable*/

/*public variable*/
pid_struct pid_left = {0},pid_right = {0};
/*private funtion decaleration*/

/*public funtion decaleration*/

/*funtion*/
void BSP_PID_CONTROLLER_INIT(pid_struct* pid, double kp, double ki, double kd)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
}
void BSP_PID_SET_TARGET(pid_struct* pid,double target)
{
	pid->target = target;
}
void BSP_PID_SET_CURRENT(pid_struct* pid,double current)
{
	pid->current = current;
}
double BSP_PID_GET_CURRENT(pid_struct* pid)
{
	return pid->current ;
}
double BSP_PID_UPDATE(pid_struct* pid)
{
	double derr,dderr,delta;
	pid->err_2 = pid->err_1;
	pid->err_1 = pid->err;
	pid->err =  pid->target - pid->current;//Uk=A*e(k)+B*e(k-1)+C*e(k-2)
	derr = pid->err - pid->err_1;
	dderr = pid->err - 2*pid->err_1 + pid->err_2;
	delta = pid->kp*derr + pid->ki*pid->err + pid->kd*dderr;
#if PID_DEBUG
	pid->delta = delta;
#endif
	
	return delta;
}
