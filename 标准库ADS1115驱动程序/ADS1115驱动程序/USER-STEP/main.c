#include "sys.h"
#include "ads1115.h"
int main(void)
{
	float ad=0;
	Sys_Init(115200);

    printf("start 1015 test function\r\n");
    ADS1115_gpio_init();
    
	while(1){
        get_AD(0xe3,0xe2);
        delay_ms(500);
	}
	
}
























