#ifndef __OLED_H
#define __OLED_H 

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdlib.h"	

//-----------------OLED�˿ڶ���---------------- 

#define OLED_SCL_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_0)//SCL
#define OLED_SCL_Set() GPIO_SetBits(GPIOA,GPIO_Pin_0)

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_1)//DIN
#define OLED_SDA_Set() GPIO_SetBits(GPIOA,GPIO_Pin_1)

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

/******************************** IIC ͨ��Э�麯�� **************************************/
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void Send_Byte(u8 dat);
void OLED_WR_Byte(u8 dat,u8 mode);

/******************************** OLED���ܺ��� *****************************************
void OLED_Display_Turn	(u8 turn)	//����\�ر�OLED��ʾ	��1������		0��Ϩ��
void OLED_Display_Color	(u8 turn)	//����OLED��ʾģʽ	��1��������ʾ	0��������ʾ��
void OLED_Display_Dir	(u8 dirX, u8 dirY)	//���û�����ʾ����1����������	0��������ʾ��
void OLED_Set_Point		(u8 x,u8 y);		//ָ��Ҫ���������ص㣨x:0-127	y:0-7��
void OLED_Clear(void)				//��������
void OLED_On(void)					//����ȫ��

void OLED_ShowChar	( u8 x, u8 y, u8 chr, u8 Char_Size )	//��ʾһ���ַ���x��0-127 y��0-63 size��8/12/16/24��
void OLED_ShowString( u8 x, u8 y, u8 *str, u8 Char_Size )	//��ʾһ���ַ���	ʾ����OLED_ShowString(0, 0, (u8 *)"abcds", 8);
void OLED_ShowNum	( u8 x, u8 y, u32 num, u8 len, u8 size )//��ʾһ������		ʾ����OLED_ShowNum(0, 0, 1231, 4, 8);
void OLED_ShowChinese(u8 x, u8 y, u8 no )					//��ʾһ������		ʾ����OLED_ShowChinese(0, 0, 1);
void OLED_ShowBMP	( u8 x0, u8 y0, u8 x1, u8 y1, u8 BMP[] )//��ʾһ��ͼƬ

void OLED_Refresh	(void)				//�Դ����
void OLED_GFill		(void)				//�����Դ�
void OLED_GClear	(void)				//����Դ�

void OLED_DrawPoint(u8 x, u8 y)			//������Ļָ��λ�õĵ㣨x:0-127	y:0-63��
void OLED_ClearPoint(u8 x, u8 y)		//Ϩ����Ļָ��λ�õĵ�

void FourPoints(u8 xstart, u8 ystart)	//�ܶ���ʾ����
--------------------------------------------------------------------------------------*/
void OLED_Display_Turn	(u8 turn);
void OLED_Display_Color	(u8 color);
void OLED_Display_Dir	(u8 dirX, u8 dirY);

void OLED_Set_Point(u8 x,u8 y);
void OLED_Clear(void);
void OLED_On(void);

//��ʾһ���ַ�
void OLED_ShowChar	( u8 x, u8 y, u8 chr, u8 Char_Size );
//��ʾһ���ַ���
void OLED_ShowString( u8 x, u8 y, u8 *str, u8 Char_Size );
//��ʾһ������
void OLED_ShowNum	( u8 x, u8 y, u32 num, u8 len, u8 size );
//��ʾһ������
void OLED_ShowChinese(u8 x, u8 y, u8 no );
//��ʾһ��ͼƬ
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

