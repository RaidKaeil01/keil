/***************STM32F103C8T6**********************
 * �ļ���  ��delay.c
 * ����    ��delay��ȷ��ʱ	
 * ��ע    ������ϵͳ��ʱ��SysTick  

********************LIGEN*************************/

#include "delay.h"
#include "cmsis_os.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���
//����delay_us,delay_ms
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/5/27
//�汾��V1.2
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.2�޸�˵��
//�������ж��е��ó�����ѭ���Ĵ���
//��ֹ��ʱ��׼ȷ,����do while�ṹ!
//////////////////////////////////////////////////////////////////////////////////	 

//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init()
{
// ���� DWT ����
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    // ��λ������
    DWT->CYCCNT = 0;
    // ʹ�� CYCCNT ����
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}								    
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
			osDelay(nms);
}   
//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(u32 nus)
{		
    uint32_t start_tick = DWT->CYCCNT;
    uint32_t delay_ticks = nus * (SystemCoreClock / 1000000); // �����Ӧ��ʱ��������

    while ((DWT->CYCCNT - start_tick) < delay_ticks);
}

