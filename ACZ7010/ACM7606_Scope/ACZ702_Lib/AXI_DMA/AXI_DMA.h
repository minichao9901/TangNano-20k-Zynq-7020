#ifndef _AXI_DMA_H_
#define _AXI_DMA_H_

#include "COMMON.h"
#include "xaxidma.h"

#define ADC_DATA_BASE		0x01800000	//0x80100000
#define ADC_DATA_LENGTH		1024	//ADC���ݴ洢���򳤶�
#define	ADC_ROUND_DONE 		(RxDone)//ADCһ�ִ�����ɱ�־


XAxiDma AxiDma0;



//��ʼ��DMA����
void AXI_DMA_Init(XAxiDma *AxiDma, uint32_t DeviceId);
void AXI_DMA_RxInt_Init(XAxiDma *AxiDma, uint16_t RxIntrId, Xil_InterruptHandler Handler);



#endif /* _PL_DMA_H_ */
