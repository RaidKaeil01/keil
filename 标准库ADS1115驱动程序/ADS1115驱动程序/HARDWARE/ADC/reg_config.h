#ifndef __REG_CONFIG_H__
#define __REG_CONFIG_H__
/*------------------------------------------------------------------------------------------------*/
//选择通道:
#define CHANNEL_1 0xC183
#define CHANNEL_2 0xD183


//地址引脚寄存器选择
#define CONVERSIONREG  0x00 //ADS数据转换寄存器		转换完成后高12位为有效位,第四位位0
#define CONFIGREG      0x01 //配置寄存器			//X 100 000 0 100 0 0 0 11		//c083
#define LOTHREAHREG    0x02 //最低阈值寄存器
#define HITHRESHREG    0x03 //最高阈值寄存器


//通讯地址,根据ADDR连接相关
#define ALAVE_ADDRESS_GND 0x48
#define ALAVE_ADDRESS_VDD 0x49
#define ALAVE_ADDRESS_SDA 0x4A
#define ALAVE_ADDRESS_SCL 0x4B

//00是通用通讯地址, 06H是清除配置寄存器地址

//15 	OS 
#define REG_STARTCONVERSION   0x8000 

//14:12	MUX选择通道
#define CHAN_AIN0_AIN1        0x0000  //通道选择
#define CHAN_AIN0_AIN3        0x1000
#define CHAN_AIN1_AIN3        0x2000
#define CHAN_AIN2_AIN3        0x3000
#define CHAN_AIN0_GND         0x4000
#define CHAN_AIN1_GND         0x5000
#define CHAN_AIN2_GND         0x6000
#define CHAN_AIN3_GND         0x7000

//11:9位,配置PGA寄存器(可放大增益)
#define PGA_FS6144             0x0000
#define PGA_FS4096             0x0200
#define PGA_FS2048             0x0400
#define PGA_FS1024             0x0600
#define PGA_FS0512             0x0800
#define PGA_FS0256             0x0A00


//8		MODE 
#define OPERATIONAL_CONT                    0x0000//连续转换
#define OPERATIONAL_ONE                     0x0100//单次转换		(默认)

// 7:5	//设置转换速率
#define DR_128SPS           0x0000
#define DR_250SPS           0x0020
#define DR_490SPS           0x0040
#define DR_920SPS           0x0060
#define DR_1600SPS          0x0080
#define DR_2400SPS          0x00A0
#define DR_3300SPS          0x00C0
#define DR_3300_SPS         0x00E0

//4		MODE
#define MODE_TRADITIONAL_COM  0x0000//比较器模式
#define MODE_WINDOW_COM       0x0010


//3		COMP_POL
#define ACTIVE_LOW          0x0000
#define ACTIVE_HIGH         0x0008

//2		COMP_LAT
#define NON_LATCHING        0x0000
#define LATCHING            0x0004

//1:0	COMP_QUE	
#define ONE_CONVERSION      0x0000
#define TWO_CONVERSION      0x0001
#define FOUR_CONVERSION     0x0002
#define DIS_CONVERSION      0x0003

#endif


