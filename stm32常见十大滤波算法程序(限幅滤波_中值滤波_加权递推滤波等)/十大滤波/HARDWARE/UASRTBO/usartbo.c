#include "usartbo.h"

u8 send_first[2]={0x03,0xfc};//����֡ͷ
u8 send_last[2]={0xfc,0x03};//����֡β
u8 send_9999[2]={0x0f,0x27};//����9999�ָ�����

/*//////////////////////////////////////////////////////////////////////////
  ������:print_host
  ����:���ϲ�����λ������֡
  ����:����ʾ�ź�
  ����:��
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




