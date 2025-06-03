#include "lcd12864.h"
#include "delay.h"
#include "math.h"
#include "stdio.h"

/* 字符显示RAM地址    4行8列 */
u8 LCD_addr[4][8]={
	{0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87},  	//第一行
	{0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97},		//第二行
	{0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F},		//第三行
	{0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F}		//第四行
};


/*! 
*  @brief      LCD串行发送一个字节
 */
void SendByte(uint8_t byte)
{
	u8 i; 
	for(i = 0;i < 8;i++)
	{
		if((byte << i) & 0x80)  //0x80(1000 0000)  只会保留最高位
		{
			SID_Set();           // 引脚输出高电平，代表发送1
		}
		else
		{
			SID_Clr();         // 引脚输出低电平，代表发送0
		}
		/*或		
		SID =	(Dbyte << i) & 0x80;

		上面那样为了方便理解
		*/
		SCLK_Clr();   //时钟线置低  允许SID变化
		delay_us(5); //延时使数据写入
		SCLK_Set();    //拉高时钟，让从机读SID
	}   
}


/*! 
 *  @brief      LCD写指令
 */
void Lcd_WriteCmd(uint8_t Cmd )
{
     delay_ms(1);     //由于我们没有写LCD正忙的检测，所以直接延时1ms，使每次写入数据或指令间隔大于1ms 便可不用写忙状态检测
     SendByte(WRITE_CMD);            //11111,RW(0),RS(0),0   
     SendByte(0xf0&Cmd);      //高四位
     SendByte(Cmd<<4);   //低四位(先执行<<)
}

/*! 
 *  @brief      LCD写数据
 *  @since      v1.0
 *  @param  Dat   要写入的数据
 *  @author     Z小旋
 */
void Lcd_WriteData(uint8_t Dat )
{
     delay_ms(1);     
     SendByte(WRITE_DAT);            //11111,RW(0),RS(1),0
     SendByte(0xf0&Dat);      //高四位
     SendByte(Dat<<4);   //低四位(先执行<<)
}
/*! 
 *  @brief      LCD初始化
 */
void Lcd_Init(void)
{
	delay_ms(50);   	//等待液晶自检（延时>40ms）
	Lcd_WriteCmd(0x30);        //功能设定:选择基本指令集
	delay_ms(1);//延时>100us
	Lcd_WriteCmd(0x30);        //功能设定:选择8bit数据流
	delay_ms(1);	//延时>37us
	Lcd_WriteCmd(0x0c);        //开显示
	delay_ms(1);	//延时>100us
	Lcd_WriteCmd(0x01);        //清除显示，并且设定地址指针为00H
	delay_ms(30);	//延时>10ms
	Lcd_WriteCmd(0x06);        //进入设定点，初始化完成
}


/*! 
 *  @brief      显示字符或汉字
 *  @since      v1.0
 *  @param  x: row(0~3)
 *  @param  y: line(0~7) 
 *  @param 	str: 要显示的字符或汉字
 *  @author     Z小旋
 */
void LCD_Display_Words(uint8_t x,uint8_t y,uint8_t*str)
{ 
	Lcd_WriteCmd(LCD_addr[x][y]); //写初始光标位置
	while(*str>0)
	{ 
		Lcd_WriteData(*str);    //写数据
		str++;     
	}
}
/*! 
 *  @brief      显示图片
 *  @author     
 */
void LCD_Display_Picture(uint8_t *img)
{
	uint8_t x,y,i;
	Lcd_WriteCmd(0x34);		//切换到扩充指令
	Lcd_WriteCmd(0x34);		//关闭图形显示
	for(i = 0; i < 1; i++)   //上下屏写入
	{
		for(y=0;y<32;y++)   //垂直Y写32次
		{  
			for(x=0;x<8;x++)   //横向X写8次
			{
				Lcd_WriteCmd(0x80 + y);		//行地址
				Lcd_WriteCmd(0x80 + x+i);		//列地址
				Lcd_WriteData(*img ++);		//写高位字节数据 D15－D8   
				Lcd_WriteData(*img ++);		//写低位字节数据 D7－D0
			}
		}
	}
	Lcd_WriteCmd(0x36);//打开图形显示		
	Lcd_WriteCmd(0x30);        //切换回基本指令
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
 *  @brief      清屏函数
 */
void LCD_Clear(void)
{
	Lcd_WriteCmd(0x01);			//清屏指令
	delay_ms(2);				//延时以待液晶稳定【至少1.6ms】
}









