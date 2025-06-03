#include "key.h"
#include "led.h"

void KEY_GPIO_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//Ê¹ÄÜB
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_IPU;//
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
    printf("KEY_GPIO-A0 _OK!!\r\n");
}


#define delay 12
int KEY_Scanf(void)
{

	char str[10]={0};
	int i;
	if(KEY0==0){
		delay_ms(12);
		if(KEY0==0){		//Ç°ÂÖ
			i=0;
			while(KEY0==0){
				delay_ms(20);
				i++;
			}; 
			if(i<delay) {
				printf("KEY0_short\r\n");
				return 1;
			}
			else {
				printf("KEY1_long\r\n");
				return 2;
			}
		}

		
	}
	return 0;
}
	
	





