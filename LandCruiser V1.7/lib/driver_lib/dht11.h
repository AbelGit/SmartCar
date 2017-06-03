#ifndef __DHT11_H
#define	__DHT11_H

#include "stm32f10x.h"

#define HIGH  1
#define LOW   0


//带参宏，可以像内联函数一样使用,输出高电平或低电平
#define DHT11_DATA_OUT(a)	if (a)	\
					GPIO_SetBits(DHT11_PORT,DHT11_PIN);\
					else		\
					GPIO_ResetBits(DHT11_PORT,DHT11_PIN)
 //读取引脚的电平
#define  DHT11_DATA_IN()	   GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)


typedef struct
{
	u8  humi_int;		//湿度的整数部分
	u8  humi_deci;	 	//湿度的小数部分
	u8  temp_int;	 	//温度的整数部分
	u8  temp_deci;	 	//温度的小数部分
	u8  check_sum;	 	//校验和
}_dht11_type;

extern _dht11_type dht11_type;

void dht11_init(void);
u8 get_dht11_data(_dht11_type *DHT11_Data);

#endif /* __DHT11_H */







