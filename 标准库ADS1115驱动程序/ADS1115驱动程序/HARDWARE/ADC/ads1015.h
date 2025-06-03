#ifndef __ADS1015_H__
#define __ADS1015_H__
#include "sys.h"


//����IO
#define ADS1015_IIC_RCC_APB2Periph_GPIOx RCC_APB2Periph_GPIOC
#define ADS1015_IIC_SCL_ GPIO_Pin_14
#define ADS1015_IIC_SDA_ GPIO_Pin_15
#define ADS1015_IIC_GPIOx GPIOC 

#define ADS1015_IIC_SCL    PCout(14) 	//SCL
#define ADS1015_IIC_SDA    PCout(15)   //���SDA	 
#define ADS1015_READ_SDA   PCin(15) 		//����SDA 

/*ADS1015    iic�ӵ�ַ*/
#define ADS1015_ADDR 0x48	//����λ����һλ����д ����  д0/ ��1

#define ADS1015_ADDR_WRITE 0x90 //1001 0000
#define ADS1015_ADDR_READ  0x91	//ADS1015_ADDR_WRITE|0X01

#define m_bitShift 4

#define reg_normal_mode 0xC183

//��غ�������
void ADS1015_gpio_init(void);	//GPIO��ʼ��
u8 write_ADS1015_16bit(u8 regaddr,u16 data);	//дdata���Ĵ�����regaddrΪ�Ĵ����ĵ�ַ
u16 read_ADS1015_16bit(u8 reg);	//��ADC�мĴ�����Ӧ�ĵ�ֵ
//���������õ�
int ADS1015_read_data(void);	//��ȡͨ��0,��Ϊ�������Ѿ�ѡ����ͨ��0
u8 ADS1015_init(void);	//���üĴ���
void test(void);

#endif
















