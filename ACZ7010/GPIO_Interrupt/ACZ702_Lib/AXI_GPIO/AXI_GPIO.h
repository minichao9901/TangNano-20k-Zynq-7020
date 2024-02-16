#ifndef _AXI_GPIO_H_
#define _AXI_GPIO_H_

#include "COMMON.h"
#include "xgpio.h"


extern XGpio AXI_GPIO0;

void AXI_GPIO_Init(XGpio *InstPtr, uint16_t DeviceId);
void AXI_GPIO_Intc_Init(XGpio *InstPtr,uint16_t IntrId, uint32_t Channel,
		Xil_InterruptHandler Handler);
void AXI_GPIO_Set_Channel(XGpio *InstPtr, uint8_t Channel, uint32_t Dri, uint32_t Data);
void AXI_GPIO_SetPin(XGpio *InstPtr, uint8_t Channel, uint8_t GPIO_Num, uint32_t Data);
uint8_t AXI_GPIO_GetPin(XGpio *InstPtr, uint8_t Channel, uint8_t GPIO_Num);
void AXI_GPIO_SetPin_Dir(XGpio *InstPtr, uint8_t Channel, uint8_t GPIO_Num, uint8_t Dir);

#endif /* AXI_GPIO_AXI_GPIO_H_ */
