#ifndef __LCD_H
#define __LCD_H


#include "stm32F4xx_hal.h"


#define LCD_DC_Pin GPIO_PIN_2
#define LCD_DC_GPIO_Port GPIOE
#define LCD_SDI_Pin GPIO_PIN_3
#define LCD_SDI_GPIO_Port GPIOE
#define LCD_CLK_Pin GPIO_PIN_4
#define LCD_CLK_GPIO_Port GPIOE
#define LCD_LED_Pin GPIO_PIN_5
#define LCD_LED_GPIO_Port GPIOE
#define LCD_SDO_Pin GPIO_PIN_6
#define LCD_SDO_GPIO_Port GPIOE
#define LCD_CS_Pin GPIO_PIN_0
#define LCD_CS_GPIO_Port GPIOE
#define LCD_RST_Pin GPIO_PIN_1
#define LCD_RST_GPIO_Port GPIOE


/////////////////////////////////////用户配置区///////////////////////////////////	 
//支持横竖屏快速定义切换
#define USE_LCM_DIR  	  0   	//定义液晶屏顺时针旋转方向 	0-0度旋转，1-180度旋转，2-270度旋转，3-90度旋转
#define USE_TP_TYPE   	RTP 	//定义触摸类型  CTP = 电容模式 , RTP = 电阻模式

#define CTP 0X80		//电容模式
#define RTP 0X00		//电阻模式

	 
//LCD重要参数集
typedef struct  
{										    
	uint16_t width;			//LCD 宽度
	uint16_t height;			//LCD 高度
	uint16_t id;				//LCD ID
	uint8_t  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint16_t	wramcmd;		//开始写gram指令
	uint16_t setxcmd;		//设置x坐标指令
	uint16_t  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	  

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
//LCD的画笔颜色和背景色	   
extern uint16_t  POINT_COLOR;//默认红色    
extern uint16_t  BACK_COLOR; //背景颜色.默认为白色

////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义----------------// 

#define LCD_CS_Clr() 	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)  // CS = 0		PD0
#define LCD_CS_Set() 	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)    // CS = 1

#define LCD_RST_Clr() HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET)  	// CS = 0		PD1
#define LCD_RST_Set() HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET)  		// CS = 0		PD1

#define LCD_DC_Clr() HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET)  		// CS = 0		PD2
#define LCD_DC_Set() HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET)  		// CS = 0		PD2

#define LCD_SDI_Clr() HAL_GPIO_WritePin(LCD_SDI_GPIO_Port, LCD_SDI_Pin, GPIO_PIN_RESET)  	// CS = 0		PD3
#define LCD_SDI_Set() HAL_GPIO_WritePin(LCD_SDI_GPIO_Port, LCD_SDI_Pin, GPIO_PIN_SET)  		// CS = 0		PD3

#define LCD_SDI_RD 		LCD_SDI_IN()		//SDA单通道时候 作为数据输出		PD3

#define LCD_CLK_Clr() HAL_GPIO_WritePin(LCD_CLK_GPIO_Port, LCD_CLK_Pin, GPIO_PIN_RESET)  	// CS = 0		PD4
#define LCD_CLK_Set() HAL_GPIO_WritePin(LCD_CLK_GPIO_Port, LCD_CLK_Pin, GPIO_PIN_SET)  		// CS = 0		PD4

#define LCD_LED_Clr() HAL_GPIO_WritePin(LCD_LED_GPIO_Port, LCD_LED_Pin, GPIO_PIN_RESET)  	// CS = 0		PD6
#define LCD_LED_Set() HAL_GPIO_WritePin(LCD_LED_GPIO_Port, LCD_LED_Pin, GPIO_PIN_SET)  		// CS = 0		PD6

#define LCD_SDO       HAL_GPIO_ReadPin(LCD_SDO_GPIO_Port ,LCD_SDO_Pin)        	//PD6
	 

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色 
#define LGRAY 					 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
	    															  
	    															  
void LCD_Init(void);													   	//初始化
void LCD_DisplayOn(void);													//开显示
void LCD_DisplayOff(void);													//关显示
void LCD_Clear(uint16_t Color);	 												//清屏
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);										//设置光标
void LCD_DrawPoint(uint16_t x,uint16_t y);											//画点
void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color);								//快速画点
void LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r);						 			//画圆
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);							//画线
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);		   				//画矩形
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);		   				//填充单色
void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);				//填充指定颜色
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);						//显示一个字符
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);  						//显示一个数字
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);				//显示 数字
void LCD_Showfloat(uint16_t x,uint16_t y,float num,uint8_t size,uint8_t mode);				//显示 数字

void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);		//显示一个字符串,12/16字体

void LCD_WR_DATA(uint16_t data);
void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(uint16_t RGB_Code);
void Load_Drow_Dialog(void);
void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);	//设置窗口				
void Show_Str(uint16_t x, uint16_t y,uint8_t *str,uint8_t size,uint8_t mode);//显示中文
void Gui_Drawbmp16(uint16_t x,uint16_t y,const unsigned char *p); //显示40*40 图片
void Gui_StrCenter(uint16_t x, uint16_t y,uint8_t *str,uint8_t size,uint8_t mode);//居中显示
void LCD_Display_Dir(uint8_t dir); //设置LCD显示方向
void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint8_t size,uint16_t color);//画一条粗线
void gui_fill_circle(uint16_t x0,uint16_t y0,uint16_t r,uint16_t color);//画实心圆
void gui_draw_hline(uint16_t x0,uint16_t y0,uint16_t len,uint16_t color);//画水平线 电容触摸屏专有部分
uint16_t LCD_ReadPoint(uint16_t x,uint16_t y);//读取指定坐标颜色



#endif













