/************************************************************************/
/*																		*/
/*	lcd_modes.h	--	VideoMode definitions		 						*/
/*																		*/
/************************************************************************/
/*	Author: Sam Bobrowicz												*/
/*	Copyright 2014, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*		This file contains the definition of the VideoMode type, and	*/
/*		also defines several common video modes							*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/* 																		*/
/*		2/17/2014(SamB): Created										*/
/*																		*/
/************************************************************************/

#ifndef LCD_MODES_H_
#define LCD_MODES_H_

typedef struct {
	char label[64]; /* Label describing the resolution */
	uint32_t width; /*Width of the active video frame*/
	uint32_t height; /*Height of the active video frame*/
	uint32_t hps; /*Start time of Horizontal sync pulse, in pixel clocks (active width + H. front porch)*/
	uint32_t hpe; /*End time of Horizontal sync pulse, in pixel clocks (active width + H. front porch + H. sync width)*/
	uint32_t hmax; /*Total number of pixel clocks per line (active width + H. front porch + H. sync width + H. back porch) */
	uint32_t hpol; /*hsync pulse polarity*/
	uint32_t vps; /*Start time of Vertical sync pulse, in lines (active height + V. front porch)*/
	uint32_t vpe; /*End time of Vertical sync pulse, in lines (active height + V. front porch + V. sync width)*/
	uint32_t vmax; /*Total number of lines per frame (active height + V. front porch + V. sync width + V. back porch) */
	uint32_t vpol; /*vsync pulse polarity*/
	double freq; /*Pixel Clock frequency*/
} VideoMode;

static const VideoMode RGB_LCD = {
	.label = "800x480@60Hz",
	.width = 800,
	.height= 480,
	.hps   = 840,
	.hpe   = 968,
	.hmax  = 1056,
	.hpol  = 0,
	.vps   = 490,
	.vpe   = 492,
	.vmax  = 525,
	.vpol  = 0,
	.freq  = 33.0
};


#endif /* LCD_MODES_H_ */
