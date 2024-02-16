#ifndef __ADC128S102_H__
#define __ADC128S102_H__

#include "ADC_Acq2DDR_IP.h"
#include "xparameters.h"
#include "xil_io.h"


#define DDR_BASE_ADDR		XPAR_PS7_DDR_0_S_AXI_BASEADDR   //0x00100000
#define MEM_BASE_ADDR		(DDR_BASE_ADDR + 0x1000000)     //0x01100000
#define ADC_DATA_BASE		(MEM_BASE_ADDR + 0x00700000)    //0x01800000
#define ADC_DATA_LENGTH		1024	//ADC数据存储区域长度

#define	ADC_IP_BASEADDR		XPAR_ADC_ACQ2DDR_IP_0_S00_AXI_BASEADDR	//ADCIP基地址
#define	REG_ADC_CONTROL		ADC_ACQ2DDR_IP_S00_AXI_SLV_REG0_OFFSET	//ADC控制寄存器偏移地址
#define	REG_ADC_MINMAX		ADC_ACQ2DDR_IP_S00_AXI_SLV_REG1_OFFSET	//ADC最小最大值寄存器偏移地址
#define	REG_ADC_STATE		ADC_ACQ2DDR_IP_S00_AXI_SLV_REG2_OFFSET	//ADC状态寄存器偏移地址
#define	REG_ADC_FREQ		ADC_ACQ2DDR_IP_S00_AXI_SLV_REG3_OFFSET	//ADC频率寄存器偏移地址
#define SET_ADC_REG(RegOffset, Data)	ADC_ACQ2DDR_IP_mWriteReg(ADC_IP_BASEADDR,RegOffset,Data)
#define GET_ADC_REG(RegOffset)			ADC_ACQ2DDR_IP_mReadReg(ADC_IP_BASEADDR,RegOffset)
#define	ADC_ROUND_DONE 		(1 & (GET_ADC_REG(REG_ADC_STATE) >> 17))//ADC一轮传输完成标志


void Set_ADC_Mode(uint32_t Channel,uint32_t Sample_Rate,
		uint32_t Trigger_Val,uint32_t Trigger_Mode);
uint32_t Get_ADC_Freq();
float Get_ADC_Vmin();
float Get_ADC_Vmax();
float Get_ADC_Vmid();
uint16_t Get_ADC_MidVal();







#endif /* ADC128S102_ADC128S102_H_ */
