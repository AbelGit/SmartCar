#ifndef __DHT11_H
#define	__DHT11_H

#include "stm32f10x.h"

#define HIGH  1
#define LOW   0


//���κ꣬��������������һ��ʹ��,����ߵ�ƽ��͵�ƽ
#define DHT11_DATA_OUT(a)	if (a)	\
					GPIO_SetBits(DHT11_PORT,DHT11_PIN);\
					else		\
					GPIO_ResetBits(DHT11_PORT,DHT11_PIN)
 //��ȡ���ŵĵ�ƽ
#define  DHT11_DATA_IN()	   GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)


typedef struct
{
	u8  humi_int;		//ʪ�ȵ���������
	u8  humi_deci;	 	//ʪ�ȵ�С������
	u8  temp_int;	 	//�¶ȵ���������
	u8  temp_deci;	 	//�¶ȵ�С������
	u8  check_sum;	 	//У���
}_dht11_type;

extern _dht11_type dht11_type;

void dht11_init(void);
u8 get_dht11_data(_dht11_type *DHT11_Data);

#endif /* __DHT11_H */







