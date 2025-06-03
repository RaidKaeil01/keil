#ifndef __ADS1115_H__
#define __ADS1115_H__
#include "sys.h"
#define ADS1115_IIC_SCL    PBout(8) 	//SCL
#define ADS1115_IIC_SDA    PBout(9)   //���SDA	 
#define ADS1115_read_SDA   PBin(9) 		//����SDA 

/*ADS1115    iic�ӵ�ַ*/
#define ADS1115_ADDR 0x48	//����λ����һλ����д ����  д0/ ��1

#define ADS1115_ADDR_WRITE 0x90 //1001 0000
#define ADS1115_ADDR_READ  0x91	//ADS1115_ADDR_WRITE|0X01

#define  DATA_REG  0x00		//ת�����ݼĴ���
#define  CONF_REG  0x01     //���������üĴ���
#define  LOTH_REG  0x02		//��ͷ�ֵ�Ĵ���
#define  HITH_REG  0x03		//��߷�ֵ�Ĵ���

#define  ch0  0xc0       //ͨ��0
#define  ch1  0xd0       //ͨ��1
#define  ch2  0xe0       //ͨ��2
#define  ch3  0xf0       //ͨ��3

#define  HCMD1    0x64   //AIN0�������� +-4.096����  ����ģʽ  01000100b
#define  LCMD1	  0xf0	 //860sps ���ڱȽ���ģʽ �������Ч  �������ź����� ÿ���ڼ�ֵⷧ 11110000b

#define reg_normal_mode 0xC183

//��غ�������
void ADS1115_gpio_init(void);	//GPIO��ʼ��
void set_ADS1115_Config(u8 LCMD,u8 HCMD);
u16 get_AD(u8 LCMD,u8 HCMD);

#endif
















