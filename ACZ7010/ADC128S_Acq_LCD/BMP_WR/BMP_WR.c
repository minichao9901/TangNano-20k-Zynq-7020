#include "BMP_WR.h"

FIL fil;
FATFS fatfs;

unsigned char read_line_buf[1920 * 3];
unsigned char Write_line_buf[1920 * 3];

void bmp_read(char * bmp,uint8_t *frame,uint32_t stride)
{
    short y,x;
    short Ximage;
    short Yimage;
    uint32_t iPixelAddr = 0;
    FRESULT res;
    unsigned char TMPBUF[64];
    unsigned int br;

    res = f_open(&fil, bmp, FA_OPEN_EXISTING | FA_READ);
    if(res != FR_OK)
    {
        return ;
    }
    res = f_read(&fil, TMPBUF, 54, &br);
    if(res != FR_OK)
    {
        return ;
    }
    Ximage=(unsigned short int)((TMPBUF[19] << 8) | TMPBUF[18]);
    Yimage=(unsigned short int)((TMPBUF[23] << 8) | TMPBUF[22]);
    iPixelAddr = (Yimage-1)*stride ;

    for(y = 0; y < Yimage ; y++)
    {
        f_read(&fil, read_line_buf, Ximage * 3, &br);
        for(x = 0; x < Ximage; x++)
        {
            frame[x * BYTES_PIXEL + iPixelAddr + 0] = read_line_buf[x * 3 + 0];
            frame[x * BYTES_PIXEL + iPixelAddr + 1] = read_line_buf[x * 3 + 1];
            frame[x * BYTES_PIXEL + iPixelAddr + 2] = read_line_buf[x * 3 + 2];
        }
        iPixelAddr -= stride;
    }
    f_close(&fil);
}


void bmp_write(char * name, char *head_buf, char *data_buf)
{
    short y,x;
    short Ximage;
    short Yimage;
    uint32_t iPixelAddr = 0;
    FRESULT res;
    unsigned int br;         // File R/W count

    memset(&Write_line_buf, 0, 1920*3) ;

    res = f_open(&fil, name, FA_CREATE_ALWAYS | FA_WRITE);
    if(res != FR_OK)
    {
        return ;
    }
    res = f_write(&fil, head_buf, 54, &br) ;
    if(res != FR_OK)
    {
        return ;
    }
    Ximage=(unsigned short)((head_buf[19] << 8) | head_buf[18]);
    Yimage=(unsigned short)((head_buf[23] << 8) | head_buf[22]);
    iPixelAddr = (Yimage-1)*Ximage*3 ;
    for(y = 0; y < Yimage ; y++)
    {
        for(x = 0; x < Ximage; x++)
        {
            Write_line_buf[x*3 + 0] = data_buf[x*3 + iPixelAddr + 0] ;
            Write_line_buf[x*3 + 1] = data_buf[x*3 + iPixelAddr + 1] ;
            Write_line_buf[x*3 + 2] = data_buf[x*3 + iPixelAddr + 2] ;
        }
        res = f_write(&fil, Write_line_buf, Ximage*3, &br) ;
        if(res != FR_OK)
        {
        	printf("Write BMP Failed!\n");
            return;
        }
        iPixelAddr -= Ximage*3;
    }

    f_close(&fil);
    printf("Write BMP Successfully!\n");
}
