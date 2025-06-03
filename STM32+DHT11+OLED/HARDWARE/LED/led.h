#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED0 PCout(13)

#define Beep PBout(11)

#define Key1 PBin(12)
#define Key2 PBin(13)
#define Key3 PBin(14)
#define Key4 PBin(15)


void LED_Init(void);//≥ı ºªØ

		 				    
#endif
