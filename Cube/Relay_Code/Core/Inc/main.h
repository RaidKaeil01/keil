/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Run_Pin GPIO_PIN_13
#define LED_Run_GPIO_Port GPIOC
#define SID_Pin GPIO_PIN_14
#define SID_GPIO_Port GPIOC
#define SCLK_Pin GPIO_PIN_15
#define SCLK_GPIO_Port GPIOC
#define Mode1_Pin GPIO_PIN_0
#define Mode1_GPIO_Port GPIOA
#define Mode2_Pin GPIO_PIN_1
#define Mode2_GPIO_Port GPIOA
#define Mode3_Pin GPIO_PIN_4
#define Mode3_GPIO_Port GPIOA
#define Mode2_1_Pin GPIO_PIN_5
#define Mode2_1_GPIO_Port GPIOA
#define Mode2_2_Pin GPIO_PIN_6
#define Mode2_2_GPIO_Port GPIOA
#define Mode2_3_Pin GPIO_PIN_7
#define Mode2_3_GPIO_Port GPIOA
#define KEY_A_Pin GPIO_PIN_0
#define KEY_A_GPIO_Port GPIOB
#define KEY_B_Pin GPIO_PIN_1
#define KEY_B_GPIO_Port GPIOB
#define KEY_C_Pin GPIO_PIN_10
#define KEY_C_GPIO_Port GPIOB
#define KEY_D_Pin GPIO_PIN_11
#define KEY_D_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
