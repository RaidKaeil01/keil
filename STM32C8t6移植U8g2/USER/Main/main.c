#include "stm32f10x.h"
#include "RCC.h"
#include "Delay.h"
#include "LED.h"
#include "IIC_OLED.h"
#include "u8g2.h"
void RCC_Configuration(void)
{
	RCC_HSE_Configuration();									//外部时钟72M
}           
void SysConfiguration(void)
{
	RCC_Configuration();										//时钟配置
	LEDInit();
	OLED12864_IoInit();											//IIC OLED 端口初始化
}
uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
	switch(msg)
		{
			case U8X8_MSG_DELAY_MILLI://Function which implements a delay, arg_int contains the amount of ms
				Delayxms(arg_int);
			break;
		
			case U8X8_MSG_DELAY_10MICRO://Function which delays 10us
				Delayxus(10);
			break;
		
			case U8X8_MSG_DELAY_100NANO://Function which delays 100ns
				__NOP();
			break;
			case U8X8_MSG_GPIO_I2C_CLOCK:
				if (arg_int) IIC_OLED_SCL_HIGH();
				else IIC_OLED_SCL_LOW();
			break;
			case U8X8_MSG_GPIO_I2C_DATA:
				if (arg_int) IIC_OLED_SDA_HIGH();
				else IIC_OLED_SDA_LOW();
			break;
			default:
				return 0; //A message was received which is not implemented, return 0 to indicate an error
	}
	return 1; // command processed successfully.
}
void draw(u8g2_t *u8g2)
{
	u8g2_ClearBuffer(u8g2); 
    u8g2_SetFontMode(u8g2, 1); 
    u8g2_SetFontDirection(u8g2, 0); 
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(u8g2, 0, 20, "U");
    
    u8g2_SetFontDirection(u8g2, 1);
    u8g2_SetFont(u8g2, u8g2_font_inb30_mn);
    u8g2_DrawStr(u8g2, 21,8,"8");
        
    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(u8g2, 51,30,"g");
    u8g2_DrawStr(u8g2, 67,30,"\xb2");
    
    u8g2_DrawHLine(u8g2, 2, 35, 47);
    u8g2_DrawHLine(u8g2, 3, 36, 47);
    u8g2_DrawVLine(u8g2, 45, 32, 12);
    u8g2_DrawVLine(u8g2, 46, 33, 12);
  
    u8g2_SetFont(u8g2, u8g2_font_4x6_tr);
    u8g2_DrawStr(u8g2, 1,54,"github.com/olikraus/u8g2");
	
	u8g2_SendBuffer(u8g2);
	Delayxms(1000);
}
int main(void)
{ 	
	u8g2_t u8g2;
	SysConfiguration();
	u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8g2_gpio_and_delay_stm32);
	u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
	u8g2_SetPowerSave(&u8g2, 0); // wake up display
	u8g2_ClearBuffer(&u8g2);
	Delayxms(1000);
	u8g2_DrawLine(&u8g2, 0,0, 127, 63);
	u8g2_DrawLine(&u8g2, 127,0, 0, 63);
	u8g2_SendBuffer(&u8g2);
	u8g2_ClearBuffer(&u8g2);
	draw(&u8g2);
	while(1)
	{	
		LED1Reverse();
		Delayxms(1000);
	}
}


