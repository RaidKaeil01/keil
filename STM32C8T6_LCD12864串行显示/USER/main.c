#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd12864.h"

int main()
{
	int num = 10;
	delay_init();
	LED_Init();		  	//初始化与LED连接的硬件接口
	lcd_GPIO_init();
	Lcd_Init();
	while(1)
	{
		/*图片显示*/
		//LCD_Display_Picture();  //这里需要自己放入图片编码数组
		//delay_ms(2000);     //这个延时必须要有
		/*汉字显示*/
		lcd12864_show_num(0,0,num,2);
//		LCD_Display_Words(0,0,(uint8_t *)"清时明月汉时关");
		LCD_Display_Words(1,0,(uint8_t *)"万里长征人未还");
		LCD_Display_Words(2,0,(uint8_t *)"但使龙城飞将在");
		LCD_Display_Words(3,0,(uint8_t *)"不教胡马度阴山");
	}
}




