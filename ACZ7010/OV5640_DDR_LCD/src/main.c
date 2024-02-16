#include "COMMON.h"
#include "DVP_Capture2DDR.h"

#define	WRITE_CAMERA_REG	DVP_CAPTURE2DDR_mWriteReg
#define	REG_ONOFF			DVP_CAPTURE2DDR_S00_AXI_SLV_REG0_OFFSET
#define	CAMERA_REG_BASEADDR XPAR_DVP_CAPTURE2DDR_0_S00_AXI_BASEADDR
#define OPEN_CAMERA			WRITE_CAMERA_REG(CAMERA_REG_BASEADDR,REG_ONOFF,1)
#define	CLOSE_CAMERA		WRITE_CAMERA_REG(CAMERA_REG_BASEADDR,REG_ONOFF,0)
#define Image_Storage_Addr	0x01800000

int main()
{
	OV5640_Init();
	OPEN_CAMERA;

	//配置VDMA，读取图像存储地址的图像数据
	run_vdma_frame_buffer(&Vdma, VDMA_ID, RGB_LCD.width, RGB_LCD.height,
			Image_Storage_Addr,0, 0,ONLY_READ);

	//初始化Display controller
	DisplayInitialize(&DispCtrl, DISP_VTC_ID, DYNCLK_BASEADDR);

	//设置VideoMode
	DisplaySetMode(&DispCtrl, &RGB_LCD);
	DisplayStart(&DispCtrl);

	return 0;
}

