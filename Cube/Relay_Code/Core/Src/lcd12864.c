#include "lcd12864.h"
#include "delay.h"
#include "math.h"
#include "stdio.h"

/* �ַ���ʾRAM��ַ    4��8�� */
u8 LCD_addr[4][8]={
	{0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87},  	//��һ��
	{0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97},		//�ڶ���
	{0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F},		//������
	{0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F}		//������
};


/*! 
*  @brief      LCD���з���һ���ֽ�
 */
void SendByte(uint8_t byte)
{
	u8 i; 
	for(i = 0;i < 8;i++)
	{
		if((byte << i) & 0x80)  //0x80(1000 0000)  ֻ�ᱣ�����λ
		{
			SID_Set();           // ��������ߵ�ƽ��������1
		}
		else
		{
			SID_Clr();         // ��������͵�ƽ��������0
		}
		/*��		
		SID =	(Dbyte << i) & 0x80;

		��������Ϊ�˷������
		*/
		SCLK_Clr();   //ʱ�����õ�  ����SID�仯
		delay_us(5); //��ʱʹ����д��
		SCLK_Set();    //����ʱ�ӣ��ôӻ���SID
	}   
}


/*! 
 *  @brief      LCDдָ��
 */
void Lcd_WriteCmd(uint8_t Cmd )
{
     delay_ms(1);     //��������û��дLCD��æ�ļ�⣬����ֱ����ʱ1ms��ʹÿ��д�����ݻ�ָ��������1ms ��ɲ���дæ״̬���
     SendByte(WRITE_CMD);            //11111,RW(0),RS(0),0   
     SendByte(0xf0&Cmd);      //����λ
     SendByte(Cmd<<4);   //����λ(��ִ��<<)
}

/*! 
 *  @brief      LCDд����
 *  @since      v1.0
 *  @param  Dat   Ҫд�������
 *  @author     ZС��
 */
void Lcd_WriteData(uint8_t Dat )
{
     delay_ms(1);     
     SendByte(WRITE_DAT);            //11111,RW(0),RS(1),0
     SendByte(0xf0&Dat);      //����λ
     SendByte(Dat<<4);   //����λ(��ִ��<<)
}
/*! 
 *  @brief      LCD��ʼ��
 */
void Lcd_Init(void)
{
	delay_ms(50);   	//�ȴ�Һ���Լ죨��ʱ>40ms��
	Lcd_WriteCmd(0x30);        //�����趨:ѡ�����ָ�
	delay_ms(1);//��ʱ>100us
	Lcd_WriteCmd(0x30);        //�����趨:ѡ��8bit������
	delay_ms(1);	//��ʱ>37us
	Lcd_WriteCmd(0x0c);        //����ʾ
	delay_ms(1);	//��ʱ>100us
	Lcd_WriteCmd(0x01);        //�����ʾ�������趨��ַָ��Ϊ00H
	delay_ms(30);	//��ʱ>10ms
	Lcd_WriteCmd(0x06);        //�����趨�㣬��ʼ�����
}


/*! 
 *  @brief      ��ʾ�ַ�����
 *  @since      v1.0
 *  @param  x: row(0~3)
 *  @param  y: line(0~7) 
 *  @param 	str: Ҫ��ʾ���ַ�����
 *  @author     ZС��
 */
void LCD_Display_Words(uint8_t x,uint8_t y,uint8_t*str)
{ 
	Lcd_WriteCmd(LCD_addr[x][y]); //д��ʼ���λ��
	while(*str>0)
	{ 
		Lcd_WriteData(*str);    //д����
		str++;     
	}
}
/*! 
 *  @brief      ��ʾͼƬ
 *  @author     
 */
void LCD_Display_Picture(uint8_t *img)
{
	uint8_t x,y,i;
	Lcd_WriteCmd(0x34);		//�л�������ָ��
	Lcd_WriteCmd(0x34);		//�ر�ͼ����ʾ
	for(i = 0; i < 1; i++)   //������д��
	{
		for(y=0;y<32;y++)   //��ֱYд32��
		{  
			for(x=0;x<8;x++)   //����Xд8��
			{
				Lcd_WriteCmd(0x80 + y);		//�е�ַ
				Lcd_WriteCmd(0x80 + x+i);		//�е�ַ
				Lcd_WriteData(*img ++);		//д��λ�ֽ����� D15��D8   
				Lcd_WriteData(*img ++);		//д��λ�ֽ����� D7��D0
			}
		}
	}
	Lcd_WriteCmd(0x36);//��ͼ����ʾ		
	Lcd_WriteCmd(0x30);        //�л��ػ���ָ��
}	


void lcd12864_show_num(uint8_t X, uint8_t Y, uint32_t number, uint8_t Length)
{
	u8 array[11];
	u8 i;
	array[Length] = 0;
  
    
	for (i = Length; i > 0; i--) {
		array[i - 1] = number % 10 + '0';
		number /= 10;
	}

	for (i = 0; i < Length - 1; i++) {
		if (array[i] == '0') {
			array[i] = ' ';
		}
		else {
			break;
		}
	}
	LCD_Display_Words(X, Y, array);
}

void lcd12864_show_float(uint8_t X, uint8_t Y, float number, uint8_t int_Length, uint8_t float_Length)
{
	int temp,temp_int,temp_float,bei = 1;
	u8 array[11],i;
	for(i = 0;i<float_Length;i++)
	{
		bei *= 10;
	}
	temp = (int)(number*bei);
	temp_int = temp/bei;
	temp_float = temp%bei;
	
	for (i = int_Length; i > 0; i--)
	{
		array[i - 1] = temp_int % 10 + '0';
		temp_int /= 10;
	}
	array[int_Length] = '.';
	for (i = float_Length; i > 0; i--)
	{
		array[i+int_Length] = temp_float % 10 + '0';
		temp_float /= 10;
	}
	for (i = 0; i < float_Length+int_Length; i++)
	{
		if (array[i] == '0')
		{
			array[i] = ' ';
		}
		else
		{
			break;
		}
	}

	LCD_Display_Words(X, Y, array);
}

/*! 
 *  @brief      ��������
 */
void LCD_Clear(void)
{
	Lcd_WriteCmd(0x01);			//����ָ��
	delay_ms(2);				//��ʱ�Դ�Һ���ȶ�������1.6ms��
}









