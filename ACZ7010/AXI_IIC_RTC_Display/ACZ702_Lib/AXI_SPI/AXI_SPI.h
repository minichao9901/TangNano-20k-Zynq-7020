#ifndef __AXI_SPI_H__
#define __AXI_SPI_H__

#include "COMMON.h"
#include "xspi.h"


extern XSpi AXI_SPI0;


/************************** º¯ÊýÉùÃ÷ ******************************/
void AXI_SPI_Init(XSpi *SpiInstPtr, uint16_t SpiDeviceId,uint8_t SPI_Mode);
void AXI_SPI_Transfer(XSpi *SpiInstPtr, uint32_t Sel_Num, uint8_t *ReadBuffer, uint8_t *WriteBuffer, uint32_t BUFFER_SIZE);

#endif
