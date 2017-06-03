
#include <stdio.h>
#include "stm32f10x.h"
#include "fire.h"
#include "driver.h"


extern _device device;

static void fire_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(FIRE_CLK, ENABLE);	

  	GPIO_InitStructure.GPIO_Pin = FIRE_PIN;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
  	GPIO_Init(FIRE_PORT, &GPIO_InitStructure);
}

void fire_init(void)
{
	fire_gpio_init();
}

void get_fire_data(void)
{
	if(RESET == GPIO_ReadInputDataBit(FIRE_PORT, FIRE_PIN))
	{
		device.FIRE.flag_data = 1;
	}else
	{
		device.FIRE.flag_data = 0;
	}
}











