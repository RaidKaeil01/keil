#include "PM25.h"

/**
  * @brief  PM25初始化程序
  * @param  无
  * @retval 无
  */

void PM25_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd (PM25_GPIO_CLK, ENABLE );					// 打开 ADC IO端口时钟
	GPIO_InitStructure.GPIO_Pin = PM25_LED_GPIO_PIN;					// 配置 ADC IO 引脚模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;					// 设置为推挽输出
	
	GPIO_Init(PM25_LED_GPIO_PORT, &GPIO_InitStructure);				// 初始化 ADC IO
	
	//PA1作为模拟通道输入引脚 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	
	GPIO_InitStructure.GPIO_Pin = PM25_VO_GPIO_PIN;
	GPIO_Init(PM25_VO_GPIO_PORT, &GPIO_InitStructure);
	
	ADCx_Init();
	
}
	
uint16_t PM25_ADC_Read(void)
{
	//设置指定ADC的规则组通道，采样时间
	return ADC_GetValue(ADC_CHANNEL, ADC_SampleTime_239Cycles5);
}


/**
  * @brief  PM25 ADC数据读取
  * @param  无
  * @retval 返回转换后的PM25模拟信号数值
  */
	
u16 PM25_GetData(void)
{
	uint16_t ADCVal;
	int dustVal = 0;
	float Voltage;

	PM25_LED_H;	//置1  开启内部LED
	delay_us(280); 	// 开启LED后的280us的等待时间
	ADCVal = PM25_ADC_Read();  //PA1 采样，读取AD值
	delay_us(19);			  //延时19us，因为这里AD采样的周期为239.5，所以AD转换一次需耗时21us，19加21再加280刚好是320us
	PM25_LED_L;	//置0  关闭内部LED
	delay_us(9680);			//需要脉宽比0.32ms/10ms的PWM信号驱动传感器中的LED
	
	Voltage = 3.3f * ADCVal / 4096.f * 2; //获得AO输出口的电压值
	
	dustVal = (0.17*Voltage-0.1)*1000;  //乘以1000单位换成ug/m3//

	if (dustVal < 0)
		dustVal = 0;            //限位//

	if (dustVal>500)        
		dustVal=500;

	return dustVal;
}

/**
  * @brief  平均值滤波法
  * @param  无
  * @retval 返回滤波后的数据
  */
u16 Get_PM25_Average_Data(void)
{
	u16 temp_val=0;
	u8 t;
	for(t=0;t<PM25_READ_TIMES;t++)	//#define PM25_READ_TIMES	20	定义读取次数,读这么多次,然后取平均值
 
	{
		temp_val+=PM25_GetData();	//读取ADC值
		delay_ms(5);
	}
	temp_val/=PM25_READ_TIMES;//得到平均值
    return (u16)temp_val;//返回算出的ADC平均值
}



