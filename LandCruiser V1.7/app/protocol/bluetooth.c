
#include <stdio.h>
#include "stm32f10x.h"
#include "bluetooth.h"
#include "motor.h"
#include "delay.h"
#include "driver.h"
#include "distance.h"
#include "find_line.h"
#include "sg90.h"
#include "usart.h"
#include "delay.h"

#define DATA_SIZE 100

_protocol protocol;                      //接收协议使用 
extern u8 USART_RX_BUF[USART_REC_LEN];  



u8 temp_char_buf[DATA_SIZE] = "0";   //温湿度
u8 humi_char_buf[DATA_SIZE] = "0";
u8 mq2_char_buf[DATA_SIZE] = "0";    //烟雾
u8 mq2_char_buf2[DATA_SIZE] = "0";
u8 l_ultra_char_buf[DATA_SIZE] = "0";   //超声波
u8 r_ultra_char_buf[DATA_SIZE] = "0";
u8 pir_char_buf[DATA_SIZE] = "0";		//人体感应
u8 shock_char_buf[DATA_SIZE] = "0";		//震动
u8 fire_char_buf[DATA_SIZE] = "0";		//火灾

static void float_to_char(void)
{
	sprintf((char*)temp_char_buf, "%d.%d", device.DHT11.temp_int, device.DHT11.temp_deci);
	sprintf((char*)humi_char_buf, "%d.%d", device.DHT11.humi_int, device.DHT11.humi_deci);	
	sprintf((char*)mq2_char_buf, "%1.2f", device.MQ_2.cnct_data);
	sprintf((char*)mq2_char_buf2, "%d", device.MQ_2.flag_data);
	sprintf((char*)pir_char_buf, "%d", device.PIR.flag_data);
	sprintf((char*)shock_char_buf, "%d", device.SHOCK.flag_data);
	sprintf((char*)fire_char_buf, "%d", device.FIRE.flag_data);
	sprintf((char*)l_ultra_char_buf, "%1.2f", device.L_ULTRASONIC.cnct_data);
	sprintf((char*)r_ultra_char_buf, "%1.2f", device.R_ULTRASONIC.cnct_data);
}

/**
  * @brief  可变长协议
  * @param  dev_num: 设备数量，*dev: 设备，*data_flag: 数据标示，*data：数据首地址
  * @retval no
  */
void pack_data(u8 dev, u8 data_flag, u8 data_len, u8 *data)
{
	u8 tx_buf[DATA_SIZE] = {0};	
	
	tx_buf[0] = '*';
	tx_buf[1] = dev;
	tx_buf[2] = data_flag;
	if(data_len == 1)
	{
		tx_buf[3] = *data;
		tx_buf[4] = '#';
		tx_buf[5] = '\0';
	}else if(data_len == 4)
	{
		tx_buf[3] = *data;
		tx_buf[4] = *(data+1);
		tx_buf[5] = *(data+2);
		tx_buf[6] = *(data+3);
		tx_buf[7] = '#';
		tx_buf[8] = '\0';
	}
	
	printf("\r\n%s\r\n", (char *)tx_buf);
}

void send_data(void)
{	
	int delay_time = 40;
	float_to_char();
	
	//dht11
	pack_data(Dht11, TEMP, 4, temp_char_buf);
	delay_ms(delay_time);
	pack_data(Dht11, HUMID, 4, humi_char_buf);
	delay_ms(delay_time);
	//mq-2
	pack_data(MQ_2, NORMAL, 4, mq2_char_buf);
	delay_ms(delay_time);
	pack_data(MQ_2, FLAG, 1, mq2_char_buf2);
	delay_ms(delay_time);
	
	//pir 
	pack_data(PIR, TTL, 1, pir_char_buf);
	delay_ms(delay_time);
	
	//shock
	pack_data(SHOCK, TTL, 1, shock_char_buf);
	delay_ms(delay_time);
	
	//fire
	pack_data(FIRE, TTL, 1, fire_char_buf);
	delay_ms(delay_time);
	
	//ultra
	pack_data(L_ULTRA, NORMAL, 4, l_ultra_char_buf);
	delay_ms(delay_time);
	pack_data(R_ULTRA, NORMAL, 4, r_ultra_char_buf);
	delay_ms(delay_time);
}


/*
接收协议：
	u8 start;
	u8 mode;
	u8 cmd;
	u8 time;
	u8 data_l;
	u8 data_h;
	u8 end;
*/

/**
  * @brief  串口接收到数据后，先对数据缓冲放到protocol中，以便于一次性解析。
  * @param  no
  * @retval no
  */
static void fill_protocol(void)
{
	u8 len = 0;
	u8 t = 0;
	
	if(USART_RX_STA & 0x8000)
	{					   
		len = USART_RX_STA&0x3fff;    //得到此次接收到的数据长度
		for(t = 0; t < len; t++)
		{
			USART_SendData(USART1, USART_RX_BUF[t]);         //向串口1发送数据
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
		}
		USART_RX_STA = 0;

		protocol.start = USART_RX_BUF[0];
		protocol.mode = USART_RX_BUF[1];
		protocol.cmd = USART_RX_BUF[2];
		protocol.time = USART_RX_BUF[3];
		protocol.data_l = USART_RX_BUF[4];
		protocol.data_h = USART_RX_BUF[5];
		protocol.end = USART_RX_BUF[6];
	}
}

void parse_recv_data(const _protocol *data)
{
	fill_protocol();
	
	switch(data->mode)
	{
		case Mannual: run_mannual(data); break;
		case Auto: run_auto(data); break;
		case FindLine: run_find_line(data); break;
		default: run_empty();
	}
}

void run_mannual(const _protocol *data)
{
	int forward_speed = 450;
	int back_speed = 450;
	int turn_speed = 600;
	int rotate_speed = 600;
	
	switch(data->cmd)
	{
		case Empty: run_empty(); break;
		case Start: car_start(400 + data->data_l); break;
		case Stop: car_stop(); break;
		case Forward: car_go_forward(forward_speed+data->data_l, forward_speed+data->data_l);  break;
		case Backward: car_go_back(back_speed+data->data_l, back_speed+data->data_l);  break;
		case TurnLeft: car_turn(turn_speed-data->data_l, turn_speed+data->data_l);  break; //data[0] 左侧spd, 右侧spd
		case TurnRight: car_turn(turn_speed+data->data_l, turn_speed-data->data_l);  break; //data[0] 左侧spd, 右侧spd
		case LeftRotate: car_rotate(rotate_speed-data->data_l, rotate_speed+data->data_l, 0); break;
		case RightRotate: car_rotate(rotate_speed+data->data_l, rotate_speed-data->data_l, 1); break;
		
		case Sg90UpDown: sg90_up_down(data->data_l); break;
		case Sg90LeftRight: sg90_left_right(data->data_l); break;
		case Sg90Reset: sg90_reset(80, 100);
		default:  run_empty();
	}
}
void run_auto(const _protocol *data)
{
	switch(data->cmd)
	{
		case Empty: run_empty(); break;
		case Start: 
		{
			run_ultra(); break;
		}
		case Stop: 
		{
			car_stop(); break;
		}
		default: run_empty(); 
	}
}

void run_find_line(const _protocol *data)
{
	switch(data->cmd)
	{
		case Empty: run_empty(); break;
		case Start: run_bfd1000(); break;
		case Stop: car_stop(); break;
		default: run_empty(); 
	}
}


void run_empty(void)
{}







