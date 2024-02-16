#include "COMMON.h"

int main(void)
{
	OV5640_Init();	//��ʼ��ov5640

	//����VDMA
	run_vdma_frame_buffer(&Vdma, VDMA_ID, RGB_LCD.width, RGB_LCD.height,
							FrameBuffer_Addr,0, 0,BOTH);
    //��ʼ��Display controller
	DisplayInitialize(&DispCtrl, DISP_VTC_ID, DYNCLK_BASEADDR);

    //����VideoMode
	DisplaySetMode(&DispCtrl, &RGB_LCD);
	DisplayStart(&DispCtrl);

    return 0;
}
