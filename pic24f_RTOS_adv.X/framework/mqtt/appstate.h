/* 
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

#ifndef MQTT_APPSTATE_H
#define	MQTT_APPSTATE_H

#include "mqtt-conf.h"
#include "uip/timer.h"

typedef void (*mqtt_callback_t) (const char* topic, uint8_t* payload, int payload_length);

struct mqtt_client {
  // TODO
  // NanodeUIP *uip;
  char client_id[MQTT_MAX_CLIENT_ID_LEN+1];
  uint16_t keep_alive;
  uint16_t message_id;
  uint8_t state;
  uint8_t ping_pending;
  uint8_t blocking_mode;
  int8_t error_code;

  uint8_t *buf;
  uint8_t pos;

  struct timer receive_timer;
  struct timer transmit_timer;

  // Publishing
  // FIXME: can we do without these buffers
  char payload_topic[32];
  uint8_t payload[MQTT_MAX_PAYLOAD_LEN];
  uint8_t payload_length;
  uint8_t payload_retain;

  // Subscribing
  const char *subscribe_topic;
  mqtt_callback_t callback;
};

#endif	/* MQTT_APPSTATE_H */
