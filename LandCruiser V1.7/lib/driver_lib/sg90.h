#ifndef __SG90_H
#define __SG90_H

#include "stm32f10x.h"
#include "timer.h"

// 转动到相应角度各自需要的值。
#define SG90_LR_000		50
#define SG90_LR_045 	75
#define SG90_LR_090		100
#define SG90_LR_135		145
#define SG90_LR_180		200

#define SG90_UD_000		60
#define SG90_UD_015		67
#define SG90_UD_030		77
#define SG90_UD_045		80
#define SG90_UD_060		94
#define SG90_UD_075		107
#define SG90_UD_090		120

void sg90_up_down(u8 chg_val);  
void sg90_left_right(u8 chg_val); 
void sg90_reset(u8 ud_val, u8 lr_val);

#endif 

