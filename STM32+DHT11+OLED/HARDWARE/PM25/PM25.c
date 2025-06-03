#include "PM25.h"

/**
  * @brief  PM25��ʼ������
  * @param  ��
  * @retval ��
  */

void PM25_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd (PM25_GPIO_CLK, ENABLE );					// �� ADC IO�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = PM25_LED_GPIO_PIN;					// ���� ADC IO ����ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;					// ����Ϊ�������
	
	GPIO_Init(PM25_LED_GPIO_PORT, &GPIO_InitStructure);				// ��ʼ�� ADC IO
	
	//PA1��Ϊģ��ͨ���������� 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	
	GPIO_InitStructure.GPIO_Pin = PM25_VO_GPIO_PIN;
	GPIO_Init(PM25_VO_GPIO_PORT, &GPIO_InitStructure);
	
	ADCx_Init();
	
}
	
uint16_t PM25_ADC_Read(void)
{
	//����ָ��ADC�Ĺ�����ͨ��������ʱ��
	return ADC_GetValue(ADC_CHANNEL, ADC_SampleTime_239Cycles5);
}


/**
  * @brief  PM25 ADC���ݶ�ȡ
  * @param  ��
  * @retval ����ת�����PM25ģ���ź���ֵ
  */
	
u16 PM25_GetData(void)
{
	uint16_t ADCVal;
	int dustVal = 0;
	float Voltage;

	PM25_LED_H;	//��1  �����ڲ�LED
	delay_us(280); 	// ����LED���280us�ĵȴ�ʱ��
	ADCVal = PM25_ADC_Read();  //PA1 ��������ȡADֵ
	delay_us(19);			  //��ʱ19us����Ϊ����AD����������Ϊ239.5������ADת��һ�����ʱ21us��19��21�ټ�280�պ���320us
	PM25_LED_L;	//��0  �ر��ڲ�LED
	delay_us(9680);			//��Ҫ�����0.32ms/10ms��PWM�ź������������е�LED
	
	Voltage = 3.3f * ADCVal / 4096.f * 2; //���AO����ڵĵ�ѹֵ
	
	dustVal = (0.17*Voltage-0.1)*1000;  //����1000��λ����ug/m3//

	if (dustVal < 0)
		dustVal = 0;            //��λ//

	if (dustVal>500)        
		dustVal=500;

	return dustVal;
}

/**
  * @brief  ƽ��ֵ�˲���
  * @param  ��
  * @retval �����˲��������
  */
u16 Get_PM25_Average_Data(void)
{
	u16 temp_val=0;
	u8 t;
	for(t=0;t<PM25_READ_TIMES;t++)	//#define PM25_READ_TIMES	20	�����ȡ����,����ô���,Ȼ��ȡƽ��ֵ
 
	{
		temp_val+=PM25_GetData();	//��ȡADCֵ
		delay_ms(5);
	}
	temp_val/=PM25_READ_TIMES;//�õ�ƽ��ֵ
    return (u16)temp_val;//���������ADCƽ��ֵ
}



