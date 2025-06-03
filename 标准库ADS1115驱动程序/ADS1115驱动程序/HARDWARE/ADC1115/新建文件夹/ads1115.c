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


//I2C总线启动
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

//I2C停止总线
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
    ADS1115_IIC_SCL=0;//拉低时钟开始数据传输
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


//I2C 读一字节
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
*函数名称:  ADS1115Config
*描	   述： 设置ADS1115包括通道配置，采样时间等等
*参	   数： HCMD ：命令字高8位(通道，量程，转换模式)
			LCMD : 命令字低8位(采样率设置 比较模式 有效电平 信号输出锁存)
*返	   回； 无
********************************************************************/
void ADS1115Config_A(u8 LCMD,u8 HCMD)
{
    u8 i=0;
    u8 Initdata[4];

    Initdata[0] = WR_REG;  // 地址0x90  器件ADR接地 写寄存器
    Initdata[1] = CONF_REG;// 配置寄存器
    Initdata[2] = HCMD;    // 配置字高字节
    Initdata[3] = LCMD;    // 配置字低字节
    ADS1115_IIC_SCL=1;
    ADS1115_IIC_Start();        //开启
    for(i=0;i<4;i++)
    {
    	ADS1115_IIC_Send_Byte(Initdata[i]);
    	ADS1115_delay(10);
    }
    ADS1115_IIC_Stop();         //关闭
}

void SetThresHold_A(u16 L_TH,u16 H_TH)        //高低阀门设置
{
   ADS1115_IIC_SCL=1;
   ADS1115_IIC_Start();      // 开启
   ADS1115_IIC_Send_Byte(WR_REG);
   ADS1115_IIC_Send_Byte(LOTH_REG);//最低阀值寄存器
   ADS1115_IIC_Send_Byte((L_TH>>8));
   ADS1115_IIC_Send_Byte(L_TH);
   ADS1115_IIC_Stop();       //关闭

   ADS1115_IIC_Start();     //开启
   ADS1115_IIC_Send_Byte(WR_REG);
   ADS1115_IIC_Send_Byte(HITH_REG);//最高阀值寄存器
   ADS1115_IIC_Send_Byte((H_TH>>8));
   ADS1115_IIC_Send_Byte(H_TH);
   ADS1115_IIC_Stop();      //关闭
}


/*******************************************************************
*函数名称:  ReadAD_A
*描	   述： 获取AD转换的值
*参	   数： 获取的值为在前面设置的那个通道
*返	   回； 无
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

u16 lvbo(u8 LCMD,u8 HCMD)        //求平均值
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






