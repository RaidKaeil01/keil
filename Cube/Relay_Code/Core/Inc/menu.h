#ifndef __MENU_H__
#define __MENU_H__

#include "main.h"
#include "lcd12864.h"
#include "Key.h"
//#include "ads1115.h"

/*** 全局变量设置 ****/
extern float tongduan_up;
extern float tongduan_down;

extern float jueyuan_timer;

extern int cichang_flag;
extern uint8_t mode;
extern uint8_t count1;

uint8_t menu1(void);
uint8_t menu2_set(uint8_t flag);
uint8_t menu3_set(uint8_t flag);

void show_tongduan(float num);	//通断菜单页面
void show_cichang(float num,char dir);
void show_jueyuan(void);

void show_set(int flag2);
void tongduan_set(void);
void cichang_set(void);

#endif
















