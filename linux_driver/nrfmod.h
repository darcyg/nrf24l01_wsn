#ifndef __NRF_SPI_H
#define __NRF_SPI_H

#include <linux/gpio.h>
#include <linux/spi/spi.h>

#include <linux/netdevice.h>   /* struct device, and other headers */

#define SPI_BUFF_SIZE	48 // need 33 at least (32 nrf buffer + 1 status)

typedef enum {INIT, // Driver currently doing config stuffs
    IDLE, // Wait for pkg to be received
    IDLE_NO_TX, // Wait for pkg to be received but cannot transmit (RTS or slot timeout)
    RX, // Currently receiving pkgs
    TX_RTS, // Sending RTS frame
    TX_WAIT, // Abort RTS frame (should not happened)
    TX // Currently transmitting pkgs
} nrf_state;

struct nrf_control {
    unsigned int gpio_ce;
    unsigned int gpio_irq;

    u8 tx_addr[5]; // Current transmit MAC addr
    u8 rx_addr_p0[5]; // Current MAC addr for ACK rcv
    u8 rx_addr_p1[5]; // MAC addr of device
    u8 rx_addr_p2[5]; // MAC addr for rts
    u8 mac_padding; // nrf MAC addr is 5 bytes, whereas eth MAC addr is 6 bytes

    nrf_state state; // Current state of processing function

    u8 config; // config register
    u8 setup_retr; // Automatic retransmission config register
    // TODO : add all registers
};

#define NRF_TRANFER_COUNT 8

struct spi_control {
    struct spi_device *spi_device; // spi device link
    struct spi_message msg; // SPI msg. Protected by nrf_priv->is_processing
    struct spi_transfer transfer[NRF_TRANFER_COUNT];
	u8 *tx_buff; // SPI TX buffer
	u8 *rx_buff; // SPI RX buffer
};

struct rx_pkg {
    unsigned int count; // packets count driver should receive for current skb
    unsigned int cur; // Current pkg id
    unsigned char *buff; // RX buff for skb
};

struct nrf_spi_gpios {
    int ce;
    int irq;
};

struct skb_queue {
	struct sk_buff **buff; // skb queue for TX
	unsigned int head; // Head of the queue (struct sk_buff*)
	unsigned int tail; // Tail of the queue (struct sk_buff*)
	spinlock_t lock; // Lock to protect queue
};

struct nrf_priv {
	struct net_device *ndev; // Net device struct
    int dev_index; // Index of current device in nrf_devices array
                   // This is used to release every spi modules at exit time

    short int irq_handle; // Interrupt handle for IRQ pin
    struct nrf_control nrf_ctl; // nrf interface
    struct spi_control spi_ctl; // spi interface

    // State machine
    int is_processing; // Currently processing nrf events (SPI active)
    spinlock_t process_lock; // Global lock to protect is_processing
    int is_irq; // gpio IRQ happened
    int is_timeout; // RX or TX timeout happened
    nrf_state state; // Current machine state
    // TODO: replace with wait_for_completion
    struct semaphore process_sem; // Used to wait for processing to complete in nrf_release
    struct task_struct* event_tsk; // Main kthread as the state machine of the driver

    // TX parameters
    struct skb_queue tx_buff; // sk_buff queue for TX
	struct sk_buff *skb; // Current TX skb
    int retransmit; // retransmit count for TX skb
    int pkg_index; // Current pkg index of TX skb

    // RX parameters
    struct rx_pkg rx_pkg; // Structure for pkg reception

    // Timer parameters
    struct hrtimer timer_wdg; // Timer for timeout detection
    struct hrtimer timer_no_tx; // Timer to protect time slots
    long wdg_sec;
    unsigned long wdg_nsec;
};

int nrfmod_execute_cmd(struct nrf_priv* priv, u8 cmd, u8* data, unsigned int len);
int nrfmod_write_register(struct nrf_priv* priv, u8 regnumber, u8* data, unsigned int len);
int nrfmod_read_register(struct nrf_priv* priv, u8 regnumber, u8* data, unsigned int len);
int nrfmod_power_up(struct nrf_priv* priv);
int nrfmod_power_down(struct nrf_priv* priv);
int nrfmod_flush_tx(struct nrf_priv* priv);
int nrfmod_flush_rx(struct nrf_priv* priv);
int nrfmod_clear_all_irq(struct nrf_priv* priv);
int nrfmod_read_status(struct nrf_priv* priv);
int nrfmod_read_config(struct nrf_priv* priv);
int nrfmod_write_setup_retr(struct nrf_priv* priv);
int nrfmod_set_as_tx(struct nrf_priv* priv);
int nrfmod_set_as_rx(struct nrf_priv* priv);
int nrfmod_write_tx_payload(struct nrf_priv* priv, u8 *data, unsigned int len);
int nrfmod_write_tx_payload_no_ack(struct nrf_priv* priv, u8 *data, unsigned int len);
int nrfmod_read_rx_payload_size(struct nrf_priv* priv);
int nrfmod_read_rx_payload(struct nrf_priv* priv, unsigned int len);
int nrfmod_init_device(struct nrf_priv* priv);

// ------------------------------------------------------------------
// Defines for all commands on SPI interface
// ------------------------------------------------------------------
// SPI command defines
#define nrf24l01_R_REGISTER     	 0x00
#define nrf24l01_W_REGISTER      	 0x20
#define nrf24l01_R_RX_PAYLOAD   	 0x61
#define nrf24l01_W_TX_PAYLOAD   	 0xA0
#define nrf24l01_FLUSH_TX       	 0xE1
#define nrf24l01_FLUSH_RX       	 0xE2
#define nrf24l01_REUSE_TX_PL    	 0xE3
#define nrf24l01_ACTIVATE            0x50
#define nrf24l01_NOP            	 0xFF
// SPI advanced command defines
#define nrf24l01_R_RX_PL_WID    	 0x60
#define nrf24l01_W_ACK_PAYLOAD  	 0xA8
#define nrf24l01_W_TX_PAYLOAD_NO_ACK 0xB0
// SPI command data mask defines
#define nrf24l01_R_REGISTER_DATA     0x1F
#define nrf24l01_W_REGISTER_DATA     0x1F

// ------------------------------------------------------------------
// Defines all registers on SPI interface
// ------------------------------------------------------------------
#define nrf24l01_CONFIG         0x00
#define nrf24l01_EN_AA          0x01
#define nrf24l01_EN_RXADDR      0x02
#define nrf24l01_SETUP_AW       0x03
#define nrf24l01_SETUP_RETR     0x04
#define nrf24l01_RF_CH          0x05
#define nrf24l01_RF_SETUP       0x06
#define nrf24l01_STATUS         0x07
#define nrf24l01_OBSERVE_TX     0x08
#define nrf24l01_CD             0x09
#define nrf24l01_RX_ADDR_P0     0x0A
#define nrf24l01_RX_ADDR_P1     0x0B
#define nrf24l01_RX_ADDR_P2     0x0C
#define nrf24l01_RX_ADDR_P3     0x0D
#define nrf24l01_RX_ADDR_P4     0x0E
#define nrf24l01_RX_ADDR_P5     0x0F
#define nrf24l01_TX_ADDR        0x10
#define nrf24l01_RX_PW_P0       0x11
#define nrf24l01_RX_PW_P1       0x12
#define nrf24l01_RX_PW_P2       0x13
#define nrf24l01_RX_PW_P3       0x14
#define nrf24l01_RX_PW_P4       0x15
#define nrf24l01_RX_PW_P5       0x16
#define nrf24l01_FIFO_STATUS    0x17
#define nrf24l01_DYNPD          0x1C
#define nrf24l01_FEATURE        0x1D


#define nrf24l01_CONFIG_DEFAULT_VAL         0x08
#define nrf24l01_EN_AA_DEFAULT_VAL          0x3F
#define nrf24l01_EN_RXADDR_DEFAULT_VAL      0x03
#define nrf24l01_SETUP_AW_DEFAULT_VAL       0x03
#define nrf24l01_SETUP_RETR_DEFAULT_VAL     0x03
#define nrf24l01_RF_CH_DEFAULT_VAL          0x02
#define nrf24l01_RF_SETUP_DEFAULT_VAL       0x0F
#define nrf24l01_STATUS_DEFAULT_VAL         0x0E
#define nrf24l01_OBSERVE_TX_DEFAULT_VAL     0x00
#define nrf24l01_CD_DEFAULT_VAL             0x00
#define nrf24l01_RX_ADDR_P0_B0_DEFAULT_VAL  0xE7
#define nrf24l01_RX_ADDR_P0_B1_DEFAULT_VAL  0xE7
#define nrf24l01_RX_ADDR_P0_B2_DEFAULT_VAL  0xE7
#define nrf24l01_RX_ADDR_P0_B3_DEFAULT_VAL  0xE7
#define nrf24l01_RX_ADDR_P0_B4_DEFAULT_VAL  0xE7
#define nrf24l01_RX_ADDR_P1_B0_DEFAULT_VAL  0xC2
#define nrf24l01_RX_ADDR_P1_B1_DEFAULT_VAL  0xC2
#define nrf24l01_RX_ADDR_P1_B2_DEFAULT_VAL  0xC2
#define nrf24l01_RX_ADDR_P1_B3_DEFAULT_VAL  0xC2
#define nrf24l01_RX_ADDR_P1_B4_DEFAULT_VAL  0xC2
#define nrf24l01_RX_ADDR_P2_DEFAULT_VAL     0xC3
#define nrf24l01_RX_ADDR_P3_DEFAULT_VAL     0xC4
#define nrf24l01_RX_ADDR_P4_DEFAULT_VAL     0xC5
#define nrf24l01_RX_ADDR_P5_DEFAULT_VAL     0xC6
#define nrf24l01_TX_ADDR_B0_DEFAULT_VAL     0xE7
#define nrf24l01_TX_ADDR_B1_DEFAULT_VAL     0xE7
#define nrf24l01_TX_ADDR_B2_DEFAULT_VAL     0xE7
#define nrf24l01_TX_ADDR_B3_DEFAULT_VAL     0xE7
#define nrf24l01_TX_ADDR_B4_DEFAULT_VAL     0xE7
#define nrf24l01_RX_PW_P0_DEFAULT_VAL       0x00
#define nrf24l01_RX_PW_P1_DEFAULT_VAL       0x00
#define nrf24l01_RX_PW_P2_DEFAULT_VAL       0x00
#define nrf24l01_RX_PW_P3_DEFAULT_VAL       0x00
#define nrf24l01_RX_PW_P4_DEFAULT_VAL       0x00
#define nrf24l01_RX_PW_P5_DEFAULT_VAL       0x00
#define nrf24l01_FIFO_STATUS_DEFAULT_VAL    0x11
#define nrf24l01_DYNPD_DEFAULT_VAL          0x00
#define nrf24l01_FEATURE_DEFAULT_VAL        0x00


////CONFIG register bitwise definitions
#define nrf24l01_CONFIG_RESERVED  0x80
#define   nrf24l01_CONFIG_MASK_RX_DR  0x40
#define   nrf24l01_CONFIG_MASK_TX_DS  0x20
#define   nrf24l01_CONFIG_MASK_MAX_RT 0x10
#define   nrf24l01_CONFIG_EN_CRC      0x08
#define   nrf24l01_CONFIG_CRCO        0x04
#define   nrf24l01_CONFIG_PWR_UP      0x02
#define   nrf24l01_CONFIG_PRIM_RX     0x01
////EN_AA register bitwise definitions
#define nrf24l01_EN_AA_RESERVED       0xC0
#define nrf24l01_EN_AA_ENAA_ALL       0x3F
#define nrf24l01_EN_AA_ENAA_P5        0x20
#define nrf24l01_EN_AA_ENAA_P4        0x10
#define nrf24l01_EN_AA_ENAA_P3        0x08
#define nrf24l01_EN_AA_ENAA_P2        0x04
#define nrf24l01_EN_AA_ENAA_P1        0x02
#define nrf24l01_EN_AA_ENAA_P0        0x01
#define nrf24l01_EN_AA_ENAA_NONE  	  0x00
////EN_RXADDR register bitwise definitions
#define nrf24l01_EN_RXADDR_RESERVED   0xC0
#define nrf24l01_EN_RXADDR_ERX_ALL    0x3F
#define nrf24l01_EN_RXADDR_ERX_P5 	  0x20
#define nrf24l01_EN_RXADDR_ERX_P4 	  0x10
#define nrf24l01_EN_RXADDR_ERX_P3 	  0x08
#define nrf24l01_EN_RXADDR_ERX_P2 	  0x04
#define nrf24l01_EN_RXADDR_ERX_P1 	  0x02
#define nrf24l01_EN_RXADDR_ERX_P0 	  0x01
#define nrf24l01_EN_RXADDR_ERX_NONE   0x00
////SETUP_AW register bitwise definitions
#define nrf24l01_SETUP_AW_RESERVED    0xFC
#define nrf24l01_SETUP_AW         	  0x03
#define nrf24l01_SETUP_AW_5BYTES  	  0x03
#define nrf24l01_SETUP_AW_4BYTES  	  0x02
#define nrf24l01_SETUP_AW_3BYTES  	  0x01
#define nrf24l01_SETUP_AW_ILLEGAL 	  0x00
////SETUP_RETR register bitwise definitions
#define nrf24l01_SETUP_RETR_ARD       0xF0
#define nrf24l01_SETUP_RETR_ARD_4000  0xF0
#define nrf24l01_SETUP_RETR_ARD_3750  0xE0
#define nrf24l01_SETUP_RETR_ARD_3500  0xD0
#define nrf24l01_SETUP_RETR_ARD_3250  0xC0
#define nrf24l01_SETUP_RETR_ARD_3000  0xB0
#define nrf24l01_SETUP_RETR_ARD_2750  0xA0
#define nrf24l01_SETUP_RETR_ARD_2500  0x90
#define nrf24l01_SETUP_RETR_ARD_2250  0x80
#define nrf24l01_SETUP_RETR_ARD_2000  0x70
#define nrf24l01_SETUP_RETR_ARD_1750  0x60
#define nrf24l01_SETUP_RETR_ARD_1500  0x50
#define nrf24l01_SETUP_RETR_ARD_1250  0x40
#define nrf24l01_SETUP_RETR_ARD_1000  0x30
#define nrf24l01_SETUP_RETR_ARD_750   0x20
#define nrf24l01_SETUP_RETR_ARD_500   0x10
#define nrf24l01_SETUP_RETR_ARD_250   0x00
#define nrf24l01_SETUP_RETR_ARC       0x0F
#define nrf24l01_SETUP_RETR_ARC_15    0x0F
#define nrf24l01_SETUP_RETR_ARC_14    0x0E
#define nrf24l01_SETUP_RETR_ARC_13    0x0D
#define nrf24l01_SETUP_RETR_ARC_12    0x0C
#define nrf24l01_SETUP_RETR_ARC_11    0x0B
#define nrf24l01_SETUP_RETR_ARC_10    0x0A
#define nrf24l01_SETUP_RETR_ARC_9     0x09
#define nrf24l01_SETUP_RETR_ARC_8     0x08
#define nrf24l01_SETUP_RETR_ARC_7     0x07
#define nrf24l01_SETUP_RETR_ARC_6     0x06
#define nrf24l01_SETUP_RETR_ARC_5     0x05
#define nrf24l01_SETUP_RETR_ARC_4     0x04
#define nrf24l01_SETUP_RETR_ARC_3     0x03
#define nrf24l01_SETUP_RETR_ARC_2     0x02
#define nrf24l01_SETUP_RETR_ARC_1     0x01
#define nrf24l01_SETUP_RETR_ARC_0     0x00
////RF_CH register bitwise definitions
#define nrf24l01_RF_CH_RESERVED       0x80
////RF_SETUP register bitwise definitions
#define nrf24l01_RF_SETUP_RESERVED    0xE0
#define nrf24l01_RF_SETUP_PLL_LOCK    0x10
#define nrf24l01_RF_SETUP_RF_DR       0x08
#define nrf24l01_RF_SETUP_RF_PWR      0x06
#define nrf24l01_RF_SETUP_RF_PWR_0    0x06
#define nrf24l01_RF_SETUP_RF_PWR_6    0x04
#define nrf24l01_RF_SETUP_RF_PWR_12   0x02
#define nrf24l01_RF_SETUP_RF_PWR_18   0x00
#define nrf24l01_RF_SETUP_LNA_HCURR   0x01
////STATUS register bitwise definitions
#define nrf24l01_STATUS_RESERVED      			  0x80
#define nrf24l01_STATUS_RX_DR         			  0x40
#define nrf24l01_STATUS_TX_DS         			  0x20
#define nrf24l01_STATUS_MAX_RT            		  0x10
#define nrf24l01_STATUS_RX_P_NO         		  0x0E
#define nrf24l01_STATUS_RX_P_NO_RX_FIFO_NOT_EMPTY 0x0E
#define nrf24l01_STATUS_RX_P_NO_UNUSED    		  0x0C
#define nrf24l01_STATUS_RX_P_NO_5     			  0x0A
#define nrf24l01_STATUS_RX_P_NO_4     			  0x08
#define nrf24l01_STATUS_RX_P_NO_3     			  0x06
#define nrf24l01_STATUS_RX_P_NO_2     			  0x04
#define nrf24l01_STATUS_RX_P_NO_1     			  0x02
#define nrf24l01_STATUS_RX_P_NO_0     		  	  0x00
#define nrf24l01_STATUS_TX_FULL           		  0x01
////OBSERVE_TX register bitwise definitions
#define nrf24l01_OBSERVE_TX_PLOS_CNT  	  0xF0
#define nrf24l01_OBSERVE_TX_ARC_CNT       0x0F
////CD register bitwise definitions
#define nrf24l01_CD_RESERVED              0xFE
#define nrf24l01_CD_CD                    0x01
////RX_PW_P0 register bitwise definitions
#define nrf24l01_RX_PW_P0_RESERVED        0xC0
////RX_PW_P0 register bitwise definitions
#define nrf24l01_RX_PW_P0_RESERVED        0xC0
////RX_PW_P1 register bitwise definitions
#define nrf24l01_RX_PW_P1_RESERVED        0xC0
////RX_PW_P2 register bitwise definitions
#define nrf24l01_RX_PW_P2_RESERVED        0xC0
////RX_PW_P3 register bitwise definitions
#define nrf24l01_RX_PW_P3_RESERVED        0xC0
////RX_PW_P4 register bitwise definitions
#define nrf24l01_RX_PW_P4_RESERVED        0xC0
////RX_PW_P5 register bitwise definitions
#define nrf24l01_RX_PW_P5_RESERVED        0xC0
////FIFO_STATUS register bitwise definitions
#define nrf24l01_FIFO_STATUS_RESERVED 0x8C
#define nrf24l01_FIFO_STATUS_TX_REUSE 0x40
#define nrf24l01_FIFO_STATUS_TX_FULL  0x20
#define nrf24l01_FIFO_STATUS_TX_EMPTY 0x10
#define nrf24l01_FIFO_STATUS_RX_FULL  0x02
#define nrf24l01_FIFO_STATUS_RX_EMPTY 0x01
//// DYNPD register bitwise definitions
#define nrf24l01_DYNPD_DPL_P0           0x01
#define nrf24l01_DYNPD_DPL_P1           0x02
#define nrf24l01_DYNPD_DPL_P2           0x04
#define nrf24l01_DYNPD_DPL_P3           0x08
#define nrf24l01_DYNPD_DPL_P4           0x10
#define nrf24l01_DYNPD_DPL_P5           0x20
//// FEATURE register bitwise definitions
#define nrf24l01_FEATURE_EN_DPL         0x04
#define nrf24l01_FEATURE_EN_ACK_PAY     0x02
#define nrf24l01_FEATURE_EN_DYN_ACK     0x01

#define nrf24l01_IRQ_MASK    	0x70
#define nrf24l01_IRQ_RX_DR 		0x40
#define nrf24l01_IRQ_TX_DS  	0x20
#define nrf24l01_IRQ_MAX_RT 	0x10

#endif
