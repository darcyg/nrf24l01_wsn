/* 
 * File:   nrf24l01_link.h
 * Author: mickey
 *
 * Created on November 5, 2014, 5:22 PM
 */

#ifndef NRF24L01_LINK_H
#define	NRF24L01_LINK_H

#ifdef	__cplusplus
extern "C" {
#endif

// For uip_ipaddr_t type
#include "../uip/uip.h"

#define NRF_TIMEOUT 50 // 50 ms

typedef enum {
    IDLE = 0,
    IDLE_NO_TX = 1,
    RX = 2,
    TX_RTS = 3,
    TX = 4,
} nrf_lock;

void nrf_initialize_uip();
void nrf_sethostaddr(uip_ipaddr_t ip);

void nrf_periodic(void);
inline nrf_lock nrf_get_state(void);
inline int nrf_can_tx(void);

void nrf_set_rxbuff(u8_t* buffer);
void nrf_send_buffer(unsigned char* addr, unsigned char* buf, int len);

void net_init(void);
void net_poll(void);

extern u16_t nrf_len;

#ifdef	__cplusplus
}
#endif

#endif	/* NRF24L01_LINK_H */

