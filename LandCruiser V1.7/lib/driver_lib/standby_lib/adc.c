/**
  * @brief  smoke_che_10   
  1. ��������MQ2 �Բ�ͬŨ�Ȳ�ͬ����������в�ͬ�ĵ���ֵ����ʹ��ʱ�����ȵĵ�������Ҫ��
  2. ����ʹ��1000ppm ��������1000ppm����У׼����ȷ����ʱ��������趨Ӧע����ʪ�ȵ�Ӱ�졣  
  3. �ϵ�20s�ſ���׼ȷ������
  4. Һ���������飬���飬���飬�ƾ�������������
  5. 100-2000 �ƾ����Ҵ����� 300-5000ppm ����/����  5000-20000ppm����    ������ ����3296
  6. if(smoke_val > 5000) ����   else if(smoke_val > 2000 && smoke_val < 5000) ����  else �ƾ� 
  **/  
  
  /**
  * @brief  
  **/  
  
  /**
  * @brief  
  **/  
  
  /**
  * @brief  
  **/  


#include <stdio.h>
#include "stm32f10x.h"
#include "adc.h"
#include "driver.h"
#include "delay.h"


#define CH_NUM  4           //Ҫʹ�õ�ADCͨ����
#define COUNT 30
#define N 30   //�˲���������

static u16 ad_value[COUNT][CH_NUM]={0};  //AD ����ֵ
static u16 after_filter[4] = {0};



//-----------------------------------����--------------------------------------- 

static void Rocker_EXTI_Key_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = ROCKER_KEY_EXTI_PIN1 | ROCKER_KEY_EXTI_PIN2;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	  //key�Ѿ����� ������IPD
	GPIO_Init(ROCKER_KEY_EXTI_PORT1, &GPIO_InitStructure);

	GPIO_EXTILineConfig(ROCKER_KEY_EXTI_LINE_PORT1, ROCKER_KEY_EXTI_LINE_PIN1);  
	GPIO_EXTILineConfig(ROCKER_KEY_EXTI_LINE_PORT1, ROCKER_KEY_EXTI_LINE_PIN2);  
}

void Rocker_EXTI_Key_init(void)
{	
	EXTI_InitTypeDef EXTI_InitStructure;												

	RCC_APB2PeriphClockCmd(ROCKER_KEY_EXTI_RCC_PORT | RCC_APB2Periph_AFIO, ENABLE);	
	
  	Rocker_EXTI_Key_GPIO_Configuration();

	EXTI_InitStructure.EXTI_Line = ROCKER_KEY_EXTI_LINE1 | ROCKER_KEY_EXTI_LINE2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;               
	
	EXTI_Init(&EXTI_InitStructure); 
}

// ----------------------------------ҡ�� --------------------------------------

//ͬʱ��ȡ4��ͨ��������
//PC 0,1,2,3  ADC1_IN10 / ADC1_IN11 / ADC1_IN12 / ADC1_IN13

/** @date			2013/8/28
    @author			Mei Jilin
	@brief		    ADC1���������3ͨ�������ݲɼ�
    @attention
       1 ----------- adc_config();                     //����adc
       2 ----------- adc_start();                      //����adc,��ʼת��
       3 ----------- get_ChannelValue(ADC_Channel_x);  //��ȡ��Ӧͨ����adcֵ
**/

/** @para none
  * @ret  none
  * @brief adc��Ӧ��gpio��ʼ��
  */
static void rocker_adc_gpio_init(void)
{
    GPIO_InitTypeDef gpio;
    RCC_APB2PeriphClockCmd(SMOKE_ADC_RCC_PORT, ENABLE);
    gpio.GPIO_Mode = GPIO_Mode_AIN;                         //����Ϊģ������
    gpio.GPIO_Pin = SMOKE_ADC_PIN;   
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SMOKE_ADC_PORT, &gpio); 
}



/** @para none
  * @ret  none
  * @brief ����dma
  */
static void rocker_adc_dma_init()
{
    DMA_InitTypeDef dma;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    DMA_DeInit(DMA1_Channel1);
    dma.DMA_PeripheralBaseAddr  = (u32)&(ADC1->DR);     //DMA��Ӧ���������ַ
    dma.DMA_MemoryBaseAddr      = (u32)&ad_value;       //�ڴ�洢����ַ
    dma.DMA_DIR                 = DMA_DIR_PeripheralSRC;//DMA��ת��ģʽΪSRCģʽ����������Ƶ��ڴ�
    dma.DMA_M2M                 = DMA_M2M_Disable;      //M2Mģʽ����
    dma.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;//�����������ݿ��Ϊ16λ
    dma.DMA_MemoryDataSize      = DMA_MemoryDataSize_HalfWord;  //DMA�������ݳߴ磬HalfWord����Ϊ16λ    
    dma.DMA_BufferSize          = 120;                       //DMA�����СCH_NUM��
    dma.DMA_MemoryInc           = DMA_MemoryInc_Enable;         //����һ�����ݺ�Ŀ���ڴ��ַ����
    dma.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;    //����һ�����ݺ��豸��ַ��ֹ����
    dma.DMA_Mode                = DMA_Mode_Circular;            //ת��ģʽ��ѭ������ģʽ��
    dma.DMA_Priority            = DMA_Priority_High;            //DMA���ȼ���
    DMA_Init(DMA1_Channel1, &dma); 
	DMA_Cmd(DMA1_Channel1,ENABLE);

	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); //DMA��������ж�
}


/** @para none
  * @ret  none
  * @brief adc����
  */
static void rocker_adc_config(void)
{
    ADC_InitTypeDef adc;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);                       //����adʱ��Ϊ72/6=12M,ad���ʱ��Ƶ��Ϊ14M
    
    ADC_DeInit(ADC1);
    adc.ADC_Mode                = ADC_Mode_Independent;     //�����ڶ���ģʽ
    adc.ADC_ContinuousConvMode  = ENABLE;                   //����ת��ģʽ
    adc.ADC_DataAlign           = ADC_DataAlign_Right;      //�����Ҷ���
    adc.ADC_ExternalTrigConv    = ADC_ExternalTrigConv_None;//��ʹ���ⲿ����
    adc.ADC_NbrOfChannel        = CH_NUM ;                       //ת��4��ͨ��
    adc.ADC_ScanConvMode        = ENABLE;                   //����ת��ģʽ
    ADC_Init(ADC1,&adc);
    //�趨ָ����ADCͨ����,����ת��˳��Ͳ���ʱ��
    ADC_RegularChannelConfig(ADC1, SMOKE_CHANNEL, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ROCKER_ADC_CHANNEL2, 2, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ROCKER_ADC_CHANNEL3, 3, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ROCKER_ADC_CHANNEL4, 4, ADC_SampleTime_239Cycles5);
    
	//����adc��DMA
    ADC_DMACmd(ADC1,ENABLE);
    //ʹ��ADC1
    ADC_Cmd(ADC1, ENABLE);
    /* Enable ADC1 reset calibaration register */   
    ADC_ResetCalibration(ADC1);	  //����У׼
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));  //�ȴ�����У׼���
    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);		//��ʼУ׼
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));	   //�ȴ�У׼���

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//����ת����ʼ��ADCͨ��DMA��ʽ���ϵĸ���RAM���� 
}


/** @para none
  * @ret  none
  * @brief ���adc����
  */
void adc_init(void)
{
    rocker_adc_gpio_init();
    rocker_adc_dma_init();
    rocker_adc_config();
}

//---------------------------DMA�ж��ж��ж������˲� ---------------------------
static void filter(void)  //��ֵ�˲� 
{
	int sum = 0;
	u8 count, i;
	for(i = 0; i < 4; i++)
	{
		for (count = 0; count < N; count++)
		{
			sum += ad_value[count][i];
		}
		after_filter[i] = sum/N;
		sum = 0;
	}
}

void DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)
	{
		filter();
		DMA_ClearITPendingBit(DMA1_IT_TC1);
	}
}	


//--------------------------------�˲�֮�������--------------------------------
/** @para ADC_Channel --- (ADC_Channel0 ~ ADC_Channel17)
  * @ret  ADC_Channel_x ��ADCֵ   rocker ��ȡ��flaot���� %1.1f�ſ���ʹ�� 
  */
static float get_adc_val(u8 channel)
{
	float ret = 0.0f;

    switch(channel)
    {
        case SMOKE_CHANNEL: ret = (float)3300 * after_filter[0]/4096;  break;  //max: 4095   min: 0   armed  
        case ROCKER_ADC_CHANNEL2: ret = (float)3.3 * after_filter[1]/4096;  break;  //max: 4095   min: 0   yaw
        case ROCKER_ADC_CHANNEL3: ret = (float)3.3 * after_filter[2]/4096;  break;  //max: 4095   min: 0   pitch
		case ROCKER_ADC_CHANNEL4: ret = (float)3.3 * after_filter[3]/4096;  break;  //max: 4095   min: 0   roll
    }
    return ret;
}

// ��ͨ���ɼ�������ݽ��н����ͷ��� 
// smoke data: smoke_val, device.smoke 
void get_mq2_data(void)
{
	float smoke_val = 0.0f;
	
	/*smoke data*/
	smoke_val = get_adc_val(SMOKE_CHANNEL);
	if(smoke_val > 5000)
	{
		device.MQ_2.flag_data = 1;  //����
		
	}else if((smoke_val > 2000) && (smoke_val < 5000))
	{
		device.MQ_2.flag_data = 2; //���� 
	}else if((smoke_val > 500) && (smoke_val < 2000))
	{
		device.MQ_2.flag_data = 3; //�ƾ� 
	}else
	{
		device.MQ_2.flag_data = 0; //��  
	}
	
	device.MQ_2.cnct_data = smoke_val;
	
}

























