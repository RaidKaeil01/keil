/***************STM32F103C8T6**********************
 * 文件名  ：delay.c
 * 描述    ：delay精确延时	
 * 备注    ：采用系统定时器SysTick  

********************LIGEN*************************/

#include "delay.h"
#include "cmsis_os.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//使用SysTick的普通计数模式对延迟进行管理
//包括delay_us,delay_ms
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/5/27
//版本：V1.2
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.2修改说明
//修正了中断中调用出现死循环的错误
//防止延时不准确,采用do while结构!
//////////////////////////////////////////////////////////////////////////////////	 

//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init()
{
// 允许 DWT 访问
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    // 复位计数器
    DWT->CYCCNT = 0;
    // 使能 CYCCNT 计数
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}								    
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
			osDelay(nms);
}   
//延时nus
//nus为要延时的us数.		    								   
void delay_us(u32 nus)
{		
    uint32_t start_tick = DWT->CYCCNT;
    uint32_t delay_ticks = nus * (SystemCoreClock / 1000000); // 计算对应的时钟周期数

    while ((DWT->CYCCNT - start_tick) < delay_ticks);
}

