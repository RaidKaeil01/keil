#include "lcd1602.h"
#include "delay.h"


/* �������� ���� */
#define LCD1602_RS_GPIO_Port GPIOA
#define LCD1602_RS_Pin GPIO_Pin_5

#define LCD1602_RW_GPIO_Port GPIOA
#define LCD1602_RW_Pin GPIO_Pin_5

#define LCD1602_E_GPIO_Port GPIOA
#define LCD1602_E_Pin GPIO_Pin_4

// λ����
#define RS_DQ_OUT(x) do{ x ? \
                                GPIO_SetBits(LCD1602_RS_GPIO_Port, LCD1602_RS_Pin) : \
                                GPIO_ResetBits(LCD1602_RS_GPIO_Port, LCD1602_RS_Pin); \
                            }while(0)   

#define RW_DQ_OUT(x) do{ }while(0)   

#define E_DQ_OUT(x) do{ x ? \
                                GPIO_SetBits(LCD1602_E_GPIO_Port, LCD1602_E_Pin) : \
                                GPIO_ResetBits(LCD1602_E_GPIO_Port, LCD1602_E_Pin); \
                            }while(0)  

/* �������� ���� */
#define LCD1602_D4_GPIO_Port GPIOA
#define LCD1602_D4_Pin GPIO_Pin_3
                            
#define LCD1602_D5_GPIO_Port GPIOA
#define LCD1602_D5_Pin GPIO_Pin_2
                            
#define LCD1602_D6_GPIO_Port GPIOA
#define LCD1602_D6_Pin GPIO_Pin_1
                            
#define LCD1602_D7_GPIO_Port GPIOA
#define LCD1602_D7_Pin GPIO_Pin_0                             
                            
/*******************************************************************************
* �� �� ��       : lcd1602_write_4digitValue
* ��������		 : LCD1602д4λֵ
* ��    ��       : value��ֵ
* ��    ��    	 : ��
*******************************************************************************/                            
void lcd1602_write_4digitValue(unsigned char value)
{
    if(value & 0x10)
    {
        GPIO_SetBits(LCD1602_D4_GPIO_Port, LCD1602_D4_Pin);
    }
    else
    {
        GPIO_ResetBits(LCD1602_D4_GPIO_Port, LCD1602_D4_Pin);
    }
    
    if(value & 0x20)
    {
        GPIO_SetBits(LCD1602_D5_GPIO_Port, LCD1602_D5_Pin);
    }
    else
    {
        GPIO_ResetBits(LCD1602_D5_GPIO_Port, LCD1602_D5_Pin);
    }
    
    if(value & 0x40)
    {
        GPIO_SetBits(LCD1602_D6_GPIO_Port, LCD1602_D6_Pin);
    }
    else
    {
        GPIO_ResetBits(LCD1602_D6_GPIO_Port, LCD1602_D6_Pin);
    }
    
    if(value & 0x80)
    {
        GPIO_SetBits(LCD1602_D7_GPIO_Port, LCD1602_D7_Pin);
    }
    else
    {
        GPIO_ResetBits(LCD1602_D7_GPIO_Port, LCD1602_D7_Pin);
    }
}

/*******************************************************************************
* �� �� ��       : lcd1602_write_cmd
* ��������		 : LCD1602д����
* ��    ��       : cmd��ָ��
* ��    ��    	 : ��
*******************************************************************************/

void lcd1602_write_cmd(unsigned char cmd)
{
	RS_DQ_OUT(0);//ѡ������
	delay_ms(1);
	RW_DQ_OUT(0);//ѡ��д
	E_DQ_OUT(0);
	lcd1602_write_4digitValue(cmd);//׼������
	delay_us(10);
	E_DQ_OUT(1);//ʹ�ܽ�E��������д��
	delay_ms(1);
	E_DQ_OUT(0);//ʹ�ܽ�E���������д��
	delay_ms(1);

	lcd1602_write_4digitValue(cmd<<4);//׼������
	delay_us(10);
	E_DQ_OUT(1);//ʹ�ܽ�E��������д��
	delay_ms(1);
	E_DQ_OUT(0);//ʹ�ܽ�E���������д��
	delay_ms(1);	
}

/*******************************************************************************
* �� �� ��       : lcd1602_write_data
* ��������		 : LCD1602д����
* ��    ��       : dat������
* ��    ��    	 : ��
*******************************************************************************/

void lcd1602_write_data(unsigned char dat) 
{
	RS_DQ_OUT(1);//ѡ������
	delay_ms(1);
	RW_DQ_OUT(0);//ѡ��д
	E_DQ_OUT(0);
	lcd1602_write_4digitValue(dat);//׼������
	delay_us(10);
	E_DQ_OUT(1);//ʹ�ܽ�E��������д��
	delay_ms(1);
	E_DQ_OUT(0);//ʹ�ܽ�E���������д��
	delay_ms(1);

	lcd1602_write_4digitValue(dat<<4);//׼������
	delay_us(10);
	E_DQ_OUT(1);//ʹ�ܽ�E��������д��
	delay_ms(1);
	E_DQ_OUT(0);//ʹ�ܽ�E���������д��	
	delay_ms(1);
}

/*******************************************************************************
* �� �� ��       : lcd1602_init
* ��������		 : LCD1602��ʼ��
* ��    ��       : ��
* ��    ��    	 : ��
*******************************************************************************/
void lcd1602_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
    
	GPIO_InitStructure.GPIO_Pin = LCD1602_RS_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(LCD1602_RS_GPIO_Port, &GPIO_InitStructure); // ��ʼ������
    
	GPIO_InitStructure.GPIO_Pin = LCD1602_RW_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(LCD1602_RW_GPIO_Port, &GPIO_InitStructure); // ��ʼ������
    
    GPIO_InitStructure.GPIO_Pin = LCD1602_E_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(LCD1602_E_GPIO_Port, &GPIO_InitStructure); // ��ʼ������
    
    GPIO_InitStructure.GPIO_Pin = LCD1602_D4_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(LCD1602_D4_GPIO_Port, &GPIO_InitStructure); // ��ʼ������
    
    GPIO_InitStructure.GPIO_Pin = LCD1602_D5_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(LCD1602_D5_GPIO_Port, &GPIO_InitStructure); // ��ʼ������
    
    GPIO_InitStructure.GPIO_Pin = LCD1602_D6_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(LCD1602_D6_GPIO_Port, &GPIO_InitStructure); // ��ʼ������
    
    GPIO_InitStructure.GPIO_Pin = LCD1602_D7_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(LCD1602_D7_GPIO_Port, &GPIO_InitStructure); // ��ʼ������
    
    lcd1602_write_cmd(0x01); // ����
    lcd1602_write_cmd(0x02); // ��ʾģʽ����긴λ
	lcd1602_write_cmd(0x28); // ��������8λ����ʾ2�У�5*7����/�ַ�
	lcd1602_write_cmd(0x0c); // ��ʾ���ܿ����޹�꣬�����˸
	lcd1602_write_cmd(0x06); // д�������ݺ������ƣ���ʾ�����ƶ�
	lcd1602_write_cmd(0x01); // ����	
    lcd1602_user_char(); // д���Զ����ַ�
}


/*******************************************************************************
* �� �� ��       : lcd1602_clear
* ��������		 : LCD1602����
* ��    ��       : ��
* ��    ��    	 : ��
*******************************************************************************/
void lcd1602_clear(void)
{
	lcd1602_write_cmd(0x01);	
}

/*******************************************************************************
* �� �� ��       : lcd1602_show_char
* ��������		 : LCD1602��ʾ�ַ�
* ��    ��       : x,y����ʾ���꣬x=0~15��y=0~1;
				   chr����ʾ�ַ�
* ��    ��    	 : ��
*******************************************************************************/
void lcd1602_show_char(unsigned char x,unsigned char y,unsigned char chr) 
{     
    if (y == 0) 
    {     
        lcd1602_write_cmd(0x80 + x); //��һ����ʾ��ַ����	    
    }    
    else 
    {     
        lcd1602_write_cmd(0xC0 + x); //�ڶ�����ʾ��ַ����	    
    }        
    lcd1602_write_data(chr); //��ʾ���� 
}

/*******************************************************************************
* �� �� ��       : lcd1602_user_char
* ��������		 : LCD1602д���Զ����ַ�
* ��    ��       : 
				   
* ��    ��    	 : ��
*******************************************************************************/

void lcd1602_user_char(void)
{
    //��һ���Զ����ַ�
    lcd1602_write_cmd(0x40); //"01 000 000"  ��1�е�ַ (D7D6Ϊ��ַ�趨������ʽ�DD5D4D3Ϊ�ַ����λ��(0--7)��D2D1D0Ϊ�ַ��е�ַ(0--7)��
    lcd1602_write_data(0x00); //"XXX 11111" ��1�����ݣ�D7D6D5ΪXXX����ʾΪ������(һ����000����D4D3D2D1D0Ϊ�ַ�������(1-������0-Ϩ��
    lcd1602_write_cmd(0x41); //"01 000 001"  ��2�е�ַ
    lcd1602_write_data(0x04); //"XXX 10001" ��2������
    lcd1602_write_cmd(0x42); //"01 000 010"  ��3�е�ַ
    lcd1602_write_data(0x0e); //"XXX 10101" ��3������
    lcd1602_write_cmd(0x43); //"01 000 011"  ��4�е�ַ
    lcd1602_write_data(0x0e); //"XXX 10001" ��4������
    lcd1602_write_cmd(0x44); //"01 000 100"  ��5�е�ַ
    lcd1602_write_data(0x0e); //"XXX 11111" ��5������
    lcd1602_write_cmd(0x45); //"01 000 101"  ��6�е�ַ
    lcd1602_write_data(0x1f); //"XXX 01010" ��6������
    lcd1602_write_cmd(0x46); //"01 000 110"  ��7�е�ַ
    lcd1602_write_data(0x04); //"XXX 11111" ��7������
    lcd1602_write_cmd(0x47); //"01 000 111"  ��8�е�ַ
    lcd1602_write_data(0x00); //"XXX 00000" ��8������ 
    //�ڶ����Զ����ַ�

    lcd1602_write_cmd(0x48); //"01 001 000"  ��1�е�ַ  
    lcd1602_write_data(0x03); //"XXX 00001" ��1������ 
    lcd1602_write_cmd(0x49); //"01 001 001"  ��2�е�ַ
    lcd1602_write_data(0x03); //"XXX 11011" ��2������
    lcd1602_write_cmd(0x4a); //"01 001 010"  ��3�е�ַ
    lcd1602_write_data(0x00); //"XXX 11101" ��3������
    lcd1602_write_cmd(0x4b); //"01 001 011"  ��4�е�ַ
    lcd1602_write_data(0x00); //"XXX 11001" ��4������
    lcd1602_write_cmd(0x4c); //"01 001 100"  ��5�е�ַ
    lcd1602_write_data(0x00); //"XXX 11101" ��5������
    lcd1602_write_cmd(0x4d); //"01 001 101"  ��6�е�ַ
    lcd1602_write_data(0x00); //"XXX 11011" ��6������
    lcd1602_write_cmd(0x4e); //"01 001 110"  ��7�е�ַ
    lcd1602_write_data(0x00); //"XXX 00001" ��7������
    lcd1602_write_cmd(0x4f); //"01 001 111"  ��8�е�ַ
    lcd1602_write_data(0x00); //"XXX 00000" ��8������ 
}

/*******************************************************************************
* �� �� ��       : lcd1602_show_string
* ��������		 : LCD1602��ʾ�ַ�
* ��    ��       : x,y����ʾ���꣬x=0~15��y=0~1;
				   str����ʾ�ַ���
* ��    ��    	 : ��
*******************************************************************************/
void lcd1602_show_string(unsigned char x,unsigned char y,unsigned char *str)
{
	unsigned char i=0;

	if(y>1||x>15)return;//���в���������ǿ���˳�

	if(y<1)	//��1����ʾ
	{	
		while(*str!='\0')//�ַ�������'\0'��β��ֻҪǰ�������ݾ���ʾ
		{
			if(i<16-x)//����ַ����ȳ�����һ����ʾ��Χ�����ڵڶ��м�����ʾ
			{
				lcd1602_write_cmd(0x80+i+x); //��һ����ʾ��ַ����	
			}
			else
			{
				lcd1602_write_cmd(0x40+0x80+i+x-16);//�ڶ�����ʾ��ַ����	
			}
			lcd1602_write_data(*str); //��ʾ����
			str++;//ָ�����
			i++;	
		}	
	}
	else	//��2����ʾ
	{
		while(*str!='\0')
		{
			if(i<16-x) //����ַ����ȳ����ڶ�����ʾ��Χ�����ڵ�һ�м�����ʾ
			{
				lcd1602_write_cmd(0x80+0x40+i+x);	
			}
			else
			{
				lcd1602_write_cmd(0x80+i+x-16);	
			}
			lcd1602_write_data(*str);
			str++;
			i++;	
		}	
	}				
}
