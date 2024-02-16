#include "COMMON.h"
#include "pic.h"

int main(void)
{
	SII9022_Init();
	LCD_Init();
	LCD_DisplayPic(0,0,gImage_pic);

    return 0;
}
