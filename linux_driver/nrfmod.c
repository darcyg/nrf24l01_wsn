#include "nrfmod.h"

// extern struct nrf_priv;

// Execute nrf24l01 commande through SPI
int nrfmod_execute_cmd(struct nrf_priv* priv, u8 cmd, u8* data, unsigned int len)
{
    int index;

    if(len>32 || len>=SPI_BUFF_SIZE) {
        printk(KERN_WARNING "len greater than 32 (%d). Drop data\n", len);
        len = 32;
    }

    // TODO : remove this line
	memset(priv->spi_ctl.rx_buff, 0, SPI_BUFF_SIZE);
    // TODO : init once
	spi_message_init(&priv->spi_ctl.msg);
	priv->spi_ctl.tx_buff[0] = cmd;
    if(data) {
        for(index=0; index<len; index++)
        {
            priv->spi_ctl.tx_buff[index+1] = data[index];
        }
    }
	priv->spi_ctl.transfer[0].tx_buf = priv->spi_ctl.tx_buff;
	priv->spi_ctl.transfer[0].rx_buf = priv->spi_ctl.rx_buff;
	priv->spi_ctl.transfer[0].len = len+1;
    // spike_ctl.transfer.cs_change = 1;

	spi_message_add_tail(&priv->spi_ctl.transfer[0], &priv->spi_ctl.msg);
	return spi_sync(priv->spi_ctl.spi_device, &priv->spi_ctl.msg);
}

int nrfmod_write_register(struct nrf_priv* priv, u8 regnumber, u8* data, unsigned int len)
{
    return nrfmod_execute_cmd(priv, (regnumber & 0x1F) | 0x20, data, len);
}

int nrfmod_read_register(struct nrf_priv* priv, u8 regnumber, u8* data, unsigned int len)
{
    return nrfmod_execute_cmd(priv, regnumber & 0x1F, data, len);
}

int nrfmod_write_setup_retr(struct nrf_priv* priv)
{
    return nrfmod_write_register(priv, nrf24l01_SETUP_RETR, &priv->nrf_ctl.setup_retr, 1);
}

int nrfmod_power_up(struct nrf_priv* priv)
{
    int status;
    priv->nrf_ctl.config |= 0x02;
    if((status = nrfmod_write_register(priv, nrf24l01_CONFIG, &priv->nrf_ctl.config, 1))) {
        return status;
    }
    // TODO : implement sleep (timer)
    // Sleep while device is in standby mode
    msleep(2);
    // At this point device is ready to operate
    return 0;
}

int nrfmod_power_down(struct nrf_priv* priv)
{
    priv->nrf_ctl.config &= 0xFD;
    return nrfmod_write_register(priv, nrf24l01_CONFIG, &priv->nrf_ctl.config, 1);
}

int nrfmod_flush_tx(struct nrf_priv* priv)
{
    return nrfmod_execute_cmd(priv, nrf24l01_FLUSH_TX, NULL, 0);
}

int nrfmod_flush_rx(struct nrf_priv* priv)
{
    return nrfmod_execute_cmd(priv, nrf24l01_FLUSH_RX, NULL, 0);
}

int nrfmod_clear_all_irq(struct nrf_priv* priv)
{
    u8 data = 0x70;
    return nrfmod_write_register(priv, nrf24l01_STATUS, &data, 1);
}

// // Useless function ???
// // TODO : implement NOP function to read status
// int nrfmod_read_status(struct nrf_priv* priv)
// {
//     int err;
//     err = nrfmod_execute_cmd(spi_ctl, 0xFF, NULL, 0);
//     if(err < 0) {
//         return err;
//     }
//     return spi_ctl->rx_buff[0];
// }
// 
// // Useless function ???
// int nrfmod_read_config(struct nrf_priv* priv)
// {
//     int err;
//     u8 config = 0;
//     err = nrfmod_read_register(spi_ctl, 0x00, &config, 1);
//     if(err < 0) {
//         return err;
//     }
//     return spi_ctl->rx_buff[1];
// }
// 
// // Useless function ???
// // int nrf24l01_irq_event_active(u8 event)
// // {
// //     int err = nrfmod_read_status(spi_ctl);
// //     if(err < 0) {
// //         return err;
// //     }
// //     return (((u8)(err)) & event);
// // }
// 
int nrfmod_set_as_tx(struct nrf_priv* priv)
{
    priv->nrf_ctl.config &= 0xFE;
    return nrfmod_write_register(priv, nrf24l01_CONFIG, &priv->nrf_ctl.config, 1);
}

int nrfmod_set_as_rx(struct nrf_priv* priv)
{
    priv->nrf_ctl.config |= 0x01;
    return nrfmod_write_register(priv, nrf24l01_CONFIG, &priv->nrf_ctl.config, 1);
}

int nrfmod_write_tx_payload(struct nrf_priv* priv, u8 *data, unsigned int len)
{
    return nrfmod_execute_cmd(priv, nrf24l01_W_TX_PAYLOAD, data, len);
}

int nrfmod_write_tx_payload_no_ack(struct nrf_priv* priv, u8 *data, unsigned int len)
{
    return nrfmod_execute_cmd(priv, nrf24l01_W_TX_PAYLOAD_NO_ACK, data, len);
}

int nrfmod_read_rx_payload_size(struct nrf_priv* priv)
{
    return nrfmod_execute_cmd(priv, nrf24l01_R_RX_PL_WID, NULL, 1);
}

int nrfmod_read_rx_payload(struct nrf_priv* priv, unsigned int len)
{
    // TODO : fix this function
    return nrfmod_execute_cmd(priv, nrf24l01_R_RX_PAYLOAD, NULL, len);
}

int nrfmod_init_device(struct nrf_priv* priv)
{
    u8 data[32];

    priv->nrf_ctl.config = // Set default RX config
        nrf24l01_CONFIG_DEFAULT_VAL | nrf24l01_CONFIG_PRIM_RX;
    nrfmod_power_down(priv);

    // Enhanced ShockBurst for pipes 0, 1 & 2
    data[0] = nrf24l01_EN_AA_ENAA_P0 | nrf24l01_EN_AA_ENAA_P1;
    nrfmod_write_register(priv, nrf24l01_EN_AA, data, 1);

    // Enable pipes 0, 1 & 2
    data[0] = nrf24l01_EN_RXADDR_ERX_P0 | nrf24l01_EN_RXADDR_ERX_P1 | nrf24l01_EN_RXADDR_ERX_P2;
    nrfmod_write_register(priv, nrf24l01_EN_RXADDR, data, 1);

    data[0] = nrf24l01_SETUP_AW_DEFAULT_VAL;
    nrfmod_write_register(priv, nrf24l01_SETUP_AW, data, 1);

    // Set short ARD to reduce timeslot size
    priv->nrf_ctl.setup_retr = nrf24l01_SETUP_RETR_ARD_250 |
              nrf24l01_SETUP_RETR_ARC_0;
    nrfmod_write_register(priv, nrf24l01_SETUP_RETR, &priv->nrf_ctl.setup_retr, 1);

    data[0] = nrf24l01_RF_CH_DEFAULT_VAL;
    nrfmod_write_register(priv, nrf24l01_RF_CH, data, 1);

    data[0] = nrf24l01_RF_SETUP_DEFAULT_VAL;
    nrfmod_write_register(priv, nrf24l01_RF_SETUP, data, 1);

    data[0] = nrf24l01_RX_ADDR_P0_B0_DEFAULT_VAL;
    data[1] = nrf24l01_RX_ADDR_P0_B1_DEFAULT_VAL;
    data[2] = nrf24l01_RX_ADDR_P0_B2_DEFAULT_VAL;
    data[3] = nrf24l01_RX_ADDR_P0_B3_DEFAULT_VAL;
    data[4] = nrf24l01_RX_ADDR_P0_B4_DEFAULT_VAL;
    memcpy(priv->nrf_ctl.rx_addr_p0, data, 5);
    nrfmod_write_register(priv, nrf24l01_RX_ADDR_P0, data, 5);

    // Set device mac address
    data[0] = nrf24l01_RX_ADDR_P1_B0_DEFAULT_VAL;
    data[1] = nrf24l01_RX_ADDR_P1_B1_DEFAULT_VAL;
    data[2] = nrf24l01_RX_ADDR_P1_B2_DEFAULT_VAL;
    data[3] = 0x12;
    data[4] = nrf24l01_RX_ADDR_P1_B4_DEFAULT_VAL;
    memcpy(priv->nrf_ctl.rx_addr_p1, data, 5);
    nrfmod_write_register(priv, nrf24l01_RX_ADDR_P1, data, 5);

    // Set broadcast address
    data[0] = 0xff;
    memcpy(priv->nrf_ctl.rx_addr_p2, data, 5);
    nrfmod_write_register(priv, nrf24l01_RX_ADDR_P2, data, 1);

    data[0] = nrf24l01_TX_ADDR_B0_DEFAULT_VAL;
    data[1] = nrf24l01_TX_ADDR_B1_DEFAULT_VAL;
    data[2] = nrf24l01_TX_ADDR_B2_DEFAULT_VAL;
    data[3] = nrf24l01_TX_ADDR_B3_DEFAULT_VAL;
    data[4] = nrf24l01_TX_ADDR_B4_DEFAULT_VAL;
    memcpy(priv->nrf_ctl.tx_addr, data, 5);
    nrfmod_write_register(priv, nrf24l01_TX_ADDR, data, 5);

    data[0] = nrf24l01_RX_PW_P0_DEFAULT_VAL;
    nrfmod_write_register(priv, nrf24l01_RX_PW_P0, data, 1);

    data[0] = nrf24l01_RX_PW_P1_DEFAULT_VAL;
    nrfmod_write_register(priv, nrf24l01_RX_PW_P1, data, 1);

    data[0] = nrf24l01_RX_PW_P2_DEFAULT_VAL;
    nrfmod_write_register(priv, nrf24l01_RX_PW_P2, data, 1);

    data[0] = nrf24l01_RX_PW_P3_DEFAULT_VAL;
    nrfmod_write_register(priv, nrf24l01_RX_PW_P3, data, 1);

    data[0] = nrf24l01_RX_PW_P4_DEFAULT_VAL;
    nrfmod_write_register(priv, nrf24l01_RX_PW_P4, data, 1);

    data[0] = nrf24l01_RX_PW_P5_DEFAULT_VAL;
    nrfmod_write_register(priv, nrf24l01_RX_PW_P5, data, 1);

    data[0] = nrf24l01_FEATURE_EN_DPL | nrf24l01_FEATURE_EN_DYN_ACK;
    nrfmod_write_register(priv, nrf24l01_FEATURE, data, 1);

    data[0] = nrf24l01_DYNPD_DPL_P0 | nrf24l01_DYNPD_DPL_P1 | nrf24l01_DYNPD_DPL_P2;
    nrfmod_write_register(priv, nrf24l01_DYNPD, data, 1);

    // printk(KERN_NOTICE "nrf : init ok !\n");
    return 0;
}
