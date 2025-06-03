#include "delay.h"
#include "Key.h"

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if (HAL_GPIO_ReadPin(KEY_A_GPIO_Port ,KEY_A_Pin) == 0)
	{
		delay_ms(20);
		while (HAL_GPIO_ReadPin(KEY_A_GPIO_Port ,KEY_A_Pin) == 0)
		{
			if (HAL_GPIO_ReadPin(KEY_D_GPIO_Port ,KEY_D_Pin) == 0)
			{
				while (HAL_GPIO_ReadPin(KEY_A_GPIO_Port ,KEY_A_Pin) == 0);
				while (HAL_GPIO_ReadPin(KEY_D_GPIO_Port ,KEY_D_Pin) == 0);
				return 'E';
			}
		}
		delay_ms(20);
		KeyNum = 'A';
	}
	if (HAL_GPIO_ReadPin(KEY_B_GPIO_Port ,KEY_B_Pin) == 0)
	{
		delay_ms(20);
		while (HAL_GPIO_ReadPin(KEY_B_GPIO_Port ,KEY_B_Pin) == 0);
		delay_ms(20);
		KeyNum = 'B';
	}
	if (HAL_GPIO_ReadPin(KEY_C_GPIO_Port ,KEY_C_Pin) == 0)
	{
		delay_ms(20);
		while (HAL_GPIO_ReadPin(KEY_C_GPIO_Port ,KEY_C_Pin) == 0);
		delay_ms(20);
		KeyNum = 'C';
	}
	if (HAL_GPIO_ReadPin(KEY_D_GPIO_Port ,KEY_D_Pin) == 0)
	{
		delay_ms(20);
		while (HAL_GPIO_ReadPin(KEY_D_GPIO_Port ,KEY_D_Pin) == 0)
		{
			if (HAL_GPIO_ReadPin(KEY_A_GPIO_Port ,KEY_A_Pin) == 0)
			{
				while (HAL_GPIO_ReadPin(KEY_D_GPIO_Port ,KEY_D_Pin) == 0);
				while (HAL_GPIO_ReadPin(KEY_A_GPIO_Port ,KEY_A_Pin) == 0);
				return 'E';
			}
		}
		delay_ms(20);
		KeyNum = 'D';
	}
	
	return KeyNum;
}
