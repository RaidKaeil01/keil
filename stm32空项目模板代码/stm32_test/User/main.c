#include "stm32f10x.h"
#include "led.h"

int main(void)
{
	// 来到这里的时候，系统的时钟已经被配置成72M�
	LED_Init();		  	//初始化与LED连接的硬件接浚
	while(1)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	}
	
}


