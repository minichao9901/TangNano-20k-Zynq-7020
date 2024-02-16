#include "COMMON.h"

int main(void)
{
	PS_IIC_Init(&SCCB,IIC_DEVICE_ID,400000);
	SII9022_Init();
	usleep(20000);  //上电到开始配置sccb至少等待20ms
	Camera_check();	//检测摄像头
	Camera_Init();//初始化摄像头

	//配置VDMA
	run_vdma_frame_buffer(&Vdma, VDMA_ID, RGB_LCD.width, RGB_LCD.height,
							FrameBuffer_Addr,0, 0,BOTH);
    //初始化Display controller
	DisplayInitialize(&DispCtrl, DISP_VTC_ID, DYNCLK_BASEADDR);

    //设置VideoMode
	DisplaySetMode(&DispCtrl, &RGB_LCD);
	DisplayStart(&DispCtrl);

	while(1)
	{
		//当OV5640和OV2640在上轮没有检测到，这轮有一个其中一个被检测到时
		//根据检测到的摄像头进行初始化，实现热插拔
		if(NO_Camera&(OV5640_Check|OV2640_Check))
		{
			NO_Camera = 0;
			if(OV5640_Check)
			{
				Camera_Init();
			}else if(OV2640_Check)
			{
				Camera_Init();
			}
		}else{
			Camera_check();
			usleep(1000000);//修改检测间隔时间
		}
	}
    return 0;
}
