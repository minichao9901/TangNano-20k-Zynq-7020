/**
  *****************************************************************************
  * 						lcd��ʾ��ؿ�
  *****************************************************************************
  *
  * @File   : lcd.c
  * @By     : Sun
  * @Version: V1.0
  * @Date   : 2021 / 11 / 04
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/
#include "lcd.h"
#include "font.h"

/******** X���Y�����귽�� ********

 ���� ���� ���� ���� ���� ���� ���� ������	X��(0~800)
|
|
|
|
��
Y��(0~480)

*********************************/

XAxiVdma     Vdma;
DisplayCtrl  DispCtrl;

uint8_t *frame = (uint8_t*)FrameBuffer_Addr;

/***************������ɫ�ͱ���ɫ***************/
uint32_t POINT_COLOR=LCD_RED;	//������ɫ��Ĭ��Ϊ��ɫ
uint32_t BACK_COLOR =LCD_WHITE; 	//���ֱ���ɫ��Ĭ��Ϊ��ɫ


/**
  *****************************************************************************
  * @��������: RGBLCD��ʼ��
  *****************************************************************************
**/
void LCD_Init()
{
	//����VDMA
	run_vdma_frame_buffer(&Vdma, VDMA_ID, RGB_LCD.width, RGB_LCD.height,
							FrameBuffer_Addr,0, 0,ONLY_READ);

    //��ʼ��Display controller
	DisplayInitialize(&DispCtrl, DISP_VTC_ID, DYNCLK_BASEADDR);

    //����VideoMode
	DisplaySetMode(&DispCtrl, &RGB_LCD);
	DisplayStart(&DispCtrl);
	LCD_Color_Fill(0,0,800,480,0xFFFFFF);
}

/**
  *****************************************************************************
  * @��������: RGBLCDˢ�£�ͼ�����ݸı�󣬱���ˢ�²�����ʾ��ͼ��
  * @ע�⣺	ʹ�ñ�����ˢ��ͼ����Ҫʱ�䣬���ڻ���ͼ�κ���ʹ�ñ�����һ����ˢ��ȫͼ
  * 		��Ҫ��ʱ����Ƶ��ˢ�£��ᵼ��ͼ������ر���
  *****************************************************************************
**/
void LCD_Refresh()
{
	//ˢ��Data Cache������ͼ��
	Xil_DCacheFlushRange((UINTPTR) FrameBuffer_Addr, 800*480*BYTES_PIXEL);
}

/**
  *****************************************************************************
  * @��������: ����
  *
  * @�������: x,y:������
  * 		 POINT_COLOR:�˵����ɫ
  *****************************************************************************
**/
void LCD_DrawPoint(uint16_t x,uint16_t y,uint32_t color)
{
	//x�޷�
	if(x>800)
		x=800;
	//y�޷�
	if(y>480)
		y=480;
	frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y] = color&0x0000ff;
	frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 1] = (color>>8)&0x0000ff;
	frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 2] = (color>>16)&0x0000ff;

	//ˢ��Cache����������
	//Xil_DCacheFlush();
}


/**
  *****************************************************************************
  * @��������: ��ָ�����������ָ����ɫ��
  *
  * @�������: (x1,y1),(x2,y2):�����ζԽ�����,�����СΪ:(x2-x1+1)*(y2-y1+1)
  * 		  color:Ҫ������ɫ
  *****************************************************************************
**/
void LCD_Color_Fill(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint32_t color)
{
	uint16_t x,y,temp;

	if(y1>y2)
	{
		temp=y1;
		y1=y2;
		y2=temp;
	}

	if(x1>x2)
	{
		temp=x1;
		x1=x2;
		x2=temp;
	}

	for(y=y1;y<=y2;y++)
	{
		for(x=x1;x<=x2;x++)
		{
			frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y] = color&0x0000ff;
			frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 1] = (color>>8)&0x0000ff;
			frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 2] = (color>>16)&0x0000ff;
		}
	}

	//ˢ��Cache����������
	//Xil_DCacheFlush();
}

/**
  *****************************************************************************
  * @��������: ����
  *
  * @�������: color:Ҫ������ɫ
  *****************************************************************************
**/
void LCD_Clear(uint32_t color)
{
	uint16_t x,y;
	for(y=0;y<480;y++)
	{
		for(x=0;x<800;x++)
		{
			frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y] = color&0x0000ff;
			frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 1] = (color>>8)&0x0000ff;
			frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 2] = (color>>16)&0x0000ff;
		}
	}

	//ˢ��Cache����������
	Xil_DCacheFlush();
}
/*******************************************************
 *@��������:��POINT_COLORָ������ɫ��ֱ��
 *@�������:uint16_t x1   : ��һ�����x����
 *			uint16_t y1   : ��һ�����y����
 *			uint16_t x2   : �ڶ������x����
 *			uint16_t y2   : �ڶ������y����
 *			uint16_t color: �ߵ���ɫ
 *ʹ��˵�� :ʹ���������ֻҪ�������㶼��LCD��ʾ��Χ�ڣ���Ҫ�������λ��
 *@����ֵ  :0:����, 1:����ʧ��
 *******************************************************/
uint8_t LCD_DrawWaveLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;

	if(y1 >= 480 - 40 - 1)
		y1 = 480 - 40 - 1;

	if(y1 <= 40)
		y1 = 40;

	if(y2 >= 480 - 40 - 1)
		y2 = 480 - 40 - 1;

	if(y2 <= 40)
		y2 = 40;

	delta_x = x2 - x1; //������������
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if (delta_x > 0)
		incx = 1; //x����Ϊ-1
	else if (delta_x == 0)
		incx = 0; //��ֱ��
	else {
		incx = -1; //x����Ϊ1
	}

	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0; //ˮƽ��
	else {
		incy = -1;
		delta_y = -delta_y;
	}

	if (delta_x > delta_y)
		distance = delta_x; //ѡȡ��������������
	else
		distance = delta_y;

	for (t = 0; t <= distance + 1; t++) //�������
	{
		//LCD_Fast_DrawPoint(uRow, uCol, POINT_COLOR); //����

		//x�޷�
		if(uRow>800)
			uRow=800;
		//y�޷�
		if(uCol>480)
			uCol=480;
		frame[BYTES_PIXEL*uRow + 800*BYTES_PIXEL*uCol] = POINT_COLOR&0x0000ff;
		frame[BYTES_PIXEL*uRow + 800*BYTES_PIXEL*uCol + 1] = (POINT_COLOR>>8)&0x0000ff;
		frame[BYTES_PIXEL*uRow + 800*BYTES_PIXEL*uCol + 2] = (POINT_COLOR>>16)&0x0000ff;


		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance) {
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance) {
			yerr -= distance;
			uCol += incy;
		}
	}
	//ˢ��Cache����������
	//Xil_DCacheFlush();

	return 0;
}
/**
  *****************************************************************************
  * @��������: ����
  *
  * @�������: x1,y1:�������
  * 		 x2,y2:�յ�����
  *****************************************************************************
**/
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint32_t color)
{
	uint16_t t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //������������
	delta_y=y2-y1;
	uRow=x1;
	uCol=y1;
	if(delta_x>0)
		incx=1;  //���õ�������
	else if(delta_x==0)
		incx=0;  //��ֱ��
	else
	{
		incx=-1;
		delta_x=-delta_x;
	}
	if(delta_y>0)
		incy=1;
	else if(delta_y==0)
		incy=0;  //ˮƽ��
	else
	{
		incy=-1;
		delta_y=-delta_y;
	}
	if(delta_x>delta_y)
		distance=delta_x;         //ѡȡ��������������
	else
		distance=delta_y;
	for(t=0;t<=distance+1;t++ )    //�������
	{
		frame[BYTES_PIXEL*uRow + 800*BYTES_PIXEL*uCol] = color&0x0000ff;
		frame[BYTES_PIXEL*uRow + 800*BYTES_PIXEL*uCol + 1] = (color>>8)&0x0000ff;
		frame[BYTES_PIXEL*uRow + 800*BYTES_PIXEL*uCol + 2] = (color>>16)&0x0000ff;

		xerr+=delta_x ;
		yerr+=delta_y ;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}

	//ˢ��Cache����������
	//Xil_DCacheFlush();
}

/**
  *****************************************************************************
  * @��������: ������
  *
  * @�������: (x1,y1),(x2,y2):���εĶԽ�����
  *****************************************************************************
**/
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color)
{
    LCD_DrawLine(x1, y1, x2, y1, color);
    LCD_DrawLine(x1, y1, x1, y2, color);
    LCD_DrawLine(x1, y2, x2, y2, color);
    LCD_DrawLine(x2, y1, x2, y2, color);
}

/**
  *****************************************************************************
  * @��������: ��ָ��λ����ʾһ���ַ�
  *
  * @�������:	x,y:��ʼ����
  * 		 	num:Ҫ��ʾ���ַ�:" "--->"~"
  * 		 	size:�����С 12/16/24/32
  * 			mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
  *
  * @Sample:	LCD_ShowChar(200, 50,'A',16,1);//��(200,50)��ʾ�ַ�'A'
  *****************************************************************************
**/
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{
    uint8_t temp,t1,t;
    uint16_t y0=y;
    uint8_t csize=(size/8+((size%8)?1:0))*(size/2);   //�õ�����һ���ַ���Ӧ������ռ���ֽ���
    num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
    for(t=0;t<csize;t++) {
        if(size==12)
            temp=asc2_1206[num][t];  //����1206����
        else if(size==16)
            temp=asc2_1608[num][t];  //����1608����
        else if(size==24)
            temp=asc2_2412[num][t];  //����2412����
        else if(size==32)
            temp=asc2_3216[num][t];  //����3216����
        else return;                 //û�е��ֿ�
        for(t1=0;t1<8;t1++) {
            if(temp&0x80)
			{
            	frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y] = POINT_COLOR&0x0000ff;
            	frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 1] = (POINT_COLOR>>8)&0x0000ff;
            	frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 2] = (POINT_COLOR>>16)&0x0000ff;
			}
            else if(mode==0)
            {
				frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y] = BACK_COLOR&0x0000ff;
				frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 1] = (BACK_COLOR>>8)&0x0000ff;
				frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 2] = (BACK_COLOR>>16)&0x0000ff;
            }
            temp<<=1;
            y++;
            if(y>=480)
                return;     //��������
            if((y-y0)==size) {
                y=y0;
                x++;
                if(x>=800)
                    return;  //��������
                break;
            }
        }
    }

    //ˢ��Cache����������
    //Xil_DCacheFlush();
}

/**
  *****************************************************************************
  * @��������: ��ʾ�ַ���
  *
  * @�������: x,y:�������
  * 		 width,height:�����С
  * 		 size:�����С
  * 		 *p:�ַ�����ʼ��ַ
  * 		 mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
  *****************************************************************************
**/
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t mode,uint8_t *p)
{
    uint8_t x0=x;
    width+=x;
    height+=y;
    while((*p<='~')&&(*p>=' ')) {	//�ж��ǲ��ǷǷ��ַ�!
        if(x>=width) {
            x=x0;
            y+=size;
        }
        if(y>=height)
            break;//�˳�
        LCD_ShowChar(x,y,*p,size,mode);
        x+=size/2;
        p++;
    }

}

/**
  *****************************************************************************
  * @��������: ����m^n
  *
  * @�������: m,n
  *
  * @����ֵ   : m^n�η�.
  *****************************************************************************
**/
uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
    while(n--)
        result*=m;
    return result;
}

/**
  *****************************************************************************
  * @��������: ��ʾ����,��λΪ0,����ʾ
  *
  * @�������: x,y:�������
  * 		 len:����(��Ҫ��ʾ��λ��)
  * 		 size:�����С
  * 		 color:��ɫ
  * 		 num:��ֵ(0~4294967295);
  *****************************************************************************
**/
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{
    uint8_t t,temp;
    uint8_t enshow=0;
    for(t=0;t<len;t++) {
       temp=(num/LCD_Pow(10,len-t-1))%10;
       if(enshow==0&&t<(len-1))
       {
            if(temp==0)
            {
                LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
                continue;
            }else enshow=1;
       }
       LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0);
    }
}


/**
  *****************************************************************************
  * @��������: ��ʾ����,��λΪ0,��ʾ0
  *
  * @�������: x,y:�������
  * 		 num:��ֵ(0~999999999);
  * 		 len:����(��Ҫ��ʾ��λ��)
  * 		 size:�����С
  * 		 mode[7]:0,�����0;1,���0.
  * 		 mode[1]:0,num=0��ʾ0;1,num=0����ʾ
  * 		 [6:2]:����
  * 		 mode[0]:0,�ǵ�����ʾ;1,������ʾ.
  *****************************************************************************
**/
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{
    uint8_t t,temp;
    uint8_t enshow=0;
    for(t=0;t<len;t++)
    {
       temp=(num/LCD_Pow(10,len-t-1))%10;
       if(enshow==0&&t<(len-1))
       {
            if(temp==0)
            {
                 if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);
                 else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);
                 continue;
            }else enshow=1;
       }
       if((mode&0X02)&&(!num))
    	   LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);
       else
    	   LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01);
    }
}

/**
  *****************************************************************************
  * @��������: ��ʾͼƬ
  *
  * @�������: x,y:�������
  * 		 pic:ͼƬ������ָ��(ͼƬ���뱣��ͷ����Ϣ)
  *****************************************************************************
**/
void LCD_DisplayPic(uint16_t x, uint16_t y,const uint8_t *Pic)
{
	uint32_t width,height;
	uint32_t x_pos, y_pos;
	uint32_t Cnt = 0;

	width = (Pic[2]<<8) + Pic[3];//��ȡͼƬ���
	height = (Pic[4]<<8) + Pic[5];//��ȡͼƬ�߶�

	for(y_pos=y;y_pos<y+height;y_pos++)
	{
		for(x_pos=x;x_pos<x+width;x_pos++)
		{
			frame[BYTES_PIXEL*x_pos + 800*BYTES_PIXEL*y_pos] =  Pic[Cnt + 8];
			frame[BYTES_PIXEL*x_pos + 800*BYTES_PIXEL*y_pos + 1] = Pic[Cnt + 1 + 8];
			frame[BYTES_PIXEL*x_pos + 800*BYTES_PIXEL*y_pos + 2] = Pic[Cnt + 2 + 8];
			Cnt+=BYTES_PIXEL;
		}
	}

	//ˢ��Cache����������
	LCD_Refresh();

}

