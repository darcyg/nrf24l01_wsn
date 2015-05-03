/*
 * pic24f RTOS based OS
 * Copyright (C) 2015 <spiriou31@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "uip/nrf24l01_link.h"
// os modules
#include "FreeRTOS.h"
#include "device.h"
#include "task.h"
#include "timers.h"
// Utils
#include <stdbool.h>
#include "logger.h"
#include "uip/uip.h"
// I/O modules
#include "nrf24l01.h"
#include "pic24f_gpio.h" // TODO: remove

typedef enum {
    IDLE_NO_TX = 1,
    IDLE,
    RX,
    TX_RTS,
    TX,
} NRF_STATE;

struct nrf24l01_link_info {
    uip_ipaddr_t ip;                          /*!< IP address */
    uip_ipaddr_t gateway;                     /*!< Gateway IP address */
    uip_ipaddr_t mask;                        /*!< Network mask */
    volatile NRF_STATE state;                 /*!< Current state of transmission */
    TimerHandle_t timer;                      /*!< Timer for timeout detection */
    volatile NRF_STATE timeout_state;
    TaskHandle_t task;

    QueueHandle_t rx_queue;                        /*!< RX buffer queue */
    QueueHandle_t tx_queue;                        /*!< TX buffer queue */
    struct nrf_buffer pool[NRF_LINK_BUFFER_COUNT]; /*!< RX/TX buffer pool */
    struct nrf_buffer *rx_buffer;                  /*!< Current buffer for RX */
    struct nrf_buffer *tx_buffer;                  /*!< Current buffer for TX */
    struct nrf_buffer *pool_head;                  /*!< head of pool */
    // RX status
    uint8_t rx_pkg_count;
    uint8_t rx_pkg_index;
    // TX status
    uint8_t tx_pkg_index;
    unsigned int retransmit_pkg;
    unsigned int retransmit;
    // Link status
    unsigned int rx_packets;
    unsigned int tx_packets;
    unsigned int rx_dropped;
    unsigned int tx_dropped;
};

// Avoid allocating this structure in FreeRTOS task stack
struct nrf24l01_link_info nrf_link;

// link management functions
static void nrf_link_task(void *arg);
static void nrf_ip2mac(uip_ipaddr_t ip_addr, uint8_t* mac_addr);
static void nrf_link_timeout_callback(void *priv);
static void nrf_irq_callback(void *priv);

// link processing functions
static inline void nrf_process_idle(struct device *dev, uint8_t nrf_status, uint8_t size);
static inline void nrf_process_rx(struct device *dev, uint8_t nrf_status, uint8_t size);
static inline void nrf_link_handle_rx(struct device *dev, uint8_t status);

static inline void nrf_process_tx_rts(struct device *dev, uint8_t nrf_status);
static inline void nrf_process_tx(struct device *dev, uint8_t nrf_status);
static inline void nrf_link_handle_tx(struct device *dev, uint8_t status);

uint8_t timer_id = 0xde;

void nrf_link_init(const char *device, uip_ipaddr_t ip, uip_ipaddr_t gateway, uip_ipaddr_t mask, UBaseType_t priority)
{
    struct device* dev = get_device(device);
    if (dev == NULL) {
        log_error("nrf dev <%s> not found", device);
        return;
    }
    // Copy network configuration
    memcpy(nrf_link.ip,      ip,      sizeof(uip_ipaddr_t));
    memcpy(nrf_link.gateway, gateway, sizeof(uip_ipaddr_t));
    memcpy(nrf_link.mask,    mask,    sizeof(uip_ipaddr_t));

    // Create nrf_link task
    xTaskCreate(nrf_link_task, "uip", 2 * configMINIMAL_STACK_SIZE, dev, priority, &nrf_link.task);
    // Create timeout timer
    nrf_link.timer = xTimerCreate("nrf", (TickType_t)pdMS_TO_TICKS(NRF_TIMEOUT), pdFALSE,
                                  &timer_id, nrf_link_timeout_callback);
    // Create rx buffer queue
    nrf_link.rx_queue = xQueueCreate(NRF_LINK_BUFFER_COUNT, sizeof(struct nrf_buffer*));
    if (!nrf_link.rx_queue) {
        log_error("fail alloc RX queue");
    }
    nrf_link.tx_queue = xQueueCreate(NRF_LINK_BUFFER_COUNT, sizeof(struct nrf_buffer*));
    if (!nrf_link.tx_queue) {
        log_error("fail alloc TX queue");
    }
}

void nrf_link_timeout_callback(void *priv)
{
    nrf_link.timeout_state = nrf_link.state;
    xTaskNotifyGive(nrf_link.task);
}

void nrf_irq_callback(void *priv)
{
    portBASE_TYPE xHigherPriorityTaskWoken;
    vTaskNotifyGiveFromISR(nrf_link.task, &xHigherPriorityTaskWoken);

    if (xHigherPriorityTaskWoken != pdFALSE) {
        taskYIELD();
    }
}

BaseType_t nrf_link_wait_rx(struct nrf_buffer **buffer, TickType_t timeout)
{
    return xQueueReceive(nrf_link.rx_queue, buffer, timeout);
}

struct nrf_buffer* nrf_link_get_buffer(void)
{
    // Function protected against concurrent access
    taskENTER_CRITICAL();
    if (!nrf_link.pool_head) {
        // buffer not available
        taskEXIT_CRITICAL();
        return NULL;
    }
    // Buffer available
    struct nrf_buffer *ret = nrf_link.pool_head;
    nrf_link.pool_head = nrf_link.pool_head->next;
    taskEXIT_CRITICAL();
    return ret;
}

void nrf_link_free_buffer(struct nrf_buffer *buf)
{
    buf->next = 0;

    // Function protected against concurrent access
    taskENTER_CRITICAL();
    if (!nrf_link.pool_head) {
        nrf_link.pool_head = buf;
        taskEXIT_CRITICAL();
        return;
    }
    // Buffer available
    struct nrf_buffer *cur = nrf_link.pool_head;
    for (; cur->next; cur = cur->next);
    cur->next = buf;
    taskEXIT_CRITICAL();
}

void nrf_link_send_buffer(struct nrf_buffer *buf)
{
    // Configure control byte
    buf->buf.ctl_byte = ((buf->len + 30) / 31) | 0x80;
    // Send buffer
    xQueueSend(nrf_link.tx_queue, &buf, 0);
    xTaskNotifyGive(nrf_link.task);
}

static void nrf_link_task(void *arg)
{
    struct device *dev = (struct device*)arg;

    // Disable device
    nrf24l01_clear_ce(dev);

    {
        // Compute RX, and TX addrs
        uint8_t addr_p1[5];
        uint8_t addr_tx[5];
        nrf_ip2mac(nrf_link.ip, addr_p1);
        nrf_ip2mac(nrf_link.gateway, addr_tx);
        // Set nrf24l01 configuration
        nrf24l01_set_configuration(dev, nrf_irq_callback, NULL,
                nrf24l01_CONFIG_DEFAULT_VAL | nrf24l01_CONFIG_PWR_UP | nrf24l01_CONFIG_PRIM_RX,
                nrf24l01_EN_AA_ENAA_P0 | nrf24l01_EN_AA_ENAA_P1,
                nrf24l01_EN_RXADDR_ERX_P0 | nrf24l01_EN_RXADDR_ERX_P1 | nrf24l01_EN_RXADDR_ERX_P2,
                nrf24l01_SETUP_AW_DEFAULT_VAL,
                nrf24l01_SETUP_RETR_ARD_250 | nrf24l01_SETUP_RETR_ARC_2,
                nrf24l01_RF_CH_DEFAULT_VAL,
                nrf24l01_RF_SETUP_DEFAULT_VAL,
                NULL,   // RX_ADDR_P0
                addr_p1,   // mac_addr,
                0xff,   // rts pipe address
                nrf24l01_RX_ADDR_P3_DEFAULT_VAL,
                nrf24l01_RX_ADDR_P4_DEFAULT_VAL,
                nrf24l01_RX_ADDR_P5_DEFAULT_VAL,
                addr_tx,   // TX_ADDR
                1,      // enable pipe 0
                1,      // enable pipe 1
                1,      // enable pipe 2
                nrf24l01_RX_PW_P3_DEFAULT_VAL,
                nrf24l01_RX_PW_P4_DEFAULT_VAL,
                nrf24l01_RX_PW_P5_DEFAULT_VAL,
                nrf24l01_DYNPD_DPL_P0 | nrf24l01_DYNPD_DPL_P1 | nrf24l01_DYNPD_DPL_P2, // Dyn payload size
                nrf24l01_FEATURE_EN_DPL | nrf24l01_FEATURE_EN_DYN_ACK);
    }
    // Wait for powerup
    vTaskDelay((TickType_t) pdMS_TO_TICKS(10));
    nrf_link.state = IDLE;
    nrf_link.timeout_state = 0;
    // nrf_timer = 0;
    // nrf_notx_timer = 0;
    // nrf_tx_len = 0; // Noting to TX
    
    // Init buffer pool
    {
        unsigned int i;
        for (i=0; i<NRF_LINK_BUFFER_COUNT-1; i++) {
            nrf_link.pool[i].len = 0;
            nrf_link.pool[i].next = &nrf_link.pool[i+1];
        }
        nrf_link.pool[i].len = 0;
        nrf_link.pool[i].next = 0;
        nrf_link.pool_head = nrf_link.pool;
    }
    // Init rx and tx buffers
    nrf_link.rx_buffer = nrf_link_get_buffer();
    nrf_link.tx_buffer = NULL;

    // Activate FEATURE REGISTER
    nrf24l01_activate_feature(dev);
    // Clear FIFO queues
    nrf24l01_flush_rx(dev);
    nrf24l01_flush_tx(dev);
    // Clear all irqs
    nrf24l01_irq_clear_all(dev);
    // Device is ready to operate, enable it
    nrf24l01_set_ce(dev);

    nrf_link.rx_packets = 0;
    nrf_link.tx_packets = 0;
    nrf_link.rx_dropped = 0;
    nrf_link.tx_dropped = 0;

    while (1) {
        ulTaskNotifyTake(pdTRUE, (TickType_t)portMAX_DELAY);
        // We reach this line because of IRQ, TX queue or timeout

        // We receive buf in the same time we tx (2 buffers)
        while(1) {
            // Clear all irqs and reload status register
            nrf24l01_irq_clear_all(dev);
            // Save the source of interrupt (RX, TX or RT)
            uint8_t status = nrf24l01_get_status(dev);

            // Handle RX frame
            nrf_link_handle_rx(dev, status);
            // Handle TX frame
            nrf_link_handle_tx(dev, status);

            if (!gpio_get(GPIO_PORTA | 0)) {
                // Interrupt still pending, process it
                continue;
            }

            if (nrf_link.timeout_state != 0) {
                if (nrf_link.timeout_state != nrf_link.state) {
                    log_info("fake timeout %d %d", nrf_link.state, nrf_link.timeout_state);
                    nrf_link.timeout_state = 0;
                } else {
                    nrf_link.timeout_state = 0;
                    // log_info("valid timeout %d", nrf_link.state);
                    // Timeout detected
                    if (nrf_link.state <= RX) {
                        if (nrf_link.state == RX) {
                            nrf_link.rx_dropped++;
                        }
                        // RX/TX_NO_IDLE timeout, go IDLE
                        nrf_link.state = IDLE;
                        // Ready for TX ?
                        nrf_link_handle_tx(dev, 0);
                    } else {
                        // Check unknown state (nrf not configured)
                        uint8_t config = nrf24l01_get_config(dev);
                        log_info("tm for %d (%x)", nrf_link.state, config);
                        nrf24l01_clear_ce(dev);
                        nrf24l01_set_as_rx(dev);
                        nrf24l01_set_ce(dev);
                        // TX/TX_RTS timeout, goto IDLE_NO_TX and start timer
                        nrf_link.state = IDLE_NO_TX;
                        xTimerStart(nrf_link.timer, (TickType_t)portMAX_DELAY);
                        // Set nrf24l01 to RX state
                        nrf24l01_flush_rx(dev);
                        nrf24l01_flush_tx(dev);
                        nrf24l01_irq_clear_all(dev);
                        nrf_link.tx_dropped++;
                    }
                }
            }
            break;
        }
    }
}

static inline void nrf_link_handle_tx(struct device *dev, uint8_t status)
{
    switch (nrf_link.state) {
        case IDLE:
            if (!nrf_link.tx_buffer) {
                if (xQueueReceive(nrf_link.tx_queue, &nrf_link.tx_buffer, 0)) {
                    // New buffer to TX
                    // Init tx rt to default value (3-5) before skb is dropped (for RTS/TX)
                    nrf_link.retransmit = NRF_RETRANSMIT;
                } else {
                    // Nothing to TX
                    break;
                }
            }

            if (!(--nrf_link.retransmit)) {
                // skb TX failed, drop skb
                log_error("TX RT");
                nrf_link_free_buffer(nrf_link.tx_buffer);
                nrf_link.tx_buffer = NULL;
                nrf_link.tx_dropped++;
                break;
            }
            // Start TX transfer
            // log_info("start TX tr %d", nrf_link.tx_buffer->len);
            // nrf_link_free_buffer(nrf_link.tx_buffer);
            // nrf_link.tx_buffer = NULL;
            uint8_t nrf_rts_frame[11];
            uint8_t addr[5];

            nrf24l01_clear_ce(dev);
            nrf24l01_set_as_tx(dev);
            nrf24l01_flush_tx(dev); // TODO : remove
            nrf24l01_irq_clear_all(dev);

            // Set TX broadcast addr
            nrf_ip2mac(nrf_link.gateway, addr);
            addr[0] = 0xff;
            nrf24l01_set_tx_addr(dev, addr, 0);


            nrf_rts_frame[0] = 0x8a;
            // src nrf addr
            memcpy(&nrf_rts_frame[1], nrf24l01_get_rx_addr(dev), 5);
            // dst nrf addr
            memcpy(&nrf_rts_frame[6], nrf24l01_get_rx_addr(dev), 5);
            nrf_rts_frame[6] = 0xc2;

            nrf24l01_write_tx_payload_no_ack(dev, nrf_rts_frame, 11);
            nrf24l01_transmit(dev);

            nrf_link.state = TX_RTS;
            // Start timer for TX time slot (RTS+skb)
            xTimerStart(nrf_link.timer, (TickType_t)portMAX_DELAY);
            break;

        case TX_RTS:
            nrf_process_tx_rts(dev, status);
            break;
        case TX:
            nrf_process_tx(dev, status);
            break;
        default:
            break;
    }
}

static inline void nrf_link_handle_rx(struct device *dev, uint8_t nrf_status)
{
    uint8_t size;

    if ((nrf_status & nrf24l01_STATUS_RX_P_NO) == 0x0e) {
        // No pkg received
        return;
    }
    // read packet size
    nrf24l01_r_rx_pl_wid(dev, &size);
    // Update local status (to save pipe number of current packet)
    nrf_status = nrf24l01_get_status(dev);

    // check if a valid packet has been received in rx pipe
    while ((nrf_status & nrf24l01_STATUS_RX_P_NO) != 0x0e) {
        if ((!size) || (size > 32)) {
            log_error("IDLE RX error (%02x %d)", nrf_status, size);
            goto get_next_pkg;
        }
        else if (!nrf_link.rx_buffer && !(nrf_link.rx_buffer = nrf_link_get_buffer())) {
            // Try to get another one
            // rx buffer not available, drop rx frame
            log_info("no rx buf %02x %d %d %d", nrf_status, size,
                     uxQueueMessagesWaiting(nrf_link.tx_queue),
                     uxQueueMessagesWaiting(nrf_link.rx_queue));
        get_next_pkg:
            nrf24l01_flush_rx(dev);
        } else {
            // Process IDLE events (switch no RX or TX state)
            if (nrf_link.state <= IDLE) {
                nrf_process_idle(dev, nrf_status, size);
            } else if (nrf_link.state == RX) {
                // Process RX events
                nrf_process_rx(dev, nrf_status, size);
            } else {
                // Cannot receive buffer
                log_info("rx in state %d", nrf_link.state);
                // TODO: cancel TX and start RX ???
                nrf24l01_flush_rx(dev);
            }
        }
        // Read size for next rx buffer
        nrf24l01_r_rx_pl_wid(dev, &size);
        // Update local status register
        nrf_status = nrf24l01_get_status(dev);
    } // End RX while
}

static void nrf_ip2mac(uip_ipaddr_t ip_addr, uint8_t* mac_addr)
{
    mac_addr[0] = (ip_addr[1] >> 8) & 0xff;
    mac_addr[1] = 0xc2;
    mac_addr[2] = 0xc2;
    mac_addr[3] = 0x12;
    mac_addr[4] = 0xc2;
}

static inline void nrf_process_idle(struct device *dev, uint8_t nrf_status, uint8_t size)
{
    // Read data into test buffer (first byte in ctl_byte, data in buffer)
    nrf24l01_read_rx_payload(dev, &nrf_link.rx_buffer->buf.ctl_byte, size);

    switch (nrf_status & nrf24l01_STATUS_RX_P_NO) {
    // check if a RTS frame as been received (rx pipe 2)
    case nrf24l01_STATUS_RX_P_NO_2:
        // Check dest addr
        // if (!memcmp(&nrf_link.rx_buffer->buf.data[5], nrf24l01_get_rx_addr(dev), 5)) {
        //     log_info("RTS me %02x %d", nrf_status, size);
        // }else {
        //     log_info("RTS other %02x %d", nrf_status, size);
        // }
        // Do not try to transmit, another node wants to because of RTS frame
        // Start NO_TX timeout timer
        xTimerStart(nrf_link.timer, (TickType_t)portMAX_DELAY);
        nrf_link.state = IDLE_NO_TX;
        break;

    // check if data frame is in the good pipe
    case nrf24l01_STATUS_RX_P_NO_1:
        // Valid data, start transfer if first byte is ok (0x80)
        if (nrf_link.rx_buffer->buf.ctl_byte & 0x80) {
            // Start a new transfert
            nrf_link.rx_pkg_count = nrf_link.rx_buffer->buf.ctl_byte & 0x3F;
            nrf_link.rx_pkg_index = 1;

            if (nrf_link.rx_pkg_count == 1) {
                log_info("pkg len 1");
                nrf_link.rx_buffer->len = size - 1;
                if (nrf_link.state == IDLE_NO_TX) {
                    // Stop IDLE_NO_TX timer
                    xTimerStop(nrf_link.timer, (TickType_t)portMAX_DELAY);
                }
                nrf_link.state = IDLE;
                // Send buffer
                xQueueSend(nrf_link.rx_queue, &nrf_link.rx_buffer, 0);
                // get new rx buffer
                nrf_link.rx_buffer = nrf_link_get_buffer();
                nrf_link.rx_packets++;
           }else {
                // Start timeout timer
                xTimerStart(nrf_link.timer, (TickType_t)portMAX_DELAY);
                nrf_link.state = RX;
                // RX state, exit idle process function
                return;
           }
        } else {
            // Ignore new transfert
            log_info("ipk %02x %02x (%d)", nrf_status, nrf_link.rx_buffer->buf.ctl_byte, size);
            nrf_link.state = IDLE;
            nrf24l01_flush_rx(dev);
        }
        break;
    default:
        log_error("RX in wrong pipe %02x", nrf_status);
    } // End switch pipe
}

static inline void nrf_process_rx(struct device *dev, uint8_t nrf_status, uint8_t size)
{
    // Read packet into rx buffer and save ctl_byte
    uint8_t last_data_byte = nrf_link.rx_buffer->buf.data[31*nrf_link.rx_pkg_index-1];
    nrf24l01_read_rx_payload(dev, &nrf_link.rx_buffer->buf.data[31*nrf_link.rx_pkg_index-1], size);
    uint8_t ctl_byte = nrf_link.rx_buffer->buf.data[31*nrf_link.rx_pkg_index-1];
    nrf_link.rx_buffer->buf.data[31*nrf_link.rx_pkg_index-1] = last_data_byte;

    if ((nrf_status & nrf24l01_STATUS_RX_P_NO) != nrf24l01_STATUS_RX_P_NO_1) {
        log_error("pipe %02x %d", nrf_status, nrf_link.rx_pkg_index);
        return;
    }
    if (nrf_link.rx_pkg_index == (ctl_byte & 0x3F)) {
        // Continue transfert
        nrf_link.rx_pkg_index++;

        if (nrf_link.rx_pkg_index == nrf_link.rx_pkg_count) {
            // Stop timeout timer
            xTimerStop(nrf_link.timer, (TickType_t)portMAX_DELAY);
            nrf_link.state = IDLE;
            nrf_link.rx_buffer->len = 31 * (nrf_link.rx_pkg_index - 1) + size - 1;
            // Send buffer
            xQueueSend(nrf_link.rx_queue, &nrf_link.rx_buffer, 0);
            // get new rx buffer
            nrf_link.rx_buffer = nrf_link_get_buffer();
            nrf_link.rx_packets++;
            return;
        // } else {
        //     // Reset timer ??? For now, if RX is not done during time slot, cancel it
        }
    } // End good pkg
    else {
        // Bad packet received, ignore and cancel RX
        // It is safer to drop package as there is no error detection system
        // This avoid timeout and start next transfert
        //
        // OK for now try to complete RX anyway until timeout
        // xTimerStop(nrf_link.timer, (TickType_t)portMAX_DELAY);
        // nrf_link.state = IDLE;
        // nrf24l01_flush_rx(dev);
        log_error("W %d %02x %d", size, ctl_byte, nrf_link.rx_pkg_index);
    }
}

static inline void nrf_process_tx_rts(struct device *dev, uint8_t nrf_status)
{
    if (nrf_status & nrf24l01_STATUS_MAX_RT) {
        // MAX_RT detected
        nrf24l01_clear_ce(dev);
        nrf24l01_flush_rx(dev);
        nrf24l01_set_as_rx(dev);
        nrf24l01_set_ce(dev);
        nrf_link.state = IDLE_NO_TX;
        // Restart timer for idle no tx delay
        xTimerStart(nrf_link.timer, (TickType_t)portMAX_DELAY);

        nrf24l01_flush_tx(dev);

        log_error("MAX_RT: rts %02x %02x", nrf_status, nrf24l01_get_config(dev));
    }
    else if (nrf_status & nrf24l01_STATUS_TX_DS) {
        // TX_DS detected
        nrf24l01_clear_ce(dev); // TODO: not needed ?

        uint8_t addr[5];
        memcpy(addr, nrf24l01_get_rx_addr(dev), 5);
        addr[0] = 0xc2; // TODO: get gateway addr
        nrf24l01_set_tx_addr(dev, addr, 1);

        nrf_link.tx_pkg_index = 0;
        nrf_link.retransmit_pkg = NRF_RETRANSMIT_PKG;

        nrf24l01_write_tx_payload(dev, &nrf_link.tx_buffer->buf.ctl_byte,
                                  ((nrf_link.tx_buffer->len + 1) >= 32 ? 32 : (nrf_link.tx_buffer->len + 1)));

        nrf24l01_transmit(dev);
        nrf_link.state = TX;
        xTimerStart(nrf_link.timer, (TickType_t)portMAX_DELAY);

        // log_info("rts done, tx");

        // // Fow now TX done
        // log_info("rts done");
        // nrf24l01_set_as_rx(dev);
        // nrf24l01_set_ce(dev);
        // nrf_link.state = IDLE_NO_TX;
        // // Restart timer for idle no tx delay
        // xTimerStart(nrf_link.timer, (TickType_t)portMAX_DELAY);
        // // Release TX buffer
        // nrf_link_free_buffer(nrf_link.tx_buffer);
        // nrf_link.tx_buffer = NULL;
    }
}

static inline void nrf_process_tx(struct device *dev, uint8_t nrf_status)
{
    if (nrf_status & nrf24l01_STATUS_MAX_RT) {
        nrf24l01_clear_ce(dev);
        // MAX_RT detected, check pkg retransmission count
        if (--nrf_link.retransmit_pkg) {
            // Reuse last tx payload
            nrf24l01_reuse_tx_pl(dev);
            // Transmit pkg
            nrf24l01_transmit(dev);
            // log_info("rt %d (%d)", nrf_link.tx_pkg_index, nrf_link.retransmit_pkg);
        } else {
            // MAX RT reached, cancel TX
            nrf24l01_set_as_rx(dev);
            nrf24l01_set_ce(dev);
            nrf_link.state = IDLE_NO_TX;
            xTimerStart(nrf_link.timer, (TickType_t)portMAX_DELAY);
            nrf24l01_flush_tx(dev);
            // log_info("MAX_RT %d", nrf_link.tx_pkg_index);
        }
    }
    else if (nrf_status & nrf24l01_STATUS_TX_DS) {
        nrf_link.tx_pkg_index++;
        if (nrf_link.tx_pkg_index >= (nrf_link.tx_buffer->buf.ctl_byte & 0x7F)) {
            nrf24l01_set_as_rx(dev);
            nrf24l01_set_ce(dev);
            // log_info("skb sent, good job (%d)", nrf_link.tx_pkg_index);
            nrf_link.state = IDLE_NO_TX;
            xTimerStart(nrf_link.timer, (TickType_t)portMAX_DELAY);
            // Free tx buffer
            nrf_link_free_buffer(nrf_link.tx_buffer);
            nrf_link.tx_buffer = NULL;
            nrf_link.tx_packets++;
        } else {
            // log_info("tx next %d (%d)", nrf_link.tx_pkg_index,
            //         (nrf_link.tx_pkg_index+1)*31 > (nrf_link.tx_buffer->len) ? (nrf_link.tx_buffer->len)%31 : 31);
            uint8_t old_data = nrf_link.tx_buffer->buf.data[31 * (nrf_link.tx_pkg_index)-1];
            nrf_link.tx_buffer->buf.data[31 * (nrf_link.tx_pkg_index)-1] = nrf_link.tx_pkg_index & 0x7F;

            nrf_link.retransmit_pkg = NRF_RETRANSMIT_PKG;

            nrf24l01_write_tx_payload(dev, &nrf_link.tx_buffer->buf.data[31 * nrf_link.tx_pkg_index-1],
                    (nrf_link.tx_pkg_index+1)*31 > (nrf_link.tx_buffer->len) ? (nrf_link.tx_buffer->len)%31+1 : 32);
            nrf24l01_transmit(dev);

            // Restore buffer data
            nrf_link.tx_buffer->buf.data[31 * (nrf_link.tx_pkg_index)-1] = old_data;

            // TODO: do not reset timer, only one timeout for one tx slot
        }
    }
}

void net_poll(TickType_t timeout)
{
    static struct nrf_buffer *tx_buffer = NULL;
    struct nrf_buffer *rx_buffer;

    // Check if a buffer is available
    if (nrf_link_wait_rx(&rx_buffer, timeout)) {
        // Process received buffer
        uip_len = rx_buffer->len;
        uip_buf = rx_buffer->buf.data;

        if (uip_len > 0) {
            uip_input();
            if (uip_len > 0) {
                rx_buffer->len = uip_len;
                // log_info("buffer to send %d", uip_len);
                nrf_link_send_buffer(rx_buffer);
            } else {
                nrf_link_free_buffer(rx_buffer);
            }
        }
    } else {
        // Process tcp/ip stack timeout
        // Try to get a valid TX buffer
        if (tx_buffer || (tx_buffer = nrf_link_get_buffer())) {
            unsigned int i;
            uip_buf = tx_buffer->buf.data;
            for(i = 0; i < UIP_CONNS; ++i) {
                uip_periodic(i);
                if(uip_len > 0) {
                    // log_info("uns %d", uip_len);
                    tx_buffer->len = uip_len;
                    nrf_link_send_buffer(tx_buffer);
                    tx_buffer = NULL;
                    break; // One connection at the time (not enough tx buffers)
                }
            }
        }
    }
}

unsigned int nrf_link_get_rx_packets(void)
{
    return nrf_link.rx_packets;
}
unsigned int nrf_link_get_rx_dropped(void)
{
    return nrf_link.rx_dropped;
}
unsigned int nrf_link_get_tx_packets(void)
{
    return nrf_link.tx_packets;
}
unsigned int nrf_link_get_tx_dropped(void)
{
    return nrf_link.tx_dropped;
}
