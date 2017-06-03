/* ADC ת������ */
#include "adc.h"
#include "stm32f10x.h"
#include "driver.h"
#include "delay.h"

static void ADC1_RCC_Configuration(void)
{	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | SMOKE_ADC_RCC_PORT, ENABLE);
}

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = SMOKE_ADC_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(SMOKE_ADC_PORT, &GPIO_InitStructure);
}

void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	/*ADC1��GPIOʱ�ӳ�ʼ��*/
	ADC1_RCC_Configuration();
	/*GPIOģʽ����*/
	GPIO_Configuration();
	
	/* ADC1 ��ʼ������ ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;    // ADC����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;         //��ֹʹ��ɨ��ת��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;    //ʹ������ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;    //��ʹ���ⲿ����
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   //�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;      //���õ�ADCͨ����Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ADC1 ͨ��������ʱ�����ã�PB.0ʹ��ADCͨ��8 */ 
	ADC_RegularChannelConfig(ADC1, SMOKE_CHANNEL, 1, ADC_SampleTime_239Cycles5);
  
	/* ʹ�� ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* ʹ�� ADC1 ��λУ׼�Ĵ��� */  
	ADC_ResetCalibration(ADC1);
	/* �ȴ�ADC1 ��λУ׼�Ĵ�����λ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* ���� ADC1 У׼ */
	ADC_StartCalibration(ADC1);
	/* �ȴ� ADC1 У׼��� */
	while(ADC_GetCalibrationStatus(ADC1));
     
	/* ���� ADC1 �������ת�� */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
	

//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
static u16 Get_Adc_Average(u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val += ADC_GetConversionValue(ADC1);
		delay_ms(5);
	}
	return temp_val/times;
} 
	 

static float get_adc_val(u8 channel)
{
	float ret = 0.0f;

	
    switch(channel)
    {
        case SMOKE_CHANNEL: ret = (float)3300 * Get_Adc_Average(30)/4096;  break;  //max: 4095   min: 0   armed  
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




