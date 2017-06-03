

//所有关于驱动文件的宏替换，硬件的初始化，都在driver.c / driver.h  文件中。
#include <stdio.h>

#include "stm32f10x_it.h"
#include "stm32f10x.h"
#include "driver.h"
#include "usart.h"
#include "delay.h"
#include "adc.h"
#include "dht11.h"
#include "timer.h"
#include "pir.h"
#include "shock.h"
#include "fire.h"
#include "distance.h"
#include "find_line.h"
#include "motor.h"
#include "bluetooth.h"
#include "sg90.h"

_device device;

//驱动初始化
void driver_init(void)
{
	Land_Nvic_Init(); 
	usart_init(115200); 		
	delay_init(72);
		
	motor_init();
	ADC_Configuration();
	dht11_init();
	pir_init();
	shock_init();
	fire_init();	
	hcsr04_init();
	bfd1000_init();
	
	TIM2_Config();
	sg90_reset(80, 100);  //监控复位 	
}

void data_init(void)
{
	device.DHT11.temp_int = 0;
	device.DHT11.temp_deci = 0;
	device.DHT11.humi_int = 0;
	device.DHT11.humi_deci = 0;
	
	device.MQ_2.flag_data = 0;
	device.MQ_2.cnct_data = 0;
	
	device.PIR.flag_data = 0;
	
	device.SHOCK.flag_data = 0;
	
	device.FIRE.flag_data = 0;
	
	device.L_ULTRASONIC.flag_data = 0;
	device.L_ULTRASONIC.cnct_data = 0;
	
	device.R_ULTRASONIC.flag_data = 0;
	device.R_ULTRASONIC.cnct_data = 0;
	
	
}

void get_data(void)
{
	/*-------sensor data---*/
	get_mq2_data();    /*MQ-2*/
//	printf("\r\n smoke_val: %f\r\n", device.MQ_2.cnct_data);   
	
	get_dht11_data(&device.DHT11);    /*Temperature*/
//	printf("\r\n temp: %d\r\n", device.DHT11.temp_int);
	
	//pir 数据交由中断服务函数处理了 
//	printf("\r\n Have people...%d\r\n", device.PIR.flag_data);   /*people detection*/
	
	get_shock_data();    /*earchquake detection*/
//	printf("\r\n %d\r\n", device.SHOCK.flag_data); 

	get_fire_data();  /*fire detection*/
//	printf("\r\n %d\r\n", device.FIRE.flag_data); 
	
	get_dis_data();   /*ultrasonic data*/
//	printf("\r\n left:%f, right:%f\r\n", device.L_ULTRASONIC.cnct_data, device.R_ULTRASONIC.cnct_data);
	
	get_bfd1000_data();  /* trace line */
	
	send_data();
}






















