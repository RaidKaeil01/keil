#include "stm32f10x_it.h"
unsigned int TIM1Counter=0;//定时1计数器
unsigned int TIM2Counter=0;//定时2计数器

/********************************************************************
*名    称:NVIC_Configration(void)
*功    能:中断分组函数，包含TIM1~4和USART1
*入口参数:无
*出口参数:无
*说    明:无
*调用方法:无
********************************************************************/
//void NVIC_Configuration(void)
//{
//	NVIC_InitTypeDef NVIC_InitStructure; 
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				//定时器1优先级分分组 
//	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;			//指定的IRQ中断 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级别 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//响应优先级别
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				//定时器2优先级分分组 
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				//指定的IRQ中断 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级别 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//响应优先级别
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				//定时器3优先级分分组 
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				//指定的IRQ中断 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级别 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//响应优先级别
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				//定时器4优先级分分组 
//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;				//指定的IRQ中断 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级别 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;			//响应优先级别
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				//定时器5优先级分分组 
//	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;				//指定的IRQ中断 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级别 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;			//响应优先级别
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				//定时器6优先级分分组 
//	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;				//指定的IRQ中断 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级别 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;			//响应优先级别
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);
//		
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				//定时器7优先级分分组 
//	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;				//指定的IRQ中断 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级别 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;			//响应优先级别
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);
//		
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				//定时器8优先级分分组 
//	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;			//指定的IRQ中断 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//抢占优先级别 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//响应优先级别
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				//串口优先级分分组 
//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//指定的IRQ中断 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//抢占优先级别 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//响应优先级别
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				//串口优先级分分组 
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//指定的IRQ中断 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//抢占优先级别 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//响应优先级别
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);
//		
//}
/********************************************************************
*名    称:TIM1_UP_IRQHandler(void)
*功    能:TIM1中断服务函数
*入口参数:无
*出口参数:无
*说    明:注意中断函数的名称
*调用方法:无
********************************************************************/
//void TIM1_UP_IRQHandler(void)
//{
//	TIM1Counter++;
//	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);//清除中断标志
//	key.scan_flag=1;
//	if(TIM1Counter==100)
//	{
//		TIM1Counter=0;
//		LED1Reverse();
//	}
//}
/********************************************************************
*名    称:TIM2_IRQHandler(void)
*功    能:TIM2中断服务函数 
*入口参数:无
*出口参数:无
*说    明:无
*调用方法:无
********************************************************************/
//void TIM2_IRQHandler(void)
//{
//	TIM2Counter++;
//	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清除中断标志
//	if(TIM2Counter==2)
//	{
//		TIM2Counter=0;
//	}
//}
/********************************************************************
*名    称:TIM3_IRQHandler(void)
*功    能:TIM3中断服务函数 
*入口参数:无
*出口参数:无
*说    明:无
*调用方法:无
*******************************************************************
void TIM3_IRQHandler(void)
{
	count2++;
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//清除中断标志
	if(count2==100)
	{
		count2=0;
		LED1Reverse();
	}
}*/
/********************************************************************
*名    称:TIM4_IRQHandler(void)
*功    能:TIM4中断服务函数 
*入口参数:无
*出口参数:无
*说    明:无
*调用方法:无
*******************************************************************
void TIM4_IRQHandler(void)
{
	count2++;
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//清除中断标志
	if(count2==100)
	{
		count2=0;
		LED1Reverse();
	}
}*/
/********************************************************************
*名    称:TIM5_IRQHandler(void)
*功    能:TIM5中断服务函数 
*入口参数:无
*出口参数:无
*说    明:无
*调用方法:无
*******************************************************************
void TIM5_IRQHandler(void)
{
	count2++;
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);//清除中断标志
	if(count2==100)
	{
		count2=0;
		LED1Reverse();
	}
}*/
/********************************************************************
*名    称:TIM6_IRQHandler(void)
*功    能:TIM6中断服务函数 
*入口参数:无
*出口参数:无
*说    明:无
*调用方法:无
*******************************************************************
void TIM6_IRQHandler(void)
{
	count2++;
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);//清除中断标志
	if(count2==100)
	{
		count2=0;
		LED1Reverse();
	}
}*/
/********************************************************************
*名    称:TIM7_IRQHandler(void)
*功    能:TIM7中断服务函数 
*入口参数:无
*出口参数:无
*说    明:无
*调用方法:无
*******************************************************************
void TIM7_IRQHandler(void)
{
	count2++;
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);//清除中断标志
	if(count2==100)
	{
		count2=0;
		LED1Reverse();
	}
}*/
/********************************************************************
*名    称:void TIM8_UP_IRQHandler(void)
*功    能:TIM8中断服务函数 
*入口参数:无
*出口参数:无
*说    明:无
*调用方法:无
*******************************************************************
void TIM8_UP_IRQHandler(void)
{
	count2++;
	TIM_ClearITPendingBit(TIM8, TIM_IT_Update);//清除中断标志
	if(count2==100)
	{
		count2=0;
		LED1Reverse();
	}
}*/
/********************************************************************
*名    称:EXIT1_IRQHandler(void)
*功    能:EXIT1外部中断1中断服务函数
*入口参数:无
*出口参数:无
*说    明:无
*调用方法:无
*******************************************************************
void EXTI1_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line1);//清除EXTI1标志
	LED1Reverse();
}*/
/********************************************************************
*名    称:EXTI9_5_IRQHandler(void)
*功    能:EXIT1外部中断9-5中断服务函数
*入口参数:无
*出口参数:无
*说    明:无
*调用方法:无
*******************************************************************/
//unsigned char EC11_Clockwise=0;			//顺时针状态
//unsigned char EC11_Counterclockwise=0;	//逆时针状态
//void EXTI9_5_IRQHandler(void)
//{
//	if(EXTI_GetFlagStatus(EXTI_Line8))				//判断中断来源
//	{
//		EXTI_ClearITPendingBit(EXTI_Line8);
//		if(EC11_Read_A()!= EC11_Read_B())			//顺时针下降沿 第二次为上升沿  AB电平相反
//		{
//			if((EC11_Read_A() == 0)&&(EC11_Read_B() == 1))
//			{
//				EC11_Clockwise = 1;
//			}
//			
//			if((EC11_Read_A() == 1)&&(EC11_Read_B() == 0))
//			{
//				if(EC11_Clockwise == 1)
//				{
//					EC11_Clockwise = 0;
//					KeyNO++;
//				}				
//			}
//		}
//		if(EC11_Read_A() == EC11_Read_B())			//逆时针下降沿 第二次为上升沿  AB电平相同
//		{
//			if((EC11_Read_A() == 0)&&(EC11_Read_B() == 0))
//			{
//				EC11_Counterclockwise = 1;
//			}
//			
//			if((EC11_Read_A() == 1)&&(EC11_Read_B() == 1))
//			{
//				if(EC11_Counterclockwise == 1)
//				{
//					EC11_Counterclockwise = 0;
//					KeyNO--;
//				}				
//			}
//		}
//					
//		
//		
//		
//		//LED1Reverse();
//		//if(EC11_Read_A()== EC11_Read_B())
//			//KeyNO--;//LED1Reverse();
//	}
//	//if(EXTI_GetFlagStatus(EXTI_Line9))
//	//{
//	//	EXTI_ClearITPendingBit(EXTI_Line9);
//	//}
//}

















/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
