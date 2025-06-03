#ifndef __OLED_H
#define __OLED_H 

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdlib.h"	

//-----------------OLED端口定义---------------- 

#define OLED_SCL_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_0)//SCL
#define OLED_SCL_Set() GPIO_SetBits(GPIOA,GPIO_Pin_0)

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_1)//DIN
#define OLED_SDA_Set() GPIO_SetBits(GPIOA,GPIO_Pin_1)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

/******************************** IIC 通信协议函数 **************************************/
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void Send_Byte(u8 dat);
void OLED_WR_Byte(u8 dat,u8 mode);

/******************************** OLED功能函数 *****************************************
void OLED_Display_Turn	(u8 turn)	//开启\关闭OLED显示	（1：开启		0：熄灭）
void OLED_Display_Color	(u8 turn)	//设置OLED显示模式	（1：反白显示	0：正常显示）
void OLED_Display_Dir	(u8 dirX, u8 dirY)	//设置画面显示方向（1：开启镜像	0：正常显示）
void OLED_Set_Point		(u8 x,u8 y);		//指定要操作的像素点（x:0-127	y:0-7）
void OLED_Clear(void)				//清屏函数
void OLED_On(void)					//点亮全屏

void OLED_ShowChar	( u8 x, u8 y, u8 chr, u8 Char_Size )	//显示一个字符（x：0-127 y：0-63 size：8/12/16/24）
void OLED_ShowString( u8 x, u8 y, u8 *str, u8 Char_Size )	//显示一个字符串	示例：OLED_ShowString(0, 0, (u8 *)"abcds", 8);
void OLED_ShowNum	( u8 x, u8 y, u32 num, u8 len, u8 size )//显示一串数字		示例：OLED_ShowNum(0, 0, 1231, 4, 8);
void OLED_ShowChinese(u8 x, u8 y, u8 no )					//显示一个汉字		示例：OLED_ShowChinese(0, 0, 1);
void OLED_ShowBMP	( u8 x0, u8 y0, u8 x1, u8 y1, u8 BMP[] )//显示一幅图片

void OLED_Refresh	(void)				//显存更新
void OLED_GFill		(void)				//填满显存
void OLED_GClear	(void)				//清空显存

void OLED_DrawPoint(u8 x, u8 y)			//点亮屏幕指定位置的点（x:0-127	y:0-63）
void OLED_ClearPoint(u8 x, u8 y)		//熄灭屏幕指定位置的点

void FourPoints(u8 xstart, u8 ystart)	//密度显示函数
--------------------------------------------------------------------------------------*/
void OLED_Display_Turn	(u8 turn);
void OLED_Display_Color	(u8 color);
void OLED_Display_Dir	(u8 dirX, u8 dirY);

void OLED_Set_Point(u8 x,u8 y);
void OLED_Clear(void);
void OLED_On(void);

//显示一个字符
void OLED_ShowChar	( u8 x, u8 y, u8 chr, u8 Char_Size );
//显示一个字符串
void OLED_ShowString( u8 x, u8 y, u8 *str, u8 Char_Size );
//显示一串数字
void OLED_ShowNum	( u8 x, u8 y, u32 num, u8 len, u8 size );
//显示一个汉字
void OLED_ShowChinese(u8 x, u8 y, u8 no );
//显示一幅图片
void OLED_ShowBMP	( u8 x0, u8 y0, u8 x1, u8 y1, u8 BMP[] );

void OLED_Refresh(void);
void OLED_GFill(void);
void OLED_GClear(void);
void OLED_DrawPoint(u8 x, u8 y);
void OLED_ClearPoint(u8 x, u8 y);

void FourPoints(u8 xstart, u8 ystart);
void Mask_Gray(u8 xstart, u8 ystart);

void OLED_Init(void);


void OLED_Test(void);

#endif

