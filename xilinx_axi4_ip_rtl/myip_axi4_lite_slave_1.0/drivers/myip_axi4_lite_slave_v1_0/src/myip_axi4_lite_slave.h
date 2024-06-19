
#ifndef MYIP_AXI4_LITE_SLAVE_H
#define MYIP_AXI4_LITE_SLAVE_H


/****************** Include Files ********************/
#include "xil_types.h"
#include "xstatus.h"

#define MYIP_AXI4_LITE_SLAVE_S00_AXI_SLV_REG0_OFFSET 0
#define MYIP_AXI4_LITE_SLAVE_S00_AXI_SLV_REG1_OFFSET 4
#define MYIP_AXI4_LITE_SLAVE_S00_AXI_SLV_REG2_OFFSET 8
#define MYIP_AXI4_LITE_SLAVE_S00_AXI_SLV_REG3_OFFSET 12
#define MYIP_AXI4_LITE_SLAVE_S00_AXI_SLV_REG4_OFFSET 16
#define MYIP_AXI4_LITE_SLAVE_S00_AXI_SLV_REG5_OFFSET 20
#define MYIP_AXI4_LITE_SLAVE_S00_AXI_SLV_REG6_OFFSET 24
#define MYIP_AXI4_LITE_SLAVE_S00_AXI_SLV_REG7_OFFSET 28
#define MYIP_AXI4_LITE_SLAVE_S00_AXI_SLV_REG8_OFFSET 32
#define MYIP_AXI4_LITE_SLAVE_S00_AXI_SLV_REG9_OFFSET 36
#define MYIP_AXI4_LITE_SLAVE_S00_AXI_SLV_REG10_OFFSET 40
#define MYIP_AXI4_LITE_SLAVE_S00_AXI_SLV_REG11_OFFSET 44
#define MYIP_AXI4_LITE_SLAVE_S00_AXI_SLV_REG12_OFFSET 48
#define MYIP_AXI4_LITE_SLAVE_S00_AXI_SLV_REG13_OFFSET 52
#define MYIP_AXI4_LITE_SLAVE_S00_AXI_SLV_REG14_OFFSET 56
#define MYIP_AXI4_LITE_SLAVE_S00_AXI_SLV_REG15_OFFSET 60


/**************************** Type Definitions *****************************/
/**
 *
 * Write a value to a MYIP_AXI4_LITE_SLAVE register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the MYIP_AXI4_LITE_SLAVEdevice.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void MYIP_AXI4_LITE_SLAVE_mWriteReg(u32 BaseAddress, unsigned RegOffset, u32 Data)
 *
 */
#define MYIP_AXI4_LITE_SLAVE_mWriteReg(BaseAddress, RegOffset, Data) \
  	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))

/**
 *
 * Read a value from a MYIP_AXI4_LITE_SLAVE register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the MYIP_AXI4_LITE_SLAVE device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	u32 MYIP_AXI4_LITE_SLAVE_mReadReg(u32 BaseAddress, unsigned RegOffset)
 *
 */
#define MYIP_AXI4_LITE_SLAVE_mReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))

/************************** Function Prototypes ****************************/
/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the MYIP_AXI4_LITE_SLAVE instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus MYIP_AXI4_LITE_SLAVE_Reg_SelfTest(void * baseaddr_p);

#endif // MYIP_AXI4_LITE_SLAVE_H
