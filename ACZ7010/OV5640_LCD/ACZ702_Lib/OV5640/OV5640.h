#ifndef _OV5640_H_
#define _OV5640_H_

#include "PS_IIC.h"
#include "COMMON.h"

#define	SCCB_ID    0x78>>1   //OV5640…Ë±∏µÿ÷∑
#define SCCB_Write(addr, data)	PS_IIC_Write_Reg(&SCCB, SCCB_ID, REG16, addr, data)
#define SCCB_Read(addr)			PS_IIC_Read_Reg(&SCCB, SCCB_ID, REG16, addr)
#define IIC_DEVICE_ID			XPAR_XIICPS_0_DEVICE_ID


void OV5640_Init();


#endif
