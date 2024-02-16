#ifndef LCD_LCD_H_
#define LCD_LCD_H_

#include "xil_types.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "stdint.h"
#include "sleep.h"

#include "math.h"
#include "xil_types.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xaxivdma_i.h"
#include "display_ctrl.h"
#include "vdma_api.h"






//*************************************************
//������ɫ
#define LCD_WHITE		0XFFFFFF	//��ɫ
#define LCD_BLACK		0X000000	//��ɫ

#define LCD_RED			0XFF0000	//��ɫ
#define LCD_GREEN		0X00FF00	//��ɫ
#define LCD_BLUE		0X0000FF	//��ɫ

#define LCD_YELLOW		0XFFFF00	//��ɫ
#define LCD_CYAN		0X00FFFF	//��ɫ
#define LCD_PURPLE		0XFF00FF	//��ɫ

#define LCD_GRAY		0XCCCCCC	//��ɫ
#define LCD_BROWN		0X8A2A2A	//��ɫ
#define LCD_VIOLET		0X8A2BE2	//������
#define LCD_PINK		0XFFC0CB	//�ۺ�ɫ
#define LCD_DARKBLUE	0X191970	//����ɫ
#define LCD_ORANGE		0XFF8000	//�ٻ�ɫ
//*************************************************

//frame buffer����ʼ��ַ
#define FrameBuffer_Addr	(XPAR_PS7_DDR_0_S_AXI_BASEADDR + 0x500000)//1100000
#define BYTES_PIXEL        3                          //ÿ�����ص��ֽ�����RGB888ռ3���ֽ�
#define DYNCLK_BASEADDR    XPAR_AXI_DYNCLK_0_BASEADDR //��̬ʱ�ӻ���ַ
#define VDMA_ID            XPAR_AXIVDMA_0_DEVICE_ID   //VDMA����ID
#define DISP_VTC_ID        XPAR_VTC_0_DEVICE_ID       //VTC����ID

extern uint8_t *frame;

//LCD�Ļ�����ɫ�ͱ���ɫ
extern uint32_t  POINT_COLOR;//Ĭ�Ϻ�ɫ
extern uint32_t  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ


extern XAxiVdma     Vdma;
extern DisplayCtrl  DispCtrl;

//��������
void LCD_Init();
void LCD_Refresh();//ˢ��ͼ��
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
void LCD_DisplayPic(uint16_t x, uint16_t y,const uint8_t *Pic);

#endif /* LCD_LCD_H_ */
