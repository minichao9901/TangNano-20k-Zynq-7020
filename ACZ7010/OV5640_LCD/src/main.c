#include "COMMON.h"

int main(void)
{
	OV5640_Init();	//初始化ov5640

	//配置VDMA
	run_vdma_frame_buffer(&Vdma, VDMA_ID, RGB_LCD.width, RGB_LCD.height,
							FrameBuffer_Addr,0, 0,BOTH);
    //初始化Display controller
	DisplayInitialize(&DispCtrl, DISP_VTC_ID, DYNCLK_BASEADDR);

    //设置VideoMode
	DisplaySetMode(&DispCtrl, &RGB_LCD);
	DisplayStart(&DispCtrl);

    return 0;
}
