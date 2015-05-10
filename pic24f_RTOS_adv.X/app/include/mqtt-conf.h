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

#ifndef MQTT_CONF_H
#define	MQTT_CONF_H

// Enable debug
#ifdef DEBUG
    #define MQTT_DEBUG
#endif

//* MQTT CONFIGURATION **********************************************
#define MQTT_MAX_PACKET_SIZE    128 // Maximum packet size
#define MQTT_DEFAULT_KEEP_ALIVE  60 // KeepAlive interval in Seconds
#define MQTTPROTOCOLVERSION       3
#define MQTT_DEFAULT_PORT      1883 // Default TCP port

#define MQTT_MAX_CLIENT_ID_LEN   23
#define MQTT_MAX_PAYLOAD_LEN    127

#endif
