#include "COMMON.h"
#include "pic.h"

int main(void)
{
	LCD_Init();
	LCD_DisplayPic(0,0,gImage_pic);

    return 0;
}
