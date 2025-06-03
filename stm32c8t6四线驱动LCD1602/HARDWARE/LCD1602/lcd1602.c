#include "lcd1602.h"
#include "delay.h"


/* 控制引脚 定义 */
#define LCD1602_RS_GPIO_Port GPIOA
#define LCD1602_RS_Pin GPIO_Pin_5

#define LCD1602_RW_GPIO_Port GPIOA
#define LCD1602_RW_Pin GPIO_Pin_5

#define LCD1602_E_GPIO_Port GPIOA
#define LCD1602_E_Pin GPIO_Pin_4

// 位定义
#define RS_DQ_OUT(x) do{ x ? \
                                GPIO_SetBits(LCD1602_RS_GPIO_Port, LCD1602_RS_Pin) : \
                                GPIO_ResetBits(LCD1602_RS_GPIO_Port, LCD1602_RS_Pin); \
                            }while(0)   

#define RW_DQ_OUT(x) do{ }while(0)   

#define E_DQ_OUT(x) do{ x ? \
                                GPIO_SetBits(LCD1602_E_GPIO_Port, LCD1602_E_Pin) : \
                                GPIO_ResetBits(LCD1602_E_GPIO_Port, LCD1602_E_Pin); \
                            }while(0)  

/* 数据引脚 定义 */
#define LCD1602_D4_GPIO_Port GPIOA
#define LCD1602_D4_Pin GPIO_Pin_3
                            
#define LCD1602_D5_GPIO_Port GPIOA
#define LCD1602_D5_Pin GPIO_Pin_2
                            
#define LCD1602_D6_GPIO_Port GPIOA
#define LCD1602_D6_Pin GPIO_Pin_1
                            
#define LCD1602_D7_GPIO_Port GPIOA
#define LCD1602_D7_Pin GPIO_Pin_0                             
                            
/*******************************************************************************
* 函 数 名       : lcd1602_write_4digitValue
* 函数功能		 : LCD1602写4位值
* 输    入       : value：值
* 输    出    	 : 无
*******************************************************************************/                            
void lcd1602_write_4digitValue(unsigned char value)
{
    if(value & 0x10)
    {
        GPIO_SetBits(LCD1602_D4_GPIO_Port, LCD1602_D4_Pin);
    }
    else
    {
        GPIO_ResetBits(LCD1602_D4_GPIO_Port, LCD1602_D4_Pin);
    }
    
    if(value & 0x20)
    {
        GPIO_SetBits(LCD1602_D5_GPIO_Port, LCD1602_D5_Pin);
    }
    else
    {
        GPIO_ResetBits(LCD1602_D5_GPIO_Port, LCD1602_D5_Pin);
    }
    
    if(value & 0x40)
    {
        GPIO_SetBits(LCD1602_D6_GPIO_Port, LCD1602_D6_Pin);
    }
    else
    {
        GPIO_ResetBits(LCD1602_D6_GPIO_Port, LCD1602_D6_Pin);
    }
    
    if(value & 0x80)
    {
        GPIO_SetBits(LCD1602_D7_GPIO_Port, LCD1602_D7_Pin);
    }
    else
    {
        GPIO_ResetBits(LCD1602_D7_GPIO_Port, LCD1602_D7_Pin);
    }
}

/*******************************************************************************
* 函 数 名       : lcd1602_write_cmd
* 函数功能		 : LCD1602写命令
* 输    入       : cmd：指令
* 输    出    	 : 无
*******************************************************************************/

void lcd1602_write_cmd(unsigned char cmd)
{
	RS_DQ_OUT(0);//选择命令
	delay_ms(1);
	RW_DQ_OUT(0);//选择写
	E_DQ_OUT(0);
	lcd1602_write_4digitValue(cmd);//准备命令
	delay_us(10);
	E_DQ_OUT(1);//使能脚E先上升沿写入
	delay_ms(1);
	E_DQ_OUT(0);//使能脚E后负跳变完成写入
	delay_ms(1);

	lcd1602_write_4digitValue(cmd<<4);//准备命令
	delay_us(10);
	E_DQ_OUT(1);//使能脚E先上升沿写入
	delay_ms(1);
	E_DQ_OUT(0);//使能脚E后负跳变完成写入
	delay_ms(1);	
}

/*******************************************************************************
* 函 数 名       : lcd1602_write_data
* 函数功能		 : LCD1602写数据
* 输    入       : dat：数据
* 输    出    	 : 无
*******************************************************************************/

void lcd1602_write_data(unsigned char dat) 
{
	RS_DQ_OUT(1);//选择数据
	delay_ms(1);
	RW_DQ_OUT(0);//选择写
	E_DQ_OUT(0);
	lcd1602_write_4digitValue(dat);//准备数据
	delay_us(10);
	E_DQ_OUT(1);//使能脚E先上升沿写入
	delay_ms(1);
	E_DQ_OUT(0);//使能脚E后负跳变完成写入
	delay_ms(1);

	lcd1602_write_4digitValue(dat<<4);//准备数据
	delay_us(10);
	E_DQ_OUT(1);//使能脚E先上升沿写入
	delay_ms(1);
	E_DQ_OUT(0);//使能脚E后负跳变完成写入	
	delay_ms(1);
}

/*******************************************************************************
* 函 数 名       : lcd1602_init
* 函数功能		 : LCD1602初始化
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void lcd1602_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
    
	GPIO_InitStructure.GPIO_Pin = LCD1602_RS_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(LCD1602_RS_GPIO_Port, &GPIO_InitStructure); // 初始化引脚
    
	GPIO_InitStructure.GPIO_Pin = LCD1602_RW_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(LCD1602_RW_GPIO_Port, &GPIO_InitStructure); // 初始化引脚
    
    GPIO_InitStructure.GPIO_Pin = LCD1602_E_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(LCD1602_E_GPIO_Port, &GPIO_InitStructure); // 初始化引脚
    
    GPIO_InitStructure.GPIO_Pin = LCD1602_D4_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(LCD1602_D4_GPIO_Port, &GPIO_InitStructure); // 初始化引脚
    
    GPIO_InitStructure.GPIO_Pin = LCD1602_D5_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(LCD1602_D5_GPIO_Port, &GPIO_InitStructure); // 初始化引脚
    
    GPIO_InitStructure.GPIO_Pin = LCD1602_D6_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(LCD1602_D6_GPIO_Port, &GPIO_InitStructure); // 初始化引脚
    
    GPIO_InitStructure.GPIO_Pin = LCD1602_D7_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(LCD1602_D7_GPIO_Port, &GPIO_InitStructure); // 初始化引脚
    
    lcd1602_write_cmd(0x01); // 清屏
    lcd1602_write_cmd(0x02); // 显示模式，光标复位
	lcd1602_write_cmd(0x28); // 数据总线8位，显示2行，5*7点阵/字符
	lcd1602_write_cmd(0x0c); // 显示功能开，无光标，光标闪烁
	lcd1602_write_cmd(0x06); // 写入新数据后光标右移，显示屏不移动
	lcd1602_write_cmd(0x01); // 清屏	
    lcd1602_user_char(); // 写入自定义字符
}


/*******************************************************************************
* 函 数 名       : lcd1602_clear
* 函数功能		 : LCD1602清屏
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void lcd1602_clear(void)
{
	lcd1602_write_cmd(0x01);	
}

/*******************************************************************************
* 函 数 名       : lcd1602_show_char
* 函数功能		 : LCD1602显示字符
* 输    入       : x,y：显示坐标，x=0~15，y=0~1;
				   chr：显示字符
* 输    出    	 : 无
*******************************************************************************/
void lcd1602_show_char(unsigned char x,unsigned char y,unsigned char chr) 
{     
    if (y == 0) 
    {     
        lcd1602_write_cmd(0x80 + x); //第一行显示地址设置	    
    }    
    else 
    {     
        lcd1602_write_cmd(0xC0 + x); //第二行显示地址设置	    
    }        
    lcd1602_write_data(chr); //显示内容 
}

/*******************************************************************************
* 函 数 名       : lcd1602_user_char
* 函数功能		 : LCD1602写入自定义字符
* 输    入       : 
				   
* 输    出    	 : 无
*******************************************************************************/

void lcd1602_user_char(void)
{
    //第一个自定义字符
    lcd1602_write_cmd(0x40); //"01 000 000"  第1行地址 (D7D6为地址设定命令形式D5D4D3为字符存放位置(0--7)，D2D1D0为字符行地址(0--7)）
    lcd1602_write_data(0x00); //"XXX 11111" 第1行数据（D7D6D5为XXX，表示为任意数(一般用000），D4D3D2D1D0为字符行数据(1-点亮，0-熄灭）
    lcd1602_write_cmd(0x41); //"01 000 001"  第2行地址
    lcd1602_write_data(0x04); //"XXX 10001" 第2行数据
    lcd1602_write_cmd(0x42); //"01 000 010"  第3行地址
    lcd1602_write_data(0x0e); //"XXX 10101" 第3行数据
    lcd1602_write_cmd(0x43); //"01 000 011"  第4行地址
    lcd1602_write_data(0x0e); //"XXX 10001" 第4行数据
    lcd1602_write_cmd(0x44); //"01 000 100"  第5行地址
    lcd1602_write_data(0x0e); //"XXX 11111" 第5行数据
    lcd1602_write_cmd(0x45); //"01 000 101"  第6行地址
    lcd1602_write_data(0x1f); //"XXX 01010" 第6行数据
    lcd1602_write_cmd(0x46); //"01 000 110"  第7行地址
    lcd1602_write_data(0x04); //"XXX 11111" 第7行数据
    lcd1602_write_cmd(0x47); //"01 000 111"  第8行地址
    lcd1602_write_data(0x00); //"XXX 00000" 第8行数据 
    //第二个自定义字符

    lcd1602_write_cmd(0x48); //"01 001 000"  第1行地址  
    lcd1602_write_data(0x03); //"XXX 00001" 第1行数据 
    lcd1602_write_cmd(0x49); //"01 001 001"  第2行地址
    lcd1602_write_data(0x03); //"XXX 11011" 第2行数据
    lcd1602_write_cmd(0x4a); //"01 001 010"  第3行地址
    lcd1602_write_data(0x00); //"XXX 11101" 第3行数据
    lcd1602_write_cmd(0x4b); //"01 001 011"  第4行地址
    lcd1602_write_data(0x00); //"XXX 11001" 第4行数据
    lcd1602_write_cmd(0x4c); //"01 001 100"  第5行地址
    lcd1602_write_data(0x00); //"XXX 11101" 第5行数据
    lcd1602_write_cmd(0x4d); //"01 001 101"  第6行地址
    lcd1602_write_data(0x00); //"XXX 11011" 第6行数据
    lcd1602_write_cmd(0x4e); //"01 001 110"  第7行地址
    lcd1602_write_data(0x00); //"XXX 00001" 第7行数据
    lcd1602_write_cmd(0x4f); //"01 001 111"  第8行地址
    lcd1602_write_data(0x00); //"XXX 00000" 第8行数据 
}

/*******************************************************************************
* 函 数 名       : lcd1602_show_string
* 函数功能		 : LCD1602显示字符
* 输    入       : x,y：显示坐标，x=0~15，y=0~1;
				   str：显示字符串
* 输    出    	 : 无
*******************************************************************************/
void lcd1602_show_string(unsigned char x,unsigned char y,unsigned char *str)
{
	unsigned char i=0;

	if(y>1||x>15)return;//行列参数不对则强制退出

	if(y<1)	//第1行显示
	{	
		while(*str!='\0')//字符串是以'\0'结尾，只要前面有内容就显示
		{
			if(i<16-x)//如果字符长度超过第一行显示范围，则在第二行继续显示
			{
				lcd1602_write_cmd(0x80+i+x); //第一行显示地址设置	
			}
			else
			{
				lcd1602_write_cmd(0x40+0x80+i+x-16);//第二行显示地址设置	
			}
			lcd1602_write_data(*str); //显示内容
			str++;//指针递增
			i++;	
		}	
	}
	else	//第2行显示
	{
		while(*str!='\0')
		{
			if(i<16-x) //如果字符长度超过第二行显示范围，则在第一行继续显示
			{
				lcd1602_write_cmd(0x80+0x40+i+x);	
			}
			else
			{
				lcd1602_write_cmd(0x80+i+x-16);	
			}
			lcd1602_write_data(*str);
			str++;
			i++;	
		}	
	}				
}
