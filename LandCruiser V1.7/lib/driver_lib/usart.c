/**
  ******************************************************************************
  * @file    usart.c
  * @author  abel li 
  * @version V1.0
  * @date    2017/1/11
  * @attention    http://www.chuxue123.com   http://www.openedv.com  
  * @brief  
	1. ���ڲ����ʿ�����   
	2. ֧��printf, ��Ӵ��ڽ��������, ����printf��һ���ַ���ʧ��bug
	3. �޸���USART_RX_STA, ʹ�ô����������ֽ���Ϊ2��14�η�
	4. ������USART_REC_LEN, ���ڶ��崮�����������յ��ֽ���(������2��14�η�)
	5. �޸���EN_USART1_RX��ʹ�ܷ�ʽ
	6. ֧�� UCOSII
	7. ֧��scanf������usart���ض���
  *****************************************************************************/ 
  
#include "usart.h"
#include "sys.h"
#include "driver.h"


/*ʹ��microLib�ķ���*/
//�ض���c�⺯��printf��USART1  �⺯����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ�USART1 */
	USART_SendData(USART1, (uint8_t) ch);
	/* �ȴ�������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	return (ch);
}

//�ض���c�⺯��scanf��USART1   �⺯����
int fgetc(FILE *f)
{
	/* �ȴ�����1�������� */
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
	
	USART_ITConfig(USART_NAME, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART_NAME, ENABLE);
	
}


void usart_init(u32 baud)
{
	usart_gpio_init();
	usart_mode_init(baud);
}








