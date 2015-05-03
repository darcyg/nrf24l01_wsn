/* 
 * File:   mqtt.h
 * Author: Simon Piriou <spiriou31@gmail.com>
 *
 * Based on NanodeMQTT.h
 * Copyright (c) 2011-2012, Nicholas Humfrey.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef MQTT_H
#define	MQTT_H

#include <stdbool.h>

#include "uip/uip.h"
#include "mqtt-conf.h"
#include "appstate.h" // For mqtt_callback_t type

enum mqtt_state {
  MQTT_STATE_WAITING,        // Waiting for TCP to connect
  MQTT_STATE_CONNECTING,     // TCP Connected and in middle of sending a CONNECT
  MQTT_STATE_CONNECT_SENT,   // Waiting for CONNACK
  MQTT_STATE_CONNECTED,      // Received CONNACK
  MQTT_STATE_PUBLISHING,     // In the middle of sending a PUBLISH
  MQTT_STATE_SUBSCRIBING,    // In the middle of sending a SUBSCRIBE
  MQTT_STATE_SUBSCRIBE_SENT, // Waiting for a SUBACK
  MQTT_STATE_PINGING,        // In the middle of sending a PING
  MQTT_STATE_DISCONNECTING,  // In the middle of sending a DISCONNECT packet
  MQTT_STATE_DISCONNECTED
};

void mqtt_init(struct mqtt_client* this, const char* client_id, mqtt_callback_t callback);

// MQTT state management
void mqtt_connect(struct mqtt_client* this, uip_ipaddr_t* ripaddr, uint16_t rport);
void mqtt_disconnect(struct mqtt_client* this);
uint8_t mqtt_connected(struct mqtt_client* this);
uint8_t mqtt_get_state(struct mqtt_client* this);
int8_t mqtt_get_error_code(struct mqtt_client* this);

bool mqtt_is_subscribe_done(struct mqtt_client* this);
bool mqtt_is_connect_done(struct mqtt_client* this);

// MQTT client configuration
void mqtt_set_client_id(struct mqtt_client* this, const char* client_id);
void mqtt_set_server_port(struct mqtt_client* this, uint16_t port);
void mqtt_set_keep_alive(struct mqtt_client* this, uint16_t secs);
void mqtt_set_blocking_mode(struct mqtt_client* this, uint8_t blocking);
void mqtt_set_callback(struct mqtt_client* this, mqtt_callback_t callback);

// MQTT publish interface
void mqtt_publish_raw(struct mqtt_client* this, const char* topic, const uint8_t* payload, uint8_t plength, uint8_t retained);
void mqtt_publish_array(struct mqtt_client* this, const char* topic, const uint8_t* payload, uint8_t plength);
void mqtt_publish_string(struct mqtt_client* this, const char* topic, const char* payload);

// MQTT subscribe interface
void mqtt_subscribe(struct mqtt_client* this, const char* topic);

#endif	/* MQTT_H */
