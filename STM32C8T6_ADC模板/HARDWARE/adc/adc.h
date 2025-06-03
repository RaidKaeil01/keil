#ifndef __ADC_H
#define __ADC_H
#include "stm32f10x.h"
#include "stdlib.h"	

void ADCx_Iint(void);
u16 Get_ADC_Value(u8 ch, u8 times);

#endif


