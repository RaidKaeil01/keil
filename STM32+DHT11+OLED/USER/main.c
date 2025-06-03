#include "stdio.h"
#include <stdlib.h>
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "pwm.h"
#include "oled.h"
#include "dht11.h"
#include "Serial.h"
#include "PM25.h"

// 显示缓冲区
char showBuffer[50] = {0, },show_page = 0,state = 0;

uint8_t RxData;	//蓝牙接收数据变量
DHT11_Data_TypeDef DHT11_Data;	//DHT11结构体
float temperature=0;        // temperature [°C]	温度
float humidity=0;			//湿度
int PM=0;					//PM2.5

/* 对应阈值 */
int temperature_L = 20,temperature_H = 40;
int humidity_L = 20,humidity_H = 60;
int PM_Value = 400;

void Key_function(void);	//按键扫描函数
void show_function(void);	//OLED显示函数
void Alarm_function(void);	//蜂鸣器报警函数
void lanya_mingling(void);	//蓝牙命令函数

int main(void)
{
    int i = 0;
    delay_init();	    //延时函数初始化
    LED_Init();		  	//初始化与LED连接的硬件接口
    OLED_Init();
    Serial_Init();
    DHT11_GPIO_Config();
    PM25_Init();
    while(1)
    {
        Key_function();
		if(show_page == 1)
		{
			lanya_mingling();
		}
        i++;
		if(Read_DHT11(&DHT11_Data) == SUCCESS)
		{
			temperature=DHT11_Data.temp_int+((float)DHT11_Data.temp_deci)/10;	//获取当前温度
			humidity=DHT11_Data.humi_int+((float)DHT11_Data.humi_deci)/10;		//获取当前湿度
			PM = Get_PM25_Average_Data();		//获取当前PM2.5
			show_function();
		}
    }
}



void Key_function(void)
{
    if(Key1 == 0)
    {
        show_page = !show_page;
        while(Key1 == 0);
        OLED_Clear();
    }

    if(Key2 == 0)
    {
        state++;
        if(state >=6)
        {
            state = 0;
        }
        while(Key2 == 0);
        OLED_Clear();
    }

    if(Key3 == 0)
    {
        if(state == 1)
        {
            temperature_L++;
            sprintf(showBuffer, "已增加温度下限阈值为：%d ℃",temperature_L);
            Serial_SendString((char*)showBuffer);
        }
        if(state == 2)
        {
            temperature_H++;
            sprintf(showBuffer, "已增加温度上限阈值为：%d ℃",temperature_H);
            Serial_SendString((char*)showBuffer);
        }
        if(state == 3)
        {
            humidity_L++;
            sprintf(showBuffer, "已增加湿度下限阈值为：%d RH",humidity_L);
            Serial_SendString((char*)showBuffer);
        }
        if(state == 4)
        {
            humidity_H++;
            sprintf(showBuffer, "已增加湿度上限阈值为：%d RH",humidity_H);
            Serial_SendString((char*)showBuffer);
        }
        if(state == 5)
        {
            PM_Value+=5;
            sprintf(showBuffer, "已增加PM2.5阈值为：%d",PM_Value);
            Serial_SendString((char*)showBuffer);
        }
        while(Key3 == 0);
    }

    if(Key4 == 0)
    {
        if(state == 1)
        {
            temperature_L--;
            sprintf(showBuffer, "已减小温度下限阈值为：%d℃",temperature_L);
            Serial_SendString((char*)showBuffer);
        }
        if(state == 2)
        {
            temperature_H--;
            sprintf(showBuffer, "已减小温度上限阈值为：%d℃",temperature_H);
            Serial_SendString((char*)showBuffer);
        }
        if(state == 3)
        {
            humidity_L--;
            sprintf(showBuffer, "已减小湿度下限阈值为：%d RH",humidity_L);
            Serial_SendString((char*)showBuffer);
        }
        if(state == 4)
        {
            humidity_H--;
            sprintf(showBuffer, "已减小湿度上限阈值为：%d RH",humidity_H);
            Serial_SendString((char*)showBuffer);
        }
        if(state == 5)
        {
            PM_Value-=5;
            sprintf(showBuffer, "已减小PM2.5阈值为：%d",PM_Value);
            Serial_SendString((char*)showBuffer);
        }
        while(Key4 == 0);
    }
}

void show_function(void)
{
    if(show_page == 0)		//如果是显示界面
    {
        Serial_SendString("\r\n------------------\r\n");
        Serial_SendString("-电子科技大学中山学院-\r\n");
        Serial_SendString("----------------------\r\n");

		sprintf(showBuffer, "调整温度上下限：命令0、1、2、3\r\n");
		Serial_SendString((char*)showBuffer);
		sprintf(showBuffer, "调整湿度上下限：命令4、5、6、7\r\n");
		Serial_SendString((char*)showBuffer);
		sprintf(showBuffer, "调整PM2.5上下限：命令8、9\r\n");
		Serial_SendString((char*)showBuffer);
		
        sprintf(showBuffer, "\r\n当前温度值:%.1f ℃\r\n",temperature);
        Serial_SendString((char*)showBuffer);
        sprintf(showBuffer, "当前湿度值:%.2f RH\r\n",humidity);
        Serial_SendString((char*)showBuffer);


        OLED_ShowString(0, 0, (u8*)"Temp:",16);
        OLED_ShowNum(40, 0, DHT11_Data.temp_int, 2,16);
        OLED_ShowString(56, 0, (u8*)".",16);
        OLED_ShowNum(64,0, DHT11_Data.temp_deci,1,16);
        OLED_ShowString(86,0, (u8*)"C",16);

        OLED_ShowString(0, 20, (u8*)"Thum:",16);
        OLED_ShowNum(40, 20, DHT11_Data.humi_int, 2,16);
        OLED_ShowString(56, 20, (u8*)".",16);
        OLED_ShowNum(64,20, DHT11_Data.humi_deci, 2,16);
        OLED_ShowString(86,20, (u8*)"RH",16);

        OLED_ShowString(0, 40, (u8*)"PM:",16);
        OLED_ShowNum(30, 40, PM, 4,16);
        Alarm_function();		//采样数据并显示后，判断是否要报警
    }

    if(show_page == 1)		//如果是设置界面
    {
        OLED_ShowString(0, 0, (u8*)"T_L:",16);
        OLED_ShowNum(30, 0, temperature_L, 2,16);
        OLED_ShowString(60, 0, (u8*)"T_H:",16);
        OLED_ShowNum(90, 0, temperature_H, 2,16);

        OLED_ShowString(0, 20, (u8*)"H_L:",16);
        OLED_ShowNum(30, 20, humidity_L, 2,16);
        OLED_ShowString(60, 20, (u8*)"H_H:",16);
        OLED_ShowNum(90, 20, humidity_H, 2,16);

        OLED_ShowString(0, 40, (u8*)"PM_V:",16);
        OLED_ShowNum(40, 40, PM_Value, 4,16);

        if(state == 1)
        {
            OLED_ShowString(48, 0, (u8*)"<",16);
        }
        if(state == 2)
        {
            OLED_ShowString(110, 0, (u8*)"<",16);
        }
        if(state == 3)
        {
            OLED_ShowString(48, 20, (u8*)"<",16);
        }
        if(state == 4)
        {
            OLED_ShowString(110, 20, (u8*)"<",16);
        }
        if(state == 5)
        {
            OLED_ShowString(80, 40, (u8*)"<",16);
        }
    }

    OLED_Refresh();
}

void Alarm_function(void)
{
    if(temperature<temperature_L||temperature>temperature_H||humidity<humidity_L||humidity>humidity_H||PM>PM_Value)	//超出阈值则报警
    {
        Beep = 0;
    }
    else
    {
        Beep = 1;
    }
}

void lanya_mingling(void)
{
    if (Serial_GetRxFlag() == 1)			//检查串口接收数据的标志位
    {
        RxData = Serial_GetRxData();		//获取串口接收的数据
        if(RxData == '0')
        {
            temperature_L++;
            sprintf(showBuffer, "已增加温度下限阈值为：%d ℃",temperature_L);
            Serial_SendString((char*)showBuffer);
        }
        if(RxData == '1')
        {
            temperature_L--;
            sprintf(showBuffer, "已减小温度下限阈值为：%d℃",temperature_L);
            Serial_SendString((char*)showBuffer);
        }
        if(RxData == '2')
        {
            temperature_H++;
            sprintf(showBuffer, "已增加温度上限阈值为：%d ℃",temperature_H);
            Serial_SendString((char*)showBuffer);
        }
        if(RxData == '3')
        {
            temperature_H--;
            sprintf(showBuffer, "已减小温度上限阈值为：%d℃",temperature_H);
            Serial_SendString((char*)showBuffer);
        }
        if(RxData == '4')
        {
            humidity_L++;
            sprintf(showBuffer, "已增加湿度下限阈值为：%d RH",humidity_L);
            Serial_SendString((char*)showBuffer);
        }
        if(RxData == '5')
        {
            humidity_L--;
            sprintf(showBuffer, "已减小湿度下限阈值为：%d RH",humidity_L);
            Serial_SendString((char*)showBuffer);
        }
        if(RxData == '6')
        {
            humidity_H++;
            sprintf(showBuffer, "已增加湿度上限阈值为：%d RH",humidity_H);
            Serial_SendString((char*)showBuffer);
        }
        if(RxData == '7')
        {
            humidity_H--;
            sprintf(showBuffer, "已减小湿度上限阈值为：%d RH",humidity_H);
            Serial_SendString((char*)showBuffer);
        }
		if(RxData == '8')
        {
            PM_Value+=5;
            sprintf(showBuffer, "已增加PM2.5阈值为：%d",PM_Value);
            Serial_SendString((char*)showBuffer);
        }
        if(RxData == '9')
        {
            PM_Value-=5;
            sprintf(showBuffer, "已减小PM2.5阈值为：%d",PM_Value);
            Serial_SendString((char*)showBuffer);
        }
    }
}
