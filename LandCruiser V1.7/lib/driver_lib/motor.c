
#include <stdio.h>
#include "stm32f10x.h"
#include "motor.h"
#include "timer.h"
#include "delay.h"
#include "bluetooth.h"

void motor_init(void)
{
	TIM3_Config();
	TIM4_Config();    //电机速度控制 
}


/*---------------------control one wheel -------------------*/
void front_left_go(const u16 speed)    //前左
{
	TIM_SetCompare1(FRONT_TIM, 0);	
	TIM_SetCompare2(FRONT_TIM, speed);	
}	
void front_left_back(const u16 speed)
{
	TIM_SetCompare1(FRONT_TIM, speed);	
	TIM_SetCompare2(FRONT_TIM, 0);	
}
void front_left_stop(const u16 speed)
{
	TIM_SetCompare1(FRONT_TIM, speed);	
	TIM_SetCompare2(FRONT_TIM, speed);	
}

void front_right_go(const u16 speed)  //前右
{
	TIM_SetCompare3(FRONT_TIM, speed);	
	TIM_SetCompare4(FRONT_TIM, 0); 		
}
void front_right_back(const u16 speed)
{
	TIM_SetCompare3(FRONT_TIM, 0);	
	TIM_SetCompare4(FRONT_TIM, speed); 		
}
void front_right_stop(const u16 speed)
{
	TIM_SetCompare3(FRONT_TIM, speed);	
	TIM_SetCompare4(FRONT_TIM, speed); 		
}

void rear_left_go(const u16 speed)  //后左
{
	TIM_SetCompare3(REAR_TIM, 0);	
	TIM_SetCompare4(REAR_TIM, speed);
}
void rear_left_back(const u16 speed)
{
	TIM_SetCompare3(REAR_TIM, speed);	
	TIM_SetCompare4(REAR_TIM, 0);
}
void rear_left_stop(const u16 speed)
{
	TIM_SetCompare3(REAR_TIM, speed);	
	TIM_SetCompare4(REAR_TIM, speed);
}

void rear_right_go(const u16 speed)  //后右 
{
	TIM_SetCompare1(REAR_TIM, 0);	
	TIM_SetCompare2(REAR_TIM, speed);	
}
void rear_right_back(const u16 speed)
{
	TIM_SetCompare1(REAR_TIM, speed);	
	TIM_SetCompare2(REAR_TIM, 0);	
}
void rear_right_stop(const u16 speed)
{
	TIM_SetCompare1(REAR_TIM, speed);	
	TIM_SetCompare2(REAR_TIM, speed);	
}
	
/*-----------------control 4 wheel--------------------*/
void car_start(const u16 spd)
{
	front_left_go(spd);
	front_right_go(spd);
	rear_left_go(spd);
	rear_right_go(spd);
}

void car_go_forward(const u16 left_spd, const u16 right_spd)
{
	front_left_go(left_spd);
	front_right_go(right_spd);
	rear_left_go(left_spd);
	rear_right_go(right_spd);
}
void car_go_back(const u16 left_spd, const u16 right_spd)
{
	front_left_back(left_spd);
	front_right_back(right_spd);
	rear_left_back(left_spd);
	rear_right_back(right_spd);
}
void car_rotate(const u16 left_spd, const u16 right_spd, const u8 direction)
{
	if(direction == 0) //left rotate
	{
		//左面两轮子后转 
		front_left_back(left_spd);
		rear_left_back(left_spd);
		//右面两轮子前转
		front_right_go(right_spd);
		rear_right_go(right_spd);
	}
	if(direction == 1)  //right rotate  
	{
		//左面两轮子前转
		front_left_go(left_spd);
		rear_left_go(left_spd);
		//右面两轮子后转 
		front_right_back(right_spd);
		rear_right_back(right_spd);
	}
}

/**
  * @brief  
  * @param  左转： left_spd < right_spd    右转： left_spd > right_spd
  * @retval no
  */
void car_turn(const u16 left_spd, const u16 right_spd)
{
	front_left_go(left_spd);
	rear_left_go(left_spd);
	
	front_right_go(right_spd);
	rear_right_go(right_spd);
}

void car_stop(void)
{
	front_left_stop(0);
	front_right_stop(0);
	
	rear_left_stop(0);
	rear_right_stop(0);
}
















