/**
  ******************************************************************************
  * @file    usart.h
  * @author  abel li 
  * @version V1.0
  * @date    2017/1/11
  * @attention    http://www.chuxue123.com   http://www.openedv.com  
  * @brief  
	1. 
	2. 
	3. 
  *****************************************************************************/ 

#ifndef __USART_H
#define __USART_H


#include <stdio.h>

#include "stm32f10x.h"
#include "sys.h"


#define USART_REC_LEN  			50  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

//如果想串口中断接收，请不要注释以下宏定义
void usart_init(u32 baud);             	

#endif


