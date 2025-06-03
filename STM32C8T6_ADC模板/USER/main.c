#include "stm32f10x.h"
#include "delay.h"
#include "adc.h"
#include "usart.h"

u16 value_channel_6= 0;

float voltage_1 = 0;


void OLED_show(void);

int main(void)
{
	// 自定义变量
	int i = 0;
	uart_init(9600);
	ADCx_Iint();
	delay_init();
	
	while(1)
	{
		i++;
		if(i%1000==0)
		{
			i = 0;
			value_channel_6 = Get_ADC_Value(ADC_Channel_6, 20);
			printf("检测到的AD值为：%d\r\n",value_channel_6);
//			//电压转换：voltage = AD_voltage * V_ref / 4096
			voltage_1 = (float) value_channel_6 * (3.3/4096);
			printf("检测到的电压值为：%.2f V\r\n",voltage_1);
		}
		
    }
}







