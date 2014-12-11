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

#ifdef	__cplusplus
extern "C" {
#endif

#include "../uip/uip.h"
#include "../uip/timer.h"

#include "appstate.h"
// TODO: include delay
// #include "../timer1.h"

// MQTT Packet Types
#define MQTT_TYPE_CONNECT     (1 << 4)  // Client request to connect to Server
#define MQTT_TYPE_CONNACK     (2 << 4)  // Connect Acknowledgment
#define MQTT_TYPE_PUBLISH     (3 << 4)  // Publish message
#define MQTT_TYPE_TYPE_PUBACK (4 << 4)  // Publish Acknowledgment
#define MQTT_TYPE_PUBREC      (5 << 4)  // Publish Received (assured delivery part 1)
#define MQTT_TYPE_PUBREL      (6 << 4)  // Publish Release (assured delivery part 2)
#define MQTT_TYPE_PUBCOMP     (7 << 4)  // Publish Complete (assured delivery part 3)
#define MQTT_TYPE_SUBSCRIBE   (8 << 4)  // Client Subscribe request
#define MQTT_TYPE_SUBACK      (9 << 4)  // Subscribe Acknowledgment
#define MQTT_TYPE_UNSUBSCRIBE (10 << 4) // Client Unsubscribe request
#define MQTT_TYPE_UNSUBACK    (11 << 4) // Unsubscribe Acknowledgment
#define MQTT_TYPE_PINGREQ     (12 << 4) // PING Request
#define MQTT_TYPE_PINGRESP    (13 << 4) // PING Response
#define MQTT_TYPE_DISCONNECT  (14 << 4) // Client is Disconnecting

// Fixed header flags (second nibble)
#define MQTT_FLAG_DUP         (0x08)
#define MQTT_FLAG_QOS_0       (0x00)
#define MQTT_FLAG_QOS_1       (0x02)
#define MQTT_FLAG_QOS_2       (0x04)
#define MQTT_FLAG_RETAIN      (0x01)

// CONNECT header flags
#define MQTT_PROTOCOL_VERSION (3)
#define MQTT_FLAG_USERNAME    (0x80)
#define MQTT_FLAG_PASSWORD    (0x40)
#define MQTT_FLAG_WILL_RETAIN (0x20)
#define MQTT_FLAG_WILL_QOS_0  (0x00)
#define MQTT_FLAG_WILL_QOS_1  (0x08)
#define MQTT_FLAG_WILL_QOS_2  (0x10)
#define MQTT_FLAG_WILL        (0x04)
#define MQTT_FLAG_CLEAN       (0x02)

// #define MQTT_DEBUG

#ifdef MQTT_DEBUG
#include "../uart2.h"
#define MQTT_DEBUG_PRINTLN(str) PrintString(str "\n");
#define MQTT_DEBUG_PRINTF(str, ...) do {sprintf(Buffer, str, __VA_ARGS__); PrintString(Buffer); } while(0);
#else
#define MQTT_DEBUG_PRINTLN(str)
#define MQTT_DEBUG_PRINTF(str, ...)
#endif

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

void mqtt_connect(struct mqtt_client* this, uip_ipaddr_t* ripaddr, uint16_t rport);
void mqtt_disconnect(struct mqtt_client* this);
uint8_t mqtt_connected(struct mqtt_client* this);
uint8_t mqtt_get_state(struct mqtt_client* this);
int8_t mqtt_get_error_code(struct mqtt_client* this);

void mqtt_set_client_id(struct mqtt_client* this, const char* client_id);
void mqtt_set_server_port(struct mqtt_client* this, uint16_t port);
void mqtt_set_keep_alive(struct mqtt_client* this, uint16_t secs);
void mqtt_set_blocking_mode(struct mqtt_client* this, uint8_t blocking);
void mqtt_set_callback(struct mqtt_client* this, mqtt_callback_t callback);

void mqtt_publish_raw(struct mqtt_client* this, const char* topic, const uint8_t* payload, uint8_t plength, uint8_t retained);
void mqtt_publish_array(struct mqtt_client* this, const char* topic, const uint8_t* payload, uint8_t plength);
void mqtt_publish_string(struct mqtt_client* this, const char* topic, const char* payload);

void mqtt_subscribe(struct mqtt_client* this, const char* topic);

// uIP Callbacks (used internally)
void mqtt_tcp_closed(struct mqtt_client* this);
void mqtt_tcp_connected(struct mqtt_client* this);
void mqtt_tcp_acked(struct mqtt_client* this);
void mqtt_tcp_receive(struct mqtt_client* this);
void mqtt_tcp_transmit(struct mqtt_client* this);
void mqtt_poll(struct mqtt_client* this);
void mqtt_check_timeout(struct mqtt_client* this);


#ifdef	__cplusplus
}
#endif

#endif	/* MQTT_H */

