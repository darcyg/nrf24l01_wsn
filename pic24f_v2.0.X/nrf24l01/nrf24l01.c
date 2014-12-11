// File: nrf24l01.c

#include "../HardwareProfile.h"
#include "nrf24l01.h"
#include "../delay.h"
#include "../spi2.h"

u8_t nrf_config;
u8_t nrf_status;
u8_t nrf_rf_ch;

void nrf24l01_init()
{
#ifdef NRF24L01_PIN_CE
    #if NRF24L01_PIN_CE == 100
        #warning NRF24L01_PIN_CE on RA0
        #ifdef PIN_IO_100
            #error PIN_IO_100 already used
        #else
            #define PIN_IO_100 1
            #define nrf24l01_CE_CONFREGISTER            TRISA
            #define nrf24l01_CE_IOREGISTER		LATA
            #define nrf24l01_CE_PINMASK                 0x01 // RA0
        #endif
    #elif NRF24L01_PIN_CE == 101
        #warning NRF24L01_PIN_CE on RA1
        #ifdef PIN_IO_101
            #error PIN_IO_101 already used
        #else
            #define PIN_IO_101 1
            #define nrf24l01_CE_CONFREGISTER            TRISA
            #define nrf24l01_CE_IOREGISTER		LATA
            #define nrf24l01_CE_PINMASK                 0x02 // RA1
        #endif
    #elif NRF24L01_PIN_CE == 5
        #warning NRF24L01_PIN_CE on RB5
        #ifdef PIN_IO_5
            #error PIN_IO_5 already used
        #else
            #define PIN_IO_5 1
            #define nrf24l01_CE_CONFREGISTER            TRISB
            #define nrf24l01_CE_IOREGISTER		LATB
            #define nrf24l01_CE_PINMASK                 0x20 // RB5
        #endif
    #elif NRF24L01_PIN_CE == 14
        #warning NRF24L01_PIN_CE on RB14
        #ifdef PIN_IO_14
            #error PIN_IO_14 already used
        #else
            #define PIN_IO_14 1
            #define nrf24l01_CE_CONFREGISTER            TRISB
            #define nrf24l01_CE_IOREGISTER		LATB
            #define nrf24l01_CE_PINMASK                 0x4000 // RB14
        #endif
    #else
        #error NRF24L01 config <NRF24L01_PIN_CE> out of range
    #endif
#else
    #error NRF24L01 config <NRF24L01_PIN_CE> not defined
#endif

#ifdef NRF24L01_PIN_CSN
    #if NRF24L01_PIN_CSN == 100
        #warning NRF24L01_PIN_CSN on RA0
        #ifdef PIN_IO_100
            #error PIN_IO_100 already used
        #else
            #define PIN_IO_100 1
            #define nrf24l01_CSN_CONFREGISTER            TRISA
            #define nrf24l01_CSN_IOREGISTER		 LATA
            #define nrf24l01_CSN_PINMASK                 0x01 // RA0
        #endif
    #elif NRF24L01_PIN_CSN == 101
        #warning NRF24L01_PIN_CSN on RA1
        #ifdef PIN_IO_101
            #error PIN_IO_101 already used
        #else
            #define PIN_IO_101 1
            #define nrf24l01_CSN_CONFREGISTER            TRISA
            #define nrf24l01_CSN_IOREGISTER		 LATA
            #define nrf24l01_CSN_PINMASK                 0x02 // RA1
        #endif
    #elif NRF24L01_PIN_CSN == 4
        #warning NRF24L01_PIN_CSN on RB4
        #ifdef PIN_IO_4
            #error PIN_IO_4 already used
        #else
            #define PIN_IO_4 1
            #define nrf24l01_CSN_CONFREGISTER            TRISB
            #define nrf24l01_CSN_IOREGISTER              LATB
            #define nrf24l01_CSN_PINMASK                 0x10 // RB4
        #endif
    #elif NRF24L01_PIN_CSN == 5
        #warning NRF24L01_PIN_CSN on RB5
        #ifdef PIN_IO_5
            #error PIN_IO_5 already used
        #else
            #define PIN_IO_5 1
            #define nrf24l01_CSN_CONFREGISTER            TRISB
            #define nrf24l01_CSN_IOREGISTER              LATB
            #define nrf24l01_CSN_PINMASK                 0x20 // RB5
        #endif
    #else
        #error NRF24L01 config <NRF24L01_PIN_CSN> out of range
    #endif
#else
    #error NRF24L01 config <NRF24L01_PIN_CSN> not defined
#endif

#ifdef NRF24L01_PIN_IRQ
    #if NRF24L01_PIN_IRQ == 4
        #warning NRF24L01_PIN_IRQ on RB4
        #ifdef PIN_IO_4
            #error PIN_IO_4 already used
        #else
            #define PIN_IO_4 1
            #define nrf24l01_IRQ_CONFREGISTER   TRISB
            #define nrf24l01_IRQ_IOREGISTER	PORTB
            #define nrf24l01_IRQ_PINMASK	0x10 // RB4

            #ifdef NRF24L01_INTERRUPT_MODE
                #warning NRF24L01_PIN_IRQ use INTERRUPT MODE
                RPINR0bits.INT1R = 4;
                INTCON2bits.INT1EP = 0;
            #else
                #warning NRF24L01_PIN_IRQ use POLLING MODE
                set_input(nrf24l01_IRQ_CONFREGISTER, nrf24l01_IRQ_PINMASK);
            #endif
        #endif
    #elif NRF24L01_PIN_IRQ == 5
        #warning NRF24L01_PIN_IRQ on RB5
        #ifdef PIN_IO_5
            #error PIN_IO_5 already used
        #else
            #define PIN_IO_5 1
            #define nrf24l01_IRQ_CONFREGISTER   TRISB
            #define nrf24l01_IRQ_IOREGISTER	PORTB
            #define nrf24l01_IRQ_PINMASK	0x20 // RB5

            #ifdef NRF24L01_INTERRUPT_MODE
                #error INTERRUPT MODE not available on RB5
            #else
                #warning NRF24L01_PIN_IRQ use POLLING MODE
                set_input(nrf24l01_IRQ_CONFREGISTER, nrf24l01_IRQ_PINMASK);
            #endif
        #endif
    #elif NRF24L01_PIN_IRQ == 100
        #warning NRF24L01_PIN_IRQ on RA0
        #ifdef PIN_IO_100
            #error PIN_IO_100 already used
        #else
            #define PIN_IO_100 1
            #define nrf24l01_IRQ_CONFREGISTER   TRISA
            #define nrf24l01_IRQ_IOREGISTER	PORTA
            #define nrf24l01_IRQ_PINMASK	0x01 // RA0

            #ifdef NRF24L01_INTERRUPT_MODE
                #warning NRF24L01_PIN_IRQ use INTERRUPT MODE
                RPINR0bits.INT1R = 5;
                INTCON2bits.INT1EP = 1;
            #else
                #warning NRF24L01_PIN_IRQ use POLLING MODE
                set_input(nrf24l01_IRQ_CONFREGISTER, nrf24l01_IRQ_PINMASK);
            #endif
        #endif
    #elif NRF24L01_PIN_IRQ == 101
        #warning NRF24L01_PIN_IRQ on RA1
        #ifdef PIN_IO_101
            #error PIN_IO_101 already used
        #else
            #define PIN_IO_101 1
            #define nrf24l01_IRQ_CONFREGISTER   TRISA
            #define nrf24l01_IRQ_IOREGISTER	PORTA
            #define nrf24l01_IRQ_PINMASK	0x02 // RA1

            #ifdef NRF24L01_INTERRUPT_MODE
                #warning NRF24L01_PIN_IRQ use INTERRUPT MODE
                RPINR0bits.INT1R = 6;
                INTCON2bits.INT1EP = 1;
            #else
                #warning NRF24L01_PIN_IRQ use POLLING MODE
                set_input(nrf24l01_IRQ_CONFREGISTER, nrf24l01_IRQ_PINMASK);
            #endif
        #endif
    #else
        #error NRF24L01 config <NRF24L01_PIN_IRQ> out of range
    #endif
#else
    #error NRF24L01 config <NRF24L01_PIN_CSN> not defined
#endif

    set_output(nrf24l01_CE_CONFREGISTER,  nrf24l01_CE_PINMASK);
    set_output(nrf24l01_CSN_CONFREGISTER, nrf24l01_CSN_PINMASK);

    nrf24l01_set_csn();
    nrf24l01_clear_ce();
}

#ifdef NRF24L01_INTERRUPT_MODE
volatile unsigned short nrf_irq = 0;

void __attribute__((__interrupt__, auto_psv)) _INT1Interrupt(void)
{
    // Clear the INT1 interrupt flag
    IFS1bits.INT1IF = 0;
    // Send signal to main function
    nrf_irq = 1;
}
#endif

void nrf24l01_initialize(unsigned char config,
						 unsigned char en_aa,
						 unsigned char en_rxaddr,
						 unsigned char setup_aw,
						 unsigned char setup_retr,
						 unsigned char rf_ch,
						 unsigned char rf_setup,
						 unsigned char * rx_addr_p0,
						 unsigned char * rx_addr_p1,
						 unsigned char rx_addr_p2,
						 unsigned char rx_addr_p3,
						 unsigned char rx_addr_p4,
						 unsigned char rx_addr_p5,
						 unsigned char * tx_addr,
						 unsigned char rx_pw_p0,
						 unsigned char rx_pw_p1,
						 unsigned char rx_pw_p2,
						 unsigned char rx_pw_p3,
						 unsigned char rx_pw_p4,
						 unsigned char rx_pw_p5,
                         unsigned char dynpd,
                         unsigned char feature)
{
	unsigned char data[5];

	data[0] = en_aa;
	nrf24l01_write_register(nrf24l01_EN_AA, data, 1);

	data[0] = en_rxaddr;
	nrf24l01_write_register(nrf24l01_EN_RXADDR, data, 1);

	data[0] = setup_aw;
	nrf24l01_write_register(nrf24l01_SETUP_AW, data, 1);

	data[0] = setup_retr;
	nrf24l01_write_register(nrf24l01_SETUP_RETR, data, 1);

    nrf_rf_ch = rf_ch;
	nrf24l01_write_register(nrf24l01_RF_CH, &nrf_rf_ch, 1);

	data[0] = rf_setup;
	nrf24l01_write_register(nrf24l01_RF_SETUP, data, 1);

	if(rx_addr_p0 != NULL)
        nrf24l01_write_register(nrf24l01_RX_ADDR_P0, rx_addr_p0, 5);
	else
	{
		data[0] = nrf24l01_RX_ADDR_P0_B0_DEFAULT_VAL;
		data[1] = nrf24l01_RX_ADDR_P0_B1_DEFAULT_VAL;
		data[2] = nrf24l01_RX_ADDR_P0_B2_DEFAULT_VAL;
		data[3] = nrf24l01_RX_ADDR_P0_B3_DEFAULT_VAL;
		data[4] = nrf24l01_RX_ADDR_P0_B4_DEFAULT_VAL;

        nrf24l01_write_register(nrf24l01_RX_ADDR_P0, data, 5);
	}

	if(rx_addr_p1 != NULL)
        nrf24l01_write_register(nrf24l01_RX_ADDR_P1, rx_addr_p1, 5);
	else
	{
		data[0] = nrf24l01_RX_ADDR_P1_B0_DEFAULT_VAL;
		data[1] = nrf24l01_RX_ADDR_P1_B1_DEFAULT_VAL;
		data[2] = nrf24l01_RX_ADDR_P1_B2_DEFAULT_VAL;
		data[3] = nrf24l01_RX_ADDR_P1_B3_DEFAULT_VAL;
		data[4] = nrf24l01_RX_ADDR_P1_B4_DEFAULT_VAL;

        nrf24l01_write_register(nrf24l01_RX_ADDR_P1, data, 5);
	}

	data[0] = rx_addr_p2;
    nrf24l01_write_register(nrf24l01_RX_ADDR_P2, data, 1);

	data[0] = rx_addr_p3;
    nrf24l01_write_register(nrf24l01_RX_ADDR_P3, data, 1);

	data[0] = rx_addr_p4;
    nrf24l01_write_register(nrf24l01_RX_ADDR_P4, data, 1);

	data[0] = rx_addr_p5;
    nrf24l01_write_register(nrf24l01_RX_ADDR_P5, data, 1);

	if(tx_addr != NULL)
        nrf24l01_write_register(nrf24l01_TX_ADDR, tx_addr, 5);
	else
	{
		data[0] = nrf24l01_TX_ADDR_B0_DEFAULT_VAL;
		data[1] = nrf24l01_TX_ADDR_B1_DEFAULT_VAL;
		data[2] = nrf24l01_TX_ADDR_B2_DEFAULT_VAL;
		data[3] = nrf24l01_TX_ADDR_B3_DEFAULT_VAL;
		data[4] = nrf24l01_TX_ADDR_B4_DEFAULT_VAL;

        nrf24l01_write_register(nrf24l01_TX_ADDR, data, 5);
	}

	data[0] = rx_pw_p0;
	nrf24l01_write_register(nrf24l01_RX_PW_P0, data, 1);

	data[0] = rx_pw_p1;
	nrf24l01_write_register(nrf24l01_RX_PW_P1, data, 1);

	data[0] = rx_pw_p2;
	nrf24l01_write_register(nrf24l01_RX_PW_P2, data, 1);

	data[0] = rx_pw_p3;
	nrf24l01_write_register(nrf24l01_RX_PW_P3, data, 1);

	data[0] = rx_pw_p4;
	nrf24l01_write_register(nrf24l01_RX_PW_P4, data, 1);

	data[0] = rx_pw_p5;
	nrf24l01_write_register(nrf24l01_RX_PW_P5, data, 1);

    data[0] = dynpd;
	nrf24l01_write_register(nrf24l01_DYNPD, data, 1);

    data[0] = feature;
	nrf24l01_write_register(nrf24l01_FEATURE, data, 1);

    nrf_config = config;
	nrf24l01_write_register(nrf24l01_CONFIG, &nrf_config, 1);

#ifdef NRF24L01_INTERRUPT_MODE
        // configure interrupt
        IFS1bits.INT1IF = 0;
        IPC5bits.INT1IP = 1; // lower priority
        IEC1bits.INT1IE = 1;
#endif
}

void nrf24l01_power_up()
{
	nrf_config |= nrf24l01_CONFIG_PWR_UP;
	nrf24l01_write_register(nrf24l01_CONFIG, &nrf_config, 1);
	delay_us(1500);
}

void nrf24l01_power_down()
{
	nrf_config &= (~nrf24l01_CONFIG_PWR_UP);
	nrf24l01_write_register(nrf24l01_CONFIG, &nrf_config, 1);
}

void nrf24l01_set_as_rx()
{
    nrf_config = nrf24l01_CONFIG_DEFAULT_VAL | nrf24l01_CONFIG_PWR_UP | nrf24l01_CONFIG_PRIM_RX;
	// nrf_config |= nrf24l01_CONFIG_PRIM_RX;
	nrf24l01_write_register(nrf24l01_CONFIG, &nrf_config, 1);
}

void nrf24l01_set_as_tx()
{
	// nrf_config &= (~nrf24l01_CONFIG_PRIM_RX);
    nrf_config = nrf24l01_CONFIG_DEFAULT_VAL | nrf24l01_CONFIG_PWR_UP;
	nrf24l01_write_register(nrf24l01_CONFIG, &nrf_config, 1);

}

inline u8_t nrf24l01_write_tx_payload(u8_t* data, unsigned int len)
{
	return nrf24l01_execute_command(nrf24l01_W_TX_PAYLOAD, data, len, 0);
}

inline u8_t nrf24l01_write_tx_payload_no_ack(u8_t* data, unsigned int len)
{
	return nrf24l01_execute_command(nrf24l01_W_TX_PAYLOAD_NO_ACK, data, len, 0);
}

inline u8_t nrf24l01_read_rx_payload(u8_t* data, unsigned int len)
{
	return nrf24l01_execute_command(nrf24l01_R_RX_PAYLOAD, data, len, 1);
}

inline u8_t nrf24l01_flush_tx()
{
	return nrf24l01_execute_command(nrf24l01_FLUSH_TX, NULL, 0, 1);
}

inline u8_t nrf24l01_flush_rx()
{
	return nrf24l01_execute_command(nrf24l01_FLUSH_RX, NULL, 0, 1);
}

inline u8_t nrf24l01_reuse_tx_pl()
{
	return nrf24l01_execute_command(nrf24l01_REUSE_TX_PL, NULL, 0, 1);
}

inline u8_t nrf24l01_nop()
{
	return nrf24l01_execute_command(nrf24l01_NOP, NULL, 0, 1);
}

inline u8_t nrf24l01_r_rx_pl_wid(u8_t* data)
{
        return nrf24l01_execute_command(nrf24l01_R_RX_PL_WTD, data, 1, 1);
}

//transmits the current tx payload
void nrf24l01_transmit()
{
	nrf24l01_set_ce();
	delay_us(20);
	nrf24l01_clear_ce();
}

inline void nrf24l01_clear_ce()
{
	nrf24l01_CE_IOREGISTER &= ~nrf24l01_CE_PINMASK;
}

inline void nrf24l01_set_ce()
{
	nrf24l01_CE_IOREGISTER |= nrf24l01_CE_PINMASK;
}

//unsigned char channel is the channel to be changed to.
u8_t nrf24l01_set_rf_ch(u8_t channel)
{
	nrf_rf_ch = channel & ~nrf24l01_RF_CH_RESERVED;
	return nrf24l01_write_register(nrf24l01_RF_CH, &nrf_rf_ch, 1);
}

//returns the value of the OBSERVE_TX register
inline u8_t nrf24l01_get_observe_tx()
{
	u8_t data;
	nrf24l01_read_register(nrf24l01_OBSERVE_TX, &data, 1);
	return data;
}

//returns the current PLOS_CNT value in OBSERVE_TX register
inline u8_t nrf24l01_get_plos_cnt()
{
	u8_t data;
	nrf24l01_read_register(nrf24l01_OBSERVE_TX, &data, 1);
	return ((data & nrf24l01_OBSERVE_TX_PLOS_CNT) >> 4);
}

//clears the PLOS_CNT field of the OBSERVE_TX register
inline u8_t nrf24l01_clear_plos_cnt()
{
	return nrf24l01_write_register(nrf24l01_RF_CH, &nrf_rf_ch, 1);
}

//return the value of the status register
inline u8_t nrf24l01_get_status()
{
	return nrf24l01_nop();
}

inline u8_t nrf24l01_get_rx_addr_2()
{
	u8_t data;
	nrf24l01_read_register(nrf24l01_RX_ADDR_P2, &data, 1);
	return data;
}

inline u8_t nrf24l01_get_fifo_status()
{
	u8_t data;
	nrf24l01_read_register(nrf24l01_FIFO_STATUS, &data, 1);
	return data;
}

u8_t nrf24l01_get_config()
{
	u8_t data;
	nrf24l01_read_register(nrf24l01_CONFIG, &data, 1);
	return data;
}

int nrf24l01_irq_pin_active()
{
	return !(nrf24l01_IRQ_IOREGISTER & nrf24l01_IRQ_PINMASK);
}

u8_t nrf24l01_irq_clear_all()
{
	u8_t data = nrf24l01_STATUS_RX_DR | nrf24l01_STATUS_TX_DS | nrf24l01_STATUS_MAX_RT;
	return nrf24l01_write_register(nrf24l01_STATUS, &data, 1);
}

inline u8_t nrf24l01_execute_command(u8_t cmd, u8_t* data, unsigned int len, u8_t copydata)
{
	nrf24l01_clear_csn();

	nrf_status = cmd;
	nrf24l01_spi_send_read(&nrf_status, 1, true);
	nrf24l01_spi_send_read(data, len, copydata);

	nrf24l01_set_csn();

	return nrf_status;
}

inline u8_t nrf24l01_write_register(u8_t reg, u8_t* data, unsigned int len)
{
	return nrf24l01_execute_command(nrf24l01_W_REGISTER | (reg & nrf24l01_W_REGISTER_DATA), data, len, 0);
}

inline u8_t nrf24l01_read_register(u8_t reg, u8_t* data, unsigned int len)
{
	return nrf24l01_execute_command(reg & nrf24l01_R_REGISTER_DATA, data, len, 1);
}

//sets the pin on the host microcontroller that is attached to the 24l01's CSN pin
void nrf24l01_clear_csn()
{
	nrf24l01_CSN_IOREGISTER &= ~nrf24l01_CSN_PINMASK;
}

//clears the pin on the host microcontroller that is attached to the 24l01's CSN pin
void nrf24l01_set_csn()
{
	nrf24l01_CSN_IOREGISTER |= nrf24l01_CSN_PINMASK;
}

inline void nrf24l01_spi_send_read(u8_t* data, unsigned int len, u8_t copydata)
{
	unsigned int count;

    if(copydata) {
        for(count = 0; count < len; count++) {
            data[count] = spi_send_read_byte(data[count]);
        }
    }
    else {
        for(count = 0; count < len; count++) {
            spi_send_read_byte(data[count]);
        }
    }
}
