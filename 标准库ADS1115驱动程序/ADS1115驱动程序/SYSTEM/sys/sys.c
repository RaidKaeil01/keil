#include "sys.h"


void Sys_Init(u32 bound ){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();			//��ʱ��ʼ��
	usart_init(bound);
	
	//�������������ǹر�SWJ����ظ��� B3 5 A13 14 15
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//ʱ�ӳ�ʼ
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	
	printf("�Ѿ��ر�SWJ���Ÿ���\r\n");
}
//turn ������������ת��Ϊ�ַ���
//mΪҪת���������� 
//strΪ���������
//nΪ��ת���ĸ���
//t��־λ ��־�Ƿ�������λ
void turn(int m,char str[],int n)//n=265
{
	int i=0,j=1,k=0;
	int t=1;//�Ƿ��λ��־λ
	char c;
	if(m<0)//��¼����
	{	str[i++]='-';
		m=-m;
	}	
	do
	{	str[i]='0'+m%10;
		m=m/10;
		i++;
	}while(m>0);

	//�������
	if(str[0]=='-')j=1;
	else j=0;
	k=i-1;
	while(j<k)
	{
		c=str[j];
		str[j]=str[k];
		str[k]=c;
		j++;
		k--;
	}
	if(t)
	{
		while(i-1<=n)//��ѭ����ʣ��λ��Ū�ɿո�	
		{
			str[i]=' ';
			i++;
		}
	}
	str[i]='\0';
//	return (i-1);
}




void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
