/* ADC 转换驱动 */
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
	/*ADC1及GPIO时钟初始化*/
	ADC1_RCC_Configuration();
	/*GPIO模式配置*/
	GPIO_Configuration();
	
	/* ADC1 初始化配置 ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;    // ADC独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;         //禁止使用扫描转换模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;    //使能连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;    //不使用外部触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   //数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;      //启用的ADC通道数目
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ADC1 通道及采样时间配置，PB.0使用ADC通道8 */ 
	ADC_RegularChannelConfig(ADC1, SMOKE_CHANNEL, 1, ADC_SampleTime_239Cycles5);
  
	/* 使能 ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* 使能 ADC1 复位校准寄存器 */  
	ADC_ResetCalibration(ADC1);
	/* 等待ADC1 复位校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* 启动 ADC1 校准 */
	ADC_StartCalibration(ADC1);
	/* 等待 ADC1 校准完成 */
	while(ADC_GetCalibrationStatus(ADC1));
     
	/* 启动 ADC1 软件触发转换 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
	

//获取通道ch的转换值，取times次,然后平均 
//times:获取次数
//返回值:通道ch的times次转换结果平均值
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

// 对通道采集后的数据进行解析和分析 
// smoke data: smoke_val, device.smoke 
void get_mq2_data(void)
{
	float smoke_val = 0.0f;
	
	/*smoke data*/
	smoke_val = get_adc_val(SMOKE_CHANNEL);
	if(smoke_val > 5000)
	{
		device.MQ_2.flag_data = 1;  //甲烷
		
	}else if((smoke_val > 2000) && (smoke_val < 5000))
	{
		device.MQ_2.flag_data = 2; //丁烷 
	}else if((smoke_val > 500) && (smoke_val < 2000))
	{
		device.MQ_2.flag_data = 3; //酒精 
	}else
	{
		device.MQ_2.flag_data = 0; //无  
	}
	device.MQ_2.cnct_data = smoke_val;	
}




