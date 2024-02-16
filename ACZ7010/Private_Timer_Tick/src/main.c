#include "COMMON.h"


int main(void)
{
	uint32_t Tick_Cnt = 0;

	//初始化通用中断控制器
	ScuGic_Init();

	//初始化私有定时器，每1s触发一次
	ScuTimer_Int_Init(1000000);

	while(1) {
		//Tick标志由私有定时器中断控制，中断服务函数在ISR.c中
		if(Tick_Flag) {
			Tick_Flag = 0;

			//打印Tick_Cnt计数值
			printf("Tick Cnt: %lu\n", Tick_Cnt);

			//Tick_Cnt范围0~59
			if(Tick_Cnt >= 59)
				Tick_Cnt = 0;
			else
				Tick_Cnt++;
		}
	}

	return 0;
}
