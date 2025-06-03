#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"

extern unsigned char BMP3[];

u8 GRAM[1024];


//延时
void IIC_delay(void)
{
	u8 t=3;
	while(t--);
}

//起始信号
void I2C_Start(void)
{
	OLED_SDA_Set();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SDA_Clr();
	IIC_delay();
	OLED_SCL_Clr();
	IIC_delay();
}

//结束信号
void I2C_Stop(void)
{
	OLED_SDA_Clr();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SDA_Set();
}

//等待信号响应
void I2C_WaitAck(void) //测数据信号的电平
{
	OLED_SDA_Set();
	IIC_delay();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SCL_Clr();
	IIC_delay();
}

//写入一个字节
void Send_Byte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(dat&0x80)	//将dat的8位从最高位依次写入
		{
			OLED_SDA_Set();
		}
		else
		{
			OLED_SDA_Clr();
		}
		IIC_delay();
		OLED_SCL_Set();
		IIC_delay();
		OLED_SCL_Clr();	//将时钟信号设置为低电平
		dat<<=1;
	}
}

/* void OLED_WR_Byte(u8 dat,u8 mode)
 * 发送一个字节
 * mode:数据/命令标志 0,表示命令;1,表示数据;*/
void OLED_WR_Byte(u8 dat,u8 mode)
{
	I2C_Start();
	Send_Byte(0x78);
	I2C_WaitAck();
	if(mode)
	{
		Send_Byte(0x40);
	}
	else
	{
		Send_Byte(0x00);
	}
	I2C_WaitAck();
	Send_Byte(dat);
	I2C_WaitAck();
	I2C_Stop();
}


/******************************** OLED功能函数 ******************************************/
void OLED_Display_Turn(u8 turn)
{
	if(turn)
	{
		OLED_WR_Byte(0xAF, OLED_CMD);	//开启显示
	}
	else
	{
		OLED_WR_Byte(0xAE, OLED_CMD);	//关闭显示
	}
}



/*--------------------------------------------------------------------------------------*/
void OLED_Display_Color(u8 color)
{
	if(color)
	{
		OLED_WR_Byte(0xA7, OLED_CMD);	//反白显示
	}
	else
	{
		OLED_WR_Byte(0xA6, OLED_CMD);	//正常显示
	}
}



/*--------------------------------------------------------------------------------------*/
void OLED_Display_Dir(u8 dirX, u8 dirY)
{
	if(dirX)
	{
		OLED_WR_Byte(0xA1, OLED_CMD);	//左右镜像
	}
	else
	{
		OLED_WR_Byte(0xA6, OLED_CMD);	//正常显示
	}

	if(dirY)
	{
		OLED_WR_Byte(0xC0, OLED_CMD);	//上下镜像
	}
	else
	{
		OLED_WR_Byte(0xC8, OLED_CMD);	//正常显示
	}
}



/*--------------------------------------------------------------------------------------*/
void OLED_Set_Point(u8 x,u8 y)
{
	OLED_WR_Byte(0xB0+y, OLED_CMD);
	OLED_WR_Byte(((x & 0xF0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte(( x & 0x0F), OLED_CMD);
}



/*--------------------------------------------------------------------------------------*/
void OLED_Clear(void)
{
	unsigned char y, x;
	for(y = 0; y < 8; y++)
	{
		OLED_WR_Byte(0xB0 + y, OLED_CMD);	//设置页（行）地址（0-7）
		OLED_WR_Byte(0x00, OLED_CMD);		//设置显示位置--列低地址
		OLED_WR_Byte(0x10, OLED_CMD);		//设置显示位置--列高地址
		
		for(x = 0; x < 128; x++)			//第n列（这一行里第n条小竖列）
		{
			OLED_WR_Byte(0x00, OLED_DATA);	//小竖列的八个点全部熄灭
		}
	}
}



/*--------------------------------------------------------------------------------------*/
void OLED_On(void)
{
	u8 y, x;
	for(y = 0; y < 8; y++)
	{
		OLED_Set_Point(0, y);
		for(x=0; x < 128; x++)		//第n列（这一行里第n个小竖列）
		{
			OLED_WR_Byte(0xFF, OLED_DATA);	//小竖列的八个点全部点亮
		}
	}
}



/*--------------------------------------------------------------------------------------*/
void OLED_ShowChar( u8 x, u8 y, u8 chr, u8 Char_Size)
{
	u8 c = 0, i = 0;
	
	c = chr - ' ';	//根据空格符号得到偏移后的值
	
	if(x > 127)		//如果起点横坐标超过了屏幕最右侧
	{
		x = 0;		//列坐标x置0，回到屏幕最左侧
		y = y + 2;	//行左边y向下移2行
	}				//将字符显示在屏幕指定行的下一行起点位置
/*-----------------------------------------------------*/
	/* 判断字号大小 */
	if(Char_Size == 8)			//字库：asc2_0806[][6]	注：字库后两位是每行的循环个数，如0806，每行循环6个
	{
		OLED_Set_Point(x, y);
		for(i = 0; i < 6; i++)
		{
			OLED_WR_Byte(asc2_0806[c][i], OLED_DATA);
		}
	}
/*-----------------------------------------------------*/
	else if(Char_Size == 12)	//字库：asc2_1206[95][12]
	{
		//两行高度上行
		OLED_Set_Point(x, y);
		for(i = 0; i < 6; i++)
		{
			OLED_WR_Byte(asc2_1206[c][i], OLED_DATA);
		}
		//两行高度下行
		OLED_Set_Point(x, y+1);
		for(i = 0; i < 6; i++)
		{
			OLED_WR_Byte(asc2_1206[c][i+6], OLED_DATA);
		}
	}
/*-----------------------------------------------------*/
	else if(Char_Size == 16)	//字库：asc2_1608[][16]
	{
		//两行高度上行
		OLED_Set_Point(x, y);
		for(i = 0; i < 8; i++)
		{
			OLED_WR_Byte(asc2_1608[c][i], OLED_DATA);
		}
		//两行高度下行
		OLED_Set_Point(x, y+1);
		for(i = 0; i < 8; i++)
		{
			OLED_WR_Byte(asc2_1608[c][i+8], OLED_DATA);
		}
	}
/*-----------------------------------------------------*/
	else						//字库：asc2_2412[][36]
	{
		//三行高度上行
		OLED_Set_Point(x, y);
		for(i = 0; i < 12; i++)
		{
			OLED_WR_Byte(asc2_2412[c][i], OLED_DATA);
		}
		//三行高度中行
		OLED_Set_Point(x, y+1);
		for(i = 0; i < 12; i++)
		{
			OLED_WR_Byte(asc2_2412[c][i+12], OLED_DATA);
		}
		//三行高度下行
		OLED_Set_Point(x, y+2);
		for(i = 0; i < 12; i++)
		{
			OLED_WR_Byte(asc2_2412[c][i+24], OLED_DATA);
		}
	}
}



/*--------------------------------------------------------------------------------------*/
void OLED_ShowString(u8 x, u8 y, u8 *str, u8 Char_Size)
{
	u8 j = 0 , x_m = 8;
/*---------------确保显示不重叠------------------------*/
	if((Char_Size == 12)||(Char_Size == 16)||(Char_Size == 24))
	{
		x_m = Char_Size/2;
	}
	else
	{
		x_m = 6;
	}
/*----------------显示字符串的实现----------------------*/
	while(str[j] != '\0')	//读取到字符串结束符之前，一直循环
	{
		OLED_ShowChar(x, y, str[j], Char_Size);	//从第一个字符开始显示
		x += x_m;		//显示完一个字符后，起点x坐标右移8格，准备显示下一个字符
		if(x > 120) //如果起始x位置大于120，为保证字符显示完整，换行，在下一行显示
		{
			x = 0;
			y += 2;
		}
		j++;	//指针移到下一个字符
	}
}



/*--------------------------------------------------------------------------------------*/
u32 OLED_Pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size)
{
	u8 t, temp, x_m;
	u8 enshow = 0;
/*---------------确保显示不重叠------------------------*/
	if((size == 12)||(size == 16)||(size == 24))
	{
		x_m = size/2;
	}
	else
	{
		x_m = 6;
	}
/*----------------显示数字的实现-----------------------*/
	for(t = 0; t < len; t++)
	{
		temp = (num / OLED_Pow(10, len -t - 1)) % 10;	//取第t位数字
		if(enshow == 0 && t < (len - 1))	//如果数串长度小于指定长度
		{
			if(temp == 0)
			{
				OLED_ShowChar(x + (size / 2)*t, y, ' ', size);
				continue;			//继续循环判断
			}
			else					//直到有数字为止
			{
				enshow = 1;
			}
		}
		OLED_ShowChar(x + x_m * t, y, temp + '0', size);
	}
}	



/*--------------------------------------------------------------------------------------*/
void OLED_ShowChinese(u8 x, u8 y, u8 no)
{
	u8 t;
	//汉字上半部分坐标（第x行，第y列）
	OLED_Set_Point(x, y);
	for(t = 0; t < 16; t++)
	{
		OLED_WR_Byte(Hzk1[no][t], OLED_DATA);
	}
	//汉字下半部分坐标（第x行，第y+1列）
	OLED_Set_Point(x, y+1);
	for(t = 0; t < 16; t++)
	{
		OLED_WR_Byte(Hzk1[no][t + 16], OLED_DATA);
	}
}



/*--------------------------------------------------------------------------------------*/
void OLED_ShowBMP(u8 x0, u8 y0, u8 x1, u8 y1, u8 BMP[])
{
	u32 j = 0;
	u8 x, y;
	for(y = y0; y < y1; y++)
	{
		OLED_Set_Point(x0,y);
		for(x = x0; x < x1; x++)
		{
			OLED_WR_Byte(BMP[j++], OLED_DATA);
		}
	}
}



/*--------------------------------------------------------------------------------------*/
void OLED_Refresh(void)
{
	u8 x, y;
	u16 j = 0;
	for(y = 0; y < 8; y++)
	{
		OLED_Set_Point(0, y);
		for(x = 0; x < 128; x++)
		{
			OLED_WR_Byte(GRAM[j++], OLED_DATA);
		}
	}
}


/*--------------------------------------------------------------------------------------*/
void OLED_GFill(void)
{
	u16 t;
	for(t = 0; t < 1024; t++)
	{
		GRAM[t] = 0xFF;
	}
}
void OLED_GClear(void)
{
	u16 t;
	for(t = 0; t < 1024; t++)
	{
		GRAM[t] = 0x00;
	}
}


/*--------------------------------------------------------------------------------------*/
void OLED_DrawPoint(u8 x, u8 y)
{
	u8 n, m;
	if((x > 127) || (y > 63))	//画点超出屏幕边界时，退出程序
	{
		return;
	}		
	n = y / 8;
	m = y % 8;
	
	GRAM[n * 128 + x] |= 0x01 << m; 
}
void OLED_ClearPoint(u8 x, u8 y)
{
	u8 n, m;
	n = y / 8;
	m = y % 8;
	
	GRAM[n * 128 + x] &= ~(0x01 << m); 
}


/*--------------------------------------------------------------------------------------*/
void FourPoints(u8 xstart, u8 ystart)
{
	u8 x, y;
	for(y = ystart; y < 64; y += 2)
	{
		for(x = xstart; x < 128; x += 2)
		{
			GRAM[y / 8 * 128 + x] |= 0x01 << (y % 8);
		}
	}
}

void Mask_Gray(u8 xstart, u8 ystart)
{
	u8 x, y, z;
	for(y = ystart; y < 64; y += 2)
	{
		for(x = xstart; x < 128; x += 3)
		{
			z = BMP3[y / 8 * 128 + x] |= 0x01 << (y % 8);
			if(z == 1)
			{
				OLED_DrawPoint(x, y);
			}
		}
	}
	OLED_Refresh();
}



/*--------------------------------------------------------------------------------------*/
//OLED的初始化
void OLED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能A端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化PA0,1
 	GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1);
	
	OLED_WR_Byte(0xAE, OLED_CMD);//--关闭显示(进入睡眠模式)，准备配置
	
	/* 开始配置 */
	OLED_WR_Byte(0x20, OLED_CMD);//设置内存地址模式
	OLED_WR_Byte(0x02, OLED_CMD);//00h 水平地址模式，01h 垂直模式，02h 页地址模式
	
	OLED_WR_Byte(0x00, OLED_CMD);//---设置列起始地址低位
	OLED_WR_Byte(0x10, OLED_CMD);//---设置列起始地址高位
					   
	OLED_WR_Byte(0x40, OLED_CMD);//--设置起始行：显存中第几行作为屏幕的第一行，之前的内容补在屏幕下方
//	OLED_WR_Byte(0xB0, OLED_CMD);//--设置目标显示位置页起始地址：B0-B7
					   
	OLED_WR_Byte(0x81, OLED_CMD);//--设置对比度
	OLED_WR_Byte(0x01, OLED_CMD);//--对比度值：00-FFh，256级
					   
//	OLED_WR_Byte(0xA4, OLED_CMD);//--A4h显示现存内容，A5h无视现存内容点亮全屏
					   
	OLED_WR_Byte(0xA6, OLED_CMD);//--A6h正常显示，A7h反色显示
					   
	OLED_WR_Byte(0xA1, OLED_CMD);//--设置镜像显示    0xa0左右镜像 0xa1正常显示
					   
	OLED_WR_Byte(0xC8, OLED_CMD);//--设置镜像显示   	0xc0上下反置 0xc8正常显示
					   
	OLED_WR_Byte(0xA8, OLED_CMD);//--设置复用率(0-63)
	OLED_WR_Byte(0x3f, OLED_CMD);//--00h-3Fh
					   
	OLED_WR_Byte(0xD3, OLED_CMD);//--设置屏幕上下偏移 (0x00~0x3F)
	OLED_WR_Byte(0x00, OLED_CMD);//--00h-FFh
					   
	OLED_WR_Byte(0xD5, OLED_CMD);//--设置显示时钟分频/震荡频率（刷新率）
	OLED_WR_Byte(0x80, OLED_CMD);//--10h-F0h，1-F，越大速度越快（适合调节手机拍照）
					   
	OLED_WR_Byte(0xD9, OLED_CMD);//--设置预充电周期
	OLED_WR_Byte(0xF1, OLED_CMD);//--将预充电设置为15个时钟，放电设置为1个时钟
					   
	OLED_WR_Byte(0xDA, OLED_CMD);//--设置列引脚硬件配置
	OLED_WR_Byte(0x12, OLED_CMD);//--12864->0x12		12832->0x02
					   
	OLED_WR_Byte(0xDB, OLED_CMD);//--设置 vcomh 反压值
	OLED_WR_Byte(0x30, OLED_CMD);//--设置VCOM取消选择级别
					   
	OLED_WR_Byte(0x8D, OLED_CMD);//--设置电荷泵启用/禁用
	OLED_WR_Byte(0x14, OLED_CMD);//--设置为(0x10) 是禁用
	OLED_Clear();
	OLED_WR_Byte(0xAF, OLED_CMD);//开启显示（进入工作模式）
}


/*--------------------------------------------------------------------------------------*/
void OLED_Test(void)
{
//	OLED_On();
	
//	OLED_Set_Point(0,0);
//	OLED_WR_Byte(0xFF, OLED_DATA);
	
/*----------------------------------*/
//	OLED_Set_Point(3,0);	//0000 0001	-> 0x01
//	OLED_WR_Byte(0x01, OLED_DATA);
	
/*----------------------------------*/
//	OLED_Set_Point(6,0);
//	OLED_WR_Byte(0x0C, OLED_DATA);
//	OLED_WR_Byte(0x12, OLED_DATA);
//	OLED_WR_Byte(0x21, OLED_DATA);
//	OLED_WR_Byte(0x42, OLED_DATA);
//	OLED_WR_Byte(0x84, OLED_DATA);
//	OLED_WR_Byte(0x42, OLED_DATA);
//	OLED_WR_Byte(0x21, OLED_DATA);
//	OLED_WR_Byte(0x12, OLED_DATA);
//	OLED_WR_Byte(0x0C, OLED_DATA);
	
/*----------------------------------*/
//	OLED_Set_Point(6,2);
//	OLED_WR_Byte(0xF3, OLED_DATA);
//	OLED_WR_Byte(0xED, OLED_DATA);
//	OLED_WR_Byte(0xDE, OLED_DATA);
//	OLED_WR_Byte(0xBD, OLED_DATA);
//	OLED_WR_Byte(0x7B, OLED_DATA);
//	OLED_WR_Byte(0xBD, OLED_DATA);
//	OLED_WR_Byte(0xDE, OLED_DATA);
//	OLED_WR_Byte(0xED, OLED_DATA);
//	OLED_WR_Byte(0xF3, OLED_DATA);

/*----------------------------------*/
//	OLED_ShowChar( 0, 4, 'f', 8);
//	OLED_ShowChar( 24, 4, 'f', 12);
//	OLED_ShowChar( 50, 4, 'f', 16);
//	OLED_ShowChar( 80, 4, 'f', 24);

/*----------------------------------*/
//	OLED_ShowString(0, 0, (u8 *)"abcds", 8);
//	OLED_ShowString(0, 1, (u8 *)"abcds", 12);
//	OLED_ShowString(0, 3, (u8 *)"abcds", 16);
//	OLED_ShowString(0, 5, (u8 *)"abcds", 24);
	
/*----------------------------------*/	
//	OLED_ShowNum(0, 0, 1231, 4, 8);
//	OLED_ShowNum(0, 1, 1231, 4, 12);
//	OLED_ShowNum(0, 3, 1231, 4, 16);
//	OLED_ShowNum(0, 5, 1231, 4, 24);

/*----------------------------------*/
//	OLED_ShowChinese(0, 0, 1);

//	OLED_ShowBMP(0, 0, 128, 7, BMP1);

/*----------------------------------*/
//	u16 t;
//	for(t = 0; t < 1024; t++)
//	{
//		GRAM[t] = 0X02;
//	}
//	OLED_Refresh();

/*----------------------------------*/
//	OLED_GClear();
//	OLED_Refresh();
//	vTaskDelay(500);
//	OLED_GFill();
//	OLED_Refresh();
//	vTaskDelay(500);

/*----------------------------------*/
//	OLED_GClear();
//	OLED_DrawPoint(0 , 0 );
//	OLED_DrawPoint(8 , 8 );
//	OLED_DrawPoint(16, 16);
//	OLED_DrawPoint(24, 24);
//	OLED_DrawPoint(32, 32);
//	OLED_DrawPoint(40, 40);
//	OLED_DrawPoint(48, 48);
//	OLED_DrawPoint(56, 56);
//	OLED_DrawPoint(63, 63);

//	OLED_Refresh();

/*--------------------------------------------------------*/
//	OLED_DrawPoint(0 , 0 ); OLED_Refresh(); vTaskDelay(20);
//	OLED_DrawPoint(8 , 8 ); OLED_Refresh(); vTaskDelay(20);
//	OLED_DrawPoint(16, 16); OLED_Refresh(); vTaskDelay(20);
//	OLED_DrawPoint(24, 24); OLED_Refresh(); vTaskDelay(20);
//	OLED_DrawPoint(32, 32); OLED_Refresh(); vTaskDelay(20);
//	OLED_DrawPoint(40, 40); OLED_Refresh(); vTaskDelay(20);
//	OLED_DrawPoint(48, 48); OLED_Refresh(); vTaskDelay(20);
//	OLED_DrawPoint(56, 56); OLED_Refresh(); vTaskDelay(20);
//	OLED_DrawPoint(63, 63); OLED_Refresh(); vTaskDelay(20);
//	
//	
//	OLED_ClearPoint(0 , 0 ); OLED_Refresh(); vTaskDelay(20);
//	OLED_ClearPoint(8 , 8 ); OLED_Refresh(); vTaskDelay(20);
//	OLED_ClearPoint(16, 16); OLED_Refresh(); vTaskDelay(20);
//	OLED_ClearPoint(24, 24); OLED_Refresh(); vTaskDelay(20);
//	OLED_ClearPoint(32, 32); OLED_Refresh(); vTaskDelay(20);
//	OLED_ClearPoint(40, 40); OLED_Refresh(); vTaskDelay(20);
//	OLED_ClearPoint(48, 48); OLED_Refresh(); vTaskDelay(20);
//	OLED_ClearPoint(56, 56); OLED_Refresh(); vTaskDelay(20);
//	OLED_ClearPoint(63, 63); OLED_Refresh(); vTaskDelay(20);

/*--------------------------------------------------------*/
//	OLED_GClear();
//	FourPoints(0,0);
//	OLED_Refresh();
//	vTaskDelay(500);
//	
//	OLED_GClear();
//	FourPoints(1,0);
//	OLED_Refresh();
//	vTaskDelay(500);
//	
//	OLED_GClear();
//	FourPoints(1,1);
//	OLED_Refresh();
//	vTaskDelay(500);
//	
//	OLED_GClear();
//	FourPoints(0,1);
//	OLED_Refresh();
//	vTaskDelay(500);


/*--------------------------------------------------------*/
//	OLED_GClear();
//	OLED_Refresh();
//	vTaskDelay(500);
//	
//	FourPoints(0,0);
//	OLED_Refresh();
//	vTaskDelay(500);
//	
//	FourPoints(1,0);
//	OLED_Refresh();
//	vTaskDelay(500);
//	
//	FourPoints(1,1);
//	OLED_Refresh();
//	vTaskDelay(500);
//	
//	FourPoints(0,1);
//	OLED_Refresh();
//	vTaskDelay(500);

/*--------------------------------------------------------*/
}















