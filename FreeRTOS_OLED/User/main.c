#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "led.h"
#include "demo.h"
#include "oled.h"
#include "bmp.h"

int main(void)
{
	// ���������ʱ��ϵͳ��ʱ���Ѿ������ó�72M
	LED_Init();		  	//��ʼ��LED����
	OLED_Init();
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
	OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ	
	OLED_Refresh();
	OLED_Clear();
	FreeRTOS_Demo();
	
	while(1);
}


