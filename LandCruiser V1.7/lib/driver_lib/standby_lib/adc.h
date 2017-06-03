#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"


//°´¼ü
void Rocker_EXTI_Key_init(void);
void Rocker_ADC_Init(void);

//device
void adc_init(void);  
void get_mq2_data(void);


#endif



