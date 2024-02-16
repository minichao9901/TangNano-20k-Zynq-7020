#ifndef SII9022_SII9022_H_
#define SII9022_SII9022_H_

#include "PS_IIC.h"
#include "COMMON.h"

#define	SII9022_ADDR	0X39
#define SII9022Write_Reg(reg,data)	PS_IIC_Write_Reg(&SII9022_IIC,SII9022_ADDR,REG8,reg,data)
#define	SII9022_IIC_Init()			PS_IIC_Init(&SII9022_IIC,XPAR_PS7_I2C_0_DEVICE_ID,100000)


extern XIicPs SII9022_IIC;

void SII9022_Init(void);

#endif /* SII9022_SII9022_H_ */
