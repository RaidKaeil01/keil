#ifndef __ADS1115_H__
#define __ADS1115_H__
#include "sys.h"
#define ADS1115_IIC_SCL    PBout(8) 	//SCL
#define ADS1115_IIC_SDA    PBout(9)   //输出SDA	 
#define ADS1115_read_SDA   PBin(9) 		//输入SDA 

/*ADS1115    iic从地址*/
#define ADS1115_ADDR 0x48	//高七位，低一位用于写 读，  写0/ 读1

#define ADS1115_ADDR_WRITE 0x90 //1001 0000
#define ADS1115_ADDR_READ  0x91	//ADS1115_ADDR_WRITE|0X01

#define  DATA_REG  0x00		//转换数据寄存器
#define  CONF_REG  0x01     //控制字设置寄存器
#define  LOTH_REG  0x02		//最低阀值寄存器
#define  HITH_REG  0x03		//最高阀值寄存器

#define  ch0  0xc0       //通道0
#define  ch1  0xd0       //通道1
#define  ch2  0xe0       //通道2
#define  ch3  0xf0       //通道3

#define  HCMD1    0x64   //AIN0单端输入 +-4.096量程  连续模式  01000100b
#define  LCMD1	  0xf0	 //860sps 窗口比较器模式 输出低有效  不锁存信号至读 每周期检测阀值 11110000b

#define reg_normal_mode 0xC183

//相关函数申明
void ADS1115_gpio_init(void);	//GPIO初始化
void set_ADS1115_Config(u8 LCMD,u8 HCMD);
u16 get_AD(u8 LCMD,u8 HCMD);

#endif
















