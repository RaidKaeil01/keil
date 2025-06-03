/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H


#include "stm32f10x.h"

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION		1																		/* 1: 抢占式调度器, 0: 协程式调度器, 无默认需定义 */
#define configUSE_IDLE_HOOK			0																		/* 1: 使能空闲任务钩子函数, 无默认需定义  */
#define configUSE_TICK_HOOK			0																		/* 1: 使能系统时钟节拍中断钩子函数, 无默认需定义 */
#define configCPU_CLOCK_HZ			SystemCoreClock		//此处为：( ( unsigned long ) 72000000 )/* 定义CPU主频, 单位: Hz, 无默认需定义 */
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )							/* 定义系统时钟节拍频率, 单位: Hz, 无默认需定义 */
#define configMAX_PRIORITIES		( 32 )																/* 定义最大优先级数, 最大优先级=configMAX_PRIORITIES-1, 无默认需定义 */
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 128 )				/* 定义空闲任务的栈空间大小, 单位: Word, 无默认需定义 */
#define configTOTAL_HEAP_SIZE		( ( size_t ) ( 17 * 1024 ) )				/* FreeRTOS堆中可用的RAM总量, 单位: Byte, 无默认需定义 17K*/
#define configMAX_TASK_NAME_LEN		( 16 )														/* 定义任务名最大字符数, 默认: 16 */
#define configUSE_TRACE_FACILITY	0																	/* 1: 使能可视化跟踪调试, 默认: 0 */
#define configUSE_16_BIT_TICKS		0																	/* 1: 定义系统时钟节拍计数器的数据类型为16位无符号数, 无默认需定义 */
#define configIDLE_SHOULD_YIELD		1																	/* 1: 使能在抢占式调度下,同优先级的任务能抢占空闲任务, 默认: 1 */

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		0																	/* 1: 启用协程, 默认: 0 */
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )												/* 定义协程的最大优先级, 最大优先级=configMAX_CO_ROUTINE_PRIORITIES-1, 无默认configUSE_CO_ROUTINES为1时需定义 */

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1		/* 设置任务优先级 */
#define INCLUDE_uxTaskPriorityGet		1		/* 获取任务优先级 */
#define INCLUDE_vTaskDelete					1		/* 删除任务 */
#define INCLUDE_vTaskCleanUpResources	0	
#define INCLUDE_vTaskSuspend				1		/* 挂起任务 */
#define INCLUDE_vTaskDelayUntil			1		/* 任务绝对延时 */
#define INCLUDE_vTaskDelay					1		/* 任务延时 */


		/*中断原本配置*/
/////* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255
////(lowest) to 0 (1?) (highest). */
////#define configKERNEL_INTERRUPT_PRIORITY 		255

/////* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
////See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
////#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	191 /* equivalent to 0xb0, or priority 11. */

/////* This is the value being used as per the ST library which permits 16
////priority values, 0 to 15.  This must correspond to the
////configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
////NVIC value of 255. */
////#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15


																	/* 相比原文件所添加的配置	 */

#define xPortSysTickHandler SysTick_Handler


/* 可选函数, 1: 使能*/
#define configUSE_PORT_OPTIMISED_TASK_SELECTION         1                       /* 1: 使用硬件计算下一个要运行的任务, 0: 使用软件算法计算下一个要运行的任务, 默认: 0 */
#define INCLUDE_uxTaskGetStackHighWaterMark             0                       /* 获取任务堆栈历史剩余最小值 */

#define INCLUDE_xTaskGetCurrentTaskHandle               1                       /* 获取当前任务的任务句柄 */



/****************************************************************
            FreeRTOS与中断服务函数有关的新配置选项                         
****************************************************************/

/* 中断嵌套行为配置 */
#ifdef __NVIC_PRIO_BITS
    #define configPRIO_BITS __NVIC_PRIO_BITS
#else
    #define configPRIO_BITS 4
#endif

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY         15                  /* 中断最低优先级 */	//模板未定义
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY    5                   /* FreeRTOS可管理的最高中断优先级 */	//模板未定义
#define configKERNEL_INTERRUPT_PRIORITY                 ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY            ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_API_CALL_INTERRUPT_PRIORITY           configMAX_SYSCALL_INTERRUPT_PRIORITY

#define xPortPendSVHandler 	PendSV_Handler
#define vPortSVCHandler 	SVC_Handler

/* 内存分配相关定义 */
#define configSUPPORT_STATIC_ALLOCATION                 0                       /* 1: 支持静态申请内存, 默认: 0 */
#define configSUPPORT_DYNAMIC_ALLOCATION                1                       /* 1: 支持动态申请内存, 默认: 1 */


/* 断言(调试找错) */
//#define vAssertCalled(char, int) printf("Error: %s, %d\r\n", char, int)
//#define configASSERT( x ) if( ( x ) == 0 ) vAssertCalled( __FILE__, __LINE__ )

#endif /* FREERTOS_CONFIG_H */

