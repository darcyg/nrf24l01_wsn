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

#include "uip/uip.h"

#include "tcp_app/tcp_server.h"
#include "logger.h"
#include "uip/nrf24l01_link.h"

#include <string.h>

static int hello_world_appcall(void);

void tcp_hello_init(void)
{
    // Init tcp server
    uip_listen(HTONS(1000), (void (*)(void))hello_world_appcall);
}

static int hello_world_appcall(void)
{
    struct tcp_hello_appstate *s = &(uip_conn->appstate.tcp_hello);

    if(uip_connected()) {
      PSOCK_INIT(&s->p, s->inputbuffer, sizeof(s->inputbuffer));
    }

    PSOCK_BEGIN(&s->p);
    PSOCK_SEND_STR(&s->p, "Hello. What is your name?\n");
    PSOCK_READTO(&s->p, '\n');
    strncpy(s->name, s->inputbuffer, sizeof(s->name));
    s->name[strlen(s->name)-2] = 0;
    PSOCK_SEND_STR(&s->p, "Hello ");
    PSOCK_SEND_STR(&s->p, s->name);
    PSOCK_SEND_STR(&s->p, " !\n");
    sprintf(s->name, "r %d/%d", nrf_link_get_rx_packets(), nrf_link_get_rx_dropped());
    PSOCK_SEND_STR(&s->p, s->name);
    PSOCK_SEND_STR(&s->p, " ");
    sprintf(s->name, "t %d/%d", nrf_link_get_tx_packets(), nrf_link_get_tx_dropped());
    PSOCK_SEND_STR(&s->p, s->name);
    PSOCK_SEND_STR(&s->p, "\n\r");
    PSOCK_CLOSE(&s->p);
    PSOCK_END(&s->p);
}
