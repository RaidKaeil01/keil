#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "NRF24L01.h"
/***************************************************************
以下为NRF24L01接收数据的模板

NRF24L01_Get_Value_Flag()返回0时，代表NRF24L01接收到了数据，此时调用
NRF24L01_GetRxBuf（）将得到的值储存在Buf这个数组中

接收到的数据格式如下

[第一位，第二位，第三位，第四位.....，第三十三位]

其中，第一位也就是Buf[0]位接收到的数据长度
第二位到第三十三位一个32个字节的数据位接受到的数据；


********************By:普普通通Ordinary*************************/


int main(void)
{
	
	uint8_t Buf[32] = {0};
 
	OLED_Init();
	NRF24L01_Init();
	OLED_ShowString(1, 1, "Len:");
	
	while (1)
	{
		if (NRF24L01_Get_Value_Flag() == 0)
		{
			NRF24L01_GetRxBuf(Buf);
		}
		
		OLED_ShowHexNum(1, 5, Buf[0], 2);
		OLED_ShowHexNum(2, 1, Buf[1], 2);
		OLED_ShowHexNum(2, 4, Buf[2], 2);
		OLED_ShowHexNum(3, 1, Buf[3], 2);
		OLED_ShowHexNum(3, 4, Buf[4], 2);
		OLED_ShowHexNum(4, 1, Buf[5], 2);
		OLED_ShowHexNum(4, 4, Buf[6], 2);
	
		
	}
}
