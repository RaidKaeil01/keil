#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "led.h"
#include "demo.h"
#include "oled.h"
#include "bmp.h"

int main(void)
{
	// 来到这里的时候，系统的时钟已经被配置成72M
	LED_Init();		  	//初始化LED连接
	OLED_Init();
	OLED_ColorTurn(0);//0正常显示，1 反色显示
	OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示	
	OLED_Refresh();
	OLED_Clear();
	FreeRTOS_Demo();
	
	while(1);
}


