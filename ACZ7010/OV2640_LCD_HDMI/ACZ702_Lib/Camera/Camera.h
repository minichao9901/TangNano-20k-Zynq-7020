#ifndef _OV5640_H_
#define _OV5640_H_

#include "PS_IIC.h"
#include "COMMON.h"

#define	SCCB_ID_OV5640    0x78>>1   //OV5640设备地址
#define	SCCB_ID_OV2640	   0x60>>1	//OV2640设备地址
#define	OV5640_ID	   0x5640	//OV5640正确ID
#define	OV2640_ID	   0x7fa2	//OV2640正确ID
#define SCCB_Write(addr, data)	        PS_IIC_Write_Reg(&SCCB, SCCB_ID_OV5640, REG16, addr, data)
#define SCCB_Read(addr)			        PS_IIC_Read_Reg(&SCCB, SCCB_ID_OV5640, REG16, addr)
#define SCCB_Write_REG8(addr, data)	    PS_IIC_Write_Reg(&SCCB, SCCB_ID_OV2640, REG8, addr, data)
#define SCCB_Read_REG8(addr)			PS_IIC_Read_Reg(&SCCB, SCCB_ID_OV2640, REG8, addr)
#define IIC_DEVICE_ID			XPAR_XIICPS_0_DEVICE_ID

extern uint8_t OV5640_Check,OV2640_Check,NO_Camera;

void Camera_check();
void Camera_Init();

extern XIicPs SCCB;

#endif
