#ifndef __DRIVER_H
#define __DRIVER_H

#include "stm32f10x.h"
#include "dht11.h"
#include "sys.h"

//usart
#define USART_RCC_PORT  RCC_APB2Periph_GPIOA
#define USART_RCC_MODE_PORT  RCC_APB2Periph_USART1
#define USART_Tx_Pin  GPIO_Pin_9
#define USART_Rx_Pin  GPIO_Pin_10
#define USART_PORT  GPIOA
#define USART_NAME  USART1


//key  PD 8,9,10,11    
#define KEY_EXTI_PIN1	GPIO_Pin_8
#define KEY_EXTI_PIN2	GPIO_Pin_9
#define KEY_EXTI_PIN3	GPIO_Pin_10
#define KEY_EXTI_PIN4	GPIO_Pin_11
#define KEY_EXTI_PIN5	GPIO_Pin_0
#define KEY_EXTI_PIN6	GPIO_Pin_1
#define KEY_EXTI_PIN7	GPIO_Pin_2
#define KEY_EXTI_PIN8	GPIO_Pin_3
#define KEY_EXTI_PIN9	GPIO_Pin_4
#define KEY_EXTI_PORT	GPIOD
#define KEY_EXTI_LINE_PORT1	GPIO_PortSourceGPIOD
#define KEY_EXTI_LINE_PIN1	GPIO_PinSource8
#define KEY_EXTI_LINE_PIN2	GPIO_PinSource9
#define KEY_EXTI_LINE_PIN3	GPIO_PinSource10
#define KEY_EXTI_LINE_PIN4	GPIO_PinSource11
#define KEY_EXTI_LINE_PIN5	GPIO_PinSource0
#define KEY_EXTI_LINE_PIN6	GPIO_PinSource1
#define KEY_EXTI_LINE_PIN7	GPIO_PinSource2
#define KEY_EXTI_LINE_PIN8	GPIO_PinSource3
#define KEY_EXTI_LINE_PIN9	GPIO_PinSource4
#define KEY_EXTI_RCC_PORT1	RCC_APB2Periph_GPIOD
#define KEY_EXTI_LINE1	EXTI_Line8
#define KEY_EXTI_LINE2	EXTI_Line9
#define KEY_EXTI_LINE3	EXTI_Line10
#define KEY_EXTI_LINE4	EXTI_Line11

#define KEY_EXTI_LINE5	EXTI_Line0
#define KEY_EXTI_LINE6	EXTI_Line1
#define KEY_EXTI_LINE7	EXTI_Line2
#define KEY_EXTI_LINE8	EXTI_Line3
#define KEY_EXTI_LINE9	EXTI_Line4

//rocker  
//按键    PC 14, 15 
#define ROCKER_KEY_EXTI_PIN1	GPIO_Pin_14	
#define ROCKER_KEY_EXTI_PIN2	GPIO_Pin_15	
#define ROCKER_KEY_EXTI_PORT1  GPIOC
#define ROCKER_KEY_EXTI_LINE_PORT1	GPIO_PortSourceGPIOC
#define ROCKER_KEY_EXTI_LINE_PIN1	GPIO_PinSource14
#define ROCKER_KEY_EXTI_LINE_PIN2	GPIO_PinSource15
#define ROCKER_KEY_EXTI_RCC_PORT	RCC_APB2Periph_GPIOC
#define ROCKER_KEY_EXTI_LINE1		EXTI_Line14
#define ROCKER_KEY_EXTI_LINE2		EXTI_Line15
//ADC  PC 0,1,2,3    ADC1->10,11,12,13
#define SMOKE_ADC_RCC_PORT	RCC_APB2Periph_GPIOC
#define SMOKE_ADC_PIN		GPIO_Pin_0
#define SMOKE_ADC_PORT		GPIOC
#define SMOKE_CHANNEL		ADC_Channel_10        // 10, 11, 12, 13

//DHT11
#define DHT11_CLK     RCC_APB2Periph_GPIOB
#define DHT11_PIN     GPIO_Pin_10                  
#define DHT11_PORT		GPIOB 

// PIR 人体红外热释电 
#define PIR_PIN		GPIO_Pin_4
#define PIR_EXTI_Line  EXTI_Line4
#define PIR_EXTI_LINE_PIN	GPIO_PinSource4
#define PIR_PORT	GPIOC
#define PIR_CLK		RCC_APB2Periph_GPIOC
#define PIR_EXTI_LINE_PORT	GPIO_PortSourceGPIOC

// SHOCK 震动传感器 
#define SHOCK_PIN		GPIO_Pin_2
#define SHOCK_EXTI_Line  EXTI_Line2
#define SHOCK_EXTI_LINE_PIN	GPIO_PinSource2
#define SHOCK_PORT	GPIOD
#define SHOCK_CLK		RCC_APB2Periph_GPIOD
#define SHOCK_EXTI_LINE_PORT	GPIO_PortSourceGPIOD

//FIRE 火焰探测 
#define FIRE_PIN	GPIO_Pin_2
#define FIRE_PORT	GPIOE
#define FIRE_CLK 	RCC_APB2Periph_GPIOE

// ultrasonic 超声波距离检测
//left
#define HC04_CLK 	RCC_APB2Periph_GPIOE
#define HC04_PORT 	GPIOE
#define HC04_TRG 	GPIO_Pin_7	
#define HC04_ECHO 	GPIO_Pin_8
#define Trg PEout(7)
#define ECHO PEin(8)
//right
#define HC04_CLK1 	RCC_APB2Periph_GPIOA
#define HC04_PORT1 	GPIOA
#define HC04_TRG1 	GPIO_Pin_6	
#define HC04_ECHO1 	GPIO_Pin_7
#define Trg1 PAout(6)
#define ECHO1 PAin(7)

//舵机云台
#define SG90_CLK  		RCC_APB2Periph_GPIOA
#define SG90_PORT 		GPIOA
#define SG90_UD_PIN		GPIO_Pin_2
#define SG90_LR_PIN  	GPIO_Pin_3
#define SG90_TIM  TIM2 

//前轮 PC 6789  TIM3
#define FORNT_CLK 	RCC_APB2Periph_GPIOC
#define FRONT_PORT  GPIOC
#define FRONT_PIN1  GPIO_Pin_6
#define FRONT_PIN2 	GPIO_Pin_7
#define FRONT_PIN3	GPIO_Pin_8
#define FRONT_PIN4	GPIO_Pin_9
#define FRONT_TIM	TIM3

//后轮 PB 6789   TIM4
#define REAR_CLK	RCC_APB2Periph_GPIOB
#define REAR_PORT 	GPIOB
#define REAR_PIN1	GPIO_Pin_6
#define REAR_PIN2	GPIO_Pin_7
#define REAR_PIN3	GPIO_Pin_8
#define REAR_PIN4	GPIO_Pin_9
#define REAR_TIM	TIM4

//巡线 
#define BFD_CLK1 	RCC_APB2Periph_GPIOE
#define BFD_PORT1 	GPIOE
#define BFD_PIN1	GPIO_Pin_9
#define BFD_PIN2	GPIO_Pin_10
#define BFD_PIN3	GPIO_Pin_11
#define BFD_PIN4	GPIO_Pin_12
#define BFD_PIN5	GPIO_Pin_13

#define BFD_CLK2	RCC_APB2Periph_GPIOB
#define BFD_PORT2 	GPIOB
#define BFD_PIN6 	GPIO_Pin_0
#define BFD_PIN7 	GPIO_Pin_1

// count from right 
#define light1  PEin(9)
#define light2  PEin(10)
#define light3  PEin(11)
#define light4  PEin(12)
#define light5  PEin(13)
#define CLP 	PBin(0)
#define NER 	PBin(1)
#define BLACK 1
#define WHITE 0


/*------------------------------------------------*/
typedef enum
{
	Dht11 = 0x30,   //字符0
	MQ_2,
	PIR,
	SHOCK,
	FIRE,
	L_ULTRA,
	R_ULTRA,
	FIND_LINE,
	SG90_UD,
	SG90_LR,
	
}_DEV;


typedef enum
{
	TEMP = 0x41,	// A
	HUMID,
	TTL,
	NORMAL,
	FLAG,
	
}_DATA_FLAG;

/*------------------------------------------------*/
//MQ_2 flag_data:  0 无， 1 甲烷， 2 丁烷， 3 乙醇 
//PIR  flag_data:  0 无人， 1 有人 
typedef struct
{
	float cnct_data;  
	u8 flag_data;  
	u8 reserve_data1;
	u8 reserve_data2;
}_data;

typedef struct 
{
	_dht11_type DHT11;
	_data PIR;
	_data MQ_2;
	_data SHOCK;
	_data FIRE;
	_data L_ULTRASONIC;
	_data R_ULTRASONIC;
	_data FindLine;
	_data SG90_UD;
	_data SG90_LR;
	
}_device;
extern _device device;


/*------------func declare -------------*/
void driver_init(void);
void data_init(void);
void get_data(void);

#endif 


