#ifndef _LWIPOPTS_H
#define _LWIPOPTS_H

// ============================================================================
// lwIP Configuration File
// ============================================================================
// This file configures the lwIP (lightweight IP) networking stack for the
// Raspberry Pi Pico 2W web server. lwIP is a small, open-source TCP/IP stack
// designed for embedded systems with limited resources.
//
// Purpose:
//   - Configures memory management, network protocols, and HTTP server features
//   - Optimized for the Pico 2W's limited RAM (264KB) and processing power
//   - Enables WiFi connectivity and HTTP web server functionality
//
// Key Configuration Areas:
//   - System mode: NO_SYS mode for threadsafe_background operation
//   - Memory management: Custom memory pools optimized for embedded use
//   - Network protocols: TCP/IP, ARP, ICMP, DHCP, DNS
//   - HTTP server: Web server with CGI and SSI support
//
// Note: This configuration is specifically tuned for the Pico 2W's CYW43 WiFi
//       chip and threadsafe_background mode, which allows WiFi operations to
//       run in the background without blocking the main application.
// ============================================================================

// ============================================================================
// System Configuration
// ============================================================================
// NO_SYS must be 1 for threadsafe_background mode
// This disables the lwIP operating system abstraction layer (no threading)
#define NO_SYS                          1
// Disable BSD socket API (not needed in NO_SYS mode)
#define LWIP_SOCKET                     0
// Disable netconn API (not needed in NO_SYS mode)
#define LWIP_NETCONN                    0

// ============================================================================
// Memory Management Configuration
// ============================================================================
// Cannot use standard libc malloc with threadsafe_background mode
// Use lwIP's custom memory pool allocator instead
#define MEM_LIBC_MALLOC                 0
// Disable memp (memory pool) malloc wrapper
#define MEMP_MEM_MALLOC                 0
// Memory alignment (4 bytes for 32-bit ARM architecture)
#define MEM_ALIGNMENT                   4
// Total heap memory size for lwIP (4KB = 4096 bytes)
// Adjust based on your application's memory needs
#define MEM_SIZE                        4096
// Number of TCP segments that can be queued
// Higher values allow more concurrent TCP connections
#define MEMP_NUM_TCP_SEG                32
// Number of ARP queue entries (for address resolution)
#define MEMP_NUM_ARP_QUEUE              10
// Packet buffer pool size (number of pbufs available)
// Each pbuf can hold network packet data
#define PBUF_POOL_SIZE                  24

// ============================================================================
// Network Protocol Configuration
// ============================================================================
// Enable ARP (Address Resolution Protocol) for IP to MAC address mapping
#define LWIP_ARP                        1
// Enable Ethernet support (required for WiFi on Pico 2W)
#define LWIP_ETHERNET                   1
// Enable ICMP (Internet Control Message Protocol) - used for ping
#define LWIP_ICMP                       1
// Enable RAW sockets (low-level network access)
#define LWIP_RAW                        1
// TCP window size (receive window) = 8 * TCP_MSS
// Larger window allows better throughput but uses more memory
#define TCP_WND                         (8 * TCP_MSS)
// TCP Maximum Segment Size (1460 bytes is standard for Ethernet)
// This is the maximum data payload per TCP packet
#define TCP_MSS                         1460
// TCP send buffer size = 8 * TCP_MSS
// Buffer for outgoing TCP data
#define TCP_SND_BUF                     (8 * TCP_MSS)

// ============================================================================
// Network Interface Configuration
// ============================================================================
// Enable network interface status change callbacks
// Allows code to be notified when interface goes up/down
#define LWIP_NETIF_STATUS_CALLBACK      1
// Enable link status change callbacks
// Notifies when physical link (WiFi) connects/disconnects
#define LWIP_NETIF_LINK_CALLBACK        1
// Enable hostname support (allows setting a device hostname)
#define LWIP_NETIF_HOSTNAME             1
// Enable DHCP (Dynamic Host Configuration Protocol)
// Allows the Pico to automatically obtain IP address from router
#define LWIP_DHCP                       1
// Enable DNS (Domain Name System) client
// Allows resolving domain names to IP addresses
#define LWIP_DNS                        1

// ============================================================================
// HTTP Server Configuration
// ============================================================================
// Enable the lwIP HTTP daemon (web server)
#define LWIP_HTTPD                      1
// Enable CGI (Common Gateway Interface) support
// Allows dynamic content generation via C functions
#define LWIP_HTTPD_CGI                  1
// Enable SSI (Server Side Includes) support
// Allows embedding dynamic content in HTML files
#define LWIP_HTTPD_SSI                  1
// Specify the filesystem data file
// This file contains the embedded HTML/CSS/JS files as C arrays
// Generated by makefsdata.py from files in the fs/ directory
#define HTTPD_FSDATA_FILE "fsdata_custom.c"

#endif
