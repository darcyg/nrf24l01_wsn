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

#include "HardwareProfile.h"
#include "timer1.h" // For timer1_get_tick

/* Here we include the header file for the application(s) we use in
   our project. */
// #include "smtp.h"
// #include "telnetd.h"
// #include "webserver.h"
// #include "dhcpc.h"
// #include "resolv.h"
// #include "webclient.h"
#include "telnetd/telnetd.h" // Telnet server
#include "uip/psock.h" // Default tcp server with psock
#include "mqtt/appstate.h" // MQTT client

typedef void tcp_appcall_fn(void);
typedef void udp_appcall_fn(void);

typedef struct uip_tcp_state {
  // telnetd state struct
  char *lines[TELNETD_CONF_NUMLINES];
  char buf[TELNETD_CONF_LINELEN];
  char bufptr;
  u8_t numsent;
  u8_t state;
  // tcp server state struct
  struct psock p;
  char inputbuffer[10];
  char name[40];
  // mqtt client state struct
  struct mqtt_client* mqtt;
} uip_tcp_appstate_t;

/* Finally we define the application function to be called by uIP. */
// #ifndef UIP_APPCALL
// #define UIP_APPCALL uip_appcall
// #endif /* UIP_APPCALL */

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

