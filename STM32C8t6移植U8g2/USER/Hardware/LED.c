#include "LED.h"
/*
LED1=PC13;
*/

#define LED1_APB_GPIO  	RCC_APB2Periph_GPIOC
#define LED1_Pin		GPIO_Pin_13	
#define LED1_GPIO       GPIOC
void LEDInit(void)											//初始化LED端口为推挽输出模式
{
	GPIO_InitTypeDef GPIO_InitStructure;						//定义GPIO结构体	
	RCC_APB2PeriphClockCmd(LED1_APB_GPIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=LED1_Pin;						//配置LED端口
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;				//配置端口的速度	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;				//配置端口模式为推挽输出模式
	GPIO_Init(LED1_GPIO,&GPIO_InitStructure);					//对端口进行初始化	
	LED1_OFF();
}


void LED1_ON(void)											//LED1亮
{
	GPIO_ResetBits(LED1_GPIO,LED1_Pin);
}
void LED1_OFF(void)											//LED1灭
{
	GPIO_SetBits(LED1_GPIO,LED1_Pin);
}

void LED1Reverse(void)										//LED1反转
{
	if(GPIO_ReadOutputDataBit(LED1_GPIO,LED1_Pin))
	GPIO_ResetBits(LED1_GPIO,LED1_Pin);
	else
	GPIO_SetBits(LED1_GPIO,LED1_Pin);
}
void PinReverse(GPIO_TypeDef* GPIOx,unsigned int GPIO_PinX)//引脚电平反转
{
	if(GPIO_ReadOutputDataBit(GPIOx,GPIO_PinX))
	GPIO_ResetBits(GPIOx,GPIO_PinX);
	else
	GPIO_SetBits(GPIOx,GPIO_PinX);
}
