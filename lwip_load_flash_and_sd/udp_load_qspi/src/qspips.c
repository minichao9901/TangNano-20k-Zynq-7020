#include "xparameters.h"
#include "xqspips.h"
#include "qspi_remote_update.h"

#define QSPI_DEVICE_ID		XPAR_XQSPIPS_0_DEVICE_ID

//发送到FLASH器件的指令
#define WRITE_STATUS_CMD	0x01
#define WRITE_CMD			0x02
#define READ_CMD			0x03
#define WRITE_DISABLE_CMD	0x04
#define READ_STATUS_CMD		0x05
#define WRITE_ENABLE_CMD	0x06
#define FAST_READ_CMD		0x0B
#define DUAL_READ_CMD		0x3B
#define QUAD_READ_CMD		0x6B
#define BULK_ERASE_CMD		0xC7
#define	SEC_ERASE_CMD		0xD8
#define READ_ID				0x9F

//FLASH BUFFER中各数据的偏移量
#define COMMAND_OFFSET		0  // FLASH instruction */
#define ADDRESS_1_OFFSET	1  // MSB byte of address to read or write */
#define ADDRESS_2_OFFSET	2  // Middle byte of address to read or write */
#define ADDRESS_3_OFFSET	3  // LSB byte of address to read or write */
#define DATA_OFFSET			4  // Start of Data for Read/Write */
#define DUMMY_OFFSET		4  // Dummy byte offset for fast, dual and quad reads
#define DUMMY_SIZE			1  // Number of dummy bytes for fast, dual and quad reads
#define RD_ID_SIZE			4  // Read ID command + 3 bytes ID response */
#define BULK_ERASE_SIZE		1  // Bulk Erase command size */
#define SEC_ERASE_SIZE		4  // Sector Erase command + Sector address */
#define OVERHEAD_SIZE		4  // control information: command and address

#define SECTOR_SIZE		0x10000
#define NUM_SECTORS		0x100
#define NUM_PAGES		0x10000
#define PAGE_SIZE		256

void FlashErase(XQspiPs *QspiPtr, u32 Address, u32 ByteCount);
void FlashWrite(XQspiPs *QspiPtr, u32 Address, u32 ByteCount, u8 Command);
void FlashRead(XQspiPs *QspiPtr, u32 Address, u32 ByteCount, u8 Command);
int FlashReadID(void);
void FlashQuadEnable(XQspiPs *QspiPtr);

static XQspiPs QspiInstance;

u8 ReadBuffer[PAGE_SIZE + DATA_OFFSET + DUMMY_SIZE];
u8 WriteBuffer[PAGE_SIZE + DATA_OFFSET];

int qspi_init()
{
    int Status;

    XQspiPs_Config *QspiConfig;

    //初始化QSPI驱动
    QspiConfig = XQspiPs_LookupConfig(QSPI_DEVICE_ID);
    if (QspiConfig == NULL) {
        return XST_FAILURE;
    }

    Status = XQspiPs_CfgInitialize(&QspiInstance, QspiConfig,
            QspiConfig->BaseAddress);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    //设置自动启动和手动片选模式
    XQspiPs_SetOptions(&QspiInstance,
    XQSPIPS_FORCE_SSELECT_OPTION | XQSPIPS_HOLD_B_DRIVE_OPTION);
    //设置QSPI时钟的分频系数
    XQspiPs_SetClkPrescaler(&QspiInstance, XQSPIPS_CLK_PRESCALE_8);
    //片选信号置为有效
    XQspiPs_SetSlaveSelect(&QspiInstance);
    //读FLASH ID
    FlashReadID();
    //使能FLASH Quad模式
    FlashQuadEnable(&QspiInstance);

    return XST_SUCCESS;
}

int qspi_update(u32 total_bytes, const u8 *flash_data)
{
    u8 *BufferPtr;
    u8 pre_precent = -1;
    u8 process_percent = 0;
    u32 writed_len = 0;
    u32 readed_len = 0;
    u32 write_addr = 0;
    u32 read_addr = 0;
    char msg[60];
    float start_time, over_time;
    float elapsed_time;
    int i;
    int total_page = total_bytes / PAGE_SIZE + 1;
    //擦除FLASH
    printf("Performing Erase Operation...\r\n");
    sent_msg("Performing Erase Operation...\r\n");
    start_time = get_time_s();
    FlashErase(&QspiInstance, 0, total_bytes);
    over_time = get_time_s();
    elapsed_time = over_time - start_time;
    printf("Erase Operation Successful.\r\n");
    printf("INFO:Elapsed time = %2.3f sec.\r\n", elapsed_time);
    sent_msg("Erase Operation Successful.\r\n");
    sprintf(msg, "INFO:Elapsed time = %2.3f sec.\r\n",elapsed_time);
    sent_msg(msg);
    //向FLASH中写入数据
    printf("Performing Program Operation...\r\n");
    sent_msg("Performing Program Operation...\r\n");
    start_time = get_time_s();
    for (i = 0; i < total_page; i++) {
        process_percent = writed_len / (float) total_bytes * 10 + (float)1/2;
        if (process_percent != pre_precent)
            process_print(process_percent);
        pre_precent = process_percent;

        memcpy(&WriteBuffer[DATA_OFFSET], &flash_data[writed_len], PAGE_SIZE);
        FlashWrite(&QspiInstance, write_addr, PAGE_SIZE, WRITE_CMD);
        writed_len += PAGE_SIZE;
        write_addr += PAGE_SIZE;
    }
    over_time = get_time_s();
    elapsed_time = over_time - start_time;
    printf("Program Operation Successful.\r\n");
    printf("INFO:Elapsed time = %2.3f sec.\r\n", elapsed_time);
    sent_msg("Program Operation Successful.\r\n");
    sprintf(msg, "INFO:Elapsed time = %2.3f sec.\r\n",elapsed_time);
    sent_msg(msg);
    //使用QUAD模式从FLASH中读出数据并进行校验
    BufferPtr = &ReadBuffer[DATA_OFFSET + DUMMY_SIZE];
    printf("Performing Verify Operation...\r\n");
    sent_msg("Performing Verify Operation...\r\n");
    memset(ReadBuffer, 0x00, sizeof(ReadBuffer));
    start_time = get_time_s();
    while (readed_len < total_bytes) {
        process_percent = readed_len / (float) total_bytes * 10 + (float)1/2;
        if (process_percent != pre_precent)
            process_print(process_percent);
        pre_precent = process_percent;

        FlashRead(&QspiInstance, read_addr, PAGE_SIZE, QUAD_READ_CMD);
        if ((readed_len + PAGE_SIZE) <= total_bytes) {
            //对比写入FLASH与从FLASH中读出的数据
            for (i = 0; i < PAGE_SIZE; i++)
                if (BufferPtr[i] != flash_data[readed_len + i])
                    goto error_printf;
        } else {
            for (i = 0; i < total_bytes - readed_len; i++)
                if (BufferPtr[i] != flash_data[readed_len + i])
                    goto error_printf;
            over_time = get_time_s();
            elapsed_time = over_time - start_time;
            printf("Verify Operation Successful.\r\n");
            printf("INFO:Elapsed time = %2.3f sec.\r\n", elapsed_time);
            sent_msg("Verify Operation Successful.\r\n");
            sprintf(msg, "INFO:Elapsed time = %2.3f sec.\r\n",elapsed_time);
            sent_msg(msg);
        }
        readed_len += PAGE_SIZE;
        read_addr += PAGE_SIZE;
    }

    return XST_SUCCESS;

error_printf:
    printf("Verify data error at address 0x%lx\tSend Data is 0x%x\tRead Data is 0x%x\r\n",
            read_addr + i, flash_data[readed_len + i], BufferPtr[i]);
    sprintf(msg, "Verify data error at address 0x%lx.\r\n",read_addr + i);
    sent_msg(msg);
    return XST_FAILURE;
}

/*****************************************************************************/
/**
 *
 * This function writes to the  serial FLASH connected to the QSPI interface.
 * All the data put into the buffer must be in the same page of the device with
 * page boundaries being on 256 byte boundaries.
 *
 * @param	QspiPtr is a pointer to the QSPI driver component to use.
 * @param	Address contains the address to write data to in the FLASH.
 * @param	ByteCount contains the number of bytes to write.
 * @param	Command is the command used to write data to the flash. QSPI
 *		device supports only Page Program command to write data to the
 *		flash.
 *
 * @return	None.
 *
 * @note		None.
 *
 ******************************************************************************/
void FlashWrite(XQspiPs *QspiPtr, u32 Address, u32 ByteCount, u8 Command)
{
    u8 WriteEnableCmd = { WRITE_ENABLE_CMD };
    u8 ReadStatusCmd[] = { READ_STATUS_CMD, 0 }; /* must send 2 bytes */
    u8 FlashStatus[2];

    /*
     * Send the write enable command to the FLASH so that it can be
     * written to, this needs to be sent as a seperate transfer before
     * the write
     */
    XQspiPs_PolledTransfer(QspiPtr, &WriteEnableCmd, NULL,
            sizeof(WriteEnableCmd));

    /*
     * Setup the write command with the specified address and data for the
     * FLASH
     */
    WriteBuffer[COMMAND_OFFSET] = Command;
    WriteBuffer[ADDRESS_1_OFFSET] = (u8) ((Address & 0xFF0000) >> 16);
    WriteBuffer[ADDRESS_2_OFFSET] = (u8) ((Address & 0xFF00) >> 8);
    WriteBuffer[ADDRESS_3_OFFSET] = (u8) (Address & 0xFF);

    /*
     * Send the write command, address, and data to the FLASH to be
     * written, no receive buffer is specified since there is nothing to
     * receive
     */
    XQspiPs_PolledTransfer(QspiPtr, WriteBuffer, NULL,
            ByteCount + OVERHEAD_SIZE);

    /*
     * Wait for the write command to the FLASH to be completed, it takes
     * some time for the data to be written
     */
    while (1) {
        /*
         * Poll the status register of the FLASH to determine when it
         * completes, by sending a read status command and receiving the
         * status byte
         */
        XQspiPs_PolledTransfer(QspiPtr, ReadStatusCmd, FlashStatus,
                sizeof(ReadStatusCmd));

        /*
         * If the status indicates the write is done, then stop waiting,
         * if a value of 0xFF in the status byte is read from the
         * device and this loop never exits, the device slave select is
         * possibly incorrect such that the device status is not being
         * read
         */
        if ((FlashStatus[1] & 0x01) == 0) {
            break;
        }
    }
}

/*****************************************************************************/
/**
 *
 * This function reads from the  serial FLASH connected to the
 * QSPI interface.
 *
 * @param	QspiPtr is a pointer to the QSPI driver component to use.
 * @param	Address contains the address to read data from in the FLASH.
 * @param	ByteCount contains the number of bytes to read.
 * @param	Command is the command used to read data from the flash. QSPI
 *		device supports one of the Read, Fast Read, Dual Read and Fast
 *		Read commands to read data from the flash.
 *
 * @return	None.
 *
 * @note		None.
 *
 ******************************************************************************/
void FlashRead(XQspiPs *QspiPtr, u32 Address, u32 ByteCount, u8 Command)
{
    /*
     * Setup the write command with the specified address and data for the
     * FLASH
     */
    WriteBuffer[COMMAND_OFFSET] = Command;
    WriteBuffer[ADDRESS_1_OFFSET] = (u8) ((Address & 0xFF0000) >> 16);
    WriteBuffer[ADDRESS_2_OFFSET] = (u8) ((Address & 0xFF00) >> 8);
    WriteBuffer[ADDRESS_3_OFFSET] = (u8) (Address & 0xFF);

    if ((Command == FAST_READ_CMD) || (Command == DUAL_READ_CMD)
            || (Command == QUAD_READ_CMD)) {
        ByteCount += DUMMY_SIZE;
    }
    /*
     * Send the read command to the FLASH to read the specified number
     * of bytes from the FLASH, send the read command and address and
     * receive the specified number of bytes of data in the data buffer
     */
    XQspiPs_PolledTransfer(QspiPtr, WriteBuffer, ReadBuffer,
            ByteCount + OVERHEAD_SIZE);
}

/*****************************************************************************/
/**
 *
 * This function erases the sectors in the  serial FLASH connected to the
 * QSPI interface.
 *
 * @param	QspiPtr is a pointer to the QSPI driver component to use.
 * @param	Address contains the address of the first sector which needs to
 *		be erased.
 * @param	ByteCount contains the total size to be erased.
 *
 * @return	None.
 *
 * @note		None.
 *
 ******************************************************************************/
void FlashErase(XQspiPs *QspiPtr, u32 Address, u32 ByteCount)
{
    u8 WriteEnableCmd = { WRITE_ENABLE_CMD };
    u8 ReadStatusCmd[] = { READ_STATUS_CMD, 0 }; /* must send 2 bytes */
    u8 FlashStatus[2];
    int Sector;
    int total_sector;
    u8 pre_precent = -1;
    u8 process_percent = 0;
    /*
     * If erase size is same as the total size of the flash, use bulk erase
     * command
     */
    if (ByteCount == (NUM_SECTORS * SECTOR_SIZE)) {
        /*
         * Send the write enable command to the FLASH so that it can be
         * written to, this needs to be sent as a seperate transfer
         * before the erase
         */
        XQspiPs_PolledTransfer(QspiPtr, &WriteEnableCmd, NULL,
                sizeof(WriteEnableCmd));

        /* Setup the bulk erase command*/
        WriteBuffer[COMMAND_OFFSET] = BULK_ERASE_CMD;

        /*
         * Send the bulk erase command; no receive buffer is specified
         * since there is nothing to receive
         */
        XQspiPs_PolledTransfer(QspiPtr, WriteBuffer, NULL,
        BULK_ERASE_SIZE);

        /* Wait for the erase command to the FLASH to be completed*/
        while (1) {
            /*
             * Poll the status register of the device to determine
             * when it completes, by sending a read status command
             * and receiving the status byte
             */
            XQspiPs_PolledTransfer(QspiPtr, ReadStatusCmd, FlashStatus,
                    sizeof(ReadStatusCmd));

            /*
             * If the status indicates the write is done, then stop
             * waiting; if a value of 0xFF in the status byte is
             * read from the device and this loop never exits, the
             * device slave select is possibly incorrect such that
             * the device status is not being read
             */
            if ((FlashStatus[1] & 0x01) == 0) {
                break;
            }
        }

        return;
    }

    /*
     * If the erase size is less than the total size of the flash, use
     * sector erase command
     */
    total_sector = (ByteCount / SECTOR_SIZE) + 1;
    for (Sector = 0; Sector < total_sector; Sector++) {
        process_percent = Sector / (float) total_sector * 10 + (float)1/2;;
        if (process_percent != pre_precent)
            process_print(process_percent);
        pre_precent = process_percent;

        /*
         * Send the write enable command to the SEEPOM so that it can be
         * written to, this needs to be sent as a seperate transfer
         * before the write
         */
        XQspiPs_PolledTransfer(QspiPtr, &WriteEnableCmd, NULL,
                sizeof(WriteEnableCmd));

        /*
         * Setup the write command with the specified address and data
         * for the FLASH
         */
        WriteBuffer[COMMAND_OFFSET] = SEC_ERASE_CMD;
        WriteBuffer[ADDRESS_1_OFFSET] = (u8) (Address >> 16);
        WriteBuffer[ADDRESS_2_OFFSET] = (u8) (Address >> 8);
        WriteBuffer[ADDRESS_3_OFFSET] = (u8) (Address & 0xFF);

        /*
         * Send the sector erase command and address; no receive buffer
         * is specified since there is nothing to receive
         */
        XQspiPs_PolledTransfer(QspiPtr, WriteBuffer, NULL,
        SEC_ERASE_SIZE);

        /*
         * Wait for the sector erse command to the
         * FLASH to be completed
         */
        while (1) {
            /*
             * Poll the status register of the device to determine
             * when it completes, by sending a read status command
             * and receiving the status byte
             */
            XQspiPs_PolledTransfer(QspiPtr, ReadStatusCmd, FlashStatus,
                    sizeof(ReadStatusCmd));

            /*
             * If the status indicates the write is done, then stop
             * waiting, if a value of 0xFF in the status byte is
             * read from the device and this loop never exits, the
             * device slave select is possibly incorrect such that
             * the device status is not being read
             */
            if ((FlashStatus[1] & 0x01) == 0) {
                break;
            }
        }

        Address += SECTOR_SIZE;
    }
}

/*****************************************************************************/
/**
 *
 * This function reads serial FLASH ID connected to the SPI interface.
 *
 * @param	None.
 *
 * @return	XST_SUCCESS if read id, otherwise XST_FAILURE.
 *
 * @note		None.
 *
 ******************************************************************************/
int FlashReadID(void)
{
    int Status;

    /* Read ID in Auto mode.*/
    WriteBuffer[COMMAND_OFFSET] = READ_ID;
    WriteBuffer[ADDRESS_1_OFFSET] = 0x23; /* 3 dummy bytes */
    WriteBuffer[ADDRESS_2_OFFSET] = 0x08;
    WriteBuffer[ADDRESS_3_OFFSET] = 0x09;

    Status = XQspiPs_PolledTransfer(&QspiInstance, WriteBuffer, ReadBuffer,
    RD_ID_SIZE);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    printf("FlashID=0x%x 0x%x 0x%x\n\r", ReadBuffer[1], ReadBuffer[2],
            ReadBuffer[3]);

    return XST_SUCCESS;
}

/*****************************************************************************/

/**
 *
 * This function enables quad mode in the serial flash connected to the
 * SPI interface.
 *
 * @param	QspiPtr is a pointer to the QSPI driver component to use.
 *
 * @return	None.
 *
 * @note		None.
 *
 ******************************************************************************/
void FlashQuadEnable(XQspiPs *QspiPtr)
{
    u8 WriteEnableCmd = { WRITE_ENABLE_CMD };
    u8 ReadStatusCmd[] = { READ_STATUS_CMD, 0 };
    u8 QuadEnableCmd[] = { WRITE_STATUS_CMD, 0 };
    u8 FlashStatus[2];

    if (ReadBuffer[1] == 0x9D) {

        XQspiPs_PolledTransfer(QspiPtr, ReadStatusCmd, FlashStatus,
                sizeof(ReadStatusCmd));

        QuadEnableCmd[1] = FlashStatus[1] | 1 << 6;

        XQspiPs_PolledTransfer(QspiPtr, &WriteEnableCmd, NULL,
                sizeof(WriteEnableCmd));

        XQspiPs_PolledTransfer(QspiPtr, QuadEnableCmd, NULL,
                sizeof(QuadEnableCmd));
    }
}
