#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

#include <linux/sched.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/interrupt.h> /* mark_bh */

#include <linux/in.h>
#include <linux/netdevice.h>   /* struct device, and other headers */
#include <linux/etherdevice.h> /* eth_type_trans */
#include <linux/ip.h>          /* struct iphdr */
#include <linux/tcp.h>         /* struct tcphdr */
#include <linux/skbuff.h>

#include <linux/kthread.h>

// nrf hardware
#include "nrf24l01.h"
// #include <linux/gpio.h>
// #include <linux/spi/spi.h>
#include <linux/delay.h> /* msleep */

#include <linux/circ_buf.h> /* tx buffer for skb */
#include <linux/workqueue.h> /* tx wq */

#include <linux/in6.h>
#include <asm/checksum.h>

MODULE_AUTHOR("Simon Piriou <spiriou31@gmail.com>");
MODULE_DESCRIPTION("nrf24l01 module");
MODULE_VERSION("0.2");
MODULE_LICENSE("Dual BSD/GPL");

// SPI parameters
// TODO : set to -1
int mp_nrf_spibus = 0;
int mp_nrf_spibus_cs = 0;
int mp_nrf_spibus_speed = 1000000;

// GPIO parameters
// TODO : set to -1
// TODO : look at linux device tree
int mp_nrf_gpio_irq = 25;
int mp_nrf_gpio_ce = 24;

int mp_nrf_timeout = NRF_TIMEOUT;

module_param(mp_nrf_spibus, int, 0);
MODULE_PARM_DESC(mp_nrf_spibus, "spi bus number for nrf24l01+ device");
module_param(mp_nrf_spibus_cs, int, 0);
MODULE_PARM_DESC(mp_nrf_spibus_cs, "spi chip select for nrf24l01+ device");
module_param(mp_nrf_spibus_speed, int, 0);
MODULE_PARM_DESC(mp_nrf_spibus_speed, "spi max speed for nrf24l01+ device");

module_param(mp_nrf_gpio_ce, int, 0);
MODULE_PARM_DESC(mp_nrf_gpio_ce, "chip enable gpio for nrf24l01+ device");
module_param(mp_nrf_gpio_irq, int, 0);
MODULE_PARM_DESC(mp_nrf_gpio_irq, "interrupt gpio for nrf24l01+ device");

module_param(mp_nrf_timeout, int, 0);
MODULE_PARM_DESC(mp_nrf_timeout, "timeout (in ms) for nrf24l01+ device");

const char this_driver_name[] = "nrf24l01";

// Keep trace of devices for interrupt handler
#define NRF_DEVICE_COUNT 16
struct nrf_priv* nrf_devices[NRF_DEVICE_COUNT];
static DEFINE_SPINLOCK(nrf_devices_lock);

inline static int nrf_do_tx_async(struct nrf_priv* priv);
inline static int nrf_do_irq_async(struct nrf_priv* priv);

int nrf_register_netdev(struct net_device *ndev)
{
    int i, err;
	struct nrf_priv *priv = netdev_priv(ndev);
    // Protect for smp
    spin_lock(&nrf_devices_lock);
    for(i=0; i<NRF_DEVICE_COUNT; i++) {
        if(nrf_devices[i] == NULL) {
            nrf_devices[i] = priv;
            spin_unlock(&nrf_devices_lock);

            priv->dev_index = i;

            if ((err = register_netdev(ndev))) {
                printk("nrf: error %i registering device \"%s\"\n",
                        err, ndev->name);
                priv->dev_index = -1;
                spin_lock(&nrf_devices_lock);
                nrf_devices[i] = NULL;
                spin_unlock(&nrf_devices_lock);
                return err;
            }
            else {
                return 0;
            }
        }
    }
    spin_unlock(&nrf_devices_lock);
    return -ENOMEM;
}

void nrf_unregister_netdev(struct net_device *ndev)
{
	struct nrf_priv *priv = netdev_priv(ndev);

    unregister_netdev(ndev);

    // Protect for smp
    spin_lock(&nrf_devices_lock);
    nrf_devices[priv->dev_index] = NULL;
    spin_unlock(&nrf_devices_lock);
}

void restart_txtimer(struct nrf_priv* priv)
{
    hrtimer_start(&priv->timer_no_tx, ktime_set(priv->wdg_sec, priv->wdg_nsec), HRTIMER_MODE_REL);
}

void restart_wdgtimer(struct nrf_priv* priv)
{
    priv->is_timeout = 0;
    hrtimer_start(&priv->timer_wdg, ktime_set(priv->wdg_sec, priv->wdg_nsec), HRTIMER_MODE_REL);
}

void stop_wdgtimer(struct nrf_priv* priv)
{
    priv->is_timeout = 0;
    hrtimer_cancel(&priv->timer_wdg);
}

static void spi_irq_completion(void *context_data)
{	
    struct nrf_priv* priv = context_data;
    spin_lock(&priv->process_lock);

    if(priv->state == TX_WAIT) {
        gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_HIGH);
        priv->state = IDLE;
    }
    up(&priv->process_sem);

    spin_unlock(&priv->process_lock);
}

static void spi_tx_completion(void *context_data)
{	
    unsigned long flags;
    int err;
    struct nrf_priv* priv = context_data;

    spin_lock_irqsave(&priv->process_lock, flags);
    if(priv->is_irq) {
        printk("TODO : this case should never happend (%02x)\n", priv->spi_ctl.rx_buff[0]);
        // Something to process now, maybe RTS from someone else
        // Clear TX FIFO (remove previous RTS that is not sent)
        spi_message_init(&priv->spi_ctl.msg);
        priv->spi_ctl.tx_buff[0] = nrf24l01_FLUSH_TX;
        priv->spi_ctl.transfer[0].tx_buf = priv->spi_ctl.tx_buff;
        priv->spi_ctl.transfer[0].rx_buf = priv->spi_ctl.rx_buff;
        priv->spi_ctl.transfer[0].len = 1;
        spi_message_add_tail(&priv->spi_ctl.transfer[0], &priv->spi_ctl.msg);

        // Do async spi transfert to clear irq flag
        priv->spi_ctl.tx_buff[1] = (nrf24l01_STATUS & 0x1F) | 0x20;
        priv->spi_ctl.tx_buff[2] = 0x70;
        priv->spi_ctl.transfer[1].tx_buf = &priv->spi_ctl.tx_buff[1];
        priv->spi_ctl.transfer[1].rx_buf = &priv->spi_ctl.rx_buff[1];
        priv->spi_ctl.transfer[1].len = 2;
        spi_message_add_tail(&priv->spi_ctl.transfer[1], &priv->spi_ctl.msg);

        priv->spi_ctl.msg.complete = spi_irq_completion;
        priv->spi_ctl.msg.context = priv;

        if((err = spi_async(priv->spi_ctl.spi_device, &priv->spi_ctl.msg))) {
            // TODO :handle this case
            printk("Warning : spi_async failed TX_RTS %d\n", err);
        }
        priv->state = IDLE_NO_TX;
        restart_txtimer(priv);
    }
    else {
        // Transmit RTS
        gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_HIGH);
        // TX_DS will be detected using irq
        priv->is_processing = 0;
        // start hrtimer (watchdog for TX_DS)
        restart_wdgtimer(priv);
    }

    spin_unlock_irqrestore(&priv->process_lock, flags);
    // Now wait for TX_DS to send first pkg with ACK
}

inline static int nrf_do_irq_async(struct nrf_priv* priv)
{
    // Do async spi transfert to clear irq flag
    spi_message_init(&priv->spi_ctl.msg);
    priv->spi_ctl.tx_buff[0] = (nrf24l01_STATUS & 0x1F) | 0x20;
    priv->spi_ctl.tx_buff[1] = 0x70;

    priv->spi_ctl.transfer[0].tx_buf = priv->spi_ctl.tx_buff;
    priv->spi_ctl.transfer[0].rx_buf = priv->spi_ctl.rx_buff;
    priv->spi_ctl.transfer[0].len = 2;

    priv->spi_ctl.msg.complete = spi_irq_completion;
    priv->spi_ctl.msg.context = priv;

    spi_message_add_tail(&priv->spi_ctl.transfer[0], &priv->spi_ctl.msg);
    return spi_async(priv->spi_ctl.spi_device, &priv->spi_ctl.msg);
}

static irqreturn_t nrf_irq_handler(int irq, void *context_data)
{
    int err;
    struct nrf_priv* priv = context_data;
    // printk("irq triggered\n");

    spin_lock(&priv->process_lock);
    if(priv->is_processing) {
        priv->is_irq = 1;
    }
    else if((err = nrf_do_irq_async(priv))) {
        // TODO :handle this case
        printk("Warning : spi_async failed IRQ %d\n", err);
        priv->is_irq = 1;
        // TODO: alternative callback (timer ?)
    }
    else {
        priv->is_irq = 0;
        priv->is_processing = 1;
    }
    spin_unlock(&priv->process_lock);
    return IRQ_HANDLED;
}

static int nrf_init_gpio(struct nrf_priv *priv)
{
    int err;

    if(priv == NULL) {
        return -EINVAL;
    }

    if ((err = gpio_request(priv->nrf_ctl.gpio_irq, NRF24L01_GPIO_IRQ_DESC)) != 0) {
        printk(KERN_WARNING "Error %d with GPIO %s\n", err, NRF24L01_GPIO_IRQ_DESC);
        goto fail_gpio_irq;
    }
    if ((err = gpio_request(priv->nrf_ctl.gpio_ce, NRF24L01_GPIO_CE_DESC)) != 0) {
        printk(KERN_WARNING "Error %d with GPIO %s\n", err, NRF24L01_GPIO_CE_DESC);
        goto fail_gpio_ce;
    }
    if ((err = gpio_direction_input(priv->nrf_ctl.gpio_irq)) != 0) {
        printk(KERN_WARNING "Error %d setting %s to input\n",
               err, NRF24L01_GPIO_IRQ_DESC);
        goto fail_gpio_irq_in;
    }
    // Set CE as output and LOW
    if ((err = gpio_direction_output(priv->nrf_ctl.gpio_ce, 0)) != 0) {
        printk(KERN_WARNING "Error %d setting %s to output\n",
               err, NRF24L01_GPIO_CE_DESC);
        goto fail_gpio_ce_out;
    }


    if ( (priv->irq_handle = gpio_to_irq(priv->nrf_ctl.gpio_irq)) < 0 ) {
        err = priv->irq_handle; 
        printk(KERN_WARNING "Error %d for IRQ mapping %s\n",
                            err, NRF24L01_GPIO_IRQ_DESC);
        goto fail_gpio_to_irq;
    }
    // printk(KERN_NOTICE "nrf IRQ Mapped int %d\n", priv->irq_handle);
    if((err = request_irq(priv->irq_handle,
                (irq_handler_t ) nrf_irq_handler,
                IRQF_TRIGGER_FALLING,
                NRF24L01_GPIO_IRQ_DESC,
                priv)) != 0) {
        printk(KERN_WARNING "Error %d for IRQ Request\n", err);
        goto fail_gpio_req_irq;
    }
    return 0;

fail_gpio_req_irq:
fail_gpio_to_irq:
    gpio_direction_input(priv->nrf_ctl.gpio_ce);
fail_gpio_ce_out:
fail_gpio_irq_in:
    gpio_free(priv->nrf_ctl.gpio_ce);
fail_gpio_ce:
    gpio_free(priv->nrf_ctl.gpio_irq);
fail_gpio_irq:
    return err;
}

static void nrf_exit_gpio(struct nrf_priv *priv)
{
    if(priv == NULL) {
        return;
    }

    free_irq(priv->irq_handle, priv);
    gpio_direction_input(priv->nrf_ctl.gpio_ce);
    gpio_free(priv->nrf_ctl.gpio_irq);
    gpio_free(priv->nrf_ctl.gpio_ce);
}

static void nrf_process_tx_rts(struct nrf_priv *priv, u8 status)
{
    unsigned long flags;
    // printk("TX RTS event detected %02x\n", priv->spi_ctl.rx_buff[0]);

    if(priv->spi_ctl.rx_buff[0] & nrf24l01_IRQ_MAX_RT) {
        // MAX_RT detected
        // This should never happened before nrf in TX mode no ack when TX_RTS
        printk("ERROR RTS MAX_RT detected %02x\n", priv->spi_ctl.rx_buff[0]);
        nrfmod_flush_tx(priv);

        // if(priv->pkg_index == 1) {
        //     // First pkg sent but no ack received
        //     // Try to transmit next time slot
        nrfmod_set_as_rx(priv);
        spin_lock_irqsave(&priv->process_lock, flags);
        priv->state = IDLE_NO_TX;
        spin_unlock_irqrestore(&priv->process_lock, flags);

        stop_wdgtimer(priv);
        restart_txtimer(priv);
        // }
        // else {
            // This case should never happened
            // Ignore
        // }
    }
    else if(priv->spi_ctl.rx_buff[0] & nrf24l01_IRQ_TX_DS) {
        // TX_DS detected
        struct iphdr *iph;
        u8 addr[5];

        // nrfmod_execute_cmd(priv, nrf24l01_FIFO_STATUS & 0x1F, NULL, 1);
        // printk("TX_DS => TX %02x %02x\n", priv->spi_ctl.rx_buff[0], priv->spi_ctl.rx_buff[1]);

        // gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_LOW);
        // nrfmod_set_as_rx(priv);
        // gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_HIGH);

        // spin_lock_irqsave(&priv->process_lock, flags);
        // priv->state = IDLE_NO_TX;
        // dev_kfree_skb(priv->skb);
        // priv->skb = NULL;
        // spin_unlock_irqrestore(&priv->process_lock, flags);

        // stop_wdgtimer(priv);
        // restart_txtimer(priv);

        // MAC addr resolution
        // TODO : build header manually
        iph = ip_hdr(priv->skb); // Garanted to work ?
        memcpy(addr, priv->nrf_ctl.rx_addr_p1, 5);
        addr[0] = ((u8*)&(iph->daddr))[3];

        // printk("--- TX --- len = %d, %d msgs, dest=%d\n",
        //         priv->skb->len-14, (priv->skb->len-14+30)/31, addr[0]);

        // Start as TX
        gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_LOW);
        // Write TX addr. TODO : check if already ok
        memcpy(&priv->nrf_ctl.tx_addr, addr, 5);
        nrfmod_write_register(priv, nrf24l01_TX_ADDR,
                              priv->nrf_ctl.tx_addr, 5);
        // Write RX addr for ACK
        memcpy(&priv->nrf_ctl.rx_addr_p0, addr, 5);
        nrfmod_write_register(priv, nrf24l01_RX_ADDR_P0,
                              priv->nrf_ctl.rx_addr_p0, 5);

        // Update setup_retr if needed to disable retransmit
        if((priv->nrf_ctl.setup_retr & nrf24l01_SETUP_RETR_ARC) != nrf24l01_SETUP_RETR_ARC_2) {
            priv->nrf_ctl.setup_retr &= (~nrf24l01_SETUP_RETR_ARC) | nrf24l01_SETUP_RETR_ARC_2;
            nrfmod_write_setup_retr(priv);
        }

        // Send pkg 1
        gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_HIGH);
        *((u8*)&(priv->skb->data[13])) = ((priv->skb->len-14+30)/31) | 0x80;
        nrfmod_write_tx_payload(priv, &(priv->skb->data[13]),
                (priv->skb->len-14+1 >= 32 ? 32: priv->skb->len-14+1));

        if(priv->spi_ctl.rx_buff[0] & nrf24l01_IRQ_MASK) {
            // Clear IRQ to transmit
            printk("Problem pkg 1 : clear IRQ %02x\n", priv->spi_ctl.rx_buff[0]);
            nrfmod_clear_all_irq(priv);
        }

        priv->pkg_index = 1;

        spin_lock_irqsave(&priv->process_lock, flags);
        priv->state = TX;
        spin_unlock_irqrestore(&priv->process_lock, flags);

        restart_wdgtimer(priv);
    }
    else {
        // Unknown state : processed in other functions
        // This should never happened
        printk("ERROR RTS RX_DS detected %02x\n", priv->spi_ctl.rx_buff[0]);
        nrfmod_clear_all_irq(priv);
        spin_lock_irqsave(&priv->process_lock, flags);
        priv->state = IDLE_NO_TX;
        spin_unlock_irqrestore(&priv->process_lock, flags);

        stop_wdgtimer(priv);
        restart_txtimer(priv);
    }
}
static void nrf_process_tx(struct nrf_priv *priv, u8 status)
{
    unsigned long flags;
    // printk("TX event detected %02x\n", priv->spi_ctl.rx_buff[0]);

    if(priv->spi_ctl.rx_buff[0] & nrf24l01_IRQ_MAX_RT) {
        // MAX_RT detected
        // printk("MAX_RT detected %02x for pkg %d\n", priv->spi_ctl.rx_buff[0], priv->pkg_index);
        // if(priv->pkg_retransmit <= 3) {
        //     // Try retransmit
        //     printk("Try to retransmit %d\n", priv->retransmit);

        //     nrfmod_clear_all_irq(priv);
        //     if(priv->spi_ctl.rx_buff[0] & nrf24l01_IRQ_MASK) {
        //         // Clear IRQ to transmit
        //         printk("Problem MAX_RT : clear IRQ %02x\n", priv->spi_ctl.rx_buff[0]);
        //     }
        //     // TODO : implement CSMA/CD
        //     priv->retransmit++;
        //     // TODO : fix transmit part (same as nrf_hw_tx)
        //     gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_HIGH);
        //     udelay(10);
        //     gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_LOW);

        //     restart_wdgtimer(priv);
        // }
        // else {
            // Packet lost, ignore skb
            // priv->retransmit++;
            // printk("MAX_RT\n");
            // priv->ndev->stats.tx_dropped++;
            // nrfmod_flush_tx(priv); // Avoid ack payload
            gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_LOW);
            nrfmod_set_as_rx(priv);
            gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_HIGH);

            spin_lock_irqsave(&priv->process_lock, flags);
            priv->state = IDLE_NO_TX;
            spin_unlock_irqrestore(&priv->process_lock, flags);

            stop_wdgtimer(priv);
            restart_txtimer(priv);

            nrfmod_flush_tx(priv);
        // }
    }

    else if(priv->spi_ctl.rx_buff[0] & nrf24l01_IRQ_TX_DS) {
        // TX_DS detected
        // printk("TX_DS detected for pkg %d\n", priv->pkg_index);

        if(priv->pkg_index >= (priv->skb->len-14+30)/31) {
            // Set RX mode
            gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_LOW);
            nrfmod_set_as_rx(priv);
            gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_HIGH);

            // printk("skb sent %02x\n", priv->spi_ctl.rx_buff[0]);
            priv->ndev->stats.tx_packets++;
            priv->ndev->stats.tx_bytes += priv->skb->len-14;

            spin_lock_irqsave(&priv->process_lock, flags);
            dev_kfree_skb(priv->skb);
            priv->skb = NULL;

            // Miss one time slot to let other node transmit
            priv->state = IDLE_NO_TX;
            spin_unlock_irqrestore(&priv->process_lock, flags);

            stop_wdgtimer(priv);
            restart_txtimer(priv);
        }
        else {
            // printk("Load next message %d (%d)\n", priv->pkg_index,
            //         (priv->pkg_index+1)*31 > (priv->skb->len-14) ? (priv->skb->len-14)%31 : 31);

            // priv->hard_retransmit = 0;
            priv->skb->data[13+31*(priv->pkg_index)] = priv->pkg_index & 0x7F;
            nrfmod_write_tx_payload(priv, &(priv->skb->data[13+31*(priv->pkg_index)]),
                    (priv->pkg_index+1)*31 > (priv->skb->len-14) ? (priv->skb->len-14)%31+1 : 32);

            if(priv->spi_ctl.rx_buff[0] & nrf24l01_IRQ_MASK) {
                // Clear IRQ to transmit
                printk("Problem TX_DS : clear IRQ %02x\n", priv->spi_ctl.rx_buff[0]);
                nrfmod_clear_all_irq(priv);
            }
            // gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_HIGH);
            // // if((skb->len-5) <= 32) { // Only one package to transmit
            // udelay(10);
            // gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_LOW);

            priv->pkg_index++;
            restart_wdgtimer(priv);
        }
    }
    else {
        // Unknown state : processed in other functions
        printk("Unknown state detected (%02x)for pkg %d\n", priv->spi_ctl.rx_buff[0], priv->pkg_index);
        // restart_wdgtimer(priv);
    }
}

inline void nrf_rx_done(struct nrf_priv* priv, int len)
{
    struct sk_buff* skb;
    len += (priv->rx_pkg.count-1)*31-1;

    spin_lock(&priv->process_lock);
    stop_wdgtimer(priv);
    priv->state = IDLE;
    spin_unlock(&priv->process_lock);

    skb = netdev_alloc_skb_ip_align(priv->ndev, roundup(len+14, 2));
    if (unlikely(!skb)) {
        printk(KERN_ALERT "Cannot allocate skb struct\n");
        priv->ndev->stats.rx_dropped++;
    }
    else {
        skb_put(skb, len+14);
        memcpy(&skb->data[14], priv->rx_pkg.buff, len);
        memcpy(skb->data, priv->ndev->dev_addr, ETH_ALEN);
        memcpy(&skb->data[6], priv->ndev->dev_addr, ETH_ALEN);
        skb->data[12] = 0x08;
        skb->data[13] = 0x00;
        
        // TODO : complete header
        skb->protocol = eth_type_trans(skb, priv->ndev);
        // skb->ip_summed = CHECKSUM_UNNECESSARY; /* don't check it */
        skb->pkt_type = PACKET_HOST;
        priv->ndev->stats.rx_packets++;
        priv->ndev->stats.rx_bytes += len;

        netif_rx(skb);
        // printk("RX Send\n");
    }
    kfree(priv->rx_pkg.buff);
}

static void nrf_process_rx(struct nrf_priv *priv, u8 status)
{
    int len;

    // printk("RX event detected %02x\n", priv->spi_ctl.rx_buff[0]);

    if((priv->spi_ctl.rx_buff[0] & nrf24l01_STATUS_RX_P_NO) != 0x0e) {
        nrfmod_read_rx_payload_size(priv);

        while((priv->spi_ctl.rx_buff[0] & nrf24l01_STATUS_RX_P_NO) != 0x0e) {
            // read the packet into FIFO
            len = priv->spi_ctl.rx_buff[1];
            if(len > 32) {
                printk("Fatal error detected in RX queue (%02x %d). Flush RX\n",
                        priv->spi_ctl.rx_buff[0], len);
                stop_wdgtimer(priv);
                nrfmod_flush_rx(priv);
                nrfmod_flush_tx(priv);
                priv->state = IDLE;
                break;
            }
            nrfmod_read_rx_payload(priv, len);

            if((priv->spi_ctl.rx_buff[0] & nrf24l01_STATUS_RX_P_NO)
                    != nrf24l01_STATUS_RX_P_NO_1) {
                printk(KERN_WARNING "RX in wrong pipe %02x\n", priv->spi_ctl.rx_buff[0]);
                nrfmod_read_rx_payload_size(priv);
                continue;
            }

            if(priv->rx_pkg.cur == (priv->spi_ctl.rx_buff[1] & 0x3F)) {
                // Continue transfert
                // printk("Continue transfert %02x\n", priv->spi_ctl.rx_buff[1]);
                memcpy(&(priv->rx_pkg.buff[31*priv->rx_pkg.cur]), &priv->spi_ctl.rx_buff[2], len-1);
                priv->rx_pkg.cur++;
                if(priv->rx_pkg.cur == priv->rx_pkg.count) {
                    nrf_rx_done(priv, len);
                    nrfmod_read_register(priv, nrf24l01_STATUS, NULL, 0);
                    break;
                }
                else {
                    // Wait for next pkg
                    restart_wdgtimer(priv);
                }
            } // End good pkg
            else {
                printk("Wrong packet received %02x %02x\n", priv->rx_pkg.cur, priv->spi_ctl.rx_buff[1]);
                // Ignore
            }

            nrfmod_read_rx_payload_size(priv);
        }
    }
}

static void nrf_process_idle(struct nrf_priv *priv, u8 status)
{
    int len;
    // printk("IDLE event detected %02x\n", priv->spi_ctl.rx_buff[0]);

    if((priv->spi_ctl.rx_buff[0] & nrf24l01_STATUS_RX_P_NO) != 0x0e) {
        nrfmod_read_rx_payload_size(priv);

        while((priv->spi_ctl.rx_buff[0] & nrf24l01_STATUS_RX_P_NO) != 0x0e) {
            // read the packet into FIFO
            len = priv->spi_ctl.rx_buff[1];
            if(len > 32) {
                printk("Fatal error detected in IDLE RX queue (%02x %d). Flush RX\n",
                        priv->spi_ctl.rx_buff[0], len);

                nrfmod_flush_rx(priv);
                nrfmod_read_rx_payload_size(priv);
                continue;
            }
            nrfmod_read_rx_payload(priv, len);

            if((priv->spi_ctl.rx_buff[0] & nrf24l01_STATUS_RX_P_NO) == nrf24l01_STATUS_RX_P_NO_2) {
                // Check dest addr
                if(!memcmp(&priv->spi_ctl.rx_buff[7], priv->nrf_ctl.rx_addr_p1, 5)) {
                    // printk("RX got RTS for ME %02x\n", priv->spi_ctl.rx_buff[0]);
                }
                else {
                    printk("RX got RTS for other %02x\n", priv->spi_ctl.rx_buff[0]);
                    // TODO set IDLE_NO_TX
                }
                // Do not try to transmit, another node wants to
                priv->state = IDLE_NO_TX;
                restart_txtimer(priv);
            }
            else if((priv->spi_ctl.rx_buff[0] & nrf24l01_STATUS_RX_P_NO) != nrf24l01_STATUS_RX_P_NO_1) {
                printk(KERN_WARNING "RX in wrong pipe %02x\n", priv->spi_ctl.rx_buff[0]);
            }
            else if(priv->spi_ctl.rx_buff[1] & 0x80) {
                // printk("Start new transfert %02x\n", priv->spi_ctl.rx_buff[1]);
                priv->rx_pkg.count = priv->spi_ctl.rx_buff[1] & 0x3F;
                priv->rx_pkg.cur = 1;
                priv->rx_pkg.buff =
                    kmalloc(sizeof(unsigned char)*((priv->rx_pkg.count)*31), GFP_KERNEL);

                if(priv->rx_pkg.buff == NULL) {
                    printk("Alloc failed. Drop received message\n");
                }
                else {
                    // printk(KERN_WARNING "RX ok, can start transfert %02x, len = %d, %d msgs\n",
                    //     priv->spi_ctl.rx_buff[1], len-1, (len-1+30)/31);
                
                    memcpy(&(priv->rx_pkg.buff[0]), &priv->spi_ctl.rx_buff[2], len-1);

                    if(priv->rx_pkg.count == 1) {
                        printk("done 1 pkg\n");
                        nrf_rx_done(priv, len);
                    }
                    else {
                        // Update status
                        nrfmod_read_register(priv, nrf24l01_STATUS, NULL, 0);
                        // TODO : this is already protected by is_processing
                        spin_lock(&priv->process_lock);
                        priv->state = RX;
                        spin_unlock(&priv->process_lock);
                        restart_wdgtimer(priv);
                        break;
                    }
                }
            }
            nrfmod_read_rx_payload_size(priv);
        }
    }
}

static void nrf_process_timeout(struct nrf_priv* priv)
{
    // This timer detects when TX_DS or RX_DX are missing
    if(priv->state == TX_RTS) {
        // Did not receive TX_DS for first pkg
        printk("ERROR TX_RTS\n");
        gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_LOW);
        nrfmod_set_as_rx(priv);
        gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_HIGH);
        nrfmod_flush_tx(priv);

        priv->state = IDLE_NO_TX;
        restart_txtimer(priv);
        stop_wdgtimer(priv); // Useless ??
    }
    else if(priv->state == RX) {
        // Cancel skb reception
        // TODO : add len
        priv->ndev->stats.rx_dropped++;
        kfree(priv->rx_pkg.buff);
        printk("Timer callback detected ! Cancel RX (%d %d)\n", priv->rx_pkg.cur, priv->rx_pkg.count);
        priv->state = IDLE;
        stop_wdgtimer(priv); // Useless ??
    }
    else if(priv->state == TX) {
        // Did not receive TX_DS for pkg
        // Here we can get either TX_DS or MAX_RT
        // Noting means that driver is in a unknown state : clean it
        printk("Timer callback detected ! TX response is missing\n");
        gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_LOW);
        nrfmod_set_as_rx(priv);
        gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_HIGH);
        nrfmod_flush_tx(priv);

        priv->state = IDLE_NO_TX;
        restart_txtimer(priv);
        stop_wdgtimer(priv); // Useless ??
    }
}


static int nrf_event_handler(void* data)
{	
    unsigned long flags;
    int err;
    u8 status;

    struct nrf_priv *priv = (struct nrf_priv*)data;
	allow_signal(SIGTERM);
    
	while(!down_interruptible(&priv->process_sem)) {
process_new_event:

        // Net device stop requested
		if (kthread_should_stop())
			break;

        // Reconfiguration needed without stopping net device
        if(priv->state == INIT) {
            continue;
        }

        // Save current status (before irq clear)
        status = priv->spi_ctl.rx_buff[0];

        // Process IDLE events (switch no RX or TX state)
        if((priv->state == IDLE_NO_TX) || (priv->state == IDLE)) {
            nrf_process_idle(priv, status);
        }
        // Process RX events
        if(priv->state == RX) {
            nrf_process_rx(priv, status);
        }
        // Process TX events
        if(priv->state == TX) {
            nrf_process_tx(priv, status);
        }
        // Process TX RTS events
        else if(priv->state == TX_RTS) {
            nrf_process_tx_rts(priv, status);
        }

        spin_lock_irqsave(&priv->process_lock, flags);

        if(priv->is_timeout) {
            // Time out detected
            // TODO: protect restart_wdg and stop_wdg to clear is_timeout if event is processed
            priv->is_timeout = 0;
            spin_unlock_irqrestore(&priv->process_lock, flags);
            nrf_process_timeout(priv);
            spin_lock_irqsave(&priv->process_lock, flags);
        }


        // Check if new IRQ happened
        if(priv->is_irq) {
            priv->is_irq = 0;
            spin_unlock_irqrestore(&priv->process_lock, flags);
            // Clear irq, get status and start over
            nrfmod_clear_all_irq(priv);
            // printk("Process again %02x\n", priv->spi_ctl.rx_buff[0]);
            goto process_new_event;
        }
        // Check if RX FIFO is empty (RX -> IDLE processing, should never happen)
        else if((priv->spi_ctl.rx_buff[0] & nrf24l01_STATUS_RX_P_NO) != 0x0e) {
            // Still something in RX FIFO : restart
            spin_unlock_irqrestore(&priv->process_lock, flags);
            nrfmod_clear_all_irq(priv);
            printk("RX not empty %02x\n", priv->spi_ctl.rx_buff[0]);
            goto process_new_event;
        }
        else if(priv->state == IDLE) {
            if(priv->skb) {
                // Retransmit current pkg
                priv->retransmit++;

                if(priv->retransmit >= 3) {
                    // Packet lost, ignore skb
                    printk("main MAX_RT: ignore skb\n");
                    priv->ndev->stats.tx_dropped++;
                    dev_kfree_skb(priv->skb);
                    priv->skb = NULL;
                }
                else {
                    if((err = nrf_do_tx_async(priv))) {
                        // TODO: handle this case
                        printk("Warning : spi_async failed nrf TX WDG 2 %d\n", err);
                        priv->state = IDLE;
                        dev_kfree_skb_irq(priv->skb);
                        priv->skb = NULL;
                        // TODO : stop net device queue
                        spin_unlock_irqrestore(&priv->process_lock, flags);
                        netif_stop_queue(priv->ndev);
                    }
                    else {
                        printk("event_handler RT %d\n", priv->retransmit);
                        // priv->is_processing = 1;
                        priv->state = TX_RTS;
                        priv->pkg_index = 0;
                        gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_LOW);
                        spin_unlock_irqrestore(&priv->process_lock, flags);
                        continue;
                    }
                }
            }

            if(priv->tx_buff.tail != priv->tx_buff.head) {
                // new pkg ready to send
                spin_lock(&priv->tx_buff.lock);
                priv->skb = priv->tx_buff.buff[priv->tx_buff.tail];
                priv->tx_buff.tail = (priv->tx_buff.tail+1)&(NET_TX_BUFFER_SIZE-1);
                spin_unlock(&priv->tx_buff.lock);


                if((err = nrf_do_tx_async(priv))) {
                    // TODO: handle this case
                    printk("Warning : spi_async failed nrf TX WDG 2 %d\n", err);
                    priv->state = IDLE;
                    dev_kfree_skb_irq(priv->skb);
                    priv->skb = NULL;
                    priv->ndev->stats.tx_dropped++;
                    // TODO : stop net device queue
                    spin_unlock_irqrestore(&priv->process_lock, flags);
                    netif_stop_queue(priv->ndev);
                }
                else {
                    // printk("New pkg in event_handler %d\n", priv->skb->len);
                    // priv->is_processing = 1;
                    priv->state = TX_RTS;
                    priv->retransmit = 0;
                    priv->pkg_index = 0;
                    gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_LOW);

                    spin_unlock_irqrestore(&priv->process_lock, flags);
                    continue;
                }
            }
            priv->is_processing = 0;
            spin_unlock_irqrestore(&priv->process_lock, flags);
            // printk("IDLE nothing to do\n");
            continue;
        }
        else {
            // Nothing to process : exit
            priv->is_processing = 0;
            spin_unlock_irqrestore(&priv->process_lock, flags);
            // printk("Nothing to process : exit\n");
            continue;
        }
    }
    printk("End process\n");
    return 0;
}

static enum hrtimer_restart nrf_txtimer_callback(struct hrtimer *timer)
{
    unsigned long flags;
    int err;
    struct nrf_priv *priv = container_of(timer, struct nrf_priv, timer_no_tx);

    spin_lock_irqsave(&priv->process_lock, flags);

    // Ready to TX another pkg
    if(priv->state != IDLE_NO_TX) {
        // printk("Nothing to do %d\n", priv->state);
        spin_unlock_irqrestore(&priv->process_lock, flags);
        return HRTIMER_NORESTART;
    }

    // printk("IDLE_NO_TX => IDLE\n");
    priv->state = IDLE;

    if(!priv->is_processing) {
        if(priv->is_irq) {
            printk("callback 2\n");
            // Clear TX FIFO (remove previous RTS that is not sent)
            if((err = nrf_do_irq_async(priv))) {
                // TODO :handle this case
                printk("Warning : spi_async failed WDG 1 %d\n", err);
            }
            else {
                priv->is_irq = 0;
                priv->is_processing = 1;
            }
            spin_unlock_irqrestore(&priv->process_lock, flags);
            return HRTIMER_NORESTART;
        }

        if(priv->skb) {
            // printk("callback 3\n");
            // Retransmit current pkg
            priv->retransmit++;

            if(priv->retransmit >= 3) {
                // Packet lost, ignore skb
                printk("MAX_RT: ignore skb\n");
                priv->ndev->stats.tx_dropped++;
                dev_kfree_skb_irq(priv->skb);
                priv->skb = NULL;
            }
            else {
                printk("RT %d\n", priv->retransmit);

                if((err = nrf_do_tx_async(priv))) {
                    // TODO: handle this case
                    printk("Warning : spi_async failed nrf TX WDG 2 %d\n", err);
                    priv->state = IDLE;
                    dev_kfree_skb_irq(priv->skb);
                    priv->skb = NULL;
                    // TODO : stop net device queue
                    spin_unlock_irqrestore(&priv->process_lock, flags);
                    netif_stop_queue(priv->ndev);
                }
                else {
                    // printk("callback 4\n");
                    priv->is_processing = 1;
                    priv->state = TX_RTS;
                    priv->pkg_index = 0;
                    gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_LOW);

                    spin_unlock_irqrestore(&priv->process_lock, flags);
                    return HRTIMER_NORESTART;
                }
            }
        }

        if(priv->tx_buff.tail != priv->tx_buff.head) {
            // new pkg ready to send
            spin_lock(&priv->tx_buff.lock);
            priv->skb = priv->tx_buff.buff[priv->tx_buff.tail];
            priv->tx_buff.tail = (priv->tx_buff.tail+1)&(NET_TX_BUFFER_SIZE-1);
            spin_unlock(&priv->tx_buff.lock);

            if((err = nrf_do_tx_async(priv))) {
                // TODO: handle this case
                printk("Warning : spi_async failed nrf TX WDG 2 %d\n", err);
                priv->state = IDLE;
                dev_kfree_skb_irq(priv->skb);
                priv->skb = NULL;
                // TODO : stop net device queue
                spin_unlock_irqrestore(&priv->process_lock, flags);
                netif_stop_queue(priv->ndev);
            }
            else {
                // printk("New pkg in callback %d\n", priv->skb->len);
                priv->is_processing = 1;
                priv->state = TX_RTS;
                priv->retransmit = 0;
                priv->pkg_index = 0;
                gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_LOW);
            }
        }
    } // End if procesing

    spin_unlock_irqrestore(&priv->process_lock, flags);
    return HRTIMER_NORESTART;
}

static enum hrtimer_restart nrf_wdgtimer_callback(struct hrtimer *timer)
{
    unsigned long flags;
    int err;
    struct nrf_priv *priv = container_of(timer, struct nrf_priv, timer_wdg);

    spin_lock_irqsave(&priv->process_lock, flags);
    priv->is_timeout = 1;

    if(priv->is_processing == 0) {
        if((err = nrf_do_irq_async(priv))) {
            printk("Warning : spi_async failed Timer callback %d\n", err);
            // Alternative callback
            up(&priv->process_sem);
        }
        else {
            // This allows to process event then handle timeout if nothing happened
            printk("Timer callback detected !\n");
            priv->is_processing = 1;
        }
    }
    spin_unlock_irqrestore(&priv->process_lock, flags);
    return HRTIMER_NORESTART;
}

/*
 * Open and close
 */

int nrf_open(struct net_device *ndev)
{
    int err;
	struct nrf_priv *priv = netdev_priv(ndev);

	// printk(KERN_NOTICE "nrf_open called\n");

    // Init event processing data
    spin_lock_init(&(priv->process_lock));
	sema_init(&priv->process_sem, 1);

    hrtimer_init(&priv->timer_wdg, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    hrtimer_init(&priv->timer_no_tx, CLOCK_MONOTONIC, HRTIMER_MODE_REL);

	priv->timer_wdg.function = nrf_wdgtimer_callback;
	priv->timer_no_tx.function = nrf_txtimer_callback;

    priv->wdg_sec = mp_nrf_timeout/1000;
    priv->wdg_nsec = (mp_nrf_timeout%1000)*1000000;
    // printk("Init timer %ld %ld\n", priv->wdg_sec, priv->wdg_nsec);

    spin_lock(&priv->process_lock);
    priv->state = INIT;
    priv->is_irq = priv->is_processing = 0;
    spin_unlock(&priv->process_lock);

    // Init tx skb queue
	spin_lock(&priv->tx_buff.lock);
    priv->tx_buff.head = 0;
    priv->tx_buff.tail = 0;
	spin_unlock(&priv->tx_buff.lock);

    // Lock process_sem to lock nrf_event_handler function
	if (down_interruptible(&priv->process_sem)) {
	    return -EBUSY;
    }

	priv->event_tsk = kthread_run(nrf_event_handler, priv, "nrf_event_handler");
	if (IS_ERR(priv->event_tsk)) {
		priv->event_tsk = NULL;
		printk("failed to create event thread\n");
		return -ENOMEM;
	}
 
    if((err = nrf_init_gpio(priv)) < 0)
    {
            printk(KERN_NOTICE "nrf_init_gpio error %d\n", err);
            goto fail_init_gpio;
    }

    // TODO : remove this line as device is already disabled
    gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_LOW);

    // Init nrf24l01 registers (and powerdown)
    if((err = nrfmod_init_device(priv)) < 0) {
        printk(KERN_NOTICE "nrf device init error\n");
        goto fail_init_nrf;
    }
	if((err = nrfmod_flush_tx(priv)) < 0) {
        goto fail_init_nrf;
    }
	if((err = nrfmod_flush_rx(priv)) < 0) {
        goto fail_init_nrf;
    }
	if((err = nrfmod_clear_all_irq(priv)) < 0) {
        goto fail_init_nrf;
    }

    // Start as RX
    priv->nrf_ctl.config |= nrf24l01_CONFIG_PRIM_RX;
    if((err = nrfmod_power_up(priv)) < 0) {
        goto fail_rx_start;
    }

    // Get MAC addr (to be sure device is working)
    // TODO : compare mac address to be sure device is ok
    if((err = nrfmod_read_register(priv, nrf24l01_RX_ADDR_P1, NULL, 5)) < 0) {
            printk(KERN_NOTICE "snull get device mac error\n");
            goto fail_nrf_check;
    }
    // printk(KERN_NOTICE "nrf mac addr (0x%02x) : %02x %02x %02x %02x %02x\n",
    //         priv->spi_ctl.rx_buff[0], 
    //         priv->spi_ctl.rx_buff[1], priv->spi_ctl.rx_buff[2],
    //         priv->spi_ctl.rx_buff[3], priv->spi_ctl.rx_buff[4],
    //         priv->spi_ctl.rx_buff[5]);

    // TODO : use setup_aw to get address length
    ndev->dev_addr[0] = 0xab; // This byte is ignored
	memcpy(&(ndev->dev_addr[1]), &(priv->spi_ctl.rx_buff[1]), ETH_ALEN-1);

    // printk("end config %02x\n", priv->nrf_ctl.config);
    // Set nrf up in RX mode
    gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_HIGH);
 
    // Enable interrupt processing
    spin_lock(&priv->process_lock);
    priv->state = IDLE;
    spin_unlock(&priv->process_lock);

    // Start network interface
	netif_start_queue(ndev);
    return 0;

  fail_nrf_check:
    nrfmod_power_down(priv);
  fail_rx_start:
  fail_init_nrf:
    nrf_exit_gpio(priv);
  fail_init_gpio:
    // Stop nrf_event_handler
    send_sig(SIGTERM, priv->event_tsk, 1);
    kthread_stop(priv->event_tsk);
    priv->event_tsk = NULL;
	up(&priv->process_sem);
    return err;
}

int nrf_release(struct net_device *ndev)
{
    int i;
    unsigned int head, tail;
	struct nrf_priv *priv = netdev_priv(ndev);

	printk(KERN_NOTICE "nrf_release called\n");

	netif_stop_queue(ndev); /* can't transmit any more */

    // Stop nrf_event_handler task
    send_sig(SIGTERM, priv->event_tsk, 1);
    kthread_stop(priv->event_tsk);
    priv->event_tsk = NULL;

    up(&priv->process_sem);
    
    // Disable device
    gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_LOW);
    nrfmod_power_down(priv);

    // Empty TX queue
	spin_lock(&priv->tx_buff.lock);
	head = priv->tx_buff.head;
	tail = ACCESS_ONCE(priv->tx_buff.tail);
    for(i=0; i<CIRC_CNT(head,tail,NET_TX_BUFFER_SIZE); i++) {
        dev_kfree_skb(priv->tx_buff.buff[(tail+i)&(NET_TX_BUFFER_SIZE-1)]);
    }
    priv->tx_buff.tail = head;
    spin_unlock(&priv->tx_buff.lock);

    nrf_exit_gpio(priv);

	printk(KERN_NOTICE "nrf closed and down\n");

	return 0;
}

/*
 * Configuration changes (passed on by ifconfig)
 */
int nrf_config(struct net_device *dev, struct ifmap *map)
{
	if (dev->flags & IFF_UP) /* can't act on a running interface */
		return -EBUSY;

	/* Don't allow changing the I/O address */
	if (map->base_addr != dev->base_addr) {
		printk(KERN_WARNING "snull: Can't change I/O address\n");
		return -EOPNOTSUPP;
	}

	/* Allow changing the IRQ */
	if (map->irq != dev->irq) {
		dev->irq = map->irq;
        	/* request_irq() is delayed to open-time */
	}

	/* ignore other fields */
	return 0;
}

static int nrf_do_tx_async(struct nrf_priv* priv)
{
    struct iphdr *iph;
    spi_message_init(&priv->spi_ctl.msg);

    // Update setup_retr if needed to disable retransmit
    if((priv->nrf_ctl.setup_retr & nrf24l01_SETUP_RETR_ARC) != nrf24l01_SETUP_RETR_ARC_0) {
        priv->nrf_ctl.setup_retr &= (~nrf24l01_SETUP_RETR_ARC) | nrf24l01_SETUP_RETR_ARC_0;

        priv->spi_ctl.tx_buff[20] = (nrf24l01_SETUP_RETR & 0x1F) | 0x20;
        priv->spi_ctl.tx_buff[21] = priv->nrf_ctl.setup_retr;
        priv->spi_ctl.transfer[3].tx_buf = priv->spi_ctl.tx_buff;
        priv->spi_ctl.transfer[3].rx_buf = priv->spi_ctl.rx_buff;
        priv->spi_ctl.transfer[3].len = 2;
        priv->spi_ctl.transfer[3].cs_change = 1;
        spi_message_add_tail(&priv->spi_ctl.transfer[3], &priv->spi_ctl.msg);
    }
    
    // Set as tx
    priv->nrf_ctl.config &= 0xFE;
    priv->spi_ctl.tx_buff[0] = (nrf24l01_CONFIG & 0x1F) | 0x20;
    priv->spi_ctl.tx_buff[1] = priv->nrf_ctl.config;
    priv->spi_ctl.transfer[0].tx_buf = priv->spi_ctl.tx_buff;
    priv->spi_ctl.transfer[0].rx_buf = priv->spi_ctl.rx_buff;
    priv->spi_ctl.transfer[0].len = 2;
    priv->spi_ctl.transfer[0].cs_change = 1;
    spi_message_add_tail(&priv->spi_ctl.transfer[0], &priv->spi_ctl.msg);

    // Set tx address
    priv->spi_ctl.tx_buff[2] = (nrf24l01_TX_ADDR & 0x1F) | 0x20;
    memcpy(&priv->spi_ctl.tx_buff[3], priv->nrf_ctl.rx_addr_p2, 5);
    priv->spi_ctl.transfer[1].tx_buf = &priv->spi_ctl.tx_buff[2];
    priv->spi_ctl.transfer[1].rx_buf = &priv->spi_ctl.rx_buff[2];
    priv->spi_ctl.transfer[1].len = 6;
    priv->spi_ctl.transfer[1].cs_change = 1;
    spi_message_add_tail(&priv->spi_ctl.transfer[1], &priv->spi_ctl.msg);

    // Send RTS
    priv->spi_ctl.tx_buff[8] = nrf24l01_W_TX_PAYLOAD_NO_ACK;
    priv->spi_ctl.tx_buff[9] = 0x8a;
    memcpy(&priv->spi_ctl.tx_buff[10], priv->nrf_ctl.rx_addr_p1, 5);
    memcpy(&priv->spi_ctl.tx_buff[15], priv->nrf_ctl.rx_addr_p1, 5);
    iph = ip_hdr(priv->skb); // TODO : use skb header
    priv->spi_ctl.tx_buff[15] = ((u8*)&(iph->daddr))[3];
    priv->spi_ctl.transfer[2].tx_buf = &priv->spi_ctl.tx_buff[8];
    priv->spi_ctl.transfer[2].rx_buf = &priv->spi_ctl.rx_buff[8];
    priv->spi_ctl.transfer[2].len = 12;
    priv->spi_ctl.transfer[2].cs_change = 1;
    spi_message_add_tail(&priv->spi_ctl.transfer[2], &priv->spi_ctl.msg);

    // Prepare spi msg
    priv->spi_ctl.msg.complete = spi_tx_completion;
    priv->spi_ctl.msg.context = priv;

    return spi_async(priv->spi_ctl.spi_device, &priv->spi_ctl.msg);
}

/*
 * Transmit a packet (called by the kernel)
 */
int nrf_tx(struct sk_buff *skb, struct net_device *ndev)
{
    // // This function add skb to driver queune and return ok
	unsigned int head, tail;
    int err;
    unsigned long flags;
	// // char *data, shortpkt[ETH_ZLEN];
	struct nrf_priv *priv = netdev_priv(ndev);

    // TODO : reimplement in correct location
	// data = skb->data;
	// len = skb->len;
	// if (len < ETH_ZLEN) {
	// 	memset(shortpkt, 0, ETH_ZLEN);
	// 	memcpy(shortpkt, skb->data, skb->len);
	// 	len = ETH_ZLEN;
	// 	data = shortpkt;
	// }

	spin_lock_irqsave(&priv->tx_buff.lock, flags);

	head = priv->tx_buff.head;
	tail = ACCESS_ONCE(priv->tx_buff.tail);

	if (CIRC_SPACE(head, tail, NET_TX_BUFFER_SIZE) >= 1) {
        priv->tx_buff.buff[head] = skb;
        // TODO : store release
		// smp_store_release(priv->tx_buff.head, (head + 1) & (NET_TX_BUFFER_SIZE - 1));
		priv->tx_buff.head = (head + 1) & (NET_TX_BUFFER_SIZE - 1);

        spin_lock(&priv->process_lock);

        // Device ready to TX (IDLE, not processing and no skb ready to TX)
        if((priv->state == IDLE) && (!priv->is_processing) && (!priv->skb)) {
            priv->skb = priv->tx_buff.buff[priv->tx_buff.tail];
            priv->tx_buff.tail = (priv->tx_buff.tail+1)&(NET_TX_BUFFER_SIZE-1);

            if((err = nrf_do_tx_async(priv))) {
                // TODO: handle this case
                printk("Warning : spi_async failed nrf TX %d\n", err);
                priv->state = IDLE;
                dev_kfree_skb_irq(priv->skb);
                priv->skb = NULL;
                // TODO : stop net device queue
                spin_unlock(&priv->process_lock);
                spin_unlock_irqrestore(&priv->tx_buff.lock, flags);
                netif_stop_queue(ndev);
                return NETDEV_TX_BUSY;
            }
            else {
                // printk("S 1\n");
                gpio_set_value(priv->nrf_ctl.gpio_ce, GPIO_LOW);
                priv->is_processing = 1;
                priv->state = TX_RTS;
                priv->pkg_index = 0;
                priv->retransmit = 0;
            }
        }
        spin_unlock(&priv->process_lock);
        spin_unlock_irqrestore(&priv->tx_buff.lock, flags);
        return NETDEV_TX_OK;
    }
    else {
        spin_unlock_irqrestore(&priv->tx_buff.lock, flags);
        printk("no space left in tx_buffer : drop skb and netif\n");
		netif_stop_queue(ndev);
        return NETDEV_TX_BUSY;
    }
}

/*
 * Deal with a transmit timeout.
 */
static void nrf_tx_timeout(struct net_device *dev)
{
    // int i;
    // unsigned int head, tail;
	// struct snull_priv *priv = netdev_priv(dev);

	// // PDEBUG("Transmit timeout at %ld, latency %ld\n", jiffies,
	// // 		jiffies - dev->trans_start);
    // //     /* Simulate a transmission interrupt to get things moving */
	// // priv->status = SNULL_TX_INTR;
	// // snull_interrupt(0, dev, NULL);
	// priv->stats.tx_errors++;


	// spin_lock(&priv->tx_buff.lock);

	// head = priv->tx_buff.head;
	// tail = ACCESS_ONCE(priv->tx_buff.tail);

    // for(i=0; i<CIRC_CNT(head,tail,NET_TX_BUFFER_SIZE); i++) {
    //     printk("timeout free skb %d %d\n", (tail+i)&(NET_TX_BUFFER_SIZE-1), head);
    //     dev_kfree_skb(priv->tx_buff.buff[(tail+i)&(NET_TX_BUFFER_SIZE-1)]);
    // }
    // priv->tx_buff.tail = head;
    // spin_unlock(&priv->tx_buff.lock);

    // up(&priv->spi_sem); // TODO : ????

	// netif_wake_queue(dev);
	// return;
}

int nrf_ioctl(struct net_device *ndev, struct ifreq *rq, int cmd)
{
	printk("ioctl\n");
	return 0;
}

struct net_device_stats *nrf_stats(struct net_device *ndev)
{
	struct nrf_priv *priv = netdev_priv(ndev);
	return &priv->ndev->stats;
}

int nrf_change_mtu(struct net_device *ndev, int new_mtu)
{
	if ((new_mtu < 68) || (new_mtu > 1500))
		return -EINVAL;
	
	ndev->mtu = new_mtu;
	return 0;
}

static const struct net_device_ops nrf_netdev_ops = {
	.ndo_open            = nrf_open,
	.ndo_stop            = nrf_release,
	.ndo_start_xmit      = nrf_tx,
	.ndo_do_ioctl        = nrf_ioctl,
	.ndo_set_config      = nrf_config,
	.ndo_get_stats       = nrf_stats,
	.ndo_change_mtu      = nrf_change_mtu,
	.ndo_tx_timeout      = nrf_tx_timeout
};

void nrf_init(struct net_device *ndev)
{
#if 0
    	/*
	 * Make the usual checks: check_region(), probe irq, ...  -ENODEV
	 * should be returned if no device found.  No resource should be
	 * grabbed: this is done on open(). 
	 */
#endif

	ether_setup(ndev); /* assign some of the fields */
	ndev->netdev_ops = &nrf_netdev_ops;
	ndev->flags     |= IFF_NOARP;
    ndev->flags     &= ~IFF_MULTICAST;

	memset(netdev_priv(ndev), 0, sizeof(struct nrf_priv));

	// spin_lock_init(&priv->lock);
	// snull_rx_ints(dev, 1);		/* enable receive interrupts */
	// snull_setup_pool(dev);
}




static int nrf_spi_probe(struct spi_device *spi_device)
{
	int err;
    struct net_device *ndev;
	struct nrf_priv *priv;

    printk(KERN_NOTICE "nrf_mod PROBE %s.%u %d %d\n", 
			dev_name(&spi_device->master->dev),
			spi_device->chip_select,
            ((struct nrf_spi_gpios*)(spi_device->controller_data))->ce,
            ((struct nrf_spi_gpios*)(spi_device->controller_data))->irq);

    err = 0;
    
    // nrf_init is called during this function
	ndev = alloc_netdev(sizeof(struct nrf_priv), "nrf%d",
			nrf_init);
	if (ndev == NULL) {
        return -ENOMEM;
    }

    SET_NETDEV_DEV(ndev, &spi_device->dev); // TODO : ???

    // Link spi device to net interface
    spi_set_drvdata(spi_device, ndev);

	priv = netdev_priv(ndev);
	memset(priv, 0, sizeof(struct nrf_priv));
    priv->ndev = ndev;

    priv->spi_ctl.spi_device = spi_device;
    priv->nrf_ctl.gpio_ce = ((struct nrf_spi_gpios*)(spi_device->controller_data))->ce;
    priv->nrf_ctl.gpio_irq = ((struct nrf_spi_gpios*)(spi_device->controller_data))->irq;

    // Allocate memory
    priv->spi_ctl.tx_buff = NULL;
    priv->spi_ctl.rx_buff = NULL;
	priv->tx_buff.buff = NULL;

	priv->spi_ctl.tx_buff = kmalloc(SPI_BUFF_SIZE, GFP_KERNEL);
	if (!priv->spi_ctl.tx_buff) {
		err = -ENOMEM;
		goto fail_alloc;
	}
	priv->spi_ctl.rx_buff = kmalloc(SPI_BUFF_SIZE, GFP_KERNEL);
	if (!priv->spi_ctl.rx_buff) {
		err = -ENOMEM;
		goto fail_alloc;
	}
    // printk("alloc skb buffer %d\n", NET_TX_BUFFER_SIZE*sizeof(*(priv->tx_buff.buff)));
	priv->tx_buff.buff =
        kmalloc(NET_TX_BUFFER_SIZE*sizeof(*(priv->tx_buff.buff)), GFP_KERNEL);
	if (!priv->tx_buff.buff) {
		err = -ENOMEM;
		goto fail_alloc;
	}
    spin_lock_init(&(priv->tx_buff.lock));

    // Register net device (driver is ready to operate)
    if((err = nrf_register_netdev(ndev))) {
		printk("snull: error %i registering device \"%s\" (too many devices ?)\n",
				err, ndev->name);
        goto fail_register;
    }
    return 0;

  fail_register:
  fail_alloc:
    if (priv->spi_ctl.tx_buff) {
    	kfree(priv->spi_ctl.tx_buff);
    	priv->spi_ctl.tx_buff = 0;
    }
    if (priv->spi_ctl.rx_buff) {
    	kfree(priv->spi_ctl.rx_buff);
    	priv->spi_ctl.rx_buff = 0;
    }
    if (priv->tx_buff.buff) {
    	kfree(priv->tx_buff.buff);
    	priv->tx_buff.buff = 0;
    }
    free_netdev(ndev);
    return err;
}

static int nrf_spi_remove(struct spi_device *spi_device)
{
    struct net_device *ndev;
	struct nrf_priv *priv;

	// if (down_interruptible(&nrfmod_dev.spi_sem))
	// 	return -EBUSY;

    printk(KERN_NOTICE "nrf_mod REMOVE %s.%u\n", 
			dev_name(&spi_device->master->dev),
			spi_device->chip_select);

    ndev = spi_get_drvdata(spi_device);

	if (ndev) {
        // Stop net interface and nrf device
		nrf_unregister_netdev(ndev);
		// snull_teardown_pool(ndev);

        // At this point, device is normally down
        // Free memory
        priv = netdev_priv(ndev);

        if (priv->spi_ctl.tx_buff) {
            kfree(priv->spi_ctl.tx_buff);
            priv->spi_ctl.tx_buff = NULL;
        }

        if (priv->spi_ctl.rx_buff) {
            kfree(priv->spi_ctl.rx_buff);
            priv->spi_ctl.rx_buff = NULL;
        }

        if (priv->tx_buff.buff) {
            kfree(priv->tx_buff.buff);
            priv->tx_buff.buff = NULL;
        }

		free_netdev(ndev);
        // printk(KERN_NOTICE "snull dev struct released\n");
	}
    // TODO : This must be freed before
    // kfree(spi_device->controller_data);
	return 0;
}


static struct spi_driver nrf_spi_driver = {
	.driver = {
		.name =	this_driver_name,
		.owner = THIS_MODULE,
	},
	.probe = nrf_spi_probe,
	.remove = nrf_spi_remove,	
};

int nrf_add_spi_device(int spibus, int spibus_cs)
{
	struct spi_master *spi_master;
	struct spi_device *spi_device;
	struct device *pdev;
	char buff[64];
	int status = 0;

	spi_master = spi_busnum_to_master(spibus);
	if (!spi_master) {
		printk(KERN_ALERT "spi_busnum_to_master(%d) returned NULL\n", spibus);
		printk(KERN_ALERT "Missing modprobe spi_bcm2708?\n");
		return -1;
	}

	spi_device = spi_alloc_device(spi_master);
	if (!spi_device) {
		put_device(&spi_master->dev); // decrement reference counter
		printk(KERN_ALERT "spi_alloc_device() failed\n");
		return -1;
	}

	spi_device->chip_select = spibus_cs;

	/* Check whether this SPI bus cs is already claimed */
	snprintf(buff, sizeof(buff), "%s.%u", 
			dev_name(&spi_device->master->dev),
			spi_device->chip_select);

	pdev = bus_find_device_by_name(spi_device->dev.bus, NULL, buff);
 	if (pdev) {
		/* We are not going to use this spi_device, so free it */ 
		spi_dev_put(spi_device);
		/* 
		 * There is already a device configured for this bus.cs  
		 * It is okay if it us, otherwise complain and fail.
		 */
		if (pdev->driver && pdev->driver->name) {
			printk(KERN_ALERT "Driver [%s] already registered for %s\n",
				pdev->driver->name, buff);
			status = -1;
		} 
	} else {
        struct nrf_spi_gpios* gpios = kmalloc(sizeof(struct nrf_spi_gpios), GFP_KERNEL);
        if(!gpios) {
            spi_dev_put(spi_device);
        }
        else {
            spi_device->max_speed_hz = mp_nrf_spibus_speed;
            spi_device->mode = SPI_MODE_0;
            spi_device->bits_per_word = 8;
            spi_device->irq = -1;
            spi_device->controller_state = NULL;

            gpios->ce = mp_nrf_gpio_ce;
            gpios->irq = mp_nrf_gpio_irq;
            spi_device->controller_data = gpios;
            strlcpy(spi_device->modalias, this_driver_name, SPI_NAME_SIZE);
            
            status = spi_add_device(spi_device);		
            if (status < 0) {	
                spi_dev_put(spi_device);
                printk(KERN_ALERT "spi_add_device() failed: %d\n", 
                    status);		
            }				
        }
	}
    // Probe function already called here
	put_device(&spi_master->dev);
	return status;
}

int nrf_init_spi(void)
{
    int err;
    if((err = spi_register_driver(&nrf_spi_driver)) < 0)
    {
        printk(KERN_WARNING "Error %d with spi_register_driver\n", err);
        return err;
    }

    // this function is used when nrf hardware is not specified in kernel
    if(mp_nrf_spibus >= 0 && mp_nrf_spibus_cs >= 0) {
        printk("spibus specified to nrf device\n");
        if((err = nrf_add_spi_device(mp_nrf_spibus, mp_nrf_spibus_cs)) < 0)
        {
            printk(KERN_ALERT "Error %d: nrf_add_spi_device\n", err);
            spi_unregister_driver(&nrf_spi_driver);
            return err;
        }
    }
    return 0;
}

void nrf_exit_spi(void)
{
    spi_unregister_driver(&nrf_spi_driver);
}

int nrf_init_module(void)
{
	int ret;
	memset(nrf_devices, 0, NRF_DEVICE_COUNT*sizeof(struct nrf_priv*));

    // Probe spi devices
	if ((ret = nrf_init_spi()) < 0) {
        return ret;
    }
	return 0;
}

void nrf_exit_module(void)
{
    int i;
    // Unregister all spi devices
    // TODO : protect ??
    for(i=0; i<NRF_DEVICE_COUNT; i++) {
        if(nrf_devices[i]) {
            spi_unregister_device(nrf_devices[i]->spi_ctl.spi_device);
        }
    }
    // At this point everything is closed
    nrf_exit_spi();
    // printk(KERN_NOTICE "Goodbye\n");
}

module_init(nrf_init_module);
module_exit(nrf_exit_module);
