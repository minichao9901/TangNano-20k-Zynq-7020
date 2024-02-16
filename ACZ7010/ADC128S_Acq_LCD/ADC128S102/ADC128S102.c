#include "ADC128S102.h"

void Set_ADC_Mode(uint32_t Channel,uint32_t Sample_Rate,
		uint32_t Trigger_Val,uint32_t Trigger_Mode)
{
	uint32_t Sample_Div;
	Sample_Div = 1000/Sample_Rate;//采样分频=原始采样率1000K/设定采样率
	SET_ADC_REG(REG_ADC_CONTROL, Channel | (Sample_Div << 3)
			| (Trigger_Val << 14) | (Trigger_Mode << 30));
}

uint32_t Get_ADC_Freq()
{
	uint32_t ADC_Freq;
	ADC_Freq = GET_ADC_REG(REG_ADC_FREQ);
	return ADC_Freq;
}

float Get_ADC_Vmin()
{
	float Vmin;
	Vmin = (GET_ADC_REG(REG_ADC_MINMAX) & 0xFFFF) *3.3/4096;
	return Vmin;
}

float Get_ADC_Vmax()
{
	float Vmax;
	Vmax = (GET_ADC_REG(REG_ADC_MINMAX) >> 16) *3.3/4096;
	return Vmax;
}

float Get_ADC_Vmid()
{
	float Vmid;
	Vmid = (GET_ADC_REG(REG_ADC_STATE) & 0xFFFF) *3.3/4096;
	return Vmid;
}

uint16_t Get_ADC_MidVal()
{
	uint16_t ADC_MidVal;
	ADC_MidVal = GET_ADC_REG(REG_ADC_STATE) & 0xFFFF;
	return ADC_MidVal;
}
