
/**
  ******************************************************************************
  * @file    main.c
  * @author  Iron
  * @version V1.0
  * @date    2017/3/2
  * @attention    http://www.chuxue123.com   http://www.openedv.com  
  * @brief  
	
  *****************************************************************************/ 
#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "driver.h"
#include "delay.h"
#include "adc.h"
#include "dht11.h"
#include "distance.h"
#include "bluetooth.h"
#include "sg90.h"
#include "usart.h"
#include "motor.h"

extern _device device;
extern _protocol protocol; 


int main(void)
{
	driver_init();  
	data_init();
	
	while(1)
	{
		get_data();  // 采集数据并上报 
		parse_recv_data(&protocol);  //解析收到的data
	}
}













