/* 
 * File:   appstate.h
 * Author: mickey
 *
 * Created on 6 décembre 2014, 02:27
 */

#ifndef APPSTATE_H
#define	APPSTATE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../HardwareProfile.h"
#include "../uip/timer.h"

typedef void (*mqtt_callback_t) (const char* topic, uint8_t* payload, int payload_length);

void mqtt_appcall(void);

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



#ifdef	__cplusplus
}
#endif

#endif	/* APPSTATE_H */

