#include "lcd12864.h"
#include "menu.h"
#include "delay.h"

uint8_t menu1(void)	//һ���˵�
{
	uint8_t KeyNum;
	int flag = 0;
	show_tongduan(3);
	while(1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum == 'B')
		{
			flag++;
			if(flag > 2)
			{
				flag = 0;
			}
		}
		if(KeyNum == 'C')
		{
			flag--;
			if(flag < 0)
			{
				flag = 2;
			}
		}
		if(KeyNum == 'E')
		{
			menu2_set(flag);	//��������˵�
		}
		if(flag == 0)
		{
			mode = 1;
			show_tongduan(3);
		}
		else if(flag == 1)
		{
			mode = 2;
			if(count1 == 0)
			{
				HAL_GPIO_WritePin(Mode2_1_GPIO_Port, Mode2_1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mode2_2_GPIO_Port, Mode2_2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mode2_3_GPIO_Port, Mode2_3_Pin, GPIO_PIN_RESET);
			}
			if(count1 == 1)
			{
				HAL_GPIO_WritePin(Mode2_1_GPIO_Port, Mode2_1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mode2_2_GPIO_Port, Mode2_2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mode2_3_GPIO_Port, Mode2_3_Pin, GPIO_PIN_RESET);
			}
			if(count1 == 2)
			{
				HAL_GPIO_WritePin(Mode2_1_GPIO_Port, Mode2_1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mode2_2_GPIO_Port, Mode2_2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mode2_3_GPIO_Port, Mode2_3_Pin, GPIO_PIN_SET);
			}
			show_cichang(2.3,'S');
		}
		else if(flag == 2)
		{
			mode = 3;
			show_jueyuan();
		}
		delay_ms(1);
	}
}


uint8_t menu2_set(uint8_t flag1)	//�����˵�
{
	uint8_t KeyNum;
	int flag2 = 1;
	while(1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum == 'A')
		{
			return 0;
		}
		if(KeyNum == 'B')
		{
			flag2++;
			if(flag2 > 3)
			{
				flag2 = 0;
			}
		}
		if(KeyNum == 'C')
		{
			flag2--;
			if(flag2 < 1)
			{
				flag2 = 3;
			}
		}
		if(KeyNum == 'D')
		{
			menu3_set(flag2);	//���������˵�
		}
		show_set(flag2);
		delay_ms(1);
	}
}


uint8_t menu3_set(uint8_t flag2)		//�����˵�
{	
	uint8_t KeyNum;
	while(1)
	{
		if(flag2 == 1)	
		{	
			KeyNum = Key_GetNum();
			if(KeyNum == 'A')
			{
				return 0;
			}
			if(KeyNum == 'B')
			{
				
			}
			if(KeyNum == 'C')
			{
				
			}
			if(KeyNum == 'D')
			{
				
			}
			tongduan_set();
		}
		if(flag2 == 2)
		{
			KeyNum = Key_GetNum();
			if(KeyNum == 'A')
			{
				return 0;
			}
			if(KeyNum == 'B')
			{
			}
			if(KeyNum == 'C')
			{
			}
			if(KeyNum == 'D')
			{
				
			}
			cichang_set();
			delay_ms(1);
		}
	}
}




void show_tongduan(float num)
{
	LCD_Display_Words(0,0,(uint8_t *)"��ǰģʽ��ͨ��  ");
	LCD_Display_Words(1,0,(uint8_t *)"����ֵ��        ");
	LCD_Display_Words(2,0,(uint8_t *)"״̬��          ");
	LCD_Display_Words(3,0,(uint8_t *)"A:  B:  �ӵ�:   ");
}

void show_cichang(float num,char dir)
{
	LCD_Display_Words(0,0,(uint8_t *)"��ǰģʽ���ų�  ");
	LCD_Display_Words(1,0,(uint8_t *)"��˹ֵ��        ");
	if(dir == 'N')
	{
		LCD_Display_Words(2,0,(uint8_t *)"�ų����ԣ�N     ");
	}
	if(dir == 'S')
	{
		LCD_Display_Words(2,0,(uint8_t *)"�ų����ԣ�S     ");
	}
	LCD_Display_Words(3,0,(uint8_t *)"                ");
}

void show_jueyuan(void)
{
	LCD_Display_Words(0,0,(uint8_t *)"��ǰģʽ����Ե  ");
	LCD_Display_Words(1,0,(uint8_t *)"�������ݣ��缫��");
	LCD_Display_Words(2,0,(uint8_t *)"      ��Ȧ���ӵ�");
	LCD_Display_Words(3,0,(uint8_t *)"����ʱ��:       ");
}

void show_set(int flag2)
{
	if(flag2 == 1)
	{
		LCD_Display_Words(0,0,(uint8_t *)"  ͨ�ϲ�������< ");
		LCD_Display_Words(1,0,(uint8_t *)"  �ų���������  ");
		LCD_Display_Words(2,0,(uint8_t *)"  �ָ���������  ");
		LCD_Display_Words(3,0,(uint8_t *)"                ");
	}
	if(flag2 == 2)
	{
		LCD_Display_Words(0,0,(uint8_t *)"  ͨ�ϲ�������  ");
		LCD_Display_Words(1,0,(uint8_t *)"  �ų���������< ");
		LCD_Display_Words(2,0,(uint8_t *)"  �ָ���������  ");
		LCD_Display_Words(3,0,(uint8_t *)"                ");
	}
	if(flag2 == 3)
	{
		LCD_Display_Words(0,0,(uint8_t *)"  ͨ�ϲ�������  ");
		LCD_Display_Words(1,0,(uint8_t *)"  �ų���������  ");
		LCD_Display_Words(2,0,(uint8_t *)"  �ָ���������< ");
		LCD_Display_Words(3,0,(uint8_t *)"                ");
	}

}

void tongduan_set(void)
{
	LCD_Display_Words(0,0,(uint8_t *)"������ֵ���ޣ�  ");
	LCD_Display_Words(1,0,(uint8_t *)"������ֵ���ޣ�  ");
	LCD_Display_Words(2,0,(uint8_t *)"                ");
	LCD_Display_Words(3,0,(uint8_t *)"                ");
}

void cichang_set(void)
{
	LCD_Display_Words(0,0,(uint8_t *)"ͬ������        ");
	LCD_Display_Words(1,0,(uint8_t *)"����ͬ��        ");
	LCD_Display_Words(2,0,(uint8_t *)"��������NS      ");
	LCD_Display_Words(3,0,(uint8_t *)"��������SN      ");
}

