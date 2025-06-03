#ifndef __lcd12864_H_
#define __lcd12864_H_


#include "sys.h"

#define WRITE_CMD	0xF8//写命令  
#define WRITE_DAT	0xFA//写数据

//接口(SID: PC1  SCLK: PC0) 
#define SID PCout(14)	//	R/W
#define SCLK PCout(15)	//	E


void lcd_GPIO_init(void);
void Lcd_Init(void);

void SendByte(u8 Dbyte);
void LCD_Clear(void);
void LCD_Display_Words(uint8_t x,uint8_t y,uint8_t*str);
void LCD_Display_Picture(uint8_t *img);
void lcd12864_show_num(u8 X, u8 Y, u32 number, u8 Length);

void Lcd_WriteData(u8 Dat );
void Lcd_WriteCmd(u8 Cmd );

#endif
