#include <stdio.h>
#include <string.h>
#include "lwip/err.h"
#include "lwip/udp.h"
#include "qspi_remote_update.h"
#include "xil_printf.h"
#include "lwip/inet.h"

u8 start_update_flag = 0;
u8 rxbuffer[MAX_FLASH_LEN];
u32 total_bytes = 0;
static struct udp_pcb *c_pcb;

void print_app_header()
{
    xil_printf("\r\n-----QSPI remote update ------\r\n");
}

//进度打印
void process_print(u8 percent)
{
    switch (percent) {
    case 0:
        sent_msg("0%..");
        xil_printf("0%%..");
        break;
    case 1:
        sent_msg("10%..");
        xil_printf("10%%..");
        break;
    case 2:
        sent_msg("20%..");
        xil_printf("20%%..");
        break;
    case 3:
        sent_msg("30%..");
        xil_printf("30%%..");
        break;
    case 4:
        sent_msg("40%..");
        xil_printf("40%%..");
        break;
    case 5:
        sent_msg("50%..");
        xil_printf("50%%..");
        break;
    case 6:
        sent_msg("60%..");
        xil_printf("60%%..");
        break;
    case 7:
        sent_msg("70%..");
        xil_printf("70%%..");
        break;
    case 8:
        sent_msg("80%..");
        xil_printf("80%%..");
        break;
    case 9:
        sent_msg("90%..");
        xil_printf("90%%..");
        break;
    case 10:
        sent_msg("100%\r\n");
        xil_printf("100%%\r\n");
    default:
        break;
    }
}

//将接收到的BOOT.bin文件写入到QSPI中
int transfer_data()
{
    char msg[60];
    if (start_update_flag) {
        xil_printf("Start QSPI Update!\r\n");
        xil_printf("file size of BOOT.bin is %lu Bytes\r\n", total_bytes);
        sprintf(msg, "file size of BOOT.bin is %lu Bytes\r\n",total_bytes);
        sent_msg(msg);
        if (qspi_update(total_bytes, rxbuffer) != XST_SUCCESS){
            sent_msg("Update Qspi Error!\r\n");
            xil_printf("Update Qspi Error!\r\n");
        }
        else
            total_bytes = 0;
    }

    start_update_flag = 0;

    return 0;
}

//获取当前时间（单位秒sec）
float get_time_s()
{
    XTime tCur = 0;
    XTime_GetTime(&tCur);
    return (tCur / (float) COUNTS_PER_SECOND);
}

//向客户端回送信息
void sent_msg(const char *msg)
{
	static struct pbuf *pbuf2sent;

	pbuf2sent = pbuf_alloc(PBUF_TRANSPORT, strlen(msg), PBUF_POOL);
    if (!pbuf2sent)
        xil_printf("Error allocating pbuf\r\n");

    memcpy(pbuf2sent->payload, msg, strlen(msg));

    //发送报文
    if (udp_send(c_pcb, pbuf2sent) != ERR_OK)
        xil_printf("UDP send error\r\n");

    //释放pbuf
    pbuf_free(pbuf2sent);
}

//UDP接收回调函数
static void recv_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p,
        ip_addr_t *ip, u16_t port)
{
    struct pbuf *q;
    q = p;
    upcb->remote_ip = *ip;
    upcb->remote_port = port;
    c_pcb = upcb;

    if (q->tot_len == 6 && !(memcmp("update", p->payload, 6))) {
        start_update_flag = 1;
        sent_msg("Start QSPI Update\r\n");
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

    pbuf_free(p);
}

void start_application()
{
    struct udp_pcb *pcb;
    err_t err;

    //Qspi初始化
    err = qspi_init();
    if (err != XST_SUCCESS) {
        xil_printf("QSPI init Failed\r\n");
    }
    xil_printf("Successfully init QSPI\r\n");

    //创建新的UDP PCB
    pcb = udp_new();
    if (!pcb) {
        xil_printf("Error creating PCB. Out of Memory\r\n");
        return;
    }

    //绑定端口
    err = udp_bind(pcb, IP_ADDR_ANY, SER_PORT);
    if (err != ERR_OK) {
        xil_printf("Unable to bind to port %d; err %d\r\n",
                SER_PORT, err);
        udp_remove(pcb);
        return;
    }
    //设置接收回调函数
    udp_recv(pcb, (udp_recv_fn) recv_callback, NULL);

    xil_printf("UDP server started @ port %d\n\r", SER_PORT);
}
