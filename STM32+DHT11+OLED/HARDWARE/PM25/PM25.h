#ifndef __PM25_H
#define	__PM25_H
#include "stm32f10x.h"
#include "adcx.h"
#include "delay.h"
#include "math.h"

#define PM25_READ_TIMES	10  //PM2.5粉尘传感器ADC循环读取次数

/***************根据自己需求更改****************/
// PM25 GPIO宏定义
#define		PM25_GPIO_CLK									RCC_APB2Periph_GPIOA
#define 	PM25_LED_GPIO_PORT						GPIOA
#define 	PM25_LED_GPIO_PIN							GPIO_Pin_0
#define 	PM25_VO_GPIO_PORT							GPIOA
#define 	PM25_VO_GPIO_PIN							GPIO_Pin_1
/*********************END**********************/

#define PM25_LED_H			GPIO_SetBits(PM25_LED_GPIO_PORT,PM25_LED_GPIO_PIN);
#define PM25_LED_L			GPIO_ResetBits(PM25_LED_GPIO_PORT,PM25_LED_GPIO_PIN);



// ADC 通道宏定义
#define   ADC_CHANNEL               		ADC_Channel_1

void PM25_Init(void);
u16 PM25_GetData(void);
u16 Get_PM25_Average_Data(void);

#endif /* __ADC_H */

