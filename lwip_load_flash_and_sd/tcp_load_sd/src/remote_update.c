/*
 * remote_update.c
 *
 *  Created on: 2024��2��2��
 *      Author: minichao
 */

#include "remote_update.h"

#include "xparameters.h"

#include "ff.h"

#include "string.h"

#include <stdio.h>

#include "lwip/err.h"

#include "lwip/tcp.h"

#include "xil_printf.h"

u8 start_update_flag = 0;

u8 rxbuffer[MAX_FLASH_LEN];

u32 total_bytes = 0;

#define FILE_NAME "BOOT.bin"

struct tcp_pcb *c_pcb;

FATFS fs;

void print_app_header()

{

	xil_printf("-----SD remote update demo------\n");

}

//����sd��

void sd_mount() {

	FRESULT status;

	BYTE work[FF_MAX_SS];

//����sd����ע���ļ�ϵͳ����

	status = f_mount(&fs, "", 1);

	if (status != FR_OK) {

		printf("%d\n", status);

		printf("It isn't FAT format\n");

		f_mkfs("", FM_FAT32, 0, work, sizeof work);

		f_mount(&fs, "", 1);

	}

}

//д����

void sd_write_data(u8 wr_dat[], u32 wr_len) {

	FIL fil;

	UINT bw;

//�������ߴ��ļ�

	f_open(&fil, FILE_NAME, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);

//�ƶ���дָ��

	f_lseek(&fil, 0);

//д����

	f_write(&fil, wr_dat, wr_len, &bw);

//�ر��ļ�

	f_close(&fil);

}

//�����յ���BOOT.bin�ļ�д�뵽SD��

int transfer_data()

{

	char msg[60];

	if (start_update_flag) {

		xil_printf("\r\nStart SD Update!\r\n");

		xil_printf("file size of BOOT.bin is %lu Bytes\r\n", total_bytes);

		sprintf(msg, "file size of BOOT.bin is %lu Bytes\r\n", total_bytes);

		sent_msg(msg);

		sd_write_data(rxbuffer, total_bytes);

		xil_printf("SD Update finish!\n");

		total_bytes = 0;

	}

	start_update_flag = 0;

	return 0;

}

//��ͻ��˻�����Ϣ

void sent_msg(const char *msg)

{

	err_t err;

	tcp_nagle_disable(c_pcb);

	if (tcp_sndbuf(c_pcb) > strlen(msg)) {

		err = tcp_write(c_pcb, msg, strlen(msg), TCP_WRITE_FLAG_COPY);

		if (err != ERR_OK)

			xil_printf("tcp_server: Error on tcp_write: %d\r\n", err);

		err = tcp_output(c_pcb);

		if (err != ERR_OK)

			xil_printf("tcp_server: Error on tcp_output: %d\r\n", err);

	} else

		xil_printf("no space in tcp_sndbuf\r\n");

}

//���ջص�����

static err_t recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p,
		err_t err)

{

	struct pbuf *q;

	if (!p) {

		tcp_close(tpcb);

		tcp_recv(tpcb, NULL);

		xil_printf("tcp connection closed\r\n");

		return ERR_OK;

	}

	q = p;

	if (q->tot_len == 6 && !(memcmp("update", p->payload, 6))) {

		start_update_flag = 1;

		sent_msg("\r\nStart SD Update\r\n");

	} else if (q->tot_len == 5 && !(memcmp("clear", p->payload, 5))) {

		start_update_flag = 0;

		total_bytes = 0;

		sent_msg("Clear received data\r\n");

		xil_printf("Clear received data\r\n");

	} else {

		while (q->tot_len != q->len) {

			memcpy(&rxbuffer[total_bytes], q->payload, q->len);

			total_bytes += q->len;

			q = q->next;

		}

		memcpy(&rxbuffer[total_bytes], q->payload, q->len);

		total_bytes += q->len;

	}

	tcp_recved(tpcb, p->tot_len);

	pbuf_free(p);

	return ERR_OK;

}

err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)

{

	xil_printf("tcp_server: Connection Accepted\r\n");

	c_pcb = newpcb; //�������ӵĿͻ���PCB

//���ý��ջص�

	tcp_recv(c_pcb, recv_callback);

	tcp_arg(c_pcb, NULL);

	return ERR_OK;

}

int start_application()
{

	struct tcp_pcb *pcb;

	err_t err;

//����SD��

	sd_mount();

	xil_printf("Successfully init SD\r\n");

	print_app_header();

//����TCP PCB

	pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);

	if (!pcb) {

		xil_printf("Error creating PCB. Out of Memory\n\r");

		return -1;

	}

//�󶨶˿ں�

	err = tcp_bind(pcb, IP_ANY_TYPE, SER_PORT);

	if (err != ERR_OK) {

		xil_printf("Unable to bind to port %d: err = %d\n\r", SER_PORT, err);

		return -2;

	}

//�˴�����Ҫ�ص��������κβ���

	tcp_arg(pcb, NULL);

//��������

	pcb = tcp_listen(pcb);

	if (!pcb) {

		xil_printf("Out of memory while tcp_listen\n\r");

		return -3;

	}

//ָ�����ڴ������ӵĻص�

	tcp_accept(pcb, accept_callback);

	xil_printf("TCP server started @ port %d\n\r", SER_PORT);

	return 0;

}
