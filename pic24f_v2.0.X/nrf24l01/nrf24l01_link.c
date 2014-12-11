
#include "../HardwareProfile.h"
#include "nrf24l01_link.h"
#include "nrf24l01.h"
#include "../delay.h"
#include "../blink_LED.h"

// UART for debug
#include "../uart2.h"

// UIP stack for uip_ipaddr_t struct
#include "../uip/uip.h"

// Current state of transmission
nrf_lock nrf_state;

// TODO : delete
u8_t start_transfert;
u8_t abcd;

// Timeout detection
clock_time_t nrf_timer = 0;
// IDLE without TX timer
clock_time_t nrf_notx_timer = 0;

// nrf TX buffer
u8_t nrf_tx_buf[NRF_TX_BUFSIZE];
u8_t* nrf_rx_buf;
u16_t nrf_tx_len;
u16_t nrf_len;
// nrf temp buffer
// TODO : remove this buffer and use tx buffer
UINT8 nrf_buffer[33];

// TX status
unsigned char nrf_lenPkg;
unsigned char nrf_curPkg;
unsigned short nrf_retransmit;

// RX status
unsigned short nrf_pkg_index;

// nrf status byte
extern u8_t nrf_status;
extern u8_t nrf_config;

void nrf_set_rxbuff(u8_t* buffer)
{
    nrf_rx_buf = buffer;
}

void nrf_ip2mac(uip_ipaddr_t ip_addr, unsigned char* mac_addr)
{
    mac_addr[0] = (ip_addr[1]>>8)&0xff;
    mac_addr[1] = 0xc2;
    mac_addr[2] = 0xc2;
    mac_addr[3] = 0x12;
    mac_addr[4] = 0xc2;
}

u8_t nrf_addr_p1[5];
void nrf_sethostaddr(uip_ipaddr_t ip)
{
    nrf_ip2mac(ip, nrf_addr_p1);

    nrf24l01_clear_ce();
    nrf24l01_write_register(nrf24l01_RX_ADDR_P1, nrf_addr_p1, 5);
    nrf24l01_set_ce();
}

void nrf_initialize_uip()
{
    // Disable device
    nrf24l01_clear_ce();

    // Init nrf24l01 device
	nrf24l01_initialize(nrf24l01_CONFIG_DEFAULT_VAL | nrf24l01_CONFIG_PWR_UP | nrf24l01_CONFIG_PRIM_RX,
						nrf24l01_EN_AA_ENAA_P0 | nrf24l01_EN_AA_ENAA_P1,
                        nrf24l01_EN_RXADDR_ERX_P0 | nrf24l01_EN_RXADDR_ERX_P1 | nrf24l01_EN_RXADDR_ERX_P2,
						nrf24l01_SETUP_AW_DEFAULT_VAL,
						nrf24l01_SETUP_RETR_ARD_250 | nrf24l01_SETUP_RETR_ARC_2,
						nrf24l01_RF_CH_DEFAULT_VAL,
						nrf24l01_RF_SETUP_DEFAULT_VAL,
						NULL, // RX_ADDR_P0
						NULL, // mac_addr,
						0xff, // rts pipe address
						nrf24l01_RX_ADDR_P3_DEFAULT_VAL,
						nrf24l01_RX_ADDR_P4_DEFAULT_VAL,
						nrf24l01_RX_ADDR_P5_DEFAULT_VAL,
						NULL, // TX_ADDR
						1, // enable pipe 0
						1, // enable pipe 1
						1, // enable pipe 2
						nrf24l01_RX_PW_P3_DEFAULT_VAL,
						nrf24l01_RX_PW_P4_DEFAULT_VAL,
						nrf24l01_RX_PW_P5_DEFAULT_VAL,
                        nrf24l01_DYNPD_DPL_P0 | nrf24l01_DYNPD_DPL_P1 | nrf24l01_DYNPD_DPL_P2, // Enable dyn payload size
                        nrf24l01_FEATURE_EN_DPL | nrf24l01_FEATURE_EN_DYN_ACK);
    // Wait for powerup
	delay_us(1500);
    nrf_state = IDLE;
    nrf_timer = 0;
    nrf_notx_timer = 0;
    nrf_tx_len = 0; // Noting to TX
    nrf_len = 0;

    // Clear FIFO queues
    nrf24l01_flush_rx();
    nrf24l01_flush_tx();
    // Clear all irqs
    nrf24l01_irq_clear_all();

    nrf24l01_set_ce();
}


inline nrf_lock nrf_get_state(void)
{
    return nrf_state;
}

inline int nrf_can_tx(void)
{
    // return (nrf_state != IDLE) && (nrf_state != IDLE_NO_TX);
    return (nrf_state == IDLE);
}

void nrf_send_buffer(unsigned char* addr, unsigned char* buf, int len)
{
    u8_t nrf_rts_frame[11];

    if(nrf_tx_len > 0) {
        PrintString("Drop previous TX\n");
    }
    nrf24l01_clear_ce();
    nrf24l01_flush_tx(); // TODO : remove
    nrf24l01_irq_clear_all();
    nrf24l01_set_as_tx();

    // Set TX addr
    // TODO: remove
    memcpy(addr, nrf_addr_p1, 5);
    addr[0] = 0xff;
    nrf24l01_write_register(nrf24l01_TX_ADDR, addr, 5);

    nrf_rts_frame[0] = 0x8a;
    memcpy(&nrf_rts_frame[1], nrf_addr_p1, 5);
    memcpy(&nrf_rts_frame[6], nrf_addr_p1, 5);
    nrf_rts_frame[6] = 0xc2;

    nrf24l01_write_tx_payload_no_ack(nrf_rts_frame, 11);
    nrf24l01_transmit();

    memcpy(&nrf_tx_buf[1], buf, len);
    nrf_tx_buf[0] = ((len+30)/31) | 0x80;
    nrf_tx_len = len;
    nrf_pkg_index = 0;
    nrf_retransmit = 0;


    nrf_state = TX_RTS;
    nrf_timer = timer1_get_tick();
}

static void nrf_process_tx_rts(u8_t status)
{
    if(status & nrf24l01_STATUS_MAX_RT) {
        // MAX_RT detected
        nrf24l01_clear_ce();
        nrf24l01_flush_rx();
        nrf24l01_set_as_rx();
        nrf24l01_set_ce();
        nrf_state = IDLE_NO_TX;
        nrf_notx_timer = timer1_get_tick();

        nrf_timer = 0;

        nrf24l01_flush_tx();

        sprintf(Buffer,"ERRO MAX_RT detected (%02x %02x) in RTS\n\r", status, nrf_config);
        PrintString(Buffer);
    }
    // if(status & nrf24l01_STATUS_RX_DR) {
    else if((nrf_status & nrf24l01_STATUS_RX_P_NO) != 0x0e) {
        nrf24l01_clear_ce();
        nrf24l01_set_as_rx();
        nrf24l01_set_ce();
        nrf24l01_flush_tx();
        nrf_state = IDLE_NO_TX;
        nrf_notx_timer = timer1_get_tick();

        sprintf(Buffer,"ERRO RX_DR detected (%02x) in RTS\n\r", nrf_status);
        PrintString(Buffer);
        nrf_timer = 0;
    }
    else if(status & nrf24l01_STATUS_TX_DS) {
        // TX_DS detected
        nrf24l01_clear_ce();

        u8_t addr[5];
        memcpy(addr, nrf_addr_p1, 5);
        addr[0] = 0xc2;
        nrf24l01_write_register(nrf24l01_TX_ADDR, addr, 5);
        nrf24l01_write_register(nrf24l01_RX_ADDR_P0, addr, 5);

        nrf24l01_write_tx_payload(nrf_tx_buf,
                ((nrf_tx_len+1) >= 32 ? 32: (nrf_tx_len+1)));

        nrf24l01_transmit();
        nrf_state = TX;

        nrf_timer = timer1_get_tick();

        // nrf_timer = 0;
        // nrf_tx_len = 0; // Noting to TX

        //
        // nrf24l01_set_as_rx();
        // nrf24l01_set_ce();
        // sprintf(Buffer, "RTS sent, good job %02x\n\r", nrf_status);
        // PrintString(Buffer);
        // nrf_state = IDLE_NO_TX;
        // nrf_timer = 0;
        // nrf_notx_timer = timer1_get_tick();
    }
}

static void nrf_process_tx(u8_t status)
{
    if(status & nrf24l01_STATUS_MAX_RT) {
        // MAX_RT detected
        // Packet lost, ignore skb
        nrf24l01_clear_ce();
        nrf24l01_set_as_rx();
        nrf24l01_set_ce();
        nrf_state = IDLE_NO_TX;
        nrf_notx_timer = timer1_get_tick();

        nrf_timer = 0;

        nrf24l01_flush_tx();

        PrintString("MAX_RT\n");
    }
    else if(status & nrf24l01_STATUS_TX_DS) {
        // TX_DS detected
        // sprintf(Buffer,"TX_DS detected %02x for pkg %d\n\r> ", status, nrf_pkg_index);
        // PrintString(Buffer);
        nrf_pkg_index++;
        if(nrf_pkg_index >= (nrf_tx_len+30)/31) {
            nrf24l01_set_as_rx();
            nrf24l01_set_ce();
            // PrintString("skb sent, good job !!!!\n");
            nrf_state = IDLE_NO_TX;
            nrf_notx_timer = timer1_get_tick();
            nrf_timer = 0;
            nrf_tx_len = 0;
        }
        else {
            // sprintf(Buffer,"Load next message %d (%d)\n\r> ", nrf_pkg_index,
            //         (nrf_pkg_index+1)*31 > (nrf_tx_len) ? (nrf_tx_len)%31 : 31);
            // PrintString(Buffer);
            nrf_retransmit = 0;
            nrf_tx_buf[31*(nrf_pkg_index)] = nrf_pkg_index & 0x7F;

            nrf24l01_write_tx_payload(&nrf_tx_buf[31*nrf_pkg_index],
                    (nrf_pkg_index+1)*31 > (nrf_tx_len) ? (nrf_tx_len)%31+1 : 32);
            // abcd = nrf24l01_get_fifo_status();
            nrf24l01_transmit();

            nrf_timer = timer1_get_tick();
        }
    }
    else {
        // Unknown state
        u8_t fifo;
        fifo = nrf24l01_get_status()&0x00ff;
        if(fifo & nrf24l01_IRQ_MASK) {
            sprintf(Buffer, "Unknown state %02x %02x\n\r", fifo, nrf_status);
            PrintString(Buffer);
        }
    }
}

static void nrf_process_rx(unsigned char status)
{
    u8_t size;
    // check if a packet has been received
    nrf24l01_r_rx_pl_wid(&size);
    while((nrf_status & nrf24l01_STATUS_RX_P_NO) != 0x0e) {
        // read the packet into FIFO
        if(size > 32) {
            sprintf(Buffer, "RX queue (%02x %d). Flush RX\n", nrf_status, size);
            PrintString(Buffer);

            nrf_timer = 0;
            nrf_state = IDLE_NO_TX;
            nrf_notx_timer = timer1_get_tick();
            nrf24l01_flush_rx();
            nrf24l01_flush_tx();
            break;
        }

        nrf24l01_read_rx_payload(nrf_buffer, size);

        if((nrf_status & nrf24l01_STATUS_RX_P_NO) != nrf24l01_STATUS_RX_P_NO_1) {
            sprintf(Buffer, "RX in wrong pipe. Ignore %02x %d\n\r", nrf_status, nrf_curPkg);
            PrintString(Buffer);
            nrf24l01_r_rx_pl_wid(&size);
            continue;
        }
        else if(nrf_curPkg == (nrf_buffer[0] & 0x3F)) {
            // Continue transfert
            // sprintf(Buffer, "\r\nContinue transfert %02x\r\n> ", nrf_buffer[0]&0xff);
            // PrintString(Buffer);
            memcpy(&nrf_rx_buf[31*nrf_curPkg], &nrf_buffer[1], size-1);
            nrf_curPkg++;

            if(nrf_curPkg == nrf_lenPkg) {
                nrf_state = IDLE;
                nrf_len = 31*(nrf_curPkg-1)+size-1;
                nrf_timer = 0;
            // sprintf(Buffer, "RX done %02x %d %d\n\r", nrf_buffer[0]&0xff, nrf_curPkg, nrf_len);
            // PrintString(Buffer);
                break;
            }
            else {
                nrf_timer = timer1_get_tick();
            }
        } // End good pkg
        else {
            // Ignore
            sprintf(Buffer, "Wrong pkg %02x %02x %d\n\r", nrf_buffer[0], start_transfert, nrf_curPkg);
            PrintString(Buffer);
            // It is safer to drop package as there is no error detection system
            // This avoid timeout and start next transfert
            // nrf_timer = 0;
            // nrf_state = IDLE;
        }
        nrf24l01_r_rx_pl_wid(&size);
    } // End RX RX_DR
}

static void nrf_process_idle(u8_t status)
{
    u8_t size;
    // check if a packet has been received
    if((nrf_status & nrf24l01_STATUS_RX_P_NO) != 0x0e) {
        // read the packet into FIFO
        nrf24l01_r_rx_pl_wid(&size);

        while((nrf_status & nrf24l01_STATUS_RX_P_NO) != 0x0e) {
            // read the packet into FIFO
            if(size > 32) {
                sprintf(Buffer, "IDLE RX queue (%02x %d). Flush RX\n", nrf_status, size);
                PrintString(Buffer);

                nrf24l01_flush_rx();
                nrf24l01_r_rx_pl_wid(&size);
                continue;
            }
            nrf24l01_read_rx_payload(nrf_buffer, size);

            if((nrf_status & nrf24l01_STATUS_RX_P_NO) == nrf24l01_STATUS_RX_P_NO_2) {
                // Check dest addr
                if(!memcmp(&nrf_buffer[6], nrf_addr_p1, 5)) {
                    // sprintf(Buffer, "RX got RTS for ME %02x %d\n", nrf_status, size);
                    // PrintString(Buffer);
                }
                else {
                    sprintf(Buffer, "RX got RTS for other %02x %d\n", nrf_status, size);
                    PrintString(Buffer);
                }
                // Do not try to transmit, another node wants to
                nrf_state = IDLE_NO_TX;
                nrf_notx_timer = timer1_get_tick(); // restart no_tx timer
            }
            else if((nrf_status & nrf24l01_STATUS_RX_P_NO) != nrf24l01_STATUS_RX_P_NO_1) {
                sprintf(Buffer, "RX in wrong pipe %02x\n", nrf_status);
                PrintString(Buffer);
            }
            else if(nrf_buffer[0] & 0x80) {
                // Start a new transfert
                blink_LED_TOGGLE();
                nrf_lenPkg = nrf_buffer[0] & 0x3F;
                start_transfert = nrf_buffer[0];
                nrf_curPkg = 1;
                memcpy(nrf_rx_buf, &nrf_buffer[1], size-1);


                if(nrf_lenPkg == 1) {
                    PrintString("Pkg len 1\n\r");
                    nrf_len = size-1;
                    nrf_state = IDLE;
                    // TODO : timer already stopped
                    nrf_timer = 0;
                }
                else {
                    // Start timeout timer
                    nrf_timer = timer1_get_tick();
                    nrf_state = RX;
                    // TODO : this line is useless
                    nrf24l01_get_status();
                    break;
                }
            }
            else {
                // Ignore new transfert
                sprintf(Buffer, "iipk %02x %02x (%d)\n\r", nrf_status, nrf_buffer[0], size);
                PrintString(Buffer);
            }
            nrf24l01_r_rx_pl_wid(&size);
        } // End while
        nrf24l01_get_status();
    }
}


void nrf_periodic()
{
    u8_t status;
    nrf_len = 0;

    // Process interrupt from nrf24l01 low level
    // if(nrf_irq) {
    if((nrf24l01_irq_pin_active()) || ((nrf_status & nrf24l01_STATUS_RX_P_NO) != 0x0e)) {
        // nrf_irq = 0;
        // Save status and clear all interrupts
        nrf24l01_irq_clear_all();

        // PrintString("in\n");
        // TODO: while loop and status check at the end
        while(1) {
            status = nrf_status; // Save status for max_rt and tx_ds

            // Process IDLE events (switch no RX or TX state)
            if((nrf_state == IDLE_NO_TX) || (nrf_state == IDLE)) {
                nrf_process_idle(status);
            }
            // Process RX events
            if(nrf_state == RX) {
                nrf_process_rx(status);
            }
            if(nrf_len > 0) {
                // skb received : exit
                return;
            }
            // Process TX events
            if(nrf_state == TX) {
                nrf_process_tx(status);
            }
            // Process TX RTS events
            else if(nrf_state == TX_RTS) {
                nrf_process_tx_rts(status);
            }
            

            if(nrf_timer && (timer1_get_tick() - nrf_timer > TIMER1_TICK_PER_SECOND/20)) {
                sprintf(Buffer, "nrf timeout %02x %02x (%d)\r\n",
                        nrf24l01_get_status(), nrf24l01_get_fifo_status(), nrf_state);
                PrintString(Buffer);

                if(nrf_state == TX) {
                    sprintf(Buffer, "TX state %d %d %d %02x\n\r", nrf_retransmit, nrf_tx_len, nrf_pkg_index, abcd);
                    PrintString(Buffer);
                }

                nrf_timer = 0;
                // Reset nrf24l01 to a known state
                nrf24l01_clear_ce();
                nrf24l01_flush_rx();
                nrf24l01_flush_tx();
                nrf24l01_irq_clear_all();
                nrf24l01_set_as_rx();
                nrf24l01_set_ce();
                nrf_state = IDLE_NO_TX;
                nrf_notx_timer = timer1_get_tick();
            }

            if(nrf_notx_timer && (timer1_get_tick() - nrf_notx_timer > TIMER1_TICK_PER_SECOND/30)) {
                // sprintf(Buffer, "NO_TX -> IDLE %02x %02x (%d)\r\n",
                //         nrf24l01_get_status(), nrf24l01_get_fifo_status(), nrf_state);
                // PrintString(Buffer);

                nrf_notx_timer = 0;
                if(nrf_state == IDLE_NO_TX) {
                    nrf_state = IDLE;
                    if(nrf_tx_len > 0) {
                        // Retransmit skb
                        nrf_retransmit++;
                        if(nrf_retransmit >= 3) {
                            PrintString("Drop skb\n\r");
                            nrf_tx_len = 0;
                        }
                        else {
                            u8_t addr[5];
                            u8_t nrf_rts_frame[11];

                            nrf24l01_clear_ce();
                            // nrf24l01_flush_tx(); // TODO : remove
                            nrf24l01_set_as_tx();

                            // Set TX addr
                            // TODO: remove
                            memcpy(addr, nrf_addr_p1, 5);
                            addr[0] = 0xff;
                            nrf24l01_write_register(nrf24l01_TX_ADDR, addr, 5);

                            nrf_rts_frame[0] = 0x8a;
                            memcpy(&nrf_rts_frame[1], nrf_addr_p1, 5);
                            memcpy(&nrf_rts_frame[6], nrf_addr_p1, 5);
                            nrf_rts_frame[6] = 0xc2;

                            nrf24l01_write_tx_payload_no_ack(nrf_rts_frame, 11);
                            nrf24l01_transmit();

                            nrf_pkg_index = 0;

                            nrf_state = TX_RTS;
                            nrf_timer = timer1_get_tick();

                            sprintf(Buffer, "Need RT %d %d\n\r", nrf_tx_len, nrf_retransmit);
                            PrintString(Buffer);
                        }
                    }
                }
            }

            // Check if RX FIFO is empty or IRQ active
            if((nrf24l01_irq_pin_active()) || ((nrf_status & nrf24l01_STATUS_RX_P_NO) != 0x0e)) {
                // Still something to process : restart
                // sprintf(Buffer, "RX not empty or IRQ %02x %d %d ",
                //         nrf_status, nrf_state, nrf24l01_irq_pin_active());
                // PrintString(Buffer);
                nrf24l01_irq_clear_all();
                // sprintf(Buffer, "=> %02x\n", nrf_status);
                // PrintString(Buffer);
                continue;
            }
            break;
        } // End while
        // sprintf(Buffer, "out %d %02x %02x\n", nrf_state, nrf24l01_get_config(), nrf_status);
        // PrintString(Buffer);
    } // End IRQ active
    else {
        // Check timeouts
        if(nrf_timer && (timer1_get_tick() - nrf_timer > TIMER1_TICK_PER_SECOND/20)) {
            sprintf(Buffer, "nrf timeout %02x %02x (%d)\r\n",
                    nrf24l01_get_status(), nrf24l01_get_fifo_status(), nrf_state);
            PrintString(Buffer);

            if(nrf_state == TX) {
                sprintf(Buffer, "TX state %d %d %d %02x\n\r", nrf_retransmit, nrf_tx_len, nrf_pkg_index, abcd);
                PrintString(Buffer);
            }

            nrf_timer = 0;
            // Reset nrf24l01 to a known state
            nrf24l01_clear_ce();
            nrf24l01_flush_rx();
            nrf24l01_flush_tx();
            nrf24l01_irq_clear_all();
            nrf24l01_set_as_rx();
            nrf24l01_set_ce();
            nrf_state = IDLE_NO_TX;
            nrf_notx_timer = timer1_get_tick();
        }

        if(nrf_notx_timer && (timer1_get_tick() - nrf_notx_timer > TIMER1_TICK_PER_SECOND/30)) {
            // sprintf(Buffer, "NO_TX -> IDLE %02x %02x (%d)\r\n",
            //         nrf24l01_get_status(), nrf24l01_get_fifo_status(), nrf_state);
            // PrintString(Buffer);

            nrf_notx_timer = 0;
            if(nrf_state == IDLE_NO_TX) {
                nrf_state = IDLE;
                if(nrf_tx_len > 0) {
                    // Retransmit skb
                    nrf_retransmit++;
                    if(nrf_retransmit >= 3) {
                        PrintString("Drop skb\n\r");
                        nrf_tx_len = 0;
                    }
                    else {
                        u8_t addr[5];
                        u8_t nrf_rts_frame[11];

                        nrf24l01_clear_ce();
                        // nrf24l01_flush_tx(); // TODO : remove
                        nrf24l01_set_as_tx();

                        // Set TX addr
                        // TODO: remove
                        memcpy(addr, nrf_addr_p1, 5);
                        addr[0] = 0xff;
                        nrf24l01_write_register(nrf24l01_TX_ADDR, addr, 5);

                        nrf_rts_frame[0] = 0x8a;
                        memcpy(&nrf_rts_frame[1], nrf_addr_p1, 5);
                        memcpy(&nrf_rts_frame[6], nrf_addr_p1, 5);
                        nrf_rts_frame[6] = 0xc2;

                        nrf24l01_write_tx_payload_no_ack(nrf_rts_frame, 11);
                        nrf24l01_transmit();

                        nrf_pkg_index = 0;

                        nrf_state = TX_RTS;
                        nrf_timer = timer1_get_tick();

                        sprintf(Buffer, "Need RT %d %d\n\r", nrf_tx_len, nrf_retransmit);
                        PrintString(Buffer);
                    }
                }
            }
        }
    }
    // else {
    //     u8_t test = nrf24l01_get_config();
    //     if(test == 0x80) {
    //         sprintf(Buffer, "nrf down !!! %d %02x %02x\n", nrf_state, test, nrf_status);
    //         PrintString(Buffer);
    //     }
    // }
} // End periodic

struct timer uip_periodic_timer;

void net_init(void)
{
    // uip task clock
    timer_set(&uip_periodic_timer, TIMER1_TICK_PER_SECOND/2);
    uip_init();
}

void net_poll(void)
{
    // Handle nrf link layer
    nrf_periodic();
    if(nrf_len > 0) {
        uip_len = nrf_len;
        // TODO
        // if(BUF->type == UIP_HTONS(UIP_ETHTYPE_IP)) {
        uip_input();
        if(uip_len > 0) {
            u8_t addr[5];
            // TODO : implement routing table
            addr[0] = 0xc2; // (((struct uip_tcpip_hdr*)uip_buf)->destipaddr[1])&0xff;
            addr[1] = 0xc2;
            addr[2] = 0xc2;
            addr[3] = 0x12;
            addr[4] = 0xc2;
            nrf_send_buffer(addr, uip_buf, uip_len);
        }
    }
    // Handle tcp ip layer
    else if(nrf_can_tx() && timer_expired(&uip_periodic_timer)) {
        int i;
        timer_reset(&uip_periodic_timer);
        for(i = 0; i < UIP_CONNS; ++i) {
            uip_periodic(i);
            if(uip_len > 0) {
                // TODO : implement routing table
                // sprintf(Buffer, "uip nsd %02x %02x %02x (%d) %02x\r\n",
                //         nrf24l01_get_status(), nrf24l01_get_config(),
                //         nrf24l01_get_fifo_status(), nrf_get_state(), nrf24l01_get_rx_addr_2());
                // PrintString(Buffer);
                u8_t addr[5];
                addr[0] = 0xc2;
                addr[1] = 0xc2;
                addr[2] = 0xc2;
                addr[3] = 0x12;
                addr[4] = 0xc2;
                nrf_send_buffer(addr, uip_buf, uip_len);
                break; // One connection at the time
            }
        }
    }
}
