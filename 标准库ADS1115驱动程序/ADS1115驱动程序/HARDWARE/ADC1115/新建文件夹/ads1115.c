#include "sys.h"
#include "stm32f10x_i2c.h"
#include "ads1115.h"
#include "usart.h"
#include "delay.h"

static void ADS1115_delay(u16 D)
{
	delay_us(2);
}

void delay_nms(u16 ms)
{
    delay_ms(ms);
}

void delay_nus(u16 us)
{
    delay_us(us);

}


/////////////////PA8 SDA////PA9 SCL///////////////////////////////////
void ADS1115_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC ,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;//A SCL SDA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

    ADS1115_IIC_SDA=1;
    ADS1115_IIC_SCL=1;
	delay_ms(5);
}


//I2C��������
void ADS1115_IIC_Start(void)
{
   ADS1115_IIC_SDA=1;
   delay_us(2);
   ADS1115_IIC_SCL=1;
   delay_us(2);
   ADS1115_IIC_SDA=0;
   delay_us(2);
   ADS1115_IIC_SCL=0;
   delay_us(2);
}

//I2Cֹͣ����
void ADS1115_IIC_Stop(void)
{
   ADS1115_IIC_SDA=0;
   delay_us(2);
   ADS1115_IIC_SCL=1;
   delay_us(2);
   ADS1115_IIC_SDA=1;
   delay_us(2);
}

void ADS1115_IIC_Send_Byte(u8 txd)
{                        
	u8 t;   
    ADS1115_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {      
        if(txd&0x80) ADS1115_IIC_SDA = 1;
        else ADS1115_IIC_SDA = 0;
        
		ADS1115_IIC_SCL=1;
		delay_us(2);
		ADS1115_IIC_SCL=0;	
		delay_us(2);
        txd = txd << 1;
    }	
    
	ADS1115_IIC_SDA = 1;
    ADS1115_IIC_SCL = 1;
    delay_us(2);
    ADS1115_IIC_SCL = 0;
} 


//I2C ��һ�ֽ�
u8 ADS1115_IIC_Recv_1Byte()
{
    u8 i,uReceiveByte = 0;
    for(i=0;i<8;i++)
    {
        ADS1115_IIC_SCL = 1;
        delay_us(2);
        uReceiveByte <<= 1;
        
        if(ADS1115_read_SDA) uReceiveByte |=0x01;
        ADS1115_IIC_SCL=0;
        delay_us(2);       
    }
    ADS1115_IIC_SCL = 0;
    delay_us(2);
    ADS1115_IIC_SDA = 0;
    delay_us(2);
    ADS1115_IIC_SCL = 1;
    delay_us(2);
    ADS1115_IIC_SCL = 0;
    delay_us(2);
    ADS1115_IIC_SDA = 1;
    return uReceiveByte;
}



/*********************************************************************
*��������:  ADS1115Config
*��	   ���� ����ADS1115����ͨ�����ã�����ʱ��ȵ�
*��	   ���� HCMD �������ָ�8λ(ͨ�������̣�ת��ģʽ)
			LCMD : �����ֵ�8λ(���������� �Ƚ�ģʽ ��Ч��ƽ �ź��������)
*��	   �أ� ��
********************************************************************/
void ADS1115Config_A(u8 LCMD,u8 HCMD)
{
    u8 i=0;
    u8 Initdata[4];

    Initdata[0] = WR_REG;  // ��ַ0x90  ����ADR�ӵ� д�Ĵ���
    Initdata[1] = CONF_REG;// ���üĴ���
    Initdata[2] = HCMD;    // �����ָ��ֽ�
    Initdata[3] = LCMD;    // �����ֵ��ֽ�
    ADS1115_IIC_SCL=1;
    ADS1115_IIC_Start();        //����
    for(i=0;i<4;i++)
    {
    	ADS1115_IIC_Send_Byte(Initdata[i]);
    	ADS1115_delay(10);
    }
    ADS1115_IIC_Stop();         //�ر�
}

void SetThresHold_A(u16 L_TH,u16 H_TH)        //�ߵͷ�������
{
   ADS1115_IIC_SCL=1;
   ADS1115_IIC_Start();      // ����
   ADS1115_IIC_Send_Byte(WR_REG);
   ADS1115_IIC_Send_Byte(LOTH_REG);//��ͷ�ֵ�Ĵ���
   ADS1115_IIC_Send_Byte((L_TH>>8));
   ADS1115_IIC_Send_Byte(L_TH);
   ADS1115_IIC_Stop();       //�ر�

   ADS1115_IIC_Start();     //����
   ADS1115_IIC_Send_Byte(WR_REG);
   ADS1115_IIC_Send_Byte(HITH_REG);//��߷�ֵ�Ĵ���
   ADS1115_IIC_Send_Byte((H_TH>>8));
   ADS1115_IIC_Send_Byte(H_TH);
   ADS1115_IIC_Stop();      //�ر�
}


/*******************************************************************
*��������:  ReadAD_A
*��	   ���� ��ȡADת����ֵ
*��	   ���� ��ȡ��ֵΪ��ǰ�����õ��Ǹ�ͨ��
*��	   �أ� ��
********************************************************************/
u16 ReadAD_A(void)
{
   u16 Data[2]={0,0};

   ADS1115_IIC_SCL=1;
   ADS1115_IIC_Start();
   ADS1115_IIC_Send_Byte(WR_REG);
   ADS1115_IIC_Send_Byte(DATA_REG);
   ADS1115_IIC_Stop();
   
   ADS1115_IIC_Start();
   ADS1115_IIC_Send_Byte(RE_REG);
   Data[0] = ADS1115_IIC_Recv_1Byte();
   Data[1] = ADS1115_IIC_Recv_1Byte();
   ADS1115_IIC_Stop();
   
   Data[0] = Data[0]<<8 | Data[1];
   return  (Data[0]);//&0x7fff
}

u16 getad(u8 LCMD,u8 HCMD)
{
   float value=0;
    ADS1115Config_A(LCMD,HCMD);		  //AINP = AIN0 and AINN = AIN1 (default)
    value=ReadAD_A();
    
    
    if(value>=0x8000)  
        value=((float)(0xffff-value)/32768.0)*4.096;
    else
        value=((float)value/32768.0)*4.096;
    
    printf("value = %f\r\n",value);
    return value;
}

u16 lvbo(u8 LCMD,u8 HCMD)        //��ƽ��ֵ
{
	int k;
	float U=0,temp;
	
	 
	for(k=0;k<500;k++)
	{
		U+=getad(LCMD,HCMD);	
	}
	temp=U;
		U=0;
	return (temp/500);	
}






