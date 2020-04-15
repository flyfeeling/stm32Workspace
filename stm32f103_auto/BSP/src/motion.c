#include "../BSP/inc/motion.h"

/*private includes*/
#include "../BSP/inc/pwm.h"
#include "../BSP/inc/encoder.h"
#include "../BSP/inc/pid.h"
#include "main.h"
#include "stdio.h"
/*private macro*/
#define WHEEL_DIAMETER	6.5//4.2f	//cm
#define WHEEL_DISTANCE  12.0f	//cm
#define PI	3.141593f					
/*private variable*/

/*public variable*/
wheel_speed_struct wheel = {0};
double delta_left = 0,delta_right = 0;
long int pwm_left, pwm_right;
/*private funtion decaleration*/
void BSP_MOTION_GET_SPEED(void)		//5cm/s - 30cm/s
{ 
	wheel.left_speed = BSP_ENCODER_GET_FREQ(LEFT_ENCODER)*PI*WHEEL_DIAMETER;
	wheel.right_speed = BSP_ENCODER_GET_FREQ(RIGHT_ENCODER)*PI*WHEEL_DIAMETER;
//	wheel.left_speed = BSP_ENCODER_GET_FREQ(LEFT_ENCODER);
//	wheel.right_speed = BSP_ENCODER_GET_FREQ(RIGHT_ENCODER);
	wheel.speed = (wheel.left_speed + wheel.right_speed)/2;
	wheel.angluar_speed = (wheel.right_speed - wheel.left_speed)/WHEEL_DISTANCE;
	//wheel.move_radius = wheel.speed/wheel.angluar_speed;
}

/*public funtion decaleration*/

/*funtion*/

void BSP_MOTION_UPDATE(void)
{
	BSP_MOTION_GET_SPEED();
	BSP_PID_SET_CURRENT(&pid_left, wheel.left_speed);
	BSP_PID_SET_CURRENT(&pid_right, wheel.right_speed);
	//change dirrection
//	if(pid_left.current*pid_left.target<0)pwm_left = 0;
//	if(pid_right.current*pid_right.target<0)pwm_right = 0;
	
	delta_left = BSP_PID_UPDATE(&pid_left);
	delta_right = BSP_PID_UPDATE(&pid_right);
	pwm_left = pwm_left + (delta_left);
	pwm_right = pwm_right + (delta_right);
	
	printf("pid: %lf %lf %ld , %lf %lf %ld \r\n", pid_left.current, pid_left.target, pwm_left, pid_right.current, pid_right.target, pwm_right);

	if(pwm_left<0)
	{
		if(pwm_left < -PWM_PERIOD)pwm_left = -PWM_PERIOD;
		BSP_PWM_SET_PULSE(PWM1, -pwm_left);
		BSP_PWM_SET_PULSE(PWM2, 0);
	}
	else
	{
		if(pwm_left > PWM_PERIOD)pwm_left = PWM_PERIOD;
		BSP_PWM_SET_PULSE(PWM2, pwm_left);
		BSP_PWM_SET_PULSE(PWM1, 0);
	}
	if(pwm_right<0)
	{
		if(pwm_right < -PWM_PERIOD)pwm_right = -PWM_PERIOD;
		BSP_PWM_SET_PULSE(PWM3, -pwm_right);
		BSP_PWM_SET_PULSE(PWM4, 0);
	}
	else
	{
		if(pwm_right > PWM_PERIOD)pwm_right = PWM_PERIOD;
		BSP_PWM_SET_PULSE(PWM4, pwm_right);
		BSP_PWM_SET_PULSE(PWM3, 0);
	}
}

#define MAX_SPEED 60

void BSP_MOTION_SET_SPEED(WHEEl_ENUM wheel, double speed)
{
	if(speed > MAX_SPEED){
		speed = MAX_SPEED;
	}else if(speed < -MAX_SPEED){
		speed = -MAX_SPEED;
	}
	
	if(wheel == LEFT_WHEEL)
	{
		BSP_PID_SET_TARGET(&pid_left, speed);
	}
	else if(wheel == RIGHT_WHEEL)
	{
		BSP_PID_SET_TARGET(&pid_right, speed);
	}
	else
	{
		Error_Handler();
	}
}