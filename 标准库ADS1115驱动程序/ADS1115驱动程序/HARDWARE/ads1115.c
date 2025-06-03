#include "sys.h"
#include "ads1115.h"

//��ʼ��IIC I/O��
void ADS1115_gpio_init(void)
{					     
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//��ʹ������IO PORTCʱ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					    //�����趨������ʼ��GPIO 

	ADS1115_IIC_SCL=1;
	ADS1115_IIC_SDA=1;
	printf("ADS1115_GPIO_INIT_OK!!\r\n");
}

void ADS1115_IIC_delay(){
	delay_us(2);
}

//����IIC��ʼ�ź�
u8 ADS1115_IIC_Start(void)
{
	ADS1115_IIC_SDA=1;
    delay_us(2);
	ADS1115_IIC_SCL=1;
    delay_us(2);
 	ADS1115_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
    delay_us(2);
    ADS1115_IIC_SCL=0;
    delay_us(2);
    return 0;
}	  
//����IICֹͣ�ź�
void ADS1115_IIC_Stop(void)
{
	ADS1115_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
    delay_us(2);
	ADS1115_IIC_SCL=1; 
	delay_us(2);
	ADS1115_IIC_SDA=1;//����I2C���߽����ź�
    delay_us(2);	
	
}

/**********************************************************************************************************/
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
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

//---------------------------------------------------------------------------19.7.22�޸�
u8 ADS1115_IIC_Recv_1Byte()
{
    u8 i,uReceiveByte = 0;
    for(i=0;i<8;i++)
    {
        ADS1115_IIC_SCL = 1;
        delay_us(2);
        uReceiveByte <<= 1;
        
        if(ADS1115_IIC_SDA) uReceiveByte |=0x01;
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

void set_ADS1115_Config(u8 LCMD,u8 HCMD)
{
    u8 i=0;
    u8 Initdata[4];

    Initdata[0] = ADS1115_ADDR_WRITE;   // ��ַ0x90  ����ADR�ӵ� д�Ĵ���
    Initdata[1] = CONF_REG;// ���üĴ��� 0x01
    Initdata[2] = HCMD;    // �����ָ��ֽ�    
    Initdata[3] = LCMD;    // �����ֵ��ֽ�
    ADS1115_IIC_SCL = 1;
    ADS1115_IIC_Start();        //����
    for(i=0;i<4;i++)
    {
    	ADS1115_IIC_Send_Byte(Initdata[i]);
    	delay_us(3);
    }
    ADS1115_IIC_Stop();         //�ر�
}


void SetThresHold_A(u16 L_TH,u16 H_TH)        //�ߵͷ�������
{
   ADS1115_IIC_SCL = 1;
   ADS1115_IIC_Start();      // ����
   ADS1115_IIC_Send_Byte(ADS1115_ADDR_WRITE);
   ADS1115_IIC_Send_Byte(LOTH_REG);//��ͷ�ֵ�Ĵ���
   ADS1115_IIC_Send_Byte((L_TH>>8));
   ADS1115_IIC_Send_Byte(L_TH);
   ADS1115_IIC_Stop();       //�ر�

   ADS1115_IIC_Start();     //����
   ADS1115_IIC_Send_Byte(ADS1115_ADDR_WRITE);
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

   ADS1115_IIC_SCL = 1;
   ADS1115_IIC_Start();
   ADS1115_IIC_Send_Byte(ADS1115_ADDR_WRITE);
   ADS1115_IIC_Send_Byte(DATA_REG);
   ADS1115_IIC_Stop();
   
   ADS1115_IIC_Start();
   ADS1115_IIC_Send_Byte(ADS1115_ADDR_READ);
   Data[0] = ADS1115_IIC_Recv_1Byte();
   Data[1] = ADS1115_IIC_Recv_1Byte();
   ADS1115_IIC_Stop();
   
   Data[0] = Data[0]<<8 | Data[1];
   return  (Data[0]);//&0x7fff
}

u16 get_AD(u8 LCMD,u8 HCMD)
{
   float value=0;
    set_ADS1115_Config(LCMD,HCMD);		  //AINP = AIN0 and AINN = AIN1 (default)
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
		U+=get_AD(LCMD,HCMD);	
	}
	temp=U;
		U=0;
	return (temp/500);	
}


