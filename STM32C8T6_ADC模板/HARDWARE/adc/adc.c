#include "adc.h"
#include "delay.h"

void ADCx_Iint(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
    //1 ʹ�ܶ˿�ʱ�Ӻ� ADC ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
    
    //2 ��������ģʽΪģ������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //3 ���� ADC �ķ�Ƶ����?
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
    //4 ��ʼ�� ADC ���������� ADC ����ģʽ���������е�
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1,&ADC_InitStructure);
    
    //5 ʹ�� ADC ��У׼
    ADC_Cmd(ADC1,ENABLE);
    
    ADC_ResetCalibration(ADC1); //��λУ׼
    while(ADC_GetResetCalibrationStatus(ADC1));
    
    ADC_StartCalibration(ADC1);  //����У׼
    while(ADC_GetCalibrationStatus(ADC1));
    
    //6 �������
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

u16 Get_ADC_Value(u8 ch, u8 times)
{
	u8 i = 0;
	u32 Temp_val = 0;
    ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);
    
    for(i=0;i<=times;i++)
    {
        ADC_SoftwareStartConvCmd(ADC1,ENABLE);  //��Ϊʹ�õ��ǵ���ת��������ÿ��ѭ����Ҫ����ת��
        while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
        Temp_val += ADC_GetConversionValue(ADC1);
        delay_ms(5);
    }
    return Temp_val/times;
}








