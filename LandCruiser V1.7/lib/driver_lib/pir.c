
#include <stdio.h>
#include "delay.h"
#include "pir.h"
#include "driver.h"

static void pir_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(PIR_CLK | RCC_APB2Periph_AFIO, ENABLE);	 //配置外部中断，需要开启复用时钟。

  	GPIO_InitStructure.GPIO_Pin = PIR_PIN;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   

  	GPIO_Init(PIR_PORT, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(PIR_EXTI_LINE_PORT, PIR_EXTI_LINE_PIN);  	
}	

static void pir_exti_init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;												

	EXTI_InitStructure.EXTI_Line = PIR_EXTI_Line;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;               
	
	EXTI_Init(&EXTI_InitStructure); 
}

void pir_init(void)
{
	pir_gpio_init();
	pir_exti_init();
}
	








