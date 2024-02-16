#include "SII9022.h"

/************************************************************************************************************
**  @brief	初始化SII9022
**  @param	无
**  @return	无
**  Sample:	SII9022_Init();//调用该函数前，请先调用IIC的初始化
************************************************************************************************************/

XIicPs SII9022_IIC;//创建SII9022_IIC设备对象

void SII9022_Init(void)
{
	SII9022_IIC_Init();
	//HW Reset
	SII9022Write_Reg(0xc7,0x00);

	SII9022Write_Reg(0xBC,0x01);
	SII9022Write_Reg(0xBD,0x00);
	SII9022Write_Reg(0xBE,0x01);

	SII9022Write_Reg(0x00,0x02);
	SII9022Write_Reg(0x01,0x3A);
	SII9022Write_Reg(0x02,0x70);
	SII9022Write_Reg(0x03,0x17);
	SII9022Write_Reg(0x04,0x98);
	SII9022Write_Reg(0x05,0x08);
	SII9022Write_Reg(0x06,0x65);
	SII9022Write_Reg(0x07,0x04);
	SII9022Write_Reg(0x08,0x60);
	SII9022Write_Reg(0x09,0x00);	//  ---RGB input
	SII9022Write_Reg(0x0A,0x00);	// ---RGB output
	SII9022Write_Reg(0x0B,0x00);	//Set AVI
	SII9022Write_Reg(0x0C,0x00);
	SII9022Write_Reg(0x0D,0x00);
	SII9022Write_Reg(0x0E,0x00);
	SII9022Write_Reg(0x0F,0x00);
	SII9022Write_Reg(0x10,0x00);
	SII9022Write_Reg(0x11,0x00);
	SII9022Write_Reg(0x12,0x00);
	SII9022Write_Reg(0x13,0x00);
	SII9022Write_Reg(0x14,0x00);
	SII9022Write_Reg(0x15,0x00);
	SII9022Write_Reg(0x16,0x00);
	SII9022Write_Reg(0x17,0x00);
	SII9022Write_Reg(0x18,0x00);
	SII9022Write_Reg(0x19,0x00);	// ---enable color convert, must do it here

	SII9022Write_Reg(0x1E,0x00);

	SII9022Write_Reg(0x60,0x04);	// ---embeded sync select
	SII9022Write_Reg(0x62,0x00);	// ---HBIT_2_HSYNC L
	SII9022Write_Reg(0x63,0x00);	// ---embede enable and HBIT_2_HSYNC M //bit6=1 for DE
	SII9022Write_Reg(0x64,0x00);	// ---FIELD2_OFST L
	SII9022Write_Reg(0x65,0x00);	// ---FIELD2_OFST M//4
	SII9022Write_Reg(0x66,0x00);	// ---HWIDTH L
	SII9022Write_Reg(0x67,0x00);	// ---HWIDTH M
	SII9022Write_Reg(0x68,0x00);	// ---VBIT_2_VSYNC
	SII9022Write_Reg(0x69,0x00);	// ---VWIDTH

	SII9022Write_Reg(0x1A,0x00);


	return;
}
