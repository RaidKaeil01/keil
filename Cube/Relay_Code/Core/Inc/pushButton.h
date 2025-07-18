#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H
#include "stdint.h"
#include "main.h"


typedef struct {
    uint8_t key_1_pressed;  // 1号按键状态
    uint8_t key_2_pressed;  // 2号按键状态
    uint8_t key_3_pressed;  // 3号按键状态
    uint8_t key_4_pressed;  // 4号按键状态
} KeyState;



/**
 * @brief 每个按键的结构体，使用状态机方式实现
 * @param button_this_time 这次读取到的按键状态
 * @param button_last_time 上次读取到的按键的状态，每次读取后需要手动的把这次读到的值给button_last_time
 * @param button_status 预留的按下，抬起，等状态，还没想好
 * @param time 被按下的时间单位10ms
*/
typedef struct pushButton
{
    /* data */
    uint8_t num;
    uint8_t button_this_time;
    uint8_t button_last_time;
    uint8_t button_status; 
    uint16_t time;

}Button;
//枚举的默认是int，在赋值的时候可能会有点问题
enum ButtonStatus {
    BTN_DEFAULT = 0,
    BTN_PUSH_DOWN,
    BTN_HOLD_ON,
    BTN_RELEASE
};

extern Button button1;
extern Button button2;
extern Button button3;
extern Button button4;
extern uint16_t numOfPeople;


void button_kernel(Button * button,GPIO_TypeDef *GPIO_port, uint16_t GPIO_Pin);
void pushButtonQueueProcess(void);

#endif
