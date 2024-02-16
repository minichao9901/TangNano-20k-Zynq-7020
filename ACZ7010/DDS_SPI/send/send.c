#include "send.h"

void AXI_SPI_Sendshort(XSpi *SpiInstPtr,unsigned short data)
{
	uint32_t StatusReg;
//	uint32_t ControlReg;
	/* 取消抑制，启动传输 */
//	ControlReg = XSpi_GetControlReg(SpiInstPtr);
//	ControlReg &= ~XSP_CR_TRANS_INHIBIT_MASK;
//	XSpi_SetControlReg(SpiInstPtr, ControlReg);

	//传输数据
	XSpi_WriteReg(SpiInstPtr->BaseAddr, XSP_DTR_OFFSET, data);
	do {
		StatusReg = XSpi_GetStatusReg(SpiInstPtr);
	} while ((StatusReg & XSP_SR_TX_EMPTY_MASK) == 0);

	//使能抑制
//	ControlReg = XSpi_GetControlReg(SpiInstPtr);
//	XSpi_SetControlReg(SpiInstPtr, ControlReg |XSP_CR_TRANS_INHIBIT_MASK);

}


void AXI_SPI_Sendshort_8(XSpi *SpiInstPtr,unsigned short data)
{
	uint32_t StatusReg;
//	uint32_t ControlReg;
	uint8_t transmit_data;

	/* 取消抑制，启动传输 */
//	ControlReg = XSpi_GetControlReg(SpiInstPtr);
//	ControlReg &= ~XSP_CR_TRANS_INHIBIT_MASK;
//	XSpi_SetControlReg(SpiInstPtr, ControlReg);

	//传输数据
	transmit_data = data >>8;
	XSpi_WriteReg(SpiInstPtr->BaseAddr, XSP_DTR_OFFSET, transmit_data);
	transmit_data = data&0xFF;
	XSpi_WriteReg(SpiInstPtr->BaseAddr, XSP_DTR_OFFSET, transmit_data);

	do {
		StatusReg = XSpi_GetStatusReg(SpiInstPtr);
	} while ((StatusReg & XSP_SR_TX_EMPTY_MASK) == 0);

	//使能抑制
//	ControlReg = XSpi_GetControlReg(SpiInstPtr);
//	XSpi_SetControlReg(SpiInstPtr, ControlReg |XSP_CR_TRANS_INHIBIT_MASK);
}
