/**
  ******************************************************************************
  * @file    usart.c
  * @author  abel li 
  * @version V1.0
  * @date    2017/1/11
  * @attention    http://www.chuxue123.com   http://www.openedv.com  
  * @brief  
	1. 串口波特率可配置   
	2. 支持printf, 添加串口接收命令功能, 修正printf第一个字符丢失的bug
	3. 修改了USART_RX_STA, 使得串口最大接收字节数为2的14次方
	4. 增加了USART_REC_LEN, 用于定义串口最大允许接收的字节数(不大于2的14次方)
	5. 修改了EN_USART1_RX的使能方式
	6. 支持 UCOSII
	7. 支持scanf函数到usart的重定向
  *****************************************************************************/ 
  
#include "usart.h"
#include "sys.h"
#include "driver.h"


/*使用microLib的方法*/
//重定向c库函数printf到USART1  库函数版
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到USART1 */
	USART_SendData(USART1, (uint8_t) ch);
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	return (ch);
}

//重定向c库函数scanf到USART1   库函数版
int fgetc(FILE *f)
{
	/* 等待串口1输入数据 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USART1);
}


static void usart_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(USART_RCC_PORT, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = USART_Tx_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART_Rx_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART_PORT, &GPIO_InitStructure);
}


static void usart_mode_init(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(USART_RCC_MODE_PORT, ENABLE);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART_NAME, &USART_InitStructure); 
	
	USART_ITConfig(USART_NAME, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART_NAME, ENABLE);
	
}


void usart_init(u32 baud)
{
	usart_gpio_init();
	usart_mode_init(baud);
}








