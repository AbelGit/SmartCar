#include <stdio.h>

#include "stm32f10x.h"
#include "driver.h"
#include "timer.h"



void TIM1_Config(void)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1 ,ENABLE);  

	/**********************************************************
	72 000 000/72=1M
	1000 000/2500=400Hz
	所以产生的PWM为400Hz
	周期为2.5ms，对应2500的计算值，1ms~2ms对应的计算值为1000~2000；
	**********************************************************/
	/* 10us 中断一次  */
	TIM_TimeBaseStructure.TIM_Period = 9;		//计数上线	 
	TIM_TimeBaseStructure.TIM_Prescaler = 71;	//pwm时钟分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//重复寄存器，用于自动更新pwm占空比
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_ARRPreloadConfig(TIM1, ENABLE);
	
	TIM_ITConfig(TIM1,	TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
}



/*2ms interrupt one time*/
//void TIM2_Config(void)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	//基础设置，时基和比较输出设置，由于这里只需定时，所以不用OC比较输出
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
//	
//	TIM_DeInit(TIM2);

//	TIM_TimeBaseStructure.TIM_Period = 1999;//装载值
//	//prescaler is 1200,that is 72000000/72/500=2000Hz;
//	TIM_TimeBaseStructure.TIM_Prescaler = 71;//分频系数
//	//set clock division 
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //or TIM_CKD_DIV2 or TIM_CKD_DIV4
//	//count up
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//	//clear the TIM5 overflow interrupt flag
//	TIM_ClearFlag(TIM2, TIM_FLAG_Update);

//	//TIM2 overflow interrupt enable
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//	TIM_Cmd(TIM2, ENABLE);
//}

static void tim2_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(SG90_CLK|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
	
	TIM_DeInit(TIM2);
	
	GPIO_InitStructure.GPIO_Pin = SG90_LR_PIN | SG90_UD_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SG90_PORT, &GPIO_InitStructure);
}
static void tim2_time_base_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	/* 50HZ, 50-250 对应：0.5ms-2.5ms  */
	TIM_TimeBaseStructure.TIM_Period = 1999; 		//20000us = 20ms, 				
	TIM_TimeBaseStructure.TIM_Prescaler = 719; 		//0.1MHZ				
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 	
}
static void tim2_oc3_init(void)
{
	TIM_OCInitTypeDef  TIM_OC3InitStructure;
	
	TIM_OC3InitStructure.TIM_OCMode = TIM_OCMode_PWM1; 			
 	TIM_OC3InitStructure.TIM_OutputState = TIM_OutputState_Enable; 		
	TIM_OC3InitStructure.TIM_Pulse = 80;     
	TIM_OC3InitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	
	TIM_OC3Init(TIM2, &TIM_OC3InitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
}
static void tim2_oc4_init(void)
{
	TIM_OCInitTypeDef  TIM_OC4InitStructure;
	
	TIM_OC4InitStructure.TIM_OCMode = TIM_OCMode_PWM1; 			
 	TIM_OC4InitStructure.TIM_OutputState = TIM_OutputState_Enable; 		
	TIM_OC4InitStructure.TIM_Pulse = 100;     
	TIM_OC4InitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	
	TIM_OC4Init(TIM2, &TIM_OC4InitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
}
static void tim2_mode_init(void)
{
	tim2_oc3_init();
	tim2_oc4_init();
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE); 
}
void TIM2_Config(void)
{
	tim2_gpio_init();
	tim2_time_base_init();
	tim2_mode_init();
}

static void tim3_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(FORNT_CLK|RCC_APB2Periph_AFIO, ENABLE);   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = FRONT_PIN1 | FRONT_PIN2 | FRONT_PIN3 | FRONT_PIN4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(FRONT_PORT, &GPIO_InitStructure);
	
}

static void tim3_time_base_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	TIM_TimeBaseStructure.TIM_Period = 2499;
	TIM_TimeBaseStructure.TIM_Prescaler = 71; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
}

static void tim3_oc_mode_init(void)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 	  
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_Pulse = 1000;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);     
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);     
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);     
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);     
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

static void tim4_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(REAR_CLK|RCC_APB2Periph_AFIO, ENABLE);   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // TIM4's clock
	
	GPIO_InitStructure.GPIO_Pin = REAR_PIN1 | REAR_PIN2 | REAR_PIN3 | REAR_PIN4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(REAR_PORT, &GPIO_InitStructure);
}

static void tim4_time_base_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	TIM_TimeBaseStructure.TIM_Period = 2499;
	TIM_TimeBaseStructure.TIM_Prescaler = 71; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
}

static void tim4_oc_mode_init(void)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 	  
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_Pulse = 1000;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);     
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);     
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);     
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);     
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

void TIM4_Config(void)
{
	tim4_gpio_init();
	tim4_time_base_init();
	tim4_oc_mode_init();
}





void TIM3_Config(void)
{
	tim3_gpio_init();
	tim3_time_base_init();
	tim3_oc_mode_init();
}






















