#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "NRF24L01.h"
/***************************************************************
以下为NRF24L01接发送数据的模板

发送的数据格式如下

[第一位，第二位，第三位，第四位.....，第三十三位]

其中，第一位也就是Buf[0]位接收到的数据长度
第二位到第三十三位一个32个字节的数据位接受到的数据；

例子如下，5代表后面跟着5个数据
{5, 0x11, 0x22, 0x33, 0x44, 0x55};

********************By:普普通通Ordinary*************************/


int main(void)
{
	
	uint8_t Buf[32] = {5, 0x12, 0x22, 0x33, 0x44, 0x55};
 
	OLED_Init();
	NRF24L01_Init();
	OLED_ShowString(1, 1, "Init Successful");
	NRF24L01_SendBuf(Buf);
	
	while (1)
	{
		
	}
}
