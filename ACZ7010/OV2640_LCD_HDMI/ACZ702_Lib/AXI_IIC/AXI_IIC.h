#ifndef __AXI_IIC_H__
#define __AXI_IIC_H__

#include "COMMON.h"
#include "xiic.h"


extern XIic AXI_IIC0;


/************************** º¯ÊýÉùÃ÷ ******************************/
void AXI_IIC_Init(XIic *InstPtr, uint16_t DeviceId);
void AXI_IIC_Write_Reg(XIic *InstPtr,uint8_t SlaveID,uint8_t RegBIT, uint16_t reg, uint8_t dat);
uint8_t AXI_IIC_Read_Reg(XIic *InstPtr,uint8_t SlaveID,uint8_t RegBIT, uint16_t reg);
void AXI_IIC_SeqWrite_Reg(XIic *InstPtr,uint8_t SlaveID,uint8_t RegBIT, uint16_t reg, uint8_t *send_dat, uint32_t send_len);
void AXI_IIC_SeqRead_Reg(XIic *InstPtr,uint8_t SlaveID, uint8_t RegBIT, uint16_t reg,uint8_t *recv_data, uint32_t recv_len);
#endif
