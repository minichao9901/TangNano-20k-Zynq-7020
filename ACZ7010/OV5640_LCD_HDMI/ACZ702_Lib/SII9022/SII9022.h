#ifndef SII9022_SII9022_H_
#define SII9022_SII9022_H_

#include "AXI_IIC.h"
#include "COMMON.h"

#define	SII9022_ADDR	0X39
#define SII9022Write_Reg(reg,data)	AXI_IIC_Write_Reg(&AXI_IIC0,SII9022_ADDR,REG8,reg,data)
#define	SII9022_IIC_Init()			AXI_IIC_Init(&AXI_IIC0,XPAR_IIC_0_DEVICE_ID)


extern XIicPs SII9022_IIC;

void SII9022_Init(void);

#endif /* SII9022_SII9022_H_ */
