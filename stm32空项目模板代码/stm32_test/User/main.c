#include "stm32f10x.h"
#include "led.h"

int main(void)
{
	// À´µ½ÕâÀïµÄÊ±ºò£¬ÏµÍ³µÄÊ±ÖÓÒÑ¾­±»ÅäÖÃ³É72M¡
	LED_Init();		  	//³õÊ¼»¯ÓëLEDÁ¬½ÓµÄÓ²¼ş½Ó¿£
	while(1)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	}
	
}


