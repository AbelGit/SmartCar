
#include <stdio.h>
#include "stm32f10x.h"
#include "timer.h"
#include "distance.h"
#include "delay.h"
#include "motor.h"
#include "bluetooth.h"

u16 count = 0;    //left or right count flag 
u16 count1 = 0;
extern _device device;


static void hc04_left_gpio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;					
 	
	RCC_APB2PeriphClockCmd(HC04_CLK, ENABLE);	
	
	GPIO_InitStructure.GPIO_Pin = HC04_ECHO;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(HC04_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = HC04_TRG;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;					
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HC04_PORT, &GPIO_InitStructure);	
	
	GPIO_ResetBits(HC04_PORT, HC04_TRG |HC04_ECHO);
}

static void hc04_right_gpio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;					
 	
	RCC_APB2PeriphClockCmd(HC04_CLK1, ENABLE);	
	
	GPIO_InitStructure.GPIO_Pin = HC04_ECHO1;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(HC04_PORT1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = HC04_TRG1;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;					
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HC04_PORT1, &GPIO_InitStructure);	
	
	GPIO_ResetBits(HC04_PORT1, HC04_TRG1 |HC04_ECHO1);
}

static void hc04_gpio_init(void)
{
	hc04_left_gpio_init();
	hc04_right_gpio_init();
}

void hcsr04_init(void)
{
	hc04_gpio_init();
	TIM1_Config();
}

static void send_signal(void) //send 15ms high electric signal 										
{
	Trg = 1;
	delay_us(15);
	Trg = 0;
}

static void send_signal1(void) //send 15ms high electric signal 										
{
	Trg1 = 1;
	delay_us(15);
	Trg1 = 0;
}

//可以精确到0.3cm
void cal_left_distance(void)
{	
	u16 time = 0;

	send_signal();
	
	while(ECHO == 0);
	device.L_ULTRASONIC.flag_data = 1;
	while(ECHO == 1);
	device.L_ULTRASONIC.flag_data = 0;
	
	time = TIM_GetCounter(HC04_LEFT_TIM);
	time = time + (count * 10);  			//x us
	device.L_ULTRASONIC.cnct_data = (time * 0.00034) / 2 * 100;  //cm 
	
	HC04_LEFT_TIM->CNT = 0;											
	count = 0; 
}

void cal_right_distance(void)
{
	u16 time = 0;

	send_signal1();
	
	while(ECHO1 == 0);
	device.R_ULTRASONIC.flag_data = 1;
	while(ECHO1 == 1);
	device.R_ULTRASONIC.flag_data = 0;
	
	time = TIM_GetCounter(HC04_LEFT_TIM);
	time = time + (count1 * 10);  			//x us
	device.R_ULTRASONIC.cnct_data = (time * 0.00034) / 2 * 100;  //cm 
	
	HC04_LEFT_TIM->CNT = 0;											
	count1 = 0;   //标识left or right
}

void get_dis_data(void)
{
	cal_left_distance();  
	cal_right_distance();   
}

static void car_rotate_angle(int angle, int speed, int direction)
{
	//u16 rotate_speed = 800;
	//u16 turn_time = 570;  //45 C  
	//u16 turn_time = 1150;  //90 C  
	//u16 turn_time = 2340;  //180 C
	
	car_rotate(speed, speed-100, direction);
	delay_ms(13*angle);
	car_stop();
}

static void car_go_back_dis(int dis, int speed)
{
	//u16 back_speed = 400;
	//u16 back_time = 1000;  //20cm
	//u16 back_time = 2000;  //45cm
	
	car_go_back(speed, speed);
	delay_ms(50*dis);
	car_stop();
}
	

//void run_ultra(void)
//{
//	u16 go_speed = 400;
//	u16 back_speed = 400;
//	u16 rotate_speed = 800;

//	u16 rotate_angle = 15;
//	
//	float safe_dis = 60.0f;
//	float danger_dis = 30.0f;
//	float left = 0.0f;
//	float right = 0.0f;
//	
//	left = device.L_ULTRASONIC.cnct_data;
//	right = device.R_ULTRASONIC.cnct_data;

//	if(left >= safe_dis)
//	{
//		car_rotate_angle(rotate_angle, rotate_speed, 0);
//		car_go_forward(go_speed, go_speed);
//	}
//	else
//	{
//		if(right >= safe_dis)
//		{
//			car_rotate_angle(rotate_angle, rotate_speed, 1);
//			car_go_forward(go_speed, go_speed);
//		}else
//		{
//			if(left >= right)
//			{
//				car_rotate_angle(rotate_angle, rotate_speed, 0);
//				car_go_forward(go_speed, go_speed);
//			}else
//			{
//				car_rotate_angle(rotate_angle, rotate_speed, 1);
//				car_go_forward(go_speed, go_speed);
//			}
//			
//			if(left > danger_dis)
//			{
//				car_rotate_angle(rotate_angle, rotate_speed, 0);
//				car_go_forward(go_speed, go_speed);
//			}else
//			{
//				car_go_back_dis(60, back_speed);
//				car_rotate_angle(rotate_angle+150, rotate_speed, 1);
//				car_go_forward(go_speed, go_speed);
//			}
//			
//			if(right < danger_dis)
//			{
//				car_go_back_dis(60, back_speed);
//				car_rotate_angle(rotate_angle+150, rotate_speed, 0);
//				car_go_forward(go_speed, go_speed);
//			}else
//			{
//				car_rotate_angle(rotate_angle, rotate_speed, 1);
//				car_go_forward(go_speed, go_speed);
//			}
//		}
//	}
//}

//void run_ultra(void)
//{
//	u16 go_speed = 400;
//	u16 back_speed = 400;
//	u16 rotate_speed = 800;

//	u16 rotate_angle = 15;
//	
//	float safe_dis = 60.0f;
//	float danger_dis = 30.0f;
//	float left = 0.0f;
//	float right = 0.0f;
//	float dis = 0.0f;
//	
//	//left = device.L_ULTRASONIC.cnct_data;
//	dis = device.L_ULTRASONIC.cnct_data;
//	right = device.R_ULTRASONIC.cnct_data;
//		
//	if((0<dis) && (dis<100))
//	{
//			if(dis < 50)
//			{
//				car_stop ();
//				delay_ms(500);
//				car_go_back (back_speed, back_speed); 
//				delay_ms(1000); 
//				car_rotate (500, 800, 0); 
//				delay_ms(1000);
//				car_go_forward(go_speed, go_speed);	
//				delay_ms(250);
//			}else if(dis > 50)
//			{
//				car_stop ();
//				delay_ms(500); 
//				car_rotate (800, 500, 1);    //youzhuan
//				delay_ms(500); 
//				car_go_forward(go_speed, go_speed);
//				delay_ms(250); 
//			}
//	}
//	else
//	{
//		car_go_forward(go_speed, go_speed);
//	}
//}


void run_ultra(void)
{
	u16 go_speed = 400;
	u16 back_speed = 400;
	u16 rotate_speed = 800;

	u16 rotate_angle = 15;
	
	float safe_dis = 60.0f;
	float danger_dis = 30.0f;
	float left = 0.0f;
	float right = 0.0f;
	float dis = 0.0f;
	
	left = device.L_ULTRASONIC.cnct_data;
	right = device.R_ULTRASONIC.cnct_data;
	
	if((left<40) || (right<40))
	{
		car_go_back (back_speed, back_speed);
		if(left > right)
		{
			car_rotate (500, 800, 0);
			delay_ms(1000);
			car_go_forward(go_speed, go_speed);
			delay_ms(1000);
		}else
		{
			car_rotate (800, 500, 1);
			delay_ms(1000);
			car_go_forward(go_speed, go_speed);
			delay_ms(1000);
		}
				
	}else
	{
		car_go_forward(go_speed, go_speed);
	}
}






