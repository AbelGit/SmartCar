
#include <stdio.h>

#include "stm32f10x.h"
#include "shock.h"
#include "driver.h"

extern _device device;

static void shock_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(SHOCK_CLK, ENABLE);	

  	GPIO_InitStructure.GPIO_Pin = SHOCK_PIN;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //待续
  	GPIO_Init(SHOCK_PORT, &GPIO_InitStructure);
}

void shock_init(void)
{
	shock_gpio_init();
}

void get_shock_data(void)
{
	if(RESET == GPIO_ReadInputDataBit(SHOCK_PORT, SHOCK_PIN))
	{
		device.SHOCK.flag_data = 1;  //产生震动 	
	}
}

