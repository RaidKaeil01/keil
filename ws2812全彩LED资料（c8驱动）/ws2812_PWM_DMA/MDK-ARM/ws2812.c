#include "main.h"
#include "string.h"
#include "stdio.h"

extern TIM_HandleTypeDef htim1;                           //声明 htim1 handle
extern DMA_HandleTypeDef hdma_tim1_ch1;                   //声明 hdma_tim1_ch1 handle

#define PULSE_1_CODE        (59)                           //1 码计数个数
#define PULSE_0_CODE        (29)                           //0 码计数个数
#define PIXEL_NUM           (64+1)                         //led 个数
#define PIXEL_DATA_LEN      (24)

#define LED_FLOW_TIME					(50)

// ws2812 结构体数组
typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
}piexel_t;
piexel_t pixel[PIXEL_NUM];

// DMA 传输的数据
uint16_t ws2812_DMA_data[PIXEL_DATA_LEN * 2] = {0};


/************************ 函数声明 ******************************/
void ws2812_RGB_set(uint8_t R,uint8_t G,uint8_t B,uint8_t pixel_id);                      //设置颜色
uint8_t ws2812_pixel_data_fill(uint8_t pixel_next,uint16_t *addr);                        //传输数据
void flow_led(unsigned char R, unsigned char G, unsigned char B);
/************************ 函数定义 ******************************/
void ws2812_RGB_set(uint8_t R,uint8_t G,uint8_t B,uint8_t led_id)
{
	if(led_id >= (PIXEL_NUM))
		return;

	pixel[led_id].r = R;
	pixel[led_id].g = G;
	pixel[led_id].b = B;

	return;
}


uint8_t ws2812_pixel_data_fill(uint8_t pixel_next,uint16_t *addr)
{
	if(pixel_next >= PIXEL_NUM)
	{
		return 0;
	}

	for (uint8_t i = 0;i < 8;i++)
	{
		//填充数组
		addr[i]      = (pixel[pixel_next].g << i) & (0x80)?PULSE_1_CODE:PULSE_0_CODE;
		addr[i + 8]  = (pixel[pixel_next].r << i) & (0x80)?PULSE_1_CODE:PULSE_0_CODE;
		addr[i + 16] = (pixel[pixel_next].b << i) & (0x80)?PULSE_1_CODE:PULSE_0_CODE;
	}
	return 1;
}

void ws2812_example_2(void)
{
    uint8_t static bri1 = 0xFF;
    //memset(ws2812_DMA_data,0x00,96);
    ws2812_RGB_set(bri1, bri1, bri1, 0);
    ws2812_RGB_set(bri1, 0x00, 0x00, 1);
    ws2812_RGB_set(0x00, bri1, 0x00, 2);
    ws2812_RGB_set(0x00, 0x00, bri1, 3);
	ws2812_RGB_set(0x00, bri1, bri1, 4);

    HAL_TIM_PWM_Start_DMA(&htim1,TIM_CHANNEL_1,(uint32_t *)ws2812_DMA_data,(48));

    HAL_Delay(100);
}

#define   FLAG		0

volatile int cnt = 0;
//TC 回调函数
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{ 
#if	FLAG
    cnt++;
    if(cnt == 2)
    {
        ws2812_pixel_data_fill(1,ws2812_DMA_data+24);
    }
    if(cnt == 4)
    {
        ws2812_pixel_data_fill(3,ws2812_DMA_data+24);
    }
    if(cnt == 6)
    {
        memset(ws2812_DMA_data+24,0x00,48);
    }
    if(cnt == 8)
    {
        cnt = 0; 
        HAL_TIM_PWM_Stop_DMA(&htim1,TIM_CHANNEL_1);
        //__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
        
    }
#else    
		cnt++;
    if(cnt <=PIXEL_NUM)
    {
        ws2812_pixel_data_fill(cnt-1,ws2812_DMA_data+24);
    }
    if(cnt == (((PIXEL_NUM+1)/2) * 2+0))
    {
        memset(ws2812_DMA_data+24,0x00,48);
    }
    if(cnt == ((((PIXEL_NUM+1)/2) * 2)+2))
    {
        cnt = 0; 
        HAL_TIM_PWM_Stop_DMA(&htim1,TIM_CHANNEL_1);
        //__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
        
    }
#endif
}
//HC 回调函数
void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
{  
#if FLAG	
    cnt++;
    if(cnt == 1)
    {
        ws2812_pixel_data_fill(0,ws2812_DMA_data);
    }
    
    if(cnt == 3)
    {
        ws2812_pixel_data_fill(2,ws2812_DMA_data);
    }
    
    if(cnt == 5)
    {
        memset(ws2812_DMA_data,0x00,48);
    }
#else
	  cnt++;
    if(cnt <= PIXEL_NUM)
    {
        ws2812_pixel_data_fill(cnt-1,ws2812_DMA_data);
    }
    
    if(cnt == (((PIXEL_NUM+1)/2) * 2)+1)
    {
        memset(ws2812_DMA_data,0x00,48);
    }
#endif
}

void pixel_show(void)
{
	HAL_TIM_PWM_Start_DMA(&htim1,TIM_CHANNEL_1,(uint32_t *)ws2812_DMA_data,(48));
}

void flow_led(unsigned char R, unsigned char G, unsigned char B)
{
	for (int i = 0; i < PIXEL_NUM; i++)
	{
		if (i > 0)
		{
			ws2812_RGB_set(0, 0, 0,i - 1);
		}
		ws2812_RGB_set(R, G, B,i);
		ws2812_RGB_set(R, G, B,i + 1);
		ws2812_RGB_set(R, G, B,i + 2);
		pixel_show();
		HAL_Delay(LED_FLOW_TIME);
	}

//	for (int i = 0; i < PIXEL_NUM - 3; i++)
//	{
//		if (i > 0)
//		{
//			ws2812_RGB_set(0, 0, 0,(PIXEL_NUM)-i);
//		}
//		ws2812_RGB_set( R, G, B,(PIXEL_NUM - 1) - i);
//		ws2812_RGB_set( R, G, B,(PIXEL_NUM - 1) - (i + 1));
//		ws2812_RGB_set(R, G, B,(PIXEL_NUM - 1) - (i + 2));
//		pixel_show();
//		HAL_Delay(LED_FLOW_TIME);
//	}
}

