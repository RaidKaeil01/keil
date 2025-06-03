#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"

extern unsigned char BMP3[];

u8 GRAM[1024];


//��ʱ
void IIC_delay(void)
{
	u8 t=3;
	while(t--);
}

//��ʼ�ź�
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

//�����ź�
void I2C_Stop(void)
{
	OLED_SDA_Clr();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SDA_Set();
}

//�ȴ��ź���Ӧ
void I2C_WaitAck(void) //�������źŵĵ�ƽ
{
	OLED_SDA_Set();
	IIC_delay();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SCL_Clr();
	IIC_delay();
}

//д��һ���ֽ�
void Send_Byte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(dat&0x80)	//��dat��8λ�����λ����д��
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
		OLED_SCL_Clr();	//��ʱ���ź�����Ϊ�͵�ƽ
		dat<<=1;
	}
}

/* void OLED_WR_Byte(u8 dat,u8 mode)
 * ����һ���ֽ�
 * mode:����/�����־ 0,��ʾ����;1,��ʾ����;*/
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


/******************************** OLED���ܺ��� ******************************************/
void OLED_Display_Turn(u8 turn)
{
	if(turn)
	{
		OLED_WR_Byte(0xAF, OLED_CMD);	//������ʾ
	}
	else
	{
		OLED_WR_Byte(0xAE, OLED_CMD);	//�ر���ʾ
	}
}



/*--------------------------------------------------------------------------------------*/
void OLED_Display_Color(u8 color)
{
	if(color)
	{
		OLED_WR_Byte(0xA7, OLED_CMD);	//������ʾ
	}
	else
	{
		OLED_WR_Byte(0xA6, OLED_CMD);	//������ʾ
	}
}



/*--------------------------------------------------------------------------------------*/
void OLED_Display_Dir(u8 dirX, u8 dirY)
{
	if(dirX)
	{
		OLED_WR_Byte(0xA1, OLED_CMD);	//���Ҿ���
	}
	else
	{
		OLED_WR_Byte(0xA6, OLED_CMD);	//������ʾ
	}

	if(dirY)
	{
		OLED_WR_Byte(0xC0, OLED_CMD);	//���¾���
	}
	else
	{
		OLED_WR_Byte(0xC8, OLED_CMD);	//������ʾ
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
		OLED_WR_Byte(0xB0 + y, OLED_CMD);	//����ҳ���У���ַ��0-7��
		OLED_WR_Byte(0x00, OLED_CMD);		//������ʾλ��--�е͵�ַ
		OLED_WR_Byte(0x10, OLED_CMD);		//������ʾλ��--�иߵ�ַ
		
		for(x = 0; x < 128; x++)			//��n�У���һ�����n��С���У�
		{
			OLED_WR_Byte(0x00, OLED_DATA);	//С���еİ˸���ȫ��Ϩ��
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
		for(x=0; x < 128; x++)		//��n�У���һ�����n��С���У�
		{
			OLED_WR_Byte(0xFF, OLED_DATA);	//С���еİ˸���ȫ������
		}
	}
}



/*--------------------------------------------------------------------------------------*/
void OLED_ShowChar( u8 x, u8 y, u8 chr, u8 Char_Size)
{
	u8 c = 0, i = 0;
	
	c = chr - ' ';	//���ݿո���ŵõ�ƫ�ƺ��ֵ
	
	if(x > 127)		//����������곬������Ļ���Ҳ�
	{
		x = 0;		//������x��0���ص���Ļ�����
		y = y + 2;	//�����y������2��
	}				//���ַ���ʾ����Ļָ���е���һ�����λ��
/*-----------------------------------------------------*/
	/* �ж��ֺŴ�С */
	if(Char_Size == 8)			//�ֿ⣺asc2_0806[][6]	ע���ֿ����λ��ÿ�е�ѭ����������0806��ÿ��ѭ��6��
	{
		OLED_Set_Point(x, y);
		for(i = 0; i < 6; i++)
		{
			OLED_WR_Byte(asc2_0806[c][i], OLED_DATA);
		}
	}
/*-----------------------------------------------------*/
	else if(Char_Size == 12)	//�ֿ⣺asc2_1206[95][12]
	{
		//���и߶�����
		OLED_Set_Point(x, y);
		for(i = 0; i < 6; i++)
		{
			OLED_WR_Byte(asc2_1206[c][i], OLED_DATA);
		}
		//���и߶�����
		OLED_Set_Point(x, y+1);
		for(i = 0; i < 6; i++)
		{
			OLED_WR_Byte(asc2_1206[c][i+6], OLED_DATA);
		}
	}
/*-----------------------------------------------------*/
	else if(Char_Size == 16)	//�ֿ⣺asc2_1608[][16]
	{
		//���и߶�����
		OLED_Set_Point(x, y);
		for(i = 0; i < 8; i++)
		{
			OLED_WR_Byte(asc2_1608[c][i], OLED_DATA);
		}
		//���и߶�����
		OLED_Set_Point(x, y+1);
		for(i = 0; i < 8; i++)
		{
			OLED_WR_Byte(asc2_1608[c][i+8], OLED_DATA);
		}
	}
/*-----------------------------------------------------*/
	else						//�ֿ⣺asc2_2412[][36]
	{
		//���и߶�����
		OLED_Set_Point(x, y);
		for(i = 0; i < 12; i++)
		{
			OLED_WR_Byte(asc2_2412[c][i], OLED_DATA);
		}
		//���и߶�����
		OLED_Set_Point(x, y+1);
		for(i = 0; i < 12; i++)
		{
			OLED_WR_Byte(asc2_2412[c][i+12], OLED_DATA);
		}
		//���и߶�����
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
/*---------------ȷ����ʾ���ص�------------------------*/
	if((Char_Size == 12)||(Char_Size == 16)||(Char_Size == 24))
	{
		x_m = Char_Size/2;
	}
	else
	{
		x_m = 6;
	}
/*----------------��ʾ�ַ�����ʵ��----------------------*/
	while(str[j] != '\0')	//��ȡ���ַ���������֮ǰ��һֱѭ��
	{
		OLED_ShowChar(x, y, str[j], Char_Size);	//�ӵ�һ���ַ���ʼ��ʾ
		x += x_m;		//��ʾ��һ���ַ������x��������8��׼����ʾ��һ���ַ�
		if(x > 120) //�����ʼxλ�ô���120��Ϊ��֤�ַ���ʾ���������У�����һ����ʾ
		{
			x = 0;
			y += 2;
		}
		j++;	//ָ���Ƶ���һ���ַ�
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
/*---------------ȷ����ʾ���ص�------------------------*/
	if((size == 12)||(size == 16)||(size == 24))
	{
		x_m = size/2;
	}
	else
	{
		x_m = 6;
	}
/*----------------��ʾ���ֵ�ʵ��-----------------------*/
	for(t = 0; t < len; t++)
	{
		temp = (num / OLED_Pow(10, len -t - 1)) % 10;	//ȡ��tλ����
		if(enshow == 0 && t < (len - 1))	//�����������С��ָ������
		{
			if(temp == 0)
			{
				OLED_ShowChar(x + (size / 2)*t, y, ' ', size);
				continue;			//����ѭ���ж�
			}
			else					//ֱ��������Ϊֹ
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
	//�����ϰ벿�����꣨��x�У���y�У�
	OLED_Set_Point(x, y);
	for(t = 0; t < 16; t++)
	{
		OLED_WR_Byte(Hzk1[no][t], OLED_DATA);
	}
	//�����°벿�����꣨��x�У���y+1�У�
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
	if((x > 127) || (y > 63))	//���㳬����Ļ�߽�ʱ���˳�����
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
//OLED�ĳ�ʼ��
void OLED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��A�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //��ʼ��PA0,1
 	GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1);
	
	OLED_WR_Byte(0xAE, OLED_CMD);//--�ر���ʾ(����˯��ģʽ)��׼������
	
	/* ��ʼ���� */
	OLED_WR_Byte(0x20, OLED_CMD);//�����ڴ��ַģʽ
	OLED_WR_Byte(0x02, OLED_CMD);//00h ˮƽ��ַģʽ��01h ��ֱģʽ��02h ҳ��ַģʽ
	
	OLED_WR_Byte(0x00, OLED_CMD);//---��������ʼ��ַ��λ
	OLED_WR_Byte(0x10, OLED_CMD);//---��������ʼ��ַ��λ
					   
	OLED_WR_Byte(0x40, OLED_CMD);//--������ʼ�У��Դ��еڼ�����Ϊ��Ļ�ĵ�һ�У�֮ǰ�����ݲ�����Ļ�·�
//	OLED_WR_Byte(0xB0, OLED_CMD);//--����Ŀ����ʾλ��ҳ��ʼ��ַ��B0-B7
					   
	OLED_WR_Byte(0x81, OLED_CMD);//--���öԱȶ�
	OLED_WR_Byte(0x01, OLED_CMD);//--�Աȶ�ֵ��00-FFh��256��
					   
//	OLED_WR_Byte(0xA4, OLED_CMD);//--A4h��ʾ�ִ����ݣ�A5h�����ִ����ݵ���ȫ��
					   
	OLED_WR_Byte(0xA6, OLED_CMD);//--A6h������ʾ��A7h��ɫ��ʾ
					   
	OLED_WR_Byte(0xA1, OLED_CMD);//--���þ�����ʾ    0xa0���Ҿ��� 0xa1������ʾ
					   
	OLED_WR_Byte(0xC8, OLED_CMD);//--���þ�����ʾ   	0xc0���·��� 0xc8������ʾ
					   
	OLED_WR_Byte(0xA8, OLED_CMD);//--���ø�����(0-63)
	OLED_WR_Byte(0x3f, OLED_CMD);//--00h-3Fh
					   
	OLED_WR_Byte(0xD3, OLED_CMD);//--������Ļ����ƫ�� (0x00~0x3F)
	OLED_WR_Byte(0x00, OLED_CMD);//--00h-FFh
					   
	OLED_WR_Byte(0xD5, OLED_CMD);//--������ʾʱ�ӷ�Ƶ/��Ƶ�ʣ�ˢ���ʣ�
	OLED_WR_Byte(0x80, OLED_CMD);//--10h-F0h��1-F��Խ���ٶ�Խ�죨�ʺϵ����ֻ����գ�
					   
	OLED_WR_Byte(0xD9, OLED_CMD);//--����Ԥ�������
	OLED_WR_Byte(0xF1, OLED_CMD);//--��Ԥ�������Ϊ15��ʱ�ӣ��ŵ�����Ϊ1��ʱ��
					   
	OLED_WR_Byte(0xDA, OLED_CMD);//--����������Ӳ������
	OLED_WR_Byte(0x12, OLED_CMD);//--12864->0x12		12832->0x02
					   
	OLED_WR_Byte(0xDB, OLED_CMD);//--���� vcomh ��ѹֵ
	OLED_WR_Byte(0x30, OLED_CMD);//--����VCOMȡ��ѡ�񼶱�
					   
	OLED_WR_Byte(0x8D, OLED_CMD);//--���õ�ɱ�����/����
	OLED_WR_Byte(0x14, OLED_CMD);//--����Ϊ(0x10) �ǽ���
	OLED_Clear();
	OLED_WR_Byte(0xAF, OLED_CMD);//������ʾ�����빤��ģʽ��
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















