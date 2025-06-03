#include "sys.h"
#include "ads1015.h"
#include "reg_config.h"



//��ʼ��IIC I/O��
void ADS1015_gpio_init(void)
{					     
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(ADS1015_IIC_RCC_APB2Periph_GPIOx,ENABLE);//��ʹ������IO PORTCʱ�� 

	GPIO_InitStructure.GPIO_Pin = ADS1015_IIC_SCL_|ADS1015_IIC_SDA_;//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz
	GPIO_Init(ADS1015_IIC_GPIOx, &GPIO_InitStructure);					    //�����趨������ʼ��GPIO 

	ADS1015_IIC_SCL=1;
	ADS1015_IIC_SDA=1;
//	while(1);
	printf("ADS1015_GPIO_INIT_OK!!\r\n");
}


void ADS1015_SDA_IN(void) {//����SDAΪ����
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	ADS1015_IIC_RCC_APB2Periph_GPIOx, ENABLE );		
	GPIO_InitStructure.GPIO_Pin  = ADS1015_IIC_SDA_;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //GPIO_Mode_IN_FLOATING; //PA15���ó����룬Ĭ������ 
	GPIO_Init(ADS1015_IIC_GPIOx, &GPIO_InitStructure);//��ʼ��GPIOB8
}
void ADS1015_SDA_OUT(void) {//����SDAΪ���
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

//����IIC��ʼ�ź�
u8 ADS1015_IIC_Start(void)
{
	ADS1015_SDA_OUT();     //sda�����
	ADS1015_IIC_SDA=1;
	ADS1015_IIC_SCL=1;
    delay_us(2);
 	ADS1015_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
    delay_us(2);
    ADS1015_IIC_SCL=0;
    return 0;
}	  
//����IICֹͣ�ź�
void ADS1015_IIC_Stop(void)
{
	ADS1015_SDA_OUT();//sda�����
	ADS1015_IIC_SCL=0; 
	ADS1015_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
    delay_us(2);
	ADS1015_IIC_SCL=1; 
	delay_us(6);
	ADS1015_IIC_SDA=1;//����I2C���߽����ź�
    delay_us(6);	
	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
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
	ADS1015_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
}
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void ADS1015_IIC_Send_Byte(u8 txd)
{                        
	u8 t;   
	ADS1015_SDA_OUT(); 	    
    ADS1015_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
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
//��1���ֽ�  
//��SCL��������,��������
//����ֵ:����������
u8 ADS1015_IIC_Read_Byte(int ack)
{
	unsigned char i,receive=0;
	ADS1015_SDA_IN();//SDA����Ϊ����
    for(i=0;i<15;i++ )
	{
		ADS1015_IIC_SCL=0;
		ADS1015_IIC_delay();
		ADS1015_IIC_SCL=1;
        receive<<=1;
        if(ADS1015_READ_SDA)receive++; 	//������  
		ADS1015_IIC_delay();
		ADS1015_IIC_SCL=0;
    }	
	ADS1015_SDA_OUT();		//����SDAΪ���	
    if (!ack)
        ADS1015_IIC_NAck();//����nACK
    else
        ADS1015_IIC_Ack(); //����ACK  	
	
    return receive;
}


//ADS1015_�Ĵ�������***********************************************************************

/*ADS1015_2214���ú���
 *IICд2���ֽ� 
 *reg:�Ĵ�����ַ
 *data1:����1
 *data2:����2
 *����ֵ:0    ����
 *     ����  �������
*/
u8 write_ADS1015_16bit(u8 regaddr,u16 data)	//дdata���Ĵ�����regaddrΪ�Ĵ����ĵ�ַ
{
//	while(1){
//        ADS1015_IIC_Start(); 
//        ADS1015_IIC_Send_Byte(0x90);
//        delay_ms(100);
//        ADS1015_IIC_Wait_Ack();
//        delay_ms(200);
//    }
    
    ADS1015_IIC_Start(); 
	ADS1015_IIC_Send_Byte(ADS1015_ADDR_WRITE);	//�����豸��ַ��д
	while(ADS1015_IIC_Wait_Ack());
	ADS1015_IIC_delay();
    
	ADS1015_IIC_Send_Byte(regaddr);	//�Ĵ�����ַ
	while(ADS1015_IIC_Wait_Ack());
	ADS1015_IIC_delay();
	
	ADS1015_IIC_Send_Byte(data>>8);	//���͸߰�λ
	while(ADS1015_IIC_Wait_Ack());
	
	ADS1015_IIC_Send_Byte(data & 0xff);	//���͵Ͱ�λ
	while(ADS1015_IIC_Wait_Ack());
	
	ADS1015_IIC_Stop();//����һ��ֹͣ����
	return 0;
}

/*��ȡADS1015_2214�Ĵ�������
 *IIC��2���ֽ� 
 *reg:�Ĵ�����ַ 
 *����ֵ:����������
 */
//res=ADS1015_Read(MANUFACTURER_ID);
u16 read_ADS1015_16bit(u8 reg) {
	u16 res=0;
	u16 res1=0;
	u16 res2=0;
    
	ADS1015_IIC_Start(); 
	ADS1015_IIC_Send_Byte(ADS1015_ADDR_WRITE);//����д����	
	while(ADS1015_IIC_Wait_Ack());	//�ȴ�Ӧ��
    
	ADS1015_IIC_delay();	
    ADS1015_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
	ADS1015_IIC_delay();
    while(ADS1015_IIC_Wait_Ack());		//�ȴ�Ӧ��
    
    
    ADS1015_IIC_Start();
	ADS1015_IIC_Send_Byte(ADS1015_ADDR_READ);//����������ַ+������
	ADS1015_IIC_delay();
    while(ADS1015_IIC_Wait_Ack());	//�ȴ�Ӧ�� 
	
	
	
	res1=ADS1015_IIC_Read_Byte(1);//��ȡ���ݸ�λ,����ACK
	ADS1015_IIC_delay();
	res2=ADS1015_IIC_Read_Byte(0);//��ȡ���ݵ�λ,����nACK
    ADS1015_IIC_Stop();			//����һ��ֹͣ����
	
//	printf("res1= 0x%x, res2= 0x%x\r\n",res1,res2);
	
	res=(res1<<8)+ res2;
	return res;		
}



/*ADS1015_2214��ʼ������*/
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
	con |= CHAN_AIN0_GND;	//��ͨ��0	//	0x4000
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
    
	write_ADS1015_16bit(0x01, 0xC183);	//���üĴ���
	delay_ms(100);
//	res = read_ADS1015_16bit(0x01);	//��ȡ���úõļĴ���,�����óɹ���ô
//	printf("reg01=0X%x  ",res);
	delay_ms(100);
	res = read_ADS1015_16bit(0x00) << 4;	//��ȡ���úõļĴ���,�����óɹ���ô
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
	write_ADS1015_16bit(0x01, reg_normal_mode);	//���üĴ���
	printf("write 0x01\r\n");
//	delay_ms(100);
//	res = read_ADS1015_16bit(0x01);	//��ȡ���úõļĴ���,�����óɹ���ô
//	printf("reg01=0X%x  \r\n",res);
//	delay_ms(100);
	res = read_ADS1015_16bit(0x00) << 4;	//��ȡ���úõļĴ���,�����óɹ���ô
	printf("-----------------reg=%d\r\n",res/4096);
}


