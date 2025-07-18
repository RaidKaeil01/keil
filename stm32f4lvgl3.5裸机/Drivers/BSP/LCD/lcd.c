#include "lcd.h"
#include "stdlib.h"
#include "font.h"
//#include "delay.h"
#include "string.h"

//LCD的画笔颜色和背景色
uint16_t POINT_COLOR=0x0000;	//画笔颜色
uint16_t BACK_COLOR=0xFFFF;  //背景色

//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LCD_DC_Pin|LCD_SDI_Pin|LCD_CLK_Pin|LCD_LED_Pin|LCD_SDO_Pin|LCD_CS_Pin|LCD_RST_Pin, GPIO_PIN_SET);
  GPIO_InitStruct.Pin = LCD_DC_Pin|LCD_SDI_Pin|LCD_CLK_Pin|LCD_LED_Pin|LCD_SDO_Pin|LCD_CS_Pin|LCD_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}


void LCD_SDI_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = LCD_SDI_Pin;                    //使用STM3cubemx是定义好的SDA_Pin为GPIO_PIN_8的标签
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_SDI_GPIO_Port, &GPIO_InitStruct);
}
void LCD_SDI_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = LCD_SDI_Pin;                    //使用STM3cubemx是定义好的SDA_Pin为GPIO_PIN_8的标签
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_SDI_GPIO_Port, &GPIO_InitStruct);
}


//写寄存器函数
//data:寄存器值
void LCD_WR_REG(uint16_t data)
{
    uint8_t i;

    LCD_CS_Clr();
    LCD_DC_Clr();//RS=0 命令

    for(i=0; i<8; i++)
    {
        if (data & 0x80)
            LCD_SDI_Set();
        else
            LCD_SDI_Clr();

        data <<= 1;
        LCD_CLK_Clr();
        LCD_CLK_Set();
    }
    LCD_CS_Set();
}
//写数据函数
//data:寄存器值
void LCD_WR_DATA(uint16_t data)
{
    uint8_t i;

    LCD_CS_Clr();
    LCD_DC_Set();//RS=1 数据

    for(i=0; i<8; i++)
    {
        if (data & 0x80)
            LCD_SDI_Set();
        else
            LCD_SDI_Clr();

        data <<= 1;
        LCD_CLK_Clr();
        LCD_CLK_Set();
    }
    LCD_CS_Set();
}

//写寄存器
//LCD_Reg:寄存器编号
//LCD_RegValue:要写入的值
void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{
    LCD_WR_REG(LCD_Reg);
    LCD_WriteRAM(LCD_RegValue);
}

//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
    LCD_WR_REG(lcddev.wramcmd);
}
//LCD写GRAM
//RGB_Code:颜色值
void LCD_WriteRAM(uint16_t RGB_Code)
{
//写十六位GRAM
    uint8_t i;
    LCD_CS_Clr();
    LCD_DC_Set();

    for(i=0; i<16; i++)
    {
        if (RGB_Code & 0x8000)
            LCD_SDI_Set();
        else
            LCD_SDI_Clr();

        RGB_Code <<= 1;
        LCD_CLK_Clr();
        LCD_CLK_Set();
    }
    LCD_CS_Set();
}

//当mdk -O1时间优化时需要设置
//延时i
void opt_delay(uint8_t i)
{
    while(i--);
}

//LCD开启显示
void LCD_DisplayOn(void)
{
    LCD_WR_REG(0X29);	//开启显示
}
//LCD关闭显示
void LCD_DisplayOff(void)
{
    LCD_WR_REG(0X28);	//关闭显示
}
//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(Xpos>>8);
    LCD_WR_DATA(Xpos&0XFF);
    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(Ypos>>8);
    LCD_WR_DATA(Ypos&0XFF);
}

//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
    LCD_SetCursor(x,y);		//设置光标位置
    LCD_WriteRAM_Prepare();	//开始写入GRAM
    LCD_WriteRAM(POINT_COLOR);
}
//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
    //设置光标位置
    LCD_SetCursor(x,y);
    //写入颜色
    LCD_WriteReg(lcddev.wramcmd,color);
}


//dir:方向选择 	0-0度旋转，1-180度旋转，2-270度旋转，3-90度旋转
void LCD_Display_Dir(uint8_t dir)
{
    if(dir==0||dir==1)			//竖屏
    {
        lcddev.dir=0;	//竖屏
        lcddev.width=320;
        lcddev.height=480;

        lcddev.wramcmd=0X2C;
        lcddev.setxcmd=0X2A;
        lcddev.setycmd=0X2B;

        if(dir==0)        //0-0度旋转
        {
            LCD_WR_REG(0x36);
            LCD_WR_DATA((1<<3)|(0<<7)|(1<<6)|(0<<5));
        }
        else							//1-180度旋转
        {
            LCD_WR_REG(0x36);
            LCD_WR_DATA((1<<3)|(1<<7)|(0<<6)|(0<<5));
        }

    }
    else if(dir==2||dir==3)
    {

        lcddev.dir=1;	//横屏
        lcddev.width=480;
        lcddev.height=320;

        lcddev.wramcmd=0X2C;
        lcddev.setxcmd=0X2A;
        lcddev.setycmd=0X2B;

        if(dir==2)				//2-270度旋转
        {
            LCD_WR_REG(0x36);
            LCD_WR_DATA((1<<3)|(1<<7)|(1<<6)|(1<<5));

        }
        else							//3-90度旋转
        {
            LCD_WR_REG(0x36);
            LCD_WR_DATA((1<<3)|(0<<7)|(0<<6)|(1<<5));
        }
    }


    //设置显示区域
    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(0);
    LCD_WR_DATA(0);
    LCD_WR_DATA((lcddev.width-1)>>8);
    LCD_WR_DATA((lcddev.width-1)&0XFF);
    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(0);
    LCD_WR_DATA(0);
    LCD_WR_DATA((lcddev.height-1)>>8);
    LCD_WR_DATA((lcddev.height-1)&0XFF);


}
//设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//width,height:窗口宽度和高度,必须大于0!!
//窗体大小:width*height.
void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
    uint16_t twidth,theight;
    twidth=sx+width-1;
    theight=sy+height-1;

    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(sx>>8);
    LCD_WR_DATA(sx&0XFF);
    LCD_WR_DATA(twidth>>8);
    LCD_WR_DATA(twidth&0XFF);
    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(sy>>8);
    LCD_WR_DATA(sy&0XFF);
    LCD_WR_DATA(theight>>8);
    LCD_WR_DATA(theight&0XFF);

}
//初始化lcd
void LCD_Init(void)
{	
	MX_GPIO_Init();
	
    LCD_RST_Set();
    HAL_Delay(1);
    LCD_RST_Clr();
    HAL_Delay(10);
    LCD_RST_Set();
    HAL_Delay(120);

//************* Start Initial Sequence **********//
    HAL_Delay(120); // Delay 120ms
    LCD_WR_REG(0x11); // Sleep Out
    HAL_Delay(120); // Delay 120ms
    LCD_WR_REG(0xf0) ;
    LCD_WR_DATA(0xc3) ;
    LCD_WR_REG(0xf0) ;
    LCD_WR_DATA(0x96) ;
    LCD_WR_REG(0x36);
    LCD_WR_DATA(0x48);
    LCD_WR_REG(0x3A);
    LCD_WR_DATA(0x55);
    LCD_WR_REG(0xB4);
    LCD_WR_DATA(0x01);
    LCD_WR_REG(0xB7) ;
    LCD_WR_DATA(0xC6) ;
    LCD_WR_REG(0xe8);
    LCD_WR_DATA(0x40);
    LCD_WR_DATA(0x8a);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x29);
    LCD_WR_DATA(0x19);
    LCD_WR_DATA(0xa5);
    LCD_WR_DATA(0x33);
    LCD_WR_REG(0xc1);
    LCD_WR_DATA(0x06);
    LCD_WR_REG(0xc2);
    LCD_WR_DATA(0xa7);
    LCD_WR_REG(0xc5);
    LCD_WR_DATA(0x18);
    LCD_WR_REG(0xe0); //Positive Voltage Gamma Control
    LCD_WR_DATA(0xf0);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x0b);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x04);
    LCD_WR_DATA(0x15);
    LCD_WR_DATA(0x2f);
    LCD_WR_DATA(0x54);
    LCD_WR_DATA(0x42);
    LCD_WR_DATA(0x3c);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x14);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0x1b);
    LCD_WR_REG(0xe1); //Negative Voltage Gamma Control
    LCD_WR_DATA(0xf0);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x0b);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x04);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x2d);
    LCD_WR_DATA(0x43);
    LCD_WR_DATA(0x42);
    LCD_WR_DATA(0x3b);
    LCD_WR_DATA(0x16);
    LCD_WR_DATA(0x14);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x1b);
    LCD_WR_REG(0xf0);
    LCD_WR_DATA(0x3c);
    LCD_WR_REG(0xf0);
    LCD_WR_DATA(0x69);
    HAL_Delay(120); //Delay 120ms
    LCD_WR_REG(0x29);// Display on
}



//读取个某点的颜色值
//x,y:坐标
//返回值:此点的颜色
uint16_t LCD_ReadPoint(uint16_t x,uint16_t y)
{
    uint8_t i,reg=0x2e;
    uint16_t color;
    if(x>=lcddev.width||y>=lcddev.height)return 0;	//超过了范围,直接返回
    LCD_SetCursor(x,y);
    LCD_CS_Clr();
    LCD_DC_Clr();

    for(i=0; i<8; i++)
    {
        if (reg & 0x80)
            LCD_SDI_Set();
        else
            LCD_SDI_Clr();

        reg <<= 1;
        LCD_CLK_Clr();
        LCD_CLK_Set();
    }


    for(i=0; i<8; i++)							//第一次空读
    {
        LCD_CLK_Clr();
        LCD_CLK_Set();
    }

    for(i=0; i<16; i++)
    {
        LCD_CLK_Clr();
        color=color << 1 | LCD_SDO;
        LCD_CLK_Set();
    }

    LCD_CS_Set();


    return color;
}

//清屏函数
//color:要清屏的填充色
void LCD_Clear(uint16_t color)
{
    uint32_t index=0;
    uint32_t totalpoint=lcddev.width;
    totalpoint*=lcddev.height; 			//得到总点数
    LCD_SetCursor(0x00,0x0000);	//设置光标位置
    LCD_WriteRAM_Prepare();     		//开始写入GRAM
    for(index=0; index<totalpoint; index++)LCD_WriteRAM(color);
}
//在指定区域内填充指定颜色
//区域大小:(xend-xsta+1)*(yend-ysta+1)
//xsta
//color:要填充的颜色
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{
    uint16_t i,j;
    uint16_t xlen=0;
    uint16_t temp;
    if((lcddev.id==0X6804)&&(lcddev.dir==1))	//6804横屏的时候特殊处理
    {
        temp=sx;
        sx=sy;
        sy=lcddev.width-ex-1;
        ex=ey;
        ey=lcddev.width-temp-1;
        lcddev.dir=0;
        lcddev.setxcmd=0X2A;
        lcddev.setycmd=0X2B;
        LCD_Fill(sx,sy,ex,ey,color);
        lcddev.dir=1;
        lcddev.setxcmd=0X2B;
        lcddev.setycmd=0X2A;
    }
    else
    {
        xlen=ex-sx+1;
        for(i=sy; i<=ey; i++)
        {
            LCD_SetCursor(sx,i);      				//设置光标位置
            LCD_WriteRAM_Prepare();     			//开始写入GRAM
            for(j=0; j<xlen; j++)LCD_WriteRAM(color);	//设置光标位置
        }
    }
}
//在指定区域内填充指定颜色块
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//color:要填充的颜色
void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{
    uint16_t height,width;
    uint16_t i,j;
    width=ex-sx+1; 			//得到填充的宽度
    height=ey-sy+1;			//高度
    for(i=0; i<height; i++)
    {
        LCD_SetCursor(sx,sy+i);   	//设置光标位置
        LCD_WriteRAM_Prepare();     //开始写入GRAM
        for(j=0; j<width; j++)LCD_WriteRAM(color[i*width+j]); //写入数据
    }
}
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    uint16_t t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    delta_x=x2-x1; //计算坐标增量
    delta_y=y2-y1;
    uRow=x1;
    uCol=y1;
    if(delta_x>0)incx=1; //设置单步方向
    else if(delta_x==0)incx=0;//垂直线
    else
    {
        incx=-1;
        delta_x=-delta_x;
    }
    if(delta_y>0)incy=1;
    else if(delta_y==0)incy=0;//水平线
    else
    {
        incy=-1;
        delta_y=-delta_y;
    }
    if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
    else distance=delta_y;
    for(t=0; t<=distance+1; t++ ) //画线输出
    {
        LCD_DrawPoint(uRow,uCol);//画点
        xerr+=delta_x ;
        yerr+=delta_y ;
        if(xerr>distance)
        {
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance)
        {
            yerr-=distance;
            uCol+=incy;
        }
    }
}
//画矩形
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    LCD_DrawLine(x1,y1,x2,y1);
    LCD_DrawLine(x1,y1,x1,y2);
    LCD_DrawLine(x1,y2,x2,y2);
    LCD_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
    int a,b;
    int di;
    a=0;
    b=r;
    di=3-(r<<1);             //判断下个点位置的标志
    while(a<=b)
    {
        LCD_DrawPoint(x0+a,y0-b);             //5
        LCD_DrawPoint(x0+b,y0-a);             //0
        LCD_DrawPoint(x0+b,y0+a);             //4
        LCD_DrawPoint(x0+a,y0+b);             //6
        LCD_DrawPoint(x0-a,y0+b);             //1
        LCD_DrawPoint(x0-b,y0+a);
        LCD_DrawPoint(x0-a,y0-b);             //2
        LCD_DrawPoint(x0-b,y0-a);             //7
        a++;
        //使用Bresenham算法画圆
        if(di<0)di +=4*a+6;
        else
        {
            di+=10+4*(a-b);
            b--;
        }
    }
}
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{
    uint8_t temp,t1,t;
    uint16_t y0=y;
    uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
    num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
    for(t=0; t<csize; t++)
    {
        if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
        else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
        else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
        else return;								//没有的字库
        for(t1=0; t1<8; t1++)
        {
            if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
            else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
            temp<<=1;
            y++;
            if(y>=lcddev.height)return;		//超区域了
            if((y-y0)==size)
            {
                y=y0;
                x++;
                if(x>=lcddev.width)return;	//超区域了
                break;
            }
        }
    }
}
//m^n函数
//返回值:m^n次方.
uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
    while(n--)result*=m;
    return result;
}
//显示数字,高位为0,则不显示
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//color:颜色
//num:数值(0~4294967295);
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{
    uint8_t t,temp;
    uint8_t enshow=0;
    for(t=0; t<len; t++)
    {
        temp=(num/LCD_Pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
                continue;
            }
            else enshow=1;

        }
        LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0);
    }
}
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{
    uint8_t t,temp;
    uint8_t enshow=0;
    for(t=0; t<len; t++)
    {
        temp=(num/LCD_Pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);
                else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);
                continue;
            }
            else enshow=1;

        }
        LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01);
    }
}
//显示字符串
//x,y:起点坐标
//width,height:区域大小
//size:字体大小
//*p:字符串起始地址
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{
    uint8_t x0=x;
    width+=x;
    height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {
        if(x>=width)
        {
            x=x0;
            y+=size;
        }
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }
}


//汉字 16*16
void GUI_DrawFont16(uint16_t x, uint16_t y,uint8_t *s,uint8_t mode)
{
    uint8_t i,j;
    uint16_t k;
    uint16_t HZnum;
    uint16_t x0=x;
    HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//自动统计汉字数目


    for (k=0; k<HZnum; k++)
    {
        if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
        {
            LCD_Set_Window(x,y,16,16);
            LCD_WriteRAM_Prepare();
            for(i=0; i<16*2; i++)
            {
                for(j=0; j<8; j++)
                {
                    if(!mode) //非叠加方式
                    {
                        if(tfont16[k].Msk[i]&(0x80>>j))	LCD_WriteRAM(POINT_COLOR);
                        else LCD_WriteRAM(BACK_COLOR);
                    }
                    else
                    {
                        //POINT_COLOR=fc;
                        if(tfont16[k].Msk[i]&(0x80>>j))	LCD_DrawPoint(x,y);//画一个点
                        x++;
                        if((x-x0)==16)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }

                }

            }


        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }

    LCD_Set_Window(0,0,lcddev.width,lcddev.height);//恢复窗口为全屏
}

//汉字 24*24
void GUI_DrawFont24(uint16_t x, uint16_t y, uint8_t *s,uint8_t mode)
{
    uint8_t i,j;
    uint16_t k;
    uint16_t HZnum;
    uint16_t x0=x;
    HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//自动统计汉字数目

    for (k=0; k<HZnum; k++)
    {
        if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
        {
            LCD_Set_Window(x,y,24,24);
            LCD_WriteRAM_Prepare();
            for(i=0; i<24*3; i++)
            {
                for(j=0; j<8; j++)
                {
                    if(!mode) //非叠加方式
                    {
                        if(tfont24[k].Msk[i]&(0x80>>j))	LCD_WriteRAM(POINT_COLOR);
                        else LCD_WriteRAM(BACK_COLOR);
                    }
                    else
                    {
                        //POINT_COLOR=fc;
                        if(tfont24[k].Msk[i]&(0x80>>j))	LCD_DrawPoint(x,y);//画一个点
                        x++;
                        if((x-x0)==24)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }


        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }

    LCD_Set_Window(0,0,lcddev.width,lcddev.height);//恢复窗口为全屏
}

//汉字 32*32
void GUI_DrawFont32(uint16_t x, uint16_t y, uint8_t *s,uint8_t mode)
{
    uint8_t i,j;
    uint16_t k;
    uint16_t HZnum;
    uint16_t x0=x;
    HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//自动统计汉字数目
    for (k=0; k<HZnum; k++)
    {
        if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
        {
            LCD_Set_Window(x,y,32,32);
            LCD_WriteRAM_Prepare();
            for(i=0; i<32*4; i++)
            {
                for(j=0; j<8; j++)
                {
                    if(!mode) //非叠加方式
                    {
                        if(tfont32[k].Msk[i]&(0x80>>j))	LCD_WriteRAM(POINT_COLOR);
                        else LCD_WriteRAM(BACK_COLOR);
                    }
                    else
                    {
                        //POINT_COLOR=fc;
                        if(tfont32[k].Msk[i]&(0x80>>j))	LCD_DrawPoint(x,y);//画一个点
                        x++;
                        if((x-x0)==32)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }


        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }

    LCD_Set_Window(0,0,lcddev.width,lcddev.height);//恢复窗口为全屏
}



//显示汉字或者字符串
void Show_Str(uint16_t x, uint16_t y,uint8_t *str,uint8_t size,uint8_t mode)
{
    uint16_t x0=x;
    uint8_t bHz=0;     //字符或者中文
    while(*str!=0)//数据未结束
    {
        if(!bHz)
        {
            if(x>(lcddev.width-size/2)||y>(lcddev.height-size))
                return;
            if(*str>0x80)bHz=1;//中文
            else              //字符
            {
                if(*str==0x0D)//换行符号
                {
                    y+=size;
                    x=x0;
                    str++;
                }
                else
                {
                    if(size>=24)//字库中没有集成12X24 16X32的英文字体,用8X16代替
                    {
                        LCD_ShowChar(x,y,*str,24,mode);
                        x+=12; //字符,为全字的一半
                    }
                    else
                    {
                        LCD_ShowChar(x,y,*str,size,mode);
                        x+=size/2; //字符,为全字的一半
                    }
                }
                str++;

            }
        }
        else //中文
        {
            if(x>(lcddev.width-size)||y>(lcddev.height-size))
                return;
            bHz=0;//有汉字库
            if(size==32)
                GUI_DrawFont32(x,y,str,mode);
            else if(size==24)
                GUI_DrawFont24(x,y,str,mode);
            else
                GUI_DrawFont16(x,y,str,mode);

            str+=2;
            x+=size;//下一个汉字偏移
        }
    }
}


//显示40*40图片
void Gui_Drawbmp16(uint16_t x,uint16_t y,const unsigned char *p) //显示40*40图片
{
    int i;
    unsigned char picH,picL;
    LCD_Set_Window(x,y,40,40);
    LCD_WriteRAM_Prepare();

    for(i=0; i<40*40; i++)
    {
        picL=*(p+i*2);	//数据低位在前
        picH=*(p+i*2+1);
        LCD_WriteRAM(picH<<8|picL);
    }
    LCD_Set_Window(0,0,lcddev.width,lcddev.height);//恢复显示窗口为全屏

}

//居中显示
void Gui_StrCenter(uint16_t x, uint16_t y, uint8_t *str,uint8_t size,uint8_t mode)
{
    uint16_t x1;
    uint16_t len=strlen((const char *)str);
    if(size>16)
    {
        x1=(lcddev.width-len*(size/2))/2;
    }
    else
    {
        x1=(lcddev.width-len*8)/2;
    }

    Show_Str(x+x1,y,str,size,mode);
}


void Load_Drow_Dialog(void)
{
    LCD_Clear(WHITE);//清屏
    POINT_COLOR=BLUE;//设置字体为蓝色
    BACK_COLOR=WHITE;
    LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//显示清屏区域
    POINT_COLOR=RED;//设置画笔蓝色
}
////////////////////////////////////////////////////////////////////////////////
//电容触摸屏专有部分
//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_hline(uint16_t x0,uint16_t y0,uint16_t len,uint16_t color)
{
    if(len==0)return;
    LCD_Fill(x0,y0,x0+len-1,y0,color);
}
//画实心圆
//x0,y0:坐标
//r:半径
//color:颜色
void gui_fill_circle(uint16_t x0,uint16_t y0,uint16_t r,uint16_t color)
{
    uint32_t i;
    uint32_t imax = ((uint32_t)r*707)/1000+1;
    uint32_t sqmax = (uint32_t)r*(uint32_t)r+(uint32_t)r/2;
    uint32_t x=r;
    gui_draw_hline(x0-r,y0,2*r,color);
    for (i=1; i<=imax; i++)
    {
        if ((i*i+x*x)>sqmax)// draw lines from outside
        {
            if (x>imax)
            {
                gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
                gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
            }
            x--;
        }
        // draw lines from inside (center)
        gui_draw_hline(x0-x,y0+i,2*x,color);
        gui_draw_hline(x0-x,y0-i,2*x,color);
    }
}

//画一条粗线
//(x1,y1),(x2,y2):线条的起始坐标
//size：线条的粗细程度
//color：线条的颜色
void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint8_t size,uint16_t color)
{
    uint16_t t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    if(x1<size|| x2<size||y1<size|| y2<size)return;
    delta_x=x2-x1; //计算坐标增量
    delta_y=y2-y1;
    uRow=x1;
    uCol=y1;
    if(delta_x>0)incx=1; //设置单步方向
    else if(delta_x==0)incx=0;//垂直线
    else
    {
        incx=-1;
        delta_x=-delta_x;
    }
    if(delta_y>0)incy=1;
    else if(delta_y==0)incy=0;//水平线
    else
    {
        incy=-1;
        delta_y=-delta_y;
    }
    if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
    else distance=delta_y;
    for(t=0; t<=distance+1; t++ ) //画线输出
    {
        gui_fill_circle(uRow,uCol,size,color);//画点
        xerr+=delta_x ;
        yerr+=delta_y ;
        if(xerr>distance)
        {
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance)
        {
            yerr-=distance;
            uCol+=incy;
        }
    }
}



void LCD_Showfloat(uint16_t x,uint16_t y,float num,uint8_t size,uint8_t mode)
{
	float temp;
	if(num<10)
	{
		temp = num*1000;
		LCD_ShowNum(x,y,(int)temp/1000,1,24);
		Show_Str(x+12,y,(uint8_t *)".",24,1);
		LCD_ShowNum(x+24,y,(int)(temp/100)%10,1,24);
		LCD_ShowNum(x+36,y,(int)(temp/10)%10,1,24);
		LCD_ShowNum(x+48,y,(int)temp%10,1,24);
	}
	else if(num>10&&(num<100))
	{
		temp = num*1000;
		LCD_ShowNum(x,y,(int)temp/1000,2,24);
		Show_Str(x+24,y,(uint8_t *)".",24,1);
		LCD_ShowNum(x+36,y,(int)(temp/100)%10,1,24);
		LCD_ShowNum(x+48,y,(int)(temp/10)%10,1,24);
		LCD_ShowNum(x+60,y,(int)temp%10,1,24);
	}
	else if(num>100&&(num<1000))
	{
		temp = num*1000;
		LCD_ShowNum(x,y,(int)temp/1000,3,24);
		Show_Str(x+36,y,(uint8_t *)".",24,1);
		LCD_ShowNum(x+48,y,(int)(temp/100)%10,1,24);
		LCD_ShowNum(x+60,y,(int)(temp/10)%10,1,24);
		LCD_ShowNum(x+72,y,(int)temp%10,1,24);
	}
	else
	{
		temp = num*1000;
		LCD_ShowNum(x,y,(int)temp/1000,4,24);
		Show_Str(x+48,y,(uint8_t *)".",24,1);
		LCD_ShowNum(x+60,y,(int)(temp/100)%10,1,24);
		LCD_ShowNum(x+72,y,(int)(temp/10)%10,1,24);
		LCD_ShowNum(x+84,y,(int)temp%10,1,24);
	}
}



























