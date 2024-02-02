#include <stdio.h>

#include "xparameters.h"

#include "netif/xadapter.h"

#include "platform.h"

#include "platform_config.h"

#include "lwipopts.h"

#include "xil_printf.h"

#include "sleep.h"

#include "lwip/priv/tcp_priv.h"

#include "lwip/init.h"

#include "lwip/inet.h"

#if LWIP_IPV6==1

#include "lwip/ip6_addr.h"

#include "lwip/ip6.h"

#else

#if LWIP_DHCP==1

#include "lwip/dhcp.h"

extern volatile int dhcp_timoutcntr;

#endif

#define DEFAULT_IP_ADDRESS "192.168.1.10"

#define DEFAULT_IP_MASK "255.255.255.0"

#define DEFAULT_GW_ADDRESS "192.168.1.1"

#endif /* LWIP_IPV6 */

extern volatile int TcpFastTmrFlag;

extern volatile int TcpSlowTmrFlag;

void platform_enable_interrupts(void);

void start_application(void);

void print_app_header(void);

int transfer_data();

struct netif server_netif;

#if LWIP_IPV6==1

static void print_ipv6(char *msg, ip_addr_t *ip)

{

	print(msg);

	xil_printf(" %s\n\r", inet6_ntoa(*ip));

}

#else

static void print_ip(char *msg, ip_addr_t *ip)

{

	print(msg);

	xil_printf("%d.%d.%d.%d\r\n", ip4_addr1(ip), ip4_addr2(ip),

	ip4_addr3(ip), ip4_addr4(ip));

}

static void print_ip_settings(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw)

{

	print_ip("Board IP: ", ip);

	print_ip("Netmask : ", mask);

	print_ip("Gateway : ", gw);

}

static void assign_default_ip(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw)

{

	int err;

	xil_printf("Configuring default IP %s \r\n", DEFAULT_IP_ADDRESS);

	err = inet_aton(DEFAULT_IP_ADDRESS, ip);

	if (!err)

		xil_printf("Invalid default IP address: %d\r\n", err);

	err = inet_aton(DEFAULT_IP_MASK, mask);

	if (!err)

		xil_printf("Invalid default IP MASK: %d\r\n", err);

	err = inet_aton(DEFAULT_GW_ADDRESS, gw);

	if (!err)

		xil_printf("Invalid default gateway address: %d\r\n", err);

}

#endif /* LWIP_IPV6 */

int main(void)

{

	struct netif *netif;

//设置开发板的MAC地址

	unsigned char mac_ethernet_address[] = {

	0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

	netif = &server_netif;

	init_platform();

	print_app_header();

//初始化lwIP

	lwip_init();

//将网络接口添加到netif，并将其设置为默认值

	if (!xemac_add(netif, NULL, NULL, NULL, mac_ethernet_address,

	PLATFORM_EMAC_BASEADDR)) {

		xil_printf("Error adding N/W interface\r\n");

		return -1;

	}

#if LWIP_IPV6==1

	netif->ip6_autoconfig_enabled = 1;

	netif_create_ip6_linklocal_address(netif, 1);

	netif_ip6_addr_set_state(netif, 0, IP6_ADDR_VALID);

	print_ipv6("\n\rlink local IPv6 address is:", &netif->ip6_addr[0]);

#endif /* LWIP_IPV6 */

	netif_set_default(netif);

//使能中断

	platform_enable_interrupts();

//指定网络是否已启动

	netif_set_up(netif);

#if (LWIP_IPV6==0)

#if (LWIP_DHCP==1)

//创建新的DHCP客户端

	dhcp_start(netif);

	dhcp_timoutcntr = 2;

	while (((netif->ip_addr.addr) == 0) && (dhcp_timoutcntr > 0))

		xemacif_input(netif);

	if (dhcp_timoutcntr <= 0) {

		if ((netif->ip_addr.addr) == 0) {

			xil_printf("ERROR: DHCP request timed out\r\n");

			assign_default_ip(&(netif->ip_addr),

			&(netif->netmask), &(netif->gw));

		}

	}

#else

	assign_default_ip(&(netif->ip_addr), &(netif->netmask), &(netif->gw));

#endif

	print_ip_settings(&(netif->ip_addr), &(netif->netmask), &(netif->gw));

#endif /* LWIP_IPV6 */

//启动应用程序

	start_application();

	while (1) {

		if (TcpFastTmrFlag) {

			tcp_fasttmr();

			TcpFastTmrFlag = 0;

		}

		if (TcpSlowTmrFlag) {

			tcp_slowtmr();

			TcpSlowTmrFlag = 0;

		}

		xemacif_input(netif);

		transfer_data();

	}

	cleanup_platform();

	return 0;

}
