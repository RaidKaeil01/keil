#include "usartbo.h"

u8 send_first[2]={0x03,0xfc};//发送帧头
u8 send_last[2]={0xfc,0x03};//发送帧尾
u8 send_9999[2]={0x0f,0x27};//发送9999分割区间

/*//////////////////////////////////////////////////////////////////////////
  函数名:print_host
  功能:整合波形上位机发送帧
  输入:待显示信号
  返回:无
//////////////////////////////////////////////////////////////////////////*/

void print_host(u16 val,u16 a)
{
	printf("%c%c",send_first[0],send_first[1]);	
	printf("%c",val);
	printf("%c",val>>8);
	printf("%c",a);
	printf("%c",a>>8);
	printf("%c%c",send_last[0],send_last[1]);
}




