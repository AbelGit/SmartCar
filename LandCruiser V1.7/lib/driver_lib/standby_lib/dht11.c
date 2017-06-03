
#include "stm32f10x.h"
#include "delay.h"
#include "dht11.h"
#include "usart.h"

DHT11_Data_TypeDef DHT11_Data;

/*
 * ��������DHT11_GPIO_Config
 * ����  ������DHT11�õ���I/O��
 * ����  ����
 * ���  ����
 */
void dht11_init(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(DHT11_CLK, ENABLE); 

  	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //ͨ���������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

  	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);		  

	GPIO_SetBits(DHT11_PORT, DHT11_PIN);	 
}

/*
 * ��������DHT11_Mode_IPU
 * ����  ��ʹDHT11-DATA���ű�Ϊ��������ģʽ
 * ����  ����
 * ���  ����
 */
static void DHT11_Mode_IPU(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;

	  GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;  /*��������ģʽΪ��������ģʽ*/ 

	  GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	 
}

/*
 * ��������DHT11_Mode_Out_PP
 * ����  ��ʹDHT11-DATA���ű�Ϊ�������ģʽ
 * ����  ����
 * ���  ����
 */
static void DHT11_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

  	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	 	 
}

/* 
 * ��DHT11��ȡһ���ֽڣ�MSB����
 */
static uint8_t Read_Byte(void)
{
	uint8_t i, temp=0;

	for(i=0;i<8;i++)    
	{	 
		/*ÿbit��50us�͵�ƽ���ÿ�ʼ����ѯֱ���ӻ����� ��50us �͵�ƽ ����*/  
		while(DHT11_DATA_IN()==Bit_RESET);

		/*DHT11 ��26~28us�ĸߵ�ƽ��ʾ��0������70us�ߵ�ƽ��ʾ��1����
		 *ͨ����� x us��ĵ�ƽ��������������״ ��x ���������ʱ 
		 */
		delay_us(40); //��ʱx us �����ʱ��Ҫ��������0������ʱ�伴��	   	  

		if(DHT11_DATA_IN()==Bit_SET)/* x us����Ϊ�ߵ�ƽ��ʾ���ݡ�1�� */
		{
			/* �ȴ�����1�ĸߵ�ƽ���� */
			while(DHT11_DATA_IN()==Bit_SET);

			temp|=(uint8_t)(0x01<<(7-i));  //�ѵ�7-iλ��1��MSB���� 
		}
		else	 // x us��Ϊ�͵�ƽ��ʾ���ݡ�0��
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); //�ѵ�7-iλ��0��MSB����
		}
	}
	return temp;
}
/*
 * һ�����������ݴ���Ϊ40bit����λ�ȳ�
 * 8bit ʪ������ + 8bit ʪ��С�� + 8bit �¶����� + 8bit �¶�С�� + 8bit У��� 
 */
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{  
	DHT11_Mode_Out_PP();  /*���ģʽ*/
	DHT11_DATA_OUT(LOW);  /*��������*/
	delay_ms(18);  /*��ʱ18ms*/
	
	DHT11_DATA_OUT(HIGH);   /*�������� ������ʱ30us*/
	delay_us(30);   //��ʱ30us

	DHT11_Mode_IPU();  /*������Ϊ���� �жϴӻ���Ӧ�ź�*/ 

	/*�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������*/   
	if(DHT11_DATA_IN()==Bit_SET)     
	{
		while(DHT11_DATA_IN()==Bit_RESET);  /*��ѯֱ���ӻ����� ��80us �͵�ƽ ��Ӧ�źŽ���*/  

		while(DHT11_DATA_IN()==Bit_SET);  /*��ѯֱ���ӻ������� 80us �ߵ�ƽ �����źŽ���*/

		DHT11_Data->humi_int= Read_Byte();  /*��ʼ��������*/   
		DHT11_Data->humi_deci= Read_Byte();
		DHT11_Data->temp_int= Read_Byte();
		DHT11_Data->temp_deci= Read_Byte();
		DHT11_Data->check_sum= Read_Byte();

		DHT11_Mode_Out_PP();  /*��ȡ���������Ÿ�Ϊ���ģʽ*/
		DHT11_DATA_OUT(HIGH);  /*��������*/
		
		/*����ȡ�������Ƿ���ȷ*/
		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)
			return SUCCESS;
		else 
			return ERROR;
	}
	else
	{		
		return ERROR;
	}   
}



