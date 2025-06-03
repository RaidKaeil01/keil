#ifndef __ADS1015_H__
#define __ADS1015_H__
#include "sys.h"


//定义IO
#define ADS1015_IIC_RCC_APB2Periph_GPIOx RCC_APB2Periph_GPIOC
#define ADS1015_IIC_SCL_ GPIO_Pin_14
#define ADS1015_IIC_SDA_ GPIO_Pin_15
#define ADS1015_IIC_GPIOx GPIOC 

#define ADS1015_IIC_SCL    PCout(14) 	//SCL
#define ADS1015_IIC_SDA    PCout(15)   //输出SDA	 
#define ADS1015_READ_SDA   PCin(15) 		//输入SDA 

/*ADS1015    iic从地址*/
#define ADS1015_ADDR 0x48	//高七位，低一位用于写 读，  写0/ 读1

#define ADS1015_ADDR_WRITE 0x90 //1001 0000
#define ADS1015_ADDR_READ  0x91	//ADS1015_ADDR_WRITE|0X01

#define m_bitShift 4

#define reg_normal_mode 0xC183

//相关函数申明
void ADS1015_gpio_init(void);	//GPIO初始化
u8 write_ADS1015_16bit(u8 regaddr,u16 data);	//写data到寄存器，regaddr为寄存器的地址
u16 read_ADS1015_16bit(u8 reg);	//读ADC中寄存器对应的的值
//主函数调用的
int ADS1015_read_data(void);	//读取通道0,因为配置中已经选择了通道0
u8 ADS1015_init(void);	//配置寄存器
void test(void);

#endif
















