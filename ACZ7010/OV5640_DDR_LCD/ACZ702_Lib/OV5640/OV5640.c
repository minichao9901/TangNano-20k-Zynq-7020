/**
  *****************************************************************************
  * 						OV5640初始化库
  *****************************************************************************
  *
  * @File   : OV5640.c
  * @By     : Sun
  * @Version: V1.2
  * @Date   : 2021 / 11 / 04
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/


#include "OV5640.h"

XIic SCCB;//创建SCCB设备对象

void OV5640_Init()
{
	uint16_t ID = 0;

	//初始化SCCB
	AXI_IIC_Init(&SCCB,IIC_DEVICE_ID);

	usleep(20000);  //OV5640上电到开始配置sccb至少等待20ms

    //读OV5640摄像头ID
    ID  = SCCB_Read(0x300b);       //0x40
    ID |= SCCB_Read(0x300a) << 8;  //0x56

    xil_printf("Camera ID = %x\r\n",ID);//打印ID

    if(ID == 0x5640)  //获取到正确的ID：OV5640
    {
		//先对寄存器进行软件复位，使寄存器恢复初始值
		//寄存器软件复位后，需要延时1ms才能配置其它寄存器
		SCCB_Write(0x3008,0x82); //Bit[7]:复位 Bit[6]:电源休眠

		//延时1ms
		usleep(1000);

		SCCB_Write(0x3103,0x11); // system clock from pad, bit[1]
		SCCB_Write(0x3008,0x82); // software reset, bit[7]
		//delay 5ms
		SCCB_Write(0x3008,0x42); // software power down, bit[6]
		SCCB_Write(0x3103,0x03); // system clock from PLL, bit[1]
		SCCB_Write(0x3017,0xff); // FREX, Vsync, HREF, PCLK, D[9:6] output enable
		SCCB_Write(0x3018,0xff); // D[5:0], GPIO[1:0] output enable
		SCCB_Write(0x3034,0x1a); // MIPI 10-bit
		SCCB_Write(0x3037,0x13); // PLL root divider, bit[4], PLL pre-divider, bit[3:0]
		SCCB_Write(0x3108,0x01); // PCLK root divider, bit[5:4], SCLK2x root divider, bit[3:2]
		SCCB_Write(0x3630,0x36); // SCLK root divider, bit[1:0]
		SCCB_Write(0x3631,0x0e);
		SCCB_Write(0x3632,0xe2);
		SCCB_Write(0x3633,0x12);
		SCCB_Write(0x3621,0xe0);
		SCCB_Write(0x3704,0xa0);
		SCCB_Write(0x3703,0x5a);
		SCCB_Write(0x3715,0x78);
		SCCB_Write(0x3717,0x01);
		SCCB_Write(0x370b,0x60);
		SCCB_Write(0x3705,0x1a);
		SCCB_Write(0x3905,0x02);
		SCCB_Write(0x3906,0x10);
		SCCB_Write(0x3901,0x0a);
		SCCB_Write(0x3731,0x12);
		SCCB_Write(0x3600,0x08); // VCM control
		SCCB_Write(0x3601,0x33); // VCM control
		SCCB_Write(0x302d,0x60); // system control
		SCCB_Write(0x3620,0x52);
		SCCB_Write(0x371b,0x20);
		SCCB_Write(0x471c,0x50);
		SCCB_Write(0x3a13,0x43); // pre-gain = 1.047x
		SCCB_Write(0x3a18,0x00); // gain ceiling
		SCCB_Write(0x3a19,0xf8); // gain ceiling = 15.5x
		SCCB_Write(0x3635,0x13);
		SCCB_Write(0x3636,0x03);
		SCCB_Write(0x3634,0x40);
		SCCB_Write(0x3622,0x01);
		// 50/60Hz detection 50/60Hz 灯光条纹过滤
		SCCB_Write(0x3c01,0x34); // Band auto, bit[7]
		SCCB_Write(0x3c04,0x28); // threshold low sum
		SCCB_Write(0x3c05,0x98); // threshold high sum
		SCCB_Write(0x3c06,0x00); // light meter 1 threshold[15:8]
		SCCB_Write(0x3c07,0x08); // light meter 1 threshold[7:0]
		SCCB_Write(0x3c08,0x00); // light meter 2 threshold[15:8]
		SCCB_Write(0x3c09,0x1c); // light meter 2 threshold[7:0]
		SCCB_Write(0x3c0a,0x9c); // sample number[15:8]
		SCCB_Write(0x3c0b,0x40); // sample number[7:0]
		SCCB_Write(0x3810,0x00); // Timing Hoffset[11:8]
		SCCB_Write(0x3811,0xff); // Timing Hoffset[7:0]
		SCCB_Write(0x3812,0x00); // Timing Voffset[10:8]
		SCCB_Write(0x3813,0xfa); // timing V offset
		SCCB_Write(0x3708,0x64);
		SCCB_Write(0x4001,0x02); // BLC start from line 2
		SCCB_Write(0x4005,0x1a); // BLC always update
		SCCB_Write(0x3000,0x00); // enable blocks
		SCCB_Write(0x3004,0xff); // enable clocks
		SCCB_Write(0x300e,0x58); // MIPI power down, DVP enable
		SCCB_Write(0x302e,0x00);
		SCCB_Write(0x4300,0x61); // RGB565   //h4300_6f(千兆网模式)--------------------------------------------------------
		SCCB_Write(0x501f,0x01); // RGB565
		SCCB_Write(0x440e,0x00);
		SCCB_Write(0x5000,0xa7); // Lenc on, raw gamma on, BPC on, WPC on, CIP on
		// AEC target 自动曝光控制
		SCCB_Write(0x3a0f,0x30); // stable range in high
		SCCB_Write(0x3a10,0x28); // stable range in low
		SCCB_Write(0x3a1b,0x30); // stable range out high
		SCCB_Write(0x3a1e,0x26); // stable range out low
		SCCB_Write(0x3a11,0x60); // fast zone high
		SCCB_Write(0x3a1f,0x14); // fast zone low
		// Lens correction for ? 镜头补偿
		SCCB_Write(0x5800,0x23);
		SCCB_Write(0x5801,0x14);
		SCCB_Write(0x5802,0x0f);
		SCCB_Write(0x5803,0x0f);
		SCCB_Write(0x5804,0x12);
		SCCB_Write(0x5805,0x26);
		SCCB_Write(0x5806,0x0c);
		SCCB_Write(0x5807,0x08);
		SCCB_Write(0x5808,0x05);
		SCCB_Write(0x5809,0x05);
		SCCB_Write(0x580a,0x08);
		SCCB_Write(0x580b,0x0d);
		SCCB_Write(0x580c,0x08);
		SCCB_Write(0x580d,0x03);
		SCCB_Write(0x580e,0x00);
		SCCB_Write(0x580f,0x00);
		SCCB_Write(0x5810,0x03);
		SCCB_Write(0x5811,0x09);
		SCCB_Write(0x5812,0x07);
		SCCB_Write(0x5813,0x03);
		SCCB_Write(0x5814,0x00);
		SCCB_Write(0x5815,0x01);
		SCCB_Write(0x5816,0x03);
		SCCB_Write(0x5817,0x08);
		SCCB_Write(0x5818,0x0d);
		SCCB_Write(0x5819,0x08);
		SCCB_Write(0x581a,0x05);
		SCCB_Write(0x581b,0x06);
		SCCB_Write(0x581c,0x08);
		SCCB_Write(0x581d,0x0e);
		SCCB_Write(0x581e,0x29);
		SCCB_Write(0x581f,0x17);
		SCCB_Write(0x5820,0x11);
		SCCB_Write(0x5821,0x11);
		SCCB_Write(0x5822,0x15);
		SCCB_Write(0x5823,0x28);
		SCCB_Write(0x5824,0x46);
		SCCB_Write(0x5825,0x26);
		SCCB_Write(0x5826,0x08);
		SCCB_Write(0x5827,0x26);
		SCCB_Write(0x5828,0x64);
		SCCB_Write(0x5829,0x26);
		SCCB_Write(0x582a,0x24);
		SCCB_Write(0x582b,0x22);
		SCCB_Write(0x582c,0x24);
		SCCB_Write(0x582d,0x24);
		SCCB_Write(0x582e,0x06);
		SCCB_Write(0x582f,0x22);
		SCCB_Write(0x5830,0x40);
		SCCB_Write(0x5831,0x42);
		SCCB_Write(0x5832,0x24);
		SCCB_Write(0x5833,0x26);
		SCCB_Write(0x5834,0x24);
		SCCB_Write(0x5835,0x22);
		SCCB_Write(0x5836,0x22);
		SCCB_Write(0x5837,0x26);
		SCCB_Write(0x5838,0x44);
		SCCB_Write(0x5839,0x24);
		SCCB_Write(0x583a,0x26);
		SCCB_Write(0x583b,0x28);
		SCCB_Write(0x583c,0x42);
		SCCB_Write(0x583d,0xce); // lenc BR offset
		// AWB 自动白平衡
		SCCB_Write(0x5180,0xff); // AWB B block
		SCCB_Write(0x5181,0xf2); // AWB control
		SCCB_Write(0x5182,0x00); // [7:4] max local counter, [3:0] max fast counter
		SCCB_Write(0x5183,0x14); // AWB advanced
		SCCB_Write(0x5184,0x25);
		SCCB_Write(0x5185,0x24);
		SCCB_Write(0x5186,0x09);
		SCCB_Write(0x5187,0x09);
		SCCB_Write(0x5188,0x09);
		SCCB_Write(0x5189,0x75);
		SCCB_Write(0x518a,0x54);
		SCCB_Write(0x518b,0xe0);
		SCCB_Write(0x518c,0xb2);
		SCCB_Write(0x518d,0x42);
		SCCB_Write(0x518e,0x3d);
		SCCB_Write(0x518f,0x56);
		SCCB_Write(0x5190,0x46);
		SCCB_Write(0x5191,0xf8); // AWB top limit
		SCCB_Write(0x5192,0x04); // AWB bottom limit
		SCCB_Write(0x5193,0x70); // red limit
		SCCB_Write(0x5194,0xf0); // green limit
		SCCB_Write(0x5195,0xf0); // blue limit
		SCCB_Write(0x5196,0x03); // AWB control
		SCCB_Write(0x5197,0x01); // local limit
		SCCB_Write(0x5198,0x04);
		SCCB_Write(0x5199,0x12);
		SCCB_Write(0x519a,0x04);
		SCCB_Write(0x519b,0x00);
		SCCB_Write(0x519c,0x06);
		SCCB_Write(0x519d,0x82);
		SCCB_Write(0x519e,0x38); // AWB control
		// Gamma 伽玛曲线
		SCCB_Write(0x5480,0x01); // Gamma bias plus on, bit[0]
		SCCB_Write(0x5481,0x08);
		SCCB_Write(0x5482,0x14);
		SCCB_Write(0x5483,0x28);
		SCCB_Write(0x5484,0x51);
		SCCB_Write(0x5485,0x65);
		SCCB_Write(0x5486,0x71);
		SCCB_Write(0x5487,0x7d);
		SCCB_Write(0x5488,0x87);
		SCCB_Write(0x5489,0x91);
		SCCB_Write(0x548a,0x9a);
		SCCB_Write(0x548b,0xaa);
		SCCB_Write(0x548c,0xb8);
		SCCB_Write(0x548d,0xcd);
		SCCB_Write(0x548e,0xdd);
		SCCB_Write(0x548f,0xea);
		SCCB_Write(0x5490,0x1d);
		// color matrix 色彩矩阵
		SCCB_Write(0x5381,0x1e); // CMX1 for Y
		SCCB_Write(0x5382,0x5b); // CMX2 for Y
		SCCB_Write(0x5383,0x08); // CMX3 for Y
		SCCB_Write(0x5384,0x0a); // CMX4 for U
		SCCB_Write(0x5385,0x7e); // CMX5 for U
		SCCB_Write(0x5386,0x88); // CMX6 for U
		SCCB_Write(0x5387,0x7c); // CMX7 for V
		SCCB_Write(0x5388,0x6c); // CMX8 for V
		SCCB_Write(0x5389,0x10); // CMX9 for V
		SCCB_Write(0x538a,0x01); // sign[9]
		SCCB_Write(0x538b,0x98); // sign[8:1]
		// UV adjust UV 色彩饱和度调整
		SCCB_Write(0x5580,0x06); // saturation on, bit[1]
		SCCB_Write(0x5583,0x40);
		SCCB_Write(0x5584,0x10);
		SCCB_Write(0x5589,0x10);
		SCCB_Write(0x558a,0x00);
		SCCB_Write(0x558b,0xf8);
		SCCB_Write(0x501d,0x40); // enable manual offset of contrast
		// CIP 锐化和降噪
		SCCB_Write(0x5300,0x08); // CIP sharpen MT threshold 1
		SCCB_Write(0x5301,0x30); // CIP sharpen MT threshold 2
		SCCB_Write(0x5302,0x10); // CIP sharpen MT offset 1
		SCCB_Write(0x5303,0x00); // CIP sharpen MT offset 2
		SCCB_Write(0x5304,0x08); // CIP DNS threshold 1
		SCCB_Write(0x5305,0x30); // CIP DNS threshold 2
		SCCB_Write(0x5306,0x08); // CIP DNS offset 1
		SCCB_Write(0x5307,0x16); // CIP DNS offset 2
		SCCB_Write(0x5309,0x08); // CIP sharpen TH threshold 1
		SCCB_Write(0x530a,0x30); // CIP sharpen TH threshold 2
		SCCB_Write(0x530b,0x04); // CIP sharpen TH offset 1
		SCCB_Write(0x530c,0x06); // CIP sharpen TH offset 2
		SCCB_Write(0x5025,0x00);
		SCCB_Write(0x3008,0x02); // wake up from standby, bit[6]

		// 12824'h720, 30fps
		// input clock 24Mhz, PCLK 42Mhz
		SCCB_Write(0x3035,0x21); // PLL  21:30fps  41:15fps  81:7.5fps
		SCCB_Write(0x3036,0x69); // PLL
		SCCB_Write(0x3c07,0x07); // lightmeter 1 threshold[7:0]
		SCCB_Write(0x3820,0x46); // flip
		SCCB_Write(0x3821,0x01); // no mirror
		SCCB_Write(0x3814,0x31); // timing X inc
		SCCB_Write(0x3815,0x31); // timing Y inc
		SCCB_Write(0x3800,0x00); // HS
		SCCB_Write(0x3801,0x00); // HS
		SCCB_Write(0x3802,0x00); // VS
		SCCB_Write(0x3803,0x00); // VS
		SCCB_Write(0x3804,0x0a); // HW SET_OV5640 +  HE}
		SCCB_Write(0x3805,0x3f); // HW SET_OV5640 +  HE}
		SCCB_Write(0x3806,0x07); // VH SET_OV5640 +  VE}
		SCCB_Write(0x3807,0x9f); // VH SET_OV5640 +  VE}
		SCCB_Write(0x3808,0x03); // DVPHO
		SCCB_Write(0x3809,0x20); // DVPHO
		SCCB_Write(0x380a,0x01); // DVPVO
		SCCB_Write(0x380b,0xe0); // DVPHO
		SCCB_Write(0x380c,0x07); // HTS
		SCCB_Write(0x380d,0x08); // HTS
		SCCB_Write(0x380e,0x03); // VTS
		SCCB_Write(0x380f,0xe8); // VTS

		SCCB_Write(0x3618,0x00);
		SCCB_Write(0x3612,0x29);
		SCCB_Write(0x3709,0x52);
		SCCB_Write(0x370c,0x03);
		SCCB_Write(0x3a02,0x02); // 60Hz max exposure
		SCCB_Write(0x3a03,0xe0); // 60Hz max exposure
		SCCB_Write(0x3a14,0x02); // 50Hz max exposure
		SCCB_Write(0x3a15,0xe0); // 50Hz max exposure
		SCCB_Write(0x4004,0x02); // BLC line number
		SCCB_Write(0x3002,0x1c); // reset JFIFO, SFIFO, JPG
		SCCB_Write(0x3006,0xc3); // disable clock of JPEG2x, JPEG
		SCCB_Write(0x4713,0x03); // JPEG mode 3
		SCCB_Write(0x4407,0x04); // Quantization scale
		SCCB_Write(0x460b,0x37);
		SCCB_Write(0x460c,0x20);
		SCCB_Write(0x4837,0x16); // MIPI global timing
		SCCB_Write(0x3824,0x04); // PCLK manual divider
		SCCB_Write(0x5001,0x83); // SDE on, CMX on, AWB on
		SCCB_Write(0x3503,0x00); // AEC/AGC on
		SCCB_Write(0x4740,0x20); // VS 1
	}
}

