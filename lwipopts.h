#ifndef _LWIPOPTS_H
#define _LWIPOPTS_H

// NO_SYS must be 1 for threadsafe_background
#define NO_SYS                          1
#define LWIP_SOCKET                     0
#define LWIP_NETCONN                    0

// Cannot use MEM_LIBC_MALLOC with threadsafe_background
#define MEM_LIBC_MALLOC                 0
#define MEMP_MEM_MALLOC                 0
#define MEM_ALIGNMENT                   4
#define MEM_SIZE                        4096
#define MEMP_NUM_TCP_SEG                32
#define MEMP_NUM_ARP_QUEUE              10
#define PBUF_POOL_SIZE                  24

#define LWIP_ARP                        1
#define LWIP_ETHERNET                   1
#define LWIP_ICMP                       1
#define LWIP_RAW                        1
#define TCP_WND                         (8 * TCP_MSS)
#define TCP_MSS                         1460
#define TCP_SND_BUF                     (8 * TCP_MSS)

#define LWIP_NETIF_STATUS_CALLBACK      1
#define LWIP_NETIF_LINK_CALLBACK        1
#define LWIP_NETIF_HOSTNAME             1
#define LWIP_DHCP                       1
#define LWIP_DNS                        1

// HTTP
#define LWIP_HTTPD                      1
#define LWIP_HTTPD_CGI                  1
#define LWIP_HTTPD_SSI                  1
#define HTTPD_FSDATA_FILE "fsdata_custom.c"

#endif
