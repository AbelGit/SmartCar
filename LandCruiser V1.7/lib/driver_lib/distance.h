#ifndef __DISTANCE_H
#define __DISTANCE_H

#include "stm32f10x.h"
#include "sys.h"
#include "driver.h"

#define HC04_LEFT_TIM  TIM1 

void hcsr04_init(void);
void cal_left_distance(void);
void cal_right_distance(void);
void get_dis_data(void);
void run_ultra(void);


#endif 

