#include "led.h"


//LED IO��ʼ��
void LED_GPIO_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PC�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED0-->PC13 �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC13
	LED0 = 0;delay_ms(200);
	LED0 = 1;delay_ms(200);
	
    printf("LED0_GPIO-C13OK!!\r\n");
}

 
