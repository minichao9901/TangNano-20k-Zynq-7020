#ifndef __PS_SPI_H__
#define __PS_SPI_H__

#include "COMMON.h"
#include "xspips.h"


extern XSpiPs SPI0,SPI1;


/************************** º¯ÊýÉùÃ÷ ******************************/
void PS_SPI_Init(XSpiPs *InstancePtr,uint16_t DeviceId, uint8_t Prescaler, uint8_t SPI_Mode);
void PS_SPI_Transfer(XSpiPs *InstancePtr, uint32_t Sel_Num, uint8_t *ReadBuffer, uint8_t *WriteBuffer, uint32_t BUFFER_SIZE);

#endif
