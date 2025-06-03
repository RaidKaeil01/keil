#include "sys.h"
#include "delay.h"
#include "led.h"
#include "lcd1602.h"


/*
 *			RS------------>PA5
 *			RW------------>GND
 *			E ------------>PA4
 *			D4------------>PA3
 *			D5------------>PA2
 *			D6------------>PA1
 *			D7------------>PA0
*/






 int main(void)
 {	
	delay_init();	    //寤舵椂鍑芥暟鍒濆鍖�	
	lcd1602_init();	 
	LED_Init();		  	//鍒濆鍖栦笌LED杩炴帴鐨勭‖浠舵帴鍙�
	while(1)
	{
		LED0=0;
		delay_ms(300);	 //寤舵椂300ms
		LED0=1;
		delay_ms(300);	//寤舵椂300ms
		lcd1602_show_string(0, 0, (unsigned char*)"ERR");
		
	}
}
















