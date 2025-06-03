#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "led.h"
#include "demo.h"
#include "oled.h"
#include "bmp.h"

void Delay(u8 t)
{
	while(t--);
}

int main(void)
{
	// 来到这里的时候，系统的时钟已经被配置成72M
	LED_Init();		  	//初始化LED连接
//	OLED_Init();
//	OLED_Clear();
//	OLED_GClear();
//	OLED_ShowBMP(0, 0, 127, 7, BMP3);
	
	
//	Mask_Gray(0, 0); OLED_Refresh(); Delay(200);
//	Mask_Gray(1, 1); OLED_Refresh(); Delay(200);
//	Mask_Gray(2, 0); OLED_Refresh(); Delay(200);
//	Mask_Gray(0, 1); OLED_Refresh(); Delay(200);
//	Mask_Gray(1, 0); OLED_Refresh(); Delay(200);
//	Mask_Gray(2, 1); OLED_Refresh(); Delay(200);

	FreeRTOS_Demo();
	while(1)
	{
//		OLED_ShowBMP(0, 0, 127, 7, BMP1);
		
	


























/*
//		OLED_DrawPoint(0 , 0 ); OLED_Refresh(); Delay(200);
//		OLED_DrawPoint(8 , 8 ); OLED_Refresh(); Delay(200);
//		OLED_DrawPoint(16, 16); OLED_Refresh(); Delay(200);
//		OLED_DrawPoint(24, 24); OLED_Refresh(); Delay(200);
//		OLED_DrawPoint(32, 32); OLED_Refresh(); Delay(200);
//		OLED_DrawPoint(40, 40); OLED_Refresh(); Delay(200);
//		OLED_DrawPoint(48, 48); OLED_Refresh(); Delay(200);
//		OLED_DrawPoint(56, 56); OLED_Refresh(); Delay(200);
//		OLED_DrawPoint(63, 63); OLED_Refresh(); Delay(200);
//		
//		
//		OLED_ClearPoint(0 , 0 ); OLED_Refresh(); Delay(200);
//		OLED_ClearPoint(8 , 8 ); OLED_Refresh(); Delay(200);
//		OLED_ClearPoint(16, 16); OLED_Refresh(); Delay(200);
//		OLED_ClearPoint(24, 24); OLED_Refresh(); Delay(200);
//		OLED_ClearPoint(32, 32); OLED_Refresh(); Delay(200);
//		OLED_ClearPoint(40, 40); OLED_Refresh(); Delay(200);
//		OLED_ClearPoint(48, 48); OLED_Refresh(); Delay(200);
//		OLED_ClearPoint(56, 56); OLED_Refresh(); Delay(200);
//	    OLED_ClearPoint(63, 63); OLED_Refresh(); Delay(200);
*/
	}
}


