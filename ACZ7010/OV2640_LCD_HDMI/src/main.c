#include "COMMON.h"

int main(void)
{
	PS_IIC_Init(&SCCB,IIC_DEVICE_ID,400000);
	SII9022_Init();
	usleep(20000);  //�ϵ絽��ʼ����sccb���ٵȴ�20ms
	Camera_check();	//�������ͷ
	Camera_Init();//��ʼ������ͷ

	//����VDMA
	run_vdma_frame_buffer(&Vdma, VDMA_ID, RGB_LCD.width, RGB_LCD.height,
							FrameBuffer_Addr,0, 0,BOTH);
    //��ʼ��Display controller
	DisplayInitialize(&DispCtrl, DISP_VTC_ID, DYNCLK_BASEADDR);

    //����VideoMode
	DisplaySetMode(&DispCtrl, &RGB_LCD);
	DisplayStart(&DispCtrl);

	while(1)
	{
		//��OV5640��OV2640������û�м�⵽��������һ������һ������⵽ʱ
		//���ݼ�⵽������ͷ���г�ʼ����ʵ���Ȳ��
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
			usleep(1000000);//�޸ļ����ʱ��
		}
	}
    return 0;
}
