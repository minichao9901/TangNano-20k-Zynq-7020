/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 */

#include "xil_io.h"
#include "xil_mmu.h"
#include "xpseudo_asm.h"
#include "remap_ocm.h"
#include <xil_cache.h>

#define GPIO_BASE  0xE000A000
#define GPIO_DIRM  (GPIO_BASE+0x0284)
#define GPIO_MASK_DATA  (GPIO_BASE+0x0010)

// OCM memory used to communicate with CPU0
#define COMM_VAL  (*(volatile u32 *)(0xFFFF0000))


int mainx()
{

	// Remap all 4 64KB blocks of OCM to top of memory
	MY_REMAP();

	// Disable L1 cache for OCM
	Xil_SetTlbAttributes(0xFFFC0000,0x04de2);           // S=b0 TEX=b100 AP=b11, Domain=b1111, C=b0, B=b0

	COMM_VAL = 0;

	/**********************************************************
     * mb bootloop
     * If not using FSBL to download Microblaze elf to DDR
     * the reset vector of the Microblaze needs to contain
     * a 'branch to self' in order to provide the Microblaze
     * valid code to run before before the debugger can halt
     * the Microblaze and download the application that 
     * places valid code at the Microblaze's reset vector
     *
     * If using SDK/XMD to download the Microblaze application
     * uncomment the following two lines of code
     **********************************************************/
	Xil_Out32(0x30000000, 0xb8000000);
	Xil_DCacheFlush();

	///////////////////////////////////////////////
	//Take PL out of reset
	//Enable emio bit 0
	//Xil_Out32(GPIO_DIRM, 0x1);
	//Set emio[0] to release PL reset
	//Xil_Out32(GPIO_MASK_DATA, 0xfffe0001);


	///////////////////////////////////////////////
	// Loop forever
//    while(1) {
//    	xil_printf("CPU0: Hello World\n\r");
//
//    	// Wait until UART TX FIFO is empty
//    	while ((Xil_In32(STDOUT_BASEADDRESS + 0x2C) & 0x08) != 0x08);
//
//    	COMM_VAL = 1;
//    	while(COMM_VAL == 1);
//    }

    return 0;
}
