#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"
#include "sys.h"



void motor_init(void);
	

/*---------------------------*/
void front_left_go(const u16 speed); 
void front_left_back(const u16 speed);
void front_left_stop(const u16 speed);

void front_right_go(const u16 speed);
void front_right_back(const u16 speed);
void front_right_stop(const u16 speed);

void rear_left_go(const u16 speed);
void rear_left_back(const u16 speed);
void rear_left_stop(const u16 speed);

void rear_right_go(const u16 speed); 
void rear_right_back(const u16 speed);
void rear_right_stop(const u16 speed);

/*-----------------------------*/
void car_start(const u16 spd);
void car_go_forward(const u16 left_spd, const u16 right_spd);
void car_stop(void);
void car_go_back(const u16 left_spd, const u16 right_spd);
void car_rotate(const u16 left_spd, const u16 right_spd, const u8 direction);
void car_turn(const u16 left_spd, const u16 right_spd);


#endif



