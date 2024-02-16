#ifndef  DDS_H
#define  DDS_H

#include "xparameters.h"
#include "DDS_Module.h"
#include "xbram.h"
#include "SinWavePoint.h"

#define RAM_A_BASE			XPAR_BRAM_0_BASEADDR
#define RAM_B_BASE			XPAR_BRAM_1_BASEADDR
#define	DDS_IP_BASE			XPAR_DDS_MODULE_0_S00_AXI_BASEADDR
#define CHAB_EN_MODE_REG	DDS_MODULE_S00_AXI_SLV_REG0_OFFSET
#define CHA_FWORD_REG		DDS_MODULE_S00_AXI_SLV_REG1_OFFSET
#define CHB_FWORD_REG		DDS_MODULE_S00_AXI_SLV_REG2_OFFSET
#define CHAB_PWORD_REG		DDS_MODULE_S00_AXI_SLV_REG3_OFFSET
#define CHAB_WIDTH_REG		DDS_MODULE_S00_AXI_SLV_REG4_OFFSET

#define	DDS_WR				DDS_MODULE_mWriteReg
#define	DDS_RD				DDS_MODULE_mReadReg

#define SET_CHAB_EN(n)		DDS_WR(DDS_IP_BASE,CHAB_EN_MODE_REG, (DDS_RD(DDS_IP_BASE,CHAB_EN_MODE_REG)&0xc)|n)
#define SET_CHAB_Mode(n)	DDS_WR(DDS_IP_BASE,CHAB_EN_MODE_REG, (DDS_RD(DDS_IP_BASE,CHAB_EN_MODE_REG)&0x3)|(n<<2))
#define SET_CHA_Freq(n)		DDS_WR(DDS_IP_BASE,CHA_FWORD_REG, n)
#define SET_CHB_Freq(n)		DDS_WR(DDS_IP_BASE,CHB_FWORD_REG, n)
#define SET_CHAB_Phas(n)	DDS_WR(DDS_IP_BASE,CHAB_PWORD_REG, n)
#define SET_CHAB_Width(n)	DDS_WR(DDS_IP_BASE,CHAB_WIDTH_REG, n)

//枚举信号输出相关
enum OUT_CHANNEL
{
	CH1_OUT=1, CH2_OUT, CH12_OUT	//1.通道一 2.通道二 3.通道1+2
};

enum OUT_WAVE
{
	SINE_OUT=1, SQUA_OUT, TRIA_OUT	//1.正弦波 2.方波 3.三角波
};

enum SETTING_MODE
{
	Freq_Set=1, Phas_Set, Width_Set	//1.频率设置 2.相位设置 3.占空比设置
};

extern u32 Out1_Freq;
extern u32 Out2_Freq;
extern u16 Out1_Phas;
extern u16 Out2_Phas;
extern u8 Out1_Width;
extern u8 Out2_Width;

extern enum OUT_CHANNEL Out_Channel;//输出通道Out_Channel
extern enum OUT_WAVE Out1_Wave,Out2_Wave;//定义输出波形Out_Wave

#define CHA_FREQ			Out1_Freq
#define CHB_FREQ			Out2_Freq
#define CHAB_PHASE			(Calculate_Pword(Out2_Phas) << 16) | (Calculate_Pword(Out1_Phas))
#define CHAB_PLUSE_WIDTH	((Out2_Width * 16384 / 100) << 16) | (Out1_Width * 16384 / 100)
#define CHAB_MODE			(((Out2_Wave==2)<<1) | (Out1_Wave==2))

void Init_DDS_ROM_tra(u32 base)
{
	int i;
	for(i=0;i<4096;i++)
	{
		XBram_WriteReg(base, i*4, i*4);
		//IOWR_16DIRECT(base, i*2, i*4);
	}
}

void Init_DDS_ROM_sine(u32 base)
{
	int i;
	for(i=0;i<4096;i++)
	{
		XBram_WriteReg(base, i*4, SinWavePoint[i]);
		//IOWR_16DIRECT(base, i*2, SinWavePoint[i]);
	}
}

void Init_DDS_ROM_squr(u32 base)
{
	int i;
	for(i=0;i<2047;i++)
	{
		XBram_WriteReg(base, i*4, 0);
		//IOWR_16DIRECT(base, i*2, 0);
	}
	for(i=2047;i<4096;i++)
	{
		XBram_WriteReg(base, i*4, 16383);
		//IOWR_16DIRECT(base, i*2, 16383);
	}
}


uint_fast64_t Calculate_Fword(uint_fast64_t Fout)
{
	uint_fast64_t fword;

	fword = Fout * 65536*65536;
	fword = fword/125000000;
	return fword;
}

uint_fast64_t Calculate_Pword(uint_fast64_t Phase)
{
	uint_fast64_t pword;
	pword = Phase * 4096;
	pword = pword / 360;
	pword = pword & 0x0000ffff;

	return pword;
}



int dds_init()
{
	Init_DDS_ROM_sine(RAM_A_BASE);
	Init_DDS_ROM_sine(RAM_B_BASE);

	SET_CHA_Freq(Calculate_Fword(CHA_FREQ));
	SET_CHB_Freq(Calculate_Fword(CHB_FREQ));
	SET_CHAB_Phas(CHAB_PHASE);
	SET_CHAB_Width(CHAB_PLUSE_WIDTH);
	SET_CHAB_EN(Out_Channel);
  return 0;
}


#endif   /* ifdef DDS_H */

