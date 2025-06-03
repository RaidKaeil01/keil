#ifndef __lcd12864_H_
#define __lcd12864_H_

#include "stm32f1xx_hal.h"

#define WRITE_CMD	0xF8//写命令  
#define WRITE_DAT	0xFA//写数据


#define SID_PORT  GPIOC
#define SID_PIN   GPIO_PIN_14

#define SCLK_PORT  GPIOC
#define SCLK_PIN   GPIO_PIN_15

// 使能 GPIO 时钟
#define SID_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define SCLK_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()

// I2C 信号控制
#define SID_Clr() HAL_GPIO_WritePin(SID_PORT, SID_PIN, GPIO_PIN_RESET)  // SCL = 0
#define SID_Set() HAL_GPIO_WritePin(SID_PORT, SID_PIN, GPIO_PIN_SET)    // SCL = 1

#define SCLK_Clr() HAL_GPIO_WritePin(SCLK_PORT, SCLK_PIN, GPIO_PIN_RESET)  // SDA = 0
#define SCLK_Set() HAL_GPIO_WritePin(SCLK_PORT, SCLK_PIN, GPIO_PIN_SET)    // SDA = 1


////接口(SID: PC1  SCLK: PC0) 
//#define SID PCout(14)	//	R/W
//#define SCLK PCout(15)	//	E


void Lcd_Init(void);

void SendByte(uint8_t Dbyte);
void LCD_Clear(void);
void LCD_Display_Words(uint8_t x,uint8_t y,uint8_t*str);
void LCD_Display_Picture(uint8_t *img);
void lcd12864_show_num(uint8_t X, uint8_t Y, uint32_t number, uint8_t Length);
void lcd12864_show_float(uint8_t X, uint8_t Y, float number, uint8_t Length, uint8_t float_Length);

void Lcd_WriteData(uint8_t Dat );
void Lcd_WriteCmd(uint8_t Cmd );

#endif
