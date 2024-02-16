#ifndef __AXI_UART_LITE_H__
#define __AXI_UART_LITE_H__

#include "COMMON.h"
#include "xuartlite.h"

extern XUartLite AXI_UART0;


void AXI_UartLite_Init(XUartLite *InstPtr, uint16_t DeviceId);
void AXI_UARTLite_Intr_Init(XUartLite *InstPtr,uint16_t IntrId,
		XUartLite_Handler SendHandler,XUartLite_Handler RecvHandler);
void AXI_UARTLite_SendData(XUartLite *UartInstPtr,uint32_t data);
void AXI_UARTLite_SendString(XUartLite *UartInstPtr, char *str);
void AXI_UARTLite_RecvData(XUartLite *UartInstPtr, uint8_t *Buffer,uint32_t Len);


#endif
