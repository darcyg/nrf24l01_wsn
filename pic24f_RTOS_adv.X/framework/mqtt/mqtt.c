/*
 * File: mqtt.c
 * Author: Simon Piriou <spiriou31@gmail.com>
 *
 * Based on NanodeMQTT.cpp
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

#include "HardwareProfile.h"
#include "uip/uip.h"
#include "mqtt.h"

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

#ifdef MQTT_DEBUG
#include "logger.h"
#define MQTT_DEBUG_PRINTLN(str)     log_debug(str)
#define MQTT_DEBUG_PRINTF(str, ...) log_debug(str, __VA_ARGS__)
#else
#define MQTT_DEBUG_PRINTLN(str)
#define MQTT_DEBUG_PRINTF(str, ...)
#endif

// uIP Callbacks (used internally)
void mqtt_tcp_closed(struct mqtt_client* this);
void mqtt_tcp_connected(struct mqtt_client* this);
void mqtt_tcp_acked(struct mqtt_client* this);
void mqtt_tcp_receive(struct mqtt_client* this);
void mqtt_tcp_transmit(struct mqtt_client* this);
void mqtt_poll(struct mqtt_client* this);
void mqtt_check_timeout(struct mqtt_client* this);

void mqtt_init(struct mqtt_client* this, const char* client_id, mqtt_callback_t callback) {
  memset(this->client_id, 0, MQTT_MAX_CLIENT_ID_LEN+1);
  mqtt_set_client_id(this, client_id);

  this->keep_alive = MQTT_DEFAULT_KEEP_ALIVE;
  this->message_id = 0;
  this->state = MQTT_STATE_DISCONNECTED;
  this->ping_pending = 0;
  this->blocking_mode = 1;
  this->error_code = 0;

  this->payload_length = 0;
  this->payload_retain = 0;
  this->subscribe_topic = NULL;
  this->callback = callback;
}

void mqtt_appcall(void)
{
  struct mqtt_client* mqtt = uip_conn->appstate.mqtt;

  if (uip_aborted()) {
    MQTT_DEBUG_PRINTLN("TCP: Connection Aborted");
    mqtt_tcp_closed(mqtt);
  }

  if (uip_timedout()) {
    MQTT_DEBUG_PRINTLN("TCP: Connection Timed Out");
    mqtt_tcp_closed(mqtt);
  }

  if (uip_closed()) {
    MQTT_DEBUG_PRINTLN("TCP: Connection Closed");
    mqtt_tcp_closed(mqtt);
  }

  if (uip_connected()) {
    MQTT_DEBUG_PRINTLN("TCP: Connection Established");
    mqtt_tcp_connected(mqtt);
  }

  if (uip_acked()) {
    MQTT_DEBUG_PRINTLN("TCP: Acked");
    mqtt_tcp_acked(mqtt);
  }

  if (uip_newdata()) {
    MQTT_DEBUG_PRINTLN("TCP: New Data");
    mqtt_tcp_receive(mqtt);
  }

  if (uip_rexmit()) {
    MQTT_DEBUG_PRINTLN("TCP: Retransmit");
    mqtt_tcp_transmit(mqtt);
  }

  if (uip_poll()) {
    mqtt_poll(mqtt);
  }

  mqtt_check_timeout(mqtt);
}

void mqtt_set_client_id(struct mqtt_client* this, const char* client_id)
{
  strncpy(this->client_id, client_id, MQTT_MAX_CLIENT_ID_LEN);
}

void mqtt_set_keep_alive(struct mqtt_client* this, uint16_t secs)
{
  this->keep_alive = secs;
}

void mqtt_set_blocking_mode(struct mqtt_client* this, uint8_t blocking)
{
  this->blocking_mode = blocking;
}

void mqtt_set_callback(struct mqtt_client* this, mqtt_callback_t callback)
{
  this->callback = callback;
}

bool mqtt_is_connect_done(struct mqtt_client* this)
{
      return !(this->state == MQTT_STATE_WAITING ||
               this->state == MQTT_STATE_CONNECTING ||
               this->state == MQTT_STATE_CONNECT_SENT);
}

// FIXME: add support for WILL
void mqtt_connect(struct mqtt_client* this, uip_ipaddr_t* ripaddr, uint16_t rport)
{
  struct uip_conn *conn;

  // Set the client ID to the MAC address, if none is set
  if (this->client_id[0] == '\0') {
      // TODO: fill with someting else
      mqtt_set_client_id(this, "default");
  }

  conn = uip_connect(ripaddr, rport, mqtt_appcall);
  if (conn) {
    // struct mqtt_app_state *s = (struct mqtt_app_state *)&(conn->appstate);
    // uip_tcp_appstate_t *s = &(uip_conn->appstate);
    // s->mqtt = this;
    conn->appstate.mqtt = this;

    this->state = MQTT_STATE_WAITING;

    // Set the keep-alive timers
    timer_set(&this->transmit_timer, CLOCK_SECOND * (this->keep_alive-2));
    timer_set(&this->receive_timer, CLOCK_SECOND * (this->keep_alive-2));

    // If in blocking mode - loop until we are connected
    if (this->blocking_mode) {
      while(!mqtt_is_connect_done(this)) {
        net_poll(pdMS_TO_TICKS(200));
      }
    }
  }
}

uint8_t mqtt_connected(struct mqtt_client* this)
{
  switch(this->state) {
    case MQTT_STATE_CONNECTED:
    case MQTT_STATE_PUBLISHING:
    case MQTT_STATE_SUBSCRIBING:
    case MQTT_STATE_SUBSCRIBE_SENT:
    case MQTT_STATE_PINGING:
      return 1;
    default:
      return 0;
  }
}

uint8_t mqtt_get_state(struct mqtt_client* this)
{
  return this->state;
}

int8_t mqtt_get_error_code(struct mqtt_client* this)
{
  return this->error_code;
}

void mqtt_disconnect(struct mqtt_client* this)
{
   MQTT_DEBUG_PRINTLN("disconnect()");
   if (mqtt_connected(this)) {
      this->state = MQTT_STATE_DISCONNECTING;
      mqtt_tcp_transmit(this);
   }
}

void mqtt_publish_raw(struct mqtt_client* this, const char* topic, const uint8_t* payload, uint8_t plength, uint8_t retained)
{
  // FIXME: check that payload isn't bigger than UIP_APPDATASIZE (or 127 bytes)
  // FIXME: can we avoid this extra buffer?
  strcpy(this->payload_topic, topic);
  memcpy(this->payload, payload, plength);
  this->payload_retain = retained;
  this->payload_length = plength;

  // If in blocking mode - loop until message has been published
  if (this->blocking_mode) {
    while (mqtt_connected(this) && this->payload_length != 0) {
      net_poll(pdMS_TO_TICKS(200));
    }
  }
}

void mqtt_publish_array(struct mqtt_client* this, const char* topic, const uint8_t* payload, uint8_t plength)
{
   mqtt_publish_raw(this, topic, payload, plength, 0);
}

void mqtt_publish_string(struct mqtt_client* this, const char* topic, const char* payload)
{
   mqtt_publish_array(this, topic, (uint8_t*)payload, strlen(payload));
}

// ** End of the public API **

bool mqtt_is_subscribe_done(struct mqtt_client* this)
{
    return !mqtt_connected(this) || !this->subscribe_topic;
}

void mqtt_subscribe(struct mqtt_client* this, const char* topic)
{
  this->subscribe_topic = topic;

  // If in blocking mode - loop until we have subscribed
  if (this->blocking_mode) {
    while(!mqtt_is_subscribe_done(this)) {
      net_poll(pdMS_TO_TICKS(200));
    }
  }
}

void mqtt_init_packet(struct mqtt_client* this, uint8_t header)
{
  this->buf = (uint8_t *)uip_appdata;
  this->pos = 0;
  this->buf[this->pos++] = header;
  this->buf[this->pos++] = 0x00;  // Packet length
}

void mqtt_append_byte(struct mqtt_client* this, uint8_t b)
{
  this->buf[this->pos++] = b;
}

void mqtt_append_word(struct mqtt_client* this, uint16_t s)
{
  this->buf[this->pos++] = (s>>8) & 0xff;
  this->buf[this->pos++] = s & 0xff;
}

void mqtt_append_string(struct mqtt_client* this, const char* str)
{
  // FIXME: support strings longer than 255 bytes
  const char* ptr = str;
  uint8_t len = 0;
  this->pos += 2;
  while (*ptr) {
    this->buf[this->pos++] = *ptr++;
    len++;
  }
  this->buf[this->pos-len-2] = 0;
  this->buf[this->pos-len-1] = len;
}

void mqtt_append_data(struct mqtt_client* this, uint8_t *data, uint8_t data_len)
{
  memcpy(&(this->buf[this->pos]), data, data_len);
  this->pos += data_len;
}

void mqtt_send_packet(struct mqtt_client* this)
{
  // FIXME: support packets longer than 127 bytes
  // Store the size of the packet (minus the fixed header)
  this->buf[1] = this->pos - 2;
  uip_send(this->buf, this->pos);

  // Restart the packet send timer
  timer_restart(&this->transmit_timer);
}

void mqtt_tcp_connected(struct mqtt_client* this)
{
  this->state = MQTT_STATE_CONNECTING;
  mqtt_tcp_transmit(this);
}

void mqtt_tcp_acked(struct mqtt_client* this)
{
  switch(this->state) {
    case MQTT_STATE_CONNECTING:
      this->state = MQTT_STATE_CONNECT_SENT;
      break;
    case MQTT_STATE_PUBLISHING:
      this->state = MQTT_STATE_CONNECTED;
      this->payload_length = 0;
      break;
    case MQTT_STATE_PINGING:
      this->state = MQTT_STATE_CONNECTED;
      this->ping_pending = 0;
      break;
    case MQTT_STATE_SUBSCRIBING:
      this->state = MQTT_STATE_SUBSCRIBE_SENT;
      this->subscribe_topic = NULL;
      break;
    case MQTT_STATE_DISCONNECTING:
      this->state = MQTT_STATE_DISCONNECTED;
      uip_close();
      break;
    default:
      MQTT_DEBUG_PRINTLN("TCP: ack in unknown state");
      break;
  }
}

void mqtt_tcp_receive(struct mqtt_client* this)
{
  uint8_t *buf = (uint8_t *)uip_appdata;
  uint8_t type = buf[0] & 0xF0;

  if (uip_datalen() == 0)
    return;

  // FIXME: check that packet isn't too long?
  // FIXME: support packets longer than 127 bytes
  // FIXME: support multiple MQTT packets in single IP packet

  switch(type) {
    case MQTT_TYPE_CONNACK: {
      uint8_t code = buf[3];
      if (code == 0) {
        MQTT_DEBUG_PRINTLN("MQTT: Connected!");
        this->state = MQTT_STATE_CONNECTED;
      } else {
        MQTT_DEBUG_PRINTF("MQTT: Connection failed (%u)", code);
        uip_close();
        this->state = MQTT_STATE_DISCONNECTED;
        this->error_code = code;
      }
      break;
    }
    case MQTT_TYPE_SUBACK:
      MQTT_DEBUG_PRINTLN("MQTT: Subscribed!");
      // FIXME: check current state before changing state
      this->state = MQTT_STATE_CONNECTED;
      break;
    case MQTT_TYPE_PINGRESP:
      MQTT_DEBUG_PRINTLN("MQTT: Pong!");
      this->ping_pending = 0;
      break;
    case MQTT_TYPE_PUBLISH:
      if (this->callback) {
        uint8_t tl = buf[3];
        // FIXME: is there a way we can NULL-terminate the string in the packet buffer?
        char topic[tl+1];
        memcpy(topic, &buf[4], tl);
        topic[tl] = 0;
        this->callback(topic, buf+4+tl, buf[1]-2-tl);
      }
      break;
    default:
      MQTT_DEBUG_PRINTF("MQTT: received unknown packet type (%u)", (type >> 4));
      break;
  }

  // Restart the packet receive timer
  // FIXME: this should only be restarted when a valid packet is received
  timer_restart(&this->receive_timer);
}

void mqtt_tcp_closed(struct mqtt_client* this)
{
  this->state = MQTT_STATE_DISCONNECTED;
  uip_close();

  // FIXME: re-establish connection automatically
}

void mqtt_poll(struct mqtt_client* this)
{
  if (this->state == MQTT_STATE_CONNECTED) {
    if (this->payload_length) {
      this->state = MQTT_STATE_PUBLISHING;
      mqtt_tcp_transmit(this);
    } else if (this->subscribe_topic) {
      this->state = MQTT_STATE_SUBSCRIBING;
      this->message_id++;
      mqtt_tcp_transmit(this);
    } else if (this->ping_pending) {
      this->state = MQTT_STATE_PINGING;
      mqtt_tcp_transmit(this);
    }
  }
}

void mqtt_check_timeout(struct mqtt_client* this)
{
  #ifdef MQTT_DEBUG
  if (timer_expired(&this->transmit_timer))
    MQTT_DEBUG_PRINTF("MQTT: not transmitted for %u seconds", this->keep_alive);

  if (timer_expired(&this->receive_timer))
    MQTT_DEBUG_PRINTF("MQTT: not received for %u seconds", this->keep_alive);
  #endif

  if (timer_expired(&this->transmit_timer) || timer_expired(&this->receive_timer)) {
    if (mqtt_connected(this)) {
      if (!this->ping_pending) {
        // Send ping on the next poll
        this->ping_pending = 1;

        // Give some extra time to send and receive the ping
        // FIXME: think of a better way of doing this - takes too long to timeout
        timer_restart(&this->receive_timer);
        timer_restart(&this->transmit_timer);
      } else {
        MQTT_DEBUG_PRINTLN("MQTT: Timed out.");
        mqtt_disconnect(this);
      }
    } else {
      MQTT_DEBUG_PRINTLN("MQTT: Aborting after time-out.");
      this->state = MQTT_STATE_DISCONNECTED;
      uip_abort();
    }
  }
}

// Transmit/re-transmit packet for the current state
void mqtt_tcp_transmit(struct mqtt_client* this)
{
  switch(this->state) {
    case MQTT_STATE_CONNECTING:
      MQTT_DEBUG_PRINTLN("MQTT: sending CONNECT");
      mqtt_init_packet(this, MQTT_TYPE_CONNECT);
      mqtt_append_string(this, "MQIsdp");
      mqtt_append_byte(this, MQTT_PROTOCOL_VERSION);
      mqtt_append_byte(this, MQTT_FLAG_CLEAN); // Connect flags
      mqtt_append_word(this, this->keep_alive);
      mqtt_append_string(this, this->client_id);
      mqtt_send_packet(this);
      break;
    case MQTT_STATE_PUBLISHING: {
      uint8_t header = MQTT_TYPE_PUBLISH;
      if (this->payload_retain)
        header |= MQTT_FLAG_RETAIN;

      MQTT_DEBUG_PRINTLN("MQTT: sending PUBLISH");
      mqtt_init_packet(this, header);
      mqtt_append_string(this, this->payload_topic);
      mqtt_append_data(this, this->payload, this->payload_length);
      mqtt_send_packet(this);
      break;
    }
    case MQTT_STATE_SUBSCRIBING:
      MQTT_DEBUG_PRINTLN("MQTT: sending SUBSCRIBE");
      mqtt_init_packet(this, MQTT_TYPE_SUBSCRIBE);
      mqtt_append_word(this, this->message_id);
      mqtt_append_string(this, this->subscribe_topic);
      mqtt_append_byte(this, 0x00);  // We only support QOS 0
      mqtt_send_packet(this);
      break;
    case MQTT_STATE_PINGING:
      MQTT_DEBUG_PRINTLN("MQTT: sending PINGREQ");
      mqtt_init_packet(this, MQTT_TYPE_PINGREQ);
      mqtt_send_packet(this);
      break;
    case MQTT_STATE_DISCONNECTING:
      MQTT_DEBUG_PRINTLN("MQTT: sending DISCONNECT");
      mqtt_init_packet(this, MQTT_TYPE_DISCONNECT);
      mqtt_send_packet(this);
      break;
    default:
      MQTT_DEBUG_PRINTF("MQTT: Unable to transmit in state %u", this->state);
      break;
  }
}
