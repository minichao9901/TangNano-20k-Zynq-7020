#ifndef LCD_LCD_H_
#define LCD_LCD_H_

//#include "xil_types.h"
//#include "stdio.h"
//#include "stdlib.h"
//#include "string.h"
//#include "stdarg.h"
//#include "stdint.h"
//#include "sleep.h"



//#include "math.h"
#include "xil_types.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xaxivdma_i.h"
#include "display_ctrl.h"
#include "vdma_api.h"
#include "PageDesign.h"




//*************************************************
//画笔颜色
#define LCD_WHITE		0XFFFF	//白色
#define LCD_BLACK		0X0000	//黑色

#define LCD_RED			0xF800	//红色
#define LCD_GREEN		0x07E0	//绿色
#define LCD_BLUE		0x001F	//蓝色

#define LCD_YELLOW		0xFFE0	//黄色
#define LCD_CYAN		0x07FF	//青色
#define LCD_PURPLE		0x780F	//紫色

#define LCD_GRAY		0xCE79	//灰色0XCCCCCC
#define LCD_BROWN		0X8945	//棕色0X8A2A2A
#define LCD_VIOLET		0X88BC	//紫罗兰0X8A2BE2
#define LCD_PINK		0XFE19	//粉红色0XFFC0CB
#define LCD_DARKBLUE	0x000F	//深蓝色
#define LCD_ORANGE		0xFC00	//橘黄色0XFF8000
//*************************************************

//frame buffer的起始地址
#define FrameBuffer_Addr	(XPAR_PS7_DDR_0_S_AXI_BASEADDR + 0x1000000)//1100000
#define BYTES_PIXEL        2                          //每个像素的字节数，RGB565占2个字节
#define DYNCLK_BASEADDR    XPAR_AXI_DYNCLK_0_BASEADDR //动态时钟基地址
#define VDMA_ID            XPAR_AXIVDMA_0_DEVICE_ID   //VDMA器件ID
#define DISP_VTC_ID        XPAR_VTC_0_DEVICE_ID       //VTC器件ID


//LCD的画笔颜色和背景色
extern uint32_t  POINT_COLOR;//默认红色
extern uint32_t  BACK_COLOR; //背景颜色.默认为白色


extern XAxiVdma     Vdma;
extern DisplayCtrl  DispCtrl;

//函数声明
void LCD_Init();
void LCD_Refresh();//刷新图像
void LCD_DrawPoint(uint16_t x,uint16_t y,uint32_t color);
void LCD_Color_Fill(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint32_t color);
void LCD_Clear(uint32_t color);
uint8_t LCD_DrawWaveLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint32_t color);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t mode,uint8_t *p);
uint32_t LCD_Pow(uint8_t m,uint8_t n);
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);
void LCD_DisplayPic(uint16_t x, uint16_t y,uint8_t const *Pic);

#endif /* LCD_LCD_H_ */
