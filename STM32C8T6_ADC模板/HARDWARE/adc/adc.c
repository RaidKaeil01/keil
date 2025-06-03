#include "adc.h"
#include "delay.h"

void ADCx_Iint(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
    //1 使能端口时钟和 ADC 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
    
    //2 设置引脚模式为模拟输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //3 设置 ADC 的分频因子?
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
    //4 初始化 ADC 参数，包括 ADC 工作模式、规则序列等
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1,&ADC_InitStructure);
    
    //5 使能 ADC 并校准
    ADC_Cmd(ADC1,ENABLE);
    
    ADC_ResetCalibration(ADC1); //复位校准
    while(ADC_GetResetCalibrationStatus(ADC1));
    
    ADC_StartCalibration(ADC1);  //开启校准
    while(ADC_GetCalibrationStatus(ADC1));
    
    //6 软件开启
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

u16 Get_ADC_Value(u8 ch, u8 times)
{
	u8 i = 0;
	u32 Temp_val = 0;
    ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);
    
    for(i=0;i<=times;i++)
    {
        ADC_SoftwareStartConvCmd(ADC1,ENABLE);  //因为使用的是单次转换，所以每次循环后都要开启转换
        while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
        Temp_val += ADC_GetConversionValue(ADC1);
        delay_ms(5);
    }
    return Temp_val/times;
}








