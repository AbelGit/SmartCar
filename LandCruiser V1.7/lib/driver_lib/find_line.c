
#include <stdio.h>
#include "stm32f10x.h"
#include "find_line.h"
#include "sys.h"
#include "driver.h"


void bfd1000_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(BFD_CLK1, ENABLE);
	RCC_APB2PeriphClockCmd(BFD_CLK2, ENABLE);		

	GPIO_InitStructure.GPIO_Pin =  BFD_PIN1 | BFD_PIN2 | BFD_PIN3 | BFD_PIN4 | BFD_PIN5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(BFD_PORT1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  BFD_PIN6 | BFD_PIN7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(BFD_PORT2, &GPIO_InitStructure);
}

void Process_One_Black(void)
{
	if(light1 == BLACK )
	{
		//turn right 
		
	}
	
	if(light2 == BLACK)
	{
		//ture right
		
	}
	
	if(light3 == BLACK)
	{
		//go forward

	}
	
	if(light4 == BLACK)
	{
		//turn left 
	
	}
	
	if(light5 == BLACK)
	{
		//ture left
		
	}
}

void Process_Two_Black(void)
{
	if((light5 == BLACK) && (light4 == BLACK) && (light2 == WHITE) && (light1 == WHITE))
	{
		//ture left
	}
}

void Process_Three_Black(void)
{
}

void Process_Others(void)
{
	if((light5 == WHITE) && (light4 == WHITE) && (light1 == WHITE) && (light2 == WHITE) && (light3 == WHITE))
	{
		  //stop 
	}
	if((light5 == BLACK) && (light4 == BLACK) && (light2 == BLACK) && (light3 == BLACK))
	{
		//go forward
	}
}


void get_bfd1000_data(void)
{
	u8 light_num = 0;
	
	light_num = light1 + light2 +  light3 + light4 + light5;
	
	if(light_num == 1)
	{
		Process_One_Black();
	}
	else if(light_num == 2)
	{
		Process_Two_Black();
	}
	else if(light_num == 3)
	{
		Process_Three_Black();
	}
	else
	{
		Process_Others();
	}
}

void run_bfd1000(void)
{
	
}



























