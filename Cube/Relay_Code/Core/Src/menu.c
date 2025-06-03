#include "lcd12864.h"
#include "menu.h"
#include "delay.h"

uint8_t menu1(void)	//一级菜单
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
			menu2_set(flag);	//进入二级菜单
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


uint8_t menu2_set(uint8_t flag1)	//二级菜单
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
			menu3_set(flag2);	//进入三级菜单
		}
		show_set(flag2);
		delay_ms(1);
	}
}


uint8_t menu3_set(uint8_t flag2)		//三级菜单
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
	LCD_Display_Words(0,0,(uint8_t *)"当前模式：通断  ");
	LCD_Display_Words(1,0,(uint8_t *)"电阻值：        ");
	LCD_Display_Words(2,0,(uint8_t *)"状态：          ");
	LCD_Display_Words(3,0,(uint8_t *)"A:  B:  接地:   ");
}

void show_cichang(float num,char dir)
{
	LCD_Display_Words(0,0,(uint8_t *)"当前模式：磁场  ");
	LCD_Display_Words(1,0,(uint8_t *)"高斯值：        ");
	if(dir == 'N')
	{
		LCD_Display_Words(2,0,(uint8_t *)"磁场极性：N     ");
	}
	if(dir == 'S')
	{
		LCD_Display_Words(2,0,(uint8_t *)"磁场极性：S     ");
	}
	LCD_Display_Words(3,0,(uint8_t *)"                ");
}

void show_jueyuan(void)
{
	LCD_Display_Words(0,0,(uint8_t *)"当前模式：绝缘  ");
	LCD_Display_Words(1,0,(uint8_t *)"测量内容：电极、");
	LCD_Display_Words(2,0,(uint8_t *)"      线圈、接地");
	LCD_Display_Words(3,0,(uint8_t *)"测量时间:       ");
}

void show_set(int flag2)
{
	if(flag2 == 1)
	{
		LCD_Display_Words(0,0,(uint8_t *)"  通断测试设置< ");
		LCD_Display_Words(1,0,(uint8_t *)"  磁场测试设置  ");
		LCD_Display_Words(2,0,(uint8_t *)"  恢复出厂设置  ");
		LCD_Display_Words(3,0,(uint8_t *)"                ");
	}
	if(flag2 == 2)
	{
		LCD_Display_Words(0,0,(uint8_t *)"  通断测试设置  ");
		LCD_Display_Words(1,0,(uint8_t *)"  磁场测试设置< ");
		LCD_Display_Words(2,0,(uint8_t *)"  恢复出厂设置  ");
		LCD_Display_Words(3,0,(uint8_t *)"                ");
	}
	if(flag2 == 3)
	{
		LCD_Display_Words(0,0,(uint8_t *)"  通断测试设置  ");
		LCD_Display_Words(1,0,(uint8_t *)"  磁场测试设置  ");
		LCD_Display_Words(2,0,(uint8_t *)"  恢复出厂设置< ");
		LCD_Display_Words(3,0,(uint8_t *)"                ");
	}

}

void tongduan_set(void)
{
	LCD_Display_Words(0,0,(uint8_t *)"被测阻值上限：  ");
	LCD_Display_Words(1,0,(uint8_t *)"被测阻值上限：  ");
	LCD_Display_Words(2,0,(uint8_t *)"                ");
	LCD_Display_Words(3,0,(uint8_t *)"                ");
}

void cichang_set(void)
{
	LCD_Display_Words(0,0,(uint8_t *)"同级报警        ");
	LCD_Display_Words(1,0,(uint8_t *)"允许同级        ");
	LCD_Display_Words(2,0,(uint8_t *)"锁定方向NS      ");
	LCD_Display_Words(3,0,(uint8_t *)"锁定方向SN      ");
}

