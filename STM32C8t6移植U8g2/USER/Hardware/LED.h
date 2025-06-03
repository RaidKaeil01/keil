#ifndef __LED_H__
#define __LED_H__
#include "STM32f10x.h"
void LEDInit(void);						//LED端口初始化
void LED1_ON(void);						//LED1亮
void LED1_OFF(void);					//LED1灭
void LED1Reverse(void);					//LED1反转
void PinReverse(GPIO_TypeDef* GPIOx,unsigned int GPIO_PinX);//引脚电平反转
#endif
