#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd12864.h"

int main()
{
	int num = 10;
	delay_init();
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	lcd_GPIO_init();
	Lcd_Init();
	while(1)
	{
		/*ͼƬ��ʾ*/
		//LCD_Display_Picture();  //������Ҫ�Լ�����ͼƬ��������
		//delay_ms(2000);     //�����ʱ����Ҫ��
		/*������ʾ*/
		lcd12864_show_num(0,0,num,2);
//		LCD_Display_Words(0,0,(uint8_t *)"��ʱ���º�ʱ��");
		LCD_Display_Words(1,0,(uint8_t *)"���ﳤ����δ��");
		LCD_Display_Words(2,0,(uint8_t *)"��ʹ���Ƿɽ���");
		LCD_Display_Words(3,0,(uint8_t *)"���̺������ɽ");
	}
}




