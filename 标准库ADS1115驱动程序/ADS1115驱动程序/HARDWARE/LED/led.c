#include "led.h"


//LED IO初始化
void LED_GPIO_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED0-->PC13 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC13
	LED0 = 0;delay_ms(200);
	LED0 = 1;delay_ms(200);
	
    printf("LED0_GPIO-C13OK!!\r\n");
}

 
