#include <stdio.h>
#include "xparameters.h"
#include "xgpio.h"
#include "sleep.h"
#include "xil_cache.h"

#define GPIO_DEVICE_ID XPAR_AXI_GPIO_0_DEVICE_ID // ����ʵ��Ӳ�������޸�
#define LED_CHANNEL 1

void test_ddr(UINTPTR addr, u32 length, u8 mode)
{
    for(UINTPTR p=addr; p<addr+length; p+=4){
    	Xil_Out32(p,0x11223344);
    }

    for(UINTPTR p=addr; p<addr+length; p+=4){
    	int v= Xil_In32(p);
    	if(mode==0){
    		xil_printf("%p->%x\r\n",p,v);
    	}
    	else if(mode==1 && v!=11223344){
    		xil_printf("%p\r\n",p);
    	}
    }
}


int main() {
    XGpio gpio;
    int Status;

    usleep(50000);
    Xil_DCacheDisable();

    xil_printf("Microblaze GPIO test!\r\n");

    // ��ʼ��GPIO
    Status = XGpio_Initialize(&gpio, GPIO_DEVICE_ID);
    if (Status != XST_SUCCESS) {
        printf("GPIO Initialization failed\n");
        return XST_FAILURE;
    }

    // ����GPIOΪ���
    XGpio_SetDataDirection(&gpio, LED_CHANNEL, 0x00);

    while (1) {
//        scanf("%c",&c);
//        while(c!='c');

    	xil_printf("Microblaze Print Now!\r\n");
        test_ddr(0x10000000,16,0); //test ddr
		test_ddr(0xfffc0000,16,0); //test ocm, remapģʽ�ſ��Է���
		test_ddr(0xfffd0000,16,0); //test ocm, remapģʽ�ſ��Է���
		test_ddr(0xfffe0000,16,0); //test ocm, remapģʽ�ſ��Է���
		test_ddr(0xffff0000,16,0); //test ocm, remapģʽ�ſ��Է���
		test_ddr(0x00000000,16,0); //test ocm0, Ĭ�Ϸ�remapģʽ���Է���
		test_ddr(0x00010000,16,0); //test ocm1, Ĭ�Ϸ�remapģʽ���Է���
		test_ddr(0x00020000,16,0); //test ocm2, Ĭ�Ϸ�remapģʽ���Է���
		test_ddr(0xFFFF0000,16,0); //test ocm3, Ĭ�Ϸ�remapģʽ���Է���


        // ���η�ת�ĸ�GPIO����
        XGpio_DiscreteWrite(&gpio, LED_CHANNEL, 0b0001);
        usleep(500000/10); // �ȴ�500����

        XGpio_DiscreteWrite(&gpio, LED_CHANNEL, 0b0010);
        usleep(500000/10);

        XGpio_DiscreteWrite(&gpio, LED_CHANNEL, 0b0100);
        usleep(500000/10);

        XGpio_DiscreteWrite(&gpio, LED_CHANNEL, 0b1000);
        usleep(500000/10);
    }

    return 0;
}
