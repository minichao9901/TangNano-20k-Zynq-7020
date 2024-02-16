/*
 * send.h
 *
 *  Created on: 2022Äê6ÔÂ10ÈÕ
 *      Author: Administrator
 */

#ifndef SEND_SEND_H_
#define SEND_SEND_H_

#include "COMMON.h"
#include "xspi.h"

void AXI_SPI_Sendshort(XSpi *SpiInstPtr,unsigned short data);
void AXI_SPI_Sendshort_8(XSpi *SpiInstPtr,unsigned short data);


#endif /* SEND_SEND_H_ */
