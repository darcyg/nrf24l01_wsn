/* 
 * File:        uip-conf.h
 * Description:	uIP configuration file
 * Author:      mickey
 *
 * Created on October 23, 2014, 6:24 PM
 */

#ifndef __UIP_CONF_H__
#define	__UIP_CONF_H__

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "FreeRTOS.h"

typedef uint8_t u8_t; // the 8-bit type used in uIP
typedef uint16_t u16_t; // the 16-bit type used in uIP
typedef unsigned short uip_stats_t; // dataype used for statistics in uIP
typedef unsigned int clock_time_t; // clock type in uIP

typedef void tcp_appcall_fn(void);
typedef void udp_appcall_fn(void);

extern void net_poll(TickType_t timeout);

#define UIP_CONF_EXTERNAL_BUFFER 1

#define UIP_CONF_MAX_CONNECTIONS 5    // Maximum number of TCP connections.
#define UIP_CONF_MAX_LISTENPORTS 40   // Maximum number of listening TCP ports.
#define UIP_CONF_BUFFER_SIZE     400  // uIP buffer size.
#define UIP_CONF_BYTE_ORDER      LITTLE_ENDIAN // CPU byte order.
#define UIP_CONF_LOGGING         0    // Logging on or off
#define UIP_CONF_UDP             0    // UDP support on or off
#define UIP_CONF_UDP_CHECKSUMS   0    // UDP checksums on or off
#define UIP_CONF_STATISTICS      0    // uIP statistics on or off
#define UIP_CONF_BROADCAST	  	 0    // Broadcast support.
#define UIP_CONF_UDP_CONNS	  	 0    // The maximum amount of concurrent UDP connections.

// uip application headers
// #include "telnetd/telnetd.h" // Telnet server
#include "tcp_app/appstate.h" // Test tcp server
#include "mqtt/appstate.h" // MQTT client

typedef struct uip_tcp_state {
  // struct telnetd_appstate    telnetd;    // telnetd server handler
  struct tcp_hello_appstate  tcp_hello; // tcp server handler
  struct mqtt_client        *mqtt;      // mqtt client handler
} uip_tcp_appstate_t;

#if 0
#define UIP_APPCALL() do { if (uip_conn && uip_conn->appcall) uip_conn->appcall(); } while (0)
#define UIP_UDP_APPCALL() do { uip_udp_conn->appcall(); } while (0)
#else
#define UIP_APPCALL uip_conn->appcall
#define UIP_UDP_APPCALL uip_udp_conn->appcall
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* __UIP_CONF_H__ */

