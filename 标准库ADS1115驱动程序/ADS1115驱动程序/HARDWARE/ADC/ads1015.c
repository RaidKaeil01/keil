#include "sys.h"
#include "ads1015.h"
#include "reg_config.h"



//初始化IIC I/O口
void ADS1015_gpio_init(void)
{					     
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(ADS1015_IIC_RCC_APB2Periph_GPIOx,ENABLE);//先使能外设IO PORTC时钟 

	GPIO_InitStructure.GPIO_Pin = ADS1015_IIC_SCL_|ADS1015_IIC_SDA_;//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
	GPIO_Init(ADS1015_IIC_GPIOx, &GPIO_InitStructure);					    //根据设定参数初始化GPIO 

	ADS1015_IIC_SCL=1;
	ADS1015_IIC_SDA=1;
//	while(1);
	printf("ADS1015_GPIO_INIT_OK!!\r\n");
}


void ADS1015_SDA_IN(void) {//设置SDA为输入
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	ADS1015_IIC_RCC_APB2Periph_GPIOx, ENABLE );		
	GPIO_InitStructure.GPIO_Pin  = ADS1015_IIC_SDA_;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //GPIO_Mode_IN_FLOATING; //PA15设置成输入，默认上拉 
	GPIO_Init(ADS1015_IIC_GPIOx, &GPIO_InitStructure);//初始化GPIOB8
}
void ADS1015_SDA_OUT(void) {//设置SDA为输出
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	ADS1015_IIC_RCC_APB2Periph_GPIOx, ENABLE );	
	GPIO_InitStructure.GPIO_Pin = ADS1015_IIC_SDA_;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP ;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADS1015_IIC_GPIOx, &GPIO_InitStructure);
}

void ADS1015_IIC_delay(){
	delay_us(5);
}

//产生IIC起始信号
u8 ADS1015_IIC_Start(void)
{
	ADS1015_SDA_OUT();     //sda线输出
	ADS1015_IIC_SDA=1;
	ADS1015_IIC_SCL=1;
    delay_us(2);
 	ADS1015_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
    delay_us(2);
    ADS1015_IIC_SCL=0;
    return 0;
}	  
//产生IIC停止信号
void ADS1015_IIC_Stop(void)
{
	ADS1015_SDA_OUT();//sda线输出
	ADS1015_IIC_SCL=0; 
	ADS1015_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
    delay_us(2);
	ADS1015_IIC_SCL=1; 
	delay_us(6);
	ADS1015_IIC_SDA=1;//发送I2C总线结束信号
    delay_us(6);	
	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 ADS1015_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
    ADS1015_SDA_IN();
	ADS1015_IIC_SDA=1;//delay_us(7); 
	ADS1015_IIC_SCL=1;//delay_us(10); 
	while(ADS1015_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			ADS1015_IIC_Stop();
			return 1;
		}
        printf("error003\r\n");
	}
//    delay_ms(900);
	ADS1015_IIC_SCL=0;//时钟输出0 	   
	return 0;  
}
//产生ACK应答
void ADS1015_IIC_Ack(void)
{
	ADS1015_IIC_SCL=0;
	ADS1015_SDA_OUT();
	ADS1015_IIC_SDA=0;
    
	delay_us(2);
	ADS1015_IIC_SCL=1;
	delay_us(5);
	ADS1015_IIC_SCL=0;
}
//不产生ACK应答		    
void ADS1015_IIC_NAck(void)
{
	ADS1015_IIC_SCL=0;
	ADS1015_SDA_OUT();
	ADS1015_IIC_SDA=1;
	delay_us(2);
	ADS1015_IIC_SCL=1;
	delay_us(5);
	ADS1015_IIC_SCL=0;
}		

/**********************************************************************************************************/
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void ADS1015_IIC_Send_Byte(u8 txd)
{                        
	u8 t;   
	ADS1015_SDA_OUT(); 	    
    ADS1015_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {      
        ADS1015_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	
		ADS1015_IIC_delay();
		ADS1015_IIC_SCL=1;
		ADS1015_IIC_delay(); 
		ADS1015_IIC_SCL=0;	
		ADS1015_IIC_delay();
    }	
	
} 	    
//读1个字节  
//在SCL的上升沿,数据锁存
//返回值:读到的数据
u8 ADS1015_IIC_Read_Byte(int ack)
{
	unsigned char i,receive=0;
	ADS1015_SDA_IN();//SDA设置为输入
    for(i=0;i<15;i++ )
	{
		ADS1015_IIC_SCL=0;
		ADS1015_IIC_delay();
		ADS1015_IIC_SCL=1;
        receive<<=1;
        if(ADS1015_READ_SDA)receive++; 	//读数据  
		ADS1015_IIC_delay();
		ADS1015_IIC_SCL=0;
    }	
	ADS1015_SDA_OUT();		//设置SDA为输出	
    if (!ack)
        ADS1015_IIC_NAck();//发送nACK
    else
        ADS1015_IIC_Ack(); //发送ACK  	
	
    return receive;
}


//ADS1015_寄存器操作***********************************************************************

/*ADS1015_2214设置函数
 *IIC写2个字节 
 *reg:寄存器地址
 *data1:数据1
 *data2:数据2
 *返回值:0    正常
 *     其他  错误代码
*/
u8 write_ADS1015_16bit(u8 regaddr,u16 data)	//写data到寄存器，regaddr为寄存器的地址
{
//	while(1){
//        ADS1015_IIC_Start(); 
//        ADS1015_IIC_Send_Byte(0x90);
//        delay_ms(100);
//        ADS1015_IIC_Wait_Ack();
//        delay_ms(200);
//    }
    
    ADS1015_IIC_Start(); 
	ADS1015_IIC_Send_Byte(ADS1015_ADDR_WRITE);	//发送设备地址，写
	while(ADS1015_IIC_Wait_Ack());
	ADS1015_IIC_delay();
    
	ADS1015_IIC_Send_Byte(regaddr);	//寄存器地址
	while(ADS1015_IIC_Wait_Ack());
	ADS1015_IIC_delay();
	
	ADS1015_IIC_Send_Byte(data>>8);	//发送高八位
	while(ADS1015_IIC_Wait_Ack());
	
	ADS1015_IIC_Send_Byte(data & 0xff);	//发送低八位
	while(ADS1015_IIC_Wait_Ack());
	
	ADS1015_IIC_Stop();//产生一个停止条件
	return 0;
}

/*读取ADS1015_2214寄存器数据
 *IIC读2个字节 
 *reg:寄存器地址 
 *返回值:读到的数据
 */
//res=ADS1015_Read(MANUFACTURER_ID);
u16 read_ADS1015_16bit(u8 reg) {
	u16 res=0;
	u16 res1=0;
	u16 res2=0;
    
	ADS1015_IIC_Start(); 
	ADS1015_IIC_Send_Byte(ADS1015_ADDR_WRITE);//发送写命令	
	while(ADS1015_IIC_Wait_Ack());	//等待应答
    
	ADS1015_IIC_delay();	
    ADS1015_IIC_Send_Byte(reg);	//写寄存器地址
	ADS1015_IIC_delay();
    while(ADS1015_IIC_Wait_Ack());		//等待应答
    
    
    ADS1015_IIC_Start();
	ADS1015_IIC_Send_Byte(ADS1015_ADDR_READ);//发送器件地址+读命令
	ADS1015_IIC_delay();
    while(ADS1015_IIC_Wait_Ack());	//等待应答 
	
	
	
	res1=ADS1015_IIC_Read_Byte(1);//读取数据高位,发送ACK
	ADS1015_IIC_delay();
	res2=ADS1015_IIC_Read_Byte(0);//读取数据低位,发送nACK
    ADS1015_IIC_Stop();			//产生一个停止条件
	
//	printf("res1= 0x%x, res2= 0x%x\r\n",res1,res2);
	
	res=(res1<<8)+ res2;
	return res;		
}



/*ADS1015_2214初始化函数*/
u8 ADS1015_init(void)
{
	u16 res;
	char str[6];
	unsigned int con=0;
	int ret;
	ADS1015_gpio_init();
	
	/*-------------------------------------------------------------------------------------------------------------------------------------*
	*/
	con |= REG_STARTCONVERSION;			//	0x8000
	con |= CHAN_AIN0_GND;	//单通道0	//	0x4000
	con |= PGA_FS6144;					//	0x0000
	con |= OPERATIONAL_CONT;			//	0x0000
	con |= DR_1600SPS;                  //	0x0080
	con |= MODE_TRADITIONAL_COM;        //	0x0000
	con |= ACTIVE_LOW;                  //	0x0000
	con |= NON_LATCHING;                //	0x0000
	con |= DIS_CONVERSION;              //	0x0003
	printf("con = 0x%x\r\n",con);
	
while(1){
	ADS1015_gpio_init();//delay_ms(300);
    
	write_ADS1015_16bit(0x01, 0xC183);	//配置寄存器
	delay_ms(100);
//	res = read_ADS1015_16bit(0x01);	//读取配置好的寄存器,看配置成功了么
//	printf("reg01=0X%x  ",res);
	delay_ms(100);
	res = read_ADS1015_16bit(0x00) << 4;	//读取配置好的寄存器,看配置成功了么
	printf("-----------------reg=0X%x\r\n",res);

}	
	printf("ADS1015_init_OK:	reg = 0x%x\r\n",res);
	
}


int ADS1015_read_data(void)
{
	u8 adc_data=0;
	adc_data =  read_ADS1015_16bit(CONVERSIONREG);
	printf("ad  =  %d \r\n",adc_data);
	
	return adc_data;
}

void test(void){
	u16 res=0;
	ADS1015_gpio_init();
    delay_ms(300);
	printf("star write 0x01\r\n");
	write_ADS1015_16bit(0x01, reg_normal_mode);	//配置寄存器
	printf("write 0x01\r\n");
//	delay_ms(100);
//	res = read_ADS1015_16bit(0x01);	//读取配置好的寄存器,看配置成功了么
//	printf("reg01=0X%x  \r\n",res);
//	delay_ms(100);
	res = read_ADS1015_16bit(0x00) << 4;	//读取配置好的寄存器,看配置成功了么
	printf("-----------------reg=%d\r\n",res/4096);
}


