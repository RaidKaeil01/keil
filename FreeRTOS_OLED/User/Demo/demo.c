#include "demo.h"
#include "oled.h"

/******************************************************************************************************/
/*FreeRTOS����*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO 1
#define START_TASK_STACK_SIZE 128
TaskHandle_t start_task_hander;
void start_task( void * pvParameters );	//��������

/* task1 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define task1_PRIO 2
#define task1_STACK_SIZE 128
TaskHandle_t task1_hander;
void task1( void * pvParameters );

/* task2 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define task2_PRIO 2
#define task2_STACK_SIZE 256
TaskHandle_t task2_hander;
void task2( void * pvParameters );

/* task3 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
//#define task3_PRIO 4
//#define task3_STACK_SIZE 128
//TaskHandle_t task3_hander;
//void task3( void * pvParameters );

/******************************************************************************************************/

void FreeRTOS_Demo(void)
{
	xTaskCreate( (TaskFunction_t		) start_task,				/* ָ����������ָ�� */
				 (char *				) "start_task", 			/* �������֣���󳤶�configMAX_TASK_NAME_LEN */
				 (configSTACK_DEPTH_TYPE) START_TASK_STACK_SIZE,	/* �����ջ��С��ע����Ϊ��λ */
				 (void *				) NULL,						/* ���ݸ��������Ĳ��� */
				 (UBaseType_t			) START_TASK_PRIO,			/* �������ȼ�����Χ0~configMAX_PRIORITIES-1 */
				 (TaskHandle_t *		) &start_task_hander );		/* �����������������������ƿ� */
	vTaskStartScheduler();
}



void start_task( void * pvParameters )
{
	taskENTER_CRITICAL();		/* �����ٽ��� */
	xTaskCreate( (TaskFunction_t		) task1,				/* ָ����������ָ�� */
				 (char *				) "task1", 				/* �������֣���󳤶�configMAX_TASK_NAME_LEN */
				 (configSTACK_DEPTH_TYPE) task1_STACK_SIZE,	/* �����ջ��С��ע����Ϊ��λ */
				 (void *				) NULL,						/* ���ݸ��������Ĳ��� */
				 (UBaseType_t			) task1_PRIO,			/* �������ȼ�����Χ0~configMAX_PRIORITIES-1 */
				 (TaskHandle_t *		) &task1_hander );		/* �����������������������ƿ� */
	
			 
	xTaskCreate( (TaskFunction_t		) task2,				/* ָ����������ָ�� */
				 (char *				) "task2", 				/* �������֣���󳤶�configMAX_TASK_NAME_LEN */
				 (configSTACK_DEPTH_TYPE) task2_STACK_SIZE,	/* �����ջ��С��ע����Ϊ��λ */
				 (void *				) NULL,						/* ���ݸ��������Ĳ��� */
				 (UBaseType_t			) task2_PRIO,			/* �������ȼ�����Χ0~configMAX_PRIORITIES-1 */
				 (TaskHandle_t *		) &task2_hander );		/* �����������������������ƿ� */
				 
				 
//	xTaskCreate( (TaskFunction_t		) task3,				/* ָ����������ָ�� */
//				 (char *				) "task3", 				/* �������֣���󳤶�configMAX_TASK_NAME_LEN */
//				 (configSTACK_DEPTH_TYPE) task3_STACK_SIZE,	/* �����ջ��С��ע����Ϊ��λ */
//				 (void *				) NULL,						/* ���ݸ��������Ĳ��� */
//				 (UBaseType_t			) task3_PRIO,			/* �������ȼ�����Χ0~configMAX_PRIORITIES-1 */
//				 (TaskHandle_t *		) &task3_hander );		/* �����������������������ƿ� */
	
		 
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
		OLED_ShowChinese(0,0,0,16,1);//��
		OLED_ShowChinese(18,0,1,16,1);//��
		OLED_ShowChinese(36,0,2,16,1);//԰
		OLED_ShowChinese(54,0,3,16,1);//��
		OLED_ShowChinese(72,0,4,16,1);//��
		OLED_ShowChinese(90,0,5,16,1);//��
		OLED_ShowChinese(108,0,6,16,1);//��
		OLED_ShowChar(48,48,t,16,1);//��ʾASCII�ַ�	   
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
 



