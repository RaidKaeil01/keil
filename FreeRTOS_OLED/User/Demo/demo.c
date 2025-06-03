#include "demo.h"
#include "oled.h"

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 1
#define START_TASK_STACK_SIZE 128
TaskHandle_t start_task_hander;
void start_task( void * pvParameters );	//启动任务

/* task1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define task1_PRIO 2
#define task1_STACK_SIZE 128
TaskHandle_t task1_hander;
void task1( void * pvParameters );

/* task2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define task2_PRIO 2
#define task2_STACK_SIZE 256
TaskHandle_t task2_hander;
void task2( void * pvParameters );

/* task3 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
//#define task3_PRIO 4
//#define task3_STACK_SIZE 128
//TaskHandle_t task3_hander;
//void task3( void * pvParameters );

/******************************************************************************************************/

void FreeRTOS_Demo(void)
{
	xTaskCreate( (TaskFunction_t		) start_task,				/* 指向任务函数的指针 */
				 (char *				) "start_task", 			/* 任务名字，最大长度configMAX_TASK_NAME_LEN */
				 (configSTACK_DEPTH_TYPE) START_TASK_STACK_SIZE,	/* 任务堆栈大小，注意字为单位 */
				 (void *				) NULL,						/* 传递给任务函数的参数 */
				 (UBaseType_t			) START_TASK_PRIO,			/* 任务优先级，范围0~configMAX_PRIORITIES-1 */
				 (TaskHandle_t *		) &start_task_hander );		/* 任务句柄，就是任务的任务控制块 */
	vTaskStartScheduler();
}



void start_task( void * pvParameters )
{
	taskENTER_CRITICAL();		/* 进入临界区 */
	xTaskCreate( (TaskFunction_t		) task1,				/* 指向任务函数的指针 */
				 (char *				) "task1", 				/* 任务名字，最大长度configMAX_TASK_NAME_LEN */
				 (configSTACK_DEPTH_TYPE) task1_STACK_SIZE,	/* 任务堆栈大小，注意字为单位 */
				 (void *				) NULL,						/* 传递给任务函数的参数 */
				 (UBaseType_t			) task1_PRIO,			/* 任务优先级，范围0~configMAX_PRIORITIES-1 */
				 (TaskHandle_t *		) &task1_hander );		/* 任务句柄，就是任务的任务控制块 */
	
			 
	xTaskCreate( (TaskFunction_t		) task2,				/* 指向任务函数的指针 */
				 (char *				) "task2", 				/* 任务名字，最大长度configMAX_TASK_NAME_LEN */
				 (configSTACK_DEPTH_TYPE) task2_STACK_SIZE,	/* 任务堆栈大小，注意字为单位 */
				 (void *				) NULL,						/* 传递给任务函数的参数 */
				 (UBaseType_t			) task2_PRIO,			/* 任务优先级，范围0~configMAX_PRIORITIES-1 */
				 (TaskHandle_t *		) &task2_hander );		/* 任务句柄，就是任务的任务控制块 */
				 
				 
//	xTaskCreate( (TaskFunction_t		) task3,				/* 指向任务函数的指针 */
//				 (char *				) "task3", 				/* 任务名字，最大长度configMAX_TASK_NAME_LEN */
//				 (configSTACK_DEPTH_TYPE) task3_STACK_SIZE,	/* 任务堆栈大小，注意字为单位 */
//				 (void *				) NULL,						/* 传递给任务函数的参数 */
//				 (UBaseType_t			) task3_PRIO,			/* 任务优先级，范围0~configMAX_PRIORITIES-1 */
//				 (TaskHandle_t *		) &task3_hander );		/* 任务句柄，就是任务的任务控制块 */
	
		 
	vTaskDelete(NULL);
	taskEXIT_CRITICAL();
}



void task1( void * pvParameters )
{
	for( ;; )
	{
		// Task code goes here.
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		vTaskDelay(500);
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		vTaskDelay(500);
	}
}

void task2( void * pvParameters )
{
	u8 t=' ';
	for( ;; )
	{
		// Task code goes here.
//		GPIO_SetBits(GPIOC,GPIO_Pin_13);
//		vTaskDelay(500);
//		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		OLED_ShowChinese(0,0,0,16,1);//中
		OLED_ShowChinese(18,0,1,16,1);//景
		OLED_ShowChinese(36,0,2,16,1);//园
		OLED_ShowChinese(54,0,3,16,1);//电
		OLED_ShowChinese(72,0,4,16,1);//子
		OLED_ShowChinese(90,0,5,16,1);//技
		OLED_ShowChinese(108,0,6,16,1);//术
		OLED_ShowChar(48,48,t,16,1);//显示ASCII字符	   
		if(t>'~')t=' ';
		OLED_Refresh();
		vTaskDelay(500);
		t++;
	}
}

//void task3( void * pvParameters )
//{
//	for( ;; )
//	{
//		// Task code goes here.
//		GPIO_SetBits(GPIOC,GPIO_Pin_13);
//		vTaskDelay(500);
//		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
//		vTaskDelay(500);
//	}
//}
 



