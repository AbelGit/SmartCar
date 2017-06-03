
//PD 8,9,10,11
/* 按键的外部中断配置 */
#include <stdio.h>

#include "stm32f10x.h"
#include "key_exti.h"
#include "rc.h"
#include "driver.h"

static void EXTI_Key_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = KEY_EXTI_PIN1 | KEY_EXTI_PIN2 | KEY_EXTI_PIN3 | KEY_EXTI_PIN4 
								| KEY_EXTI_PIN5 | KEY_EXTI_PIN6 | KEY_EXTI_PIN7 | KEY_EXTI_PIN8 
								| KEY_EXTI_PIN9;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//上拉输入   
	GPIO_Init(KEY_EXTI_PORT, &GPIO_InitStructure);

	GPIO_EXTILineConfig(KEY_EXTI_LINE_PORT1, KEY_EXTI_LINE_PIN1);  
	GPIO_EXTILineConfig(KEY_EXTI_LINE_PORT1, KEY_EXTI_LINE_PIN2);  
	GPIO_EXTILineConfig(KEY_EXTI_LINE_PORT1, KEY_EXTI_LINE_PIN3); 
	GPIO_EXTILineConfig(KEY_EXTI_LINE_PORT1, KEY_EXTI_LINE_PIN4);
	
	GPIO_EXTILineConfig(KEY_EXTI_LINE_PORT1, KEY_EXTI_LINE_PIN5);  
	GPIO_EXTILineConfig(KEY_EXTI_LINE_PORT1, KEY_EXTI_LINE_PIN6);  
	GPIO_EXTILineConfig(KEY_EXTI_LINE_PORT1, KEY_EXTI_LINE_PIN7); 
	GPIO_EXTILineConfig(KEY_EXTI_LINE_PORT1, KEY_EXTI_LINE_PIN8);
	GPIO_EXTILineConfig(KEY_EXTI_LINE_PORT1, KEY_EXTI_LINE_PIN9);

}

void EXTI_Key_init(void)
{	
	EXTI_InitTypeDef EXTI_InitStructure;												

	RCC_APB2PeriphClockCmd(KEY_EXTI_RCC_PORT1 | RCC_APB2Periph_AFIO, ENABLE);	
	
  	EXTI_Key_GPIO_Configuration();

	EXTI_InitStructure.EXTI_Line = KEY_EXTI_LINE1 | KEY_EXTI_LINE2 | KEY_EXTI_LINE3 | KEY_EXTI_LINE4 
								 | KEY_EXTI_LINE5 | KEY_EXTI_LINE6 | KEY_EXTI_LINE7 | KEY_EXTI_LINE8 
								 | KEY_EXTI_LINE9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;               
	
	EXTI_Init(&EXTI_InitStructure); 
}

