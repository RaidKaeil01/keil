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

// ��ʾ������
char showBuffer[50] = {0, },show_page = 0,state = 0;

uint8_t RxData;	//�����������ݱ���
DHT11_Data_TypeDef DHT11_Data;	//DHT11�ṹ��
float temperature=0;        // temperature [��C]	�¶�
float humidity=0;			//ʪ��
int PM=0;					//PM2.5

/* ��Ӧ��ֵ */
int temperature_L = 20,temperature_H = 40;
int humidity_L = 20,humidity_H = 60;
int PM_Value = 400;

void Key_function(void);	//����ɨ�躯��
void show_function(void);	//OLED��ʾ����
void Alarm_function(void);	//��������������
void lanya_mingling(void);	//���������

int main(void)
{
    int i = 0;
    delay_init();	    //��ʱ������ʼ��
    LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
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
			temperature=DHT11_Data.temp_int+((float)DHT11_Data.temp_deci)/10;	//��ȡ��ǰ�¶�
			humidity=DHT11_Data.humi_int+((float)DHT11_Data.humi_deci)/10;		//��ȡ��ǰʪ��
			PM = Get_PM25_Average_Data();		//��ȡ��ǰPM2.5
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
            sprintf(showBuffer, "�������¶�������ֵΪ��%d ��",temperature_L);
            Serial_SendString((char*)showBuffer);
        }
        if(state == 2)
        {
            temperature_H++;
            sprintf(showBuffer, "�������¶�������ֵΪ��%d ��",temperature_H);
            Serial_SendString((char*)showBuffer);
        }
        if(state == 3)
        {
            humidity_L++;
            sprintf(showBuffer, "������ʪ��������ֵΪ��%d RH",humidity_L);
            Serial_SendString((char*)showBuffer);
        }
        if(state == 4)
        {
            humidity_H++;
            sprintf(showBuffer, "������ʪ��������ֵΪ��%d RH",humidity_H);
            Serial_SendString((char*)showBuffer);
        }
        if(state == 5)
        {
            PM_Value+=5;
            sprintf(showBuffer, "������PM2.5��ֵΪ��%d",PM_Value);
            Serial_SendString((char*)showBuffer);
        }
        while(Key3 == 0);
    }

    if(Key4 == 0)
    {
        if(state == 1)
        {
            temperature_L--;
            sprintf(showBuffer, "�Ѽ�С�¶�������ֵΪ��%d��",temperature_L);
            Serial_SendString((char*)showBuffer);
        }
        if(state == 2)
        {
            temperature_H--;
            sprintf(showBuffer, "�Ѽ�С�¶�������ֵΪ��%d��",temperature_H);
            Serial_SendString((char*)showBuffer);
        }
        if(state == 3)
        {
            humidity_L--;
            sprintf(showBuffer, "�Ѽ�Сʪ��������ֵΪ��%d RH",humidity_L);
            Serial_SendString((char*)showBuffer);
        }
        if(state == 4)
        {
            humidity_H--;
            sprintf(showBuffer, "�Ѽ�Сʪ��������ֵΪ��%d RH",humidity_H);
            Serial_SendString((char*)showBuffer);
        }
        if(state == 5)
        {
            PM_Value-=5;
            sprintf(showBuffer, "�Ѽ�СPM2.5��ֵΪ��%d",PM_Value);
            Serial_SendString((char*)showBuffer);
        }
        while(Key4 == 0);
    }
}

void show_function(void)
{
    if(show_page == 0)		//�������ʾ����
    {
        Serial_SendString("\r\n------------------\r\n");
        Serial_SendString("-���ӿƼ���ѧ��ɽѧԺ-\r\n");
        Serial_SendString("----------------------\r\n");

		sprintf(showBuffer, "�����¶������ޣ�����0��1��2��3\r\n");
		Serial_SendString((char*)showBuffer);
		sprintf(showBuffer, "����ʪ�������ޣ�����4��5��6��7\r\n");
		Serial_SendString((char*)showBuffer);
		sprintf(showBuffer, "����PM2.5�����ޣ�����8��9\r\n");
		Serial_SendString((char*)showBuffer);
		
        sprintf(showBuffer, "\r\n��ǰ�¶�ֵ:%.1f ��\r\n",temperature);
        Serial_SendString((char*)showBuffer);
        sprintf(showBuffer, "��ǰʪ��ֵ:%.2f RH\r\n",humidity);
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
        Alarm_function();		//�������ݲ���ʾ���ж��Ƿ�Ҫ����
    }

    if(show_page == 1)		//��������ý���
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
    if(temperature<temperature_L||temperature>temperature_H||humidity<humidity_L||humidity>humidity_H||PM>PM_Value)	//������ֵ�򱨾�
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
    if (Serial_GetRxFlag() == 1)			//��鴮�ڽ������ݵı�־λ
    {
        RxData = Serial_GetRxData();		//��ȡ���ڽ��յ�����
        if(RxData == '0')
        {
            temperature_L++;
            sprintf(showBuffer, "�������¶�������ֵΪ��%d ��",temperature_L);
            Serial_SendString((char*)showBuffer);
        }
        if(RxData == '1')
        {
            temperature_L--;
            sprintf(showBuffer, "�Ѽ�С�¶�������ֵΪ��%d��",temperature_L);
            Serial_SendString((char*)showBuffer);
        }
        if(RxData == '2')
        {
            temperature_H++;
            sprintf(showBuffer, "�������¶�������ֵΪ��%d ��",temperature_H);
            Serial_SendString((char*)showBuffer);
        }
        if(RxData == '3')
        {
            temperature_H--;
            sprintf(showBuffer, "�Ѽ�С�¶�������ֵΪ��%d��",temperature_H);
            Serial_SendString((char*)showBuffer);
        }
        if(RxData == '4')
        {
            humidity_L++;
            sprintf(showBuffer, "������ʪ��������ֵΪ��%d RH",humidity_L);
            Serial_SendString((char*)showBuffer);
        }
        if(RxData == '5')
        {
            humidity_L--;
            sprintf(showBuffer, "�Ѽ�Сʪ��������ֵΪ��%d RH",humidity_L);
            Serial_SendString((char*)showBuffer);
        }
        if(RxData == '6')
        {
            humidity_H++;
            sprintf(showBuffer, "������ʪ��������ֵΪ��%d RH",humidity_H);
            Serial_SendString((char*)showBuffer);
        }
        if(RxData == '7')
        {
            humidity_H--;
            sprintf(showBuffer, "�Ѽ�Сʪ��������ֵΪ��%d RH",humidity_H);
            Serial_SendString((char*)showBuffer);
        }
		if(RxData == '8')
        {
            PM_Value+=5;
            sprintf(showBuffer, "������PM2.5��ֵΪ��%d",PM_Value);
            Serial_SendString((char*)showBuffer);
        }
        if(RxData == '9')
        {
            PM_Value-=5;
            sprintf(showBuffer, "�Ѽ�СPM2.5��ֵΪ��%d",PM_Value);
            Serial_SendString((char*)showBuffer);
        }
    }
}
