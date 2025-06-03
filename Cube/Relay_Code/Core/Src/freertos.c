/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "delay.h"
#include "lcd12864.h"
#include "pushButton.h"
#include "menu.h"
#include "usart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId Relay_TaskHandle;
osThreadId pushButtonHandle;
osThreadId LEDHandle;
osMessageQId pushButtonQueueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void RELAY_TASK(void const * argument);
void PushButton(void const * argument);
void LED_task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of pushButtonQueue */
  osMessageQDef(pushButtonQueue, 16, uint16_t);
  pushButtonQueueHandle = osMessageCreate(osMessageQ(pushButtonQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of Relay_Task */
  osThreadDef(Relay_Task, RELAY_TASK, osPriorityIdle, 0, 128);
  Relay_TaskHandle = osThreadCreate(osThread(Relay_Task), NULL);

  /* definition and creation of pushButton */
  osThreadDef(pushButton, PushButton, osPriorityIdle, 0, 128);
  pushButtonHandle = osThreadCreate(osThread(pushButton), NULL);

  /* definition and creation of LED */
  osThreadDef(LED, LED_task, osPriorityIdle, 0, 128);
  LEDHandle = osThreadCreate(osThread(LED), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    delay_ms(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_RELAY_TASK */
/**
* @brief Function implementing the Relay_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_RELAY_TASK */
void RELAY_TASK(void const * argument)
{
  /* USER CODE BEGIN RELAY_TASK */
	uint16_t count = 0;

  /* Infinite loop */
  for(;;)
  {
		if(mode == 1)
		{
			HAL_GPIO_WritePin(Mode1_GPIO_Port, Mode1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(Mode2_GPIO_Port, Mode2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(Mode3_GPIO_Port, Mode3_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Mode2_1_GPIO_Port, Mode2_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Mode2_2_GPIO_Port, Mode2_2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Mode2_3_GPIO_Port, Mode2_3_Pin, GPIO_PIN_RESET);
		}
		if(mode == 2)
		{
			count++;
			if(count == 2000)
			{
				count = 0;
				count1++;
				if(count1 == 3)
				{
					count1 = 0;
				}
			}
			HAL_GPIO_WritePin(Mode1_GPIO_Port, Mode1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Mode2_GPIO_Port, Mode2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Mode3_GPIO_Port, Mode3_Pin, GPIO_PIN_RESET);
		}
		if(mode == 3)
		{
			HAL_GPIO_WritePin(Mode1_GPIO_Port, Mode1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Mode2_GPIO_Port, Mode2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Mode3_GPIO_Port, Mode3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(Mode2_1_GPIO_Port, Mode2_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Mode2_2_GPIO_Port, Mode2_2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Mode2_3_GPIO_Port, Mode2_3_Pin, GPIO_PIN_RESET);
		}
    osDelay(1);
  }
  /* USER CODE END RELAY_TASK */
}

/* USER CODE BEGIN Header_PushButton */
/**
* @brief Function implementing the pushButton thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PushButton */
void PushButton(void const * argument)
{
  /* USER CODE BEGIN PushButton */
	uint8_t i;
	delay_init();
	Lcd_Init();
	LCD_Display_Words(0,0,(uint8_t *)"系统启动中");
	for(;i<3;i++)
	{
		LCD_Display_Words(0,5+i,(uint8_t *)".");
		delay_ms(500);
	}
	LCD_Clear();
  /* Infinite loop */
  for(;;)
  {
		menu1();
    delay_ms(1);
  }
  /* USER CODE END PushButton */
}

/* USER CODE BEGIN Header_LED_task */
/**
* @brief Function implementing the LED thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LED_task */
void LED_task(void const * argument)
{
  /* USER CODE BEGIN LED_task */

  /* Infinite loop */
  for(;;)
  {	
		HAL_GPIO_TogglePin(LED_Run_GPIO_Port, LED_Run_Pin);
		printf("aaa");
    osDelay(500);
  }
  /* USER CODE END LED_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

