/*
 * tcp_server.h
 *
 *  Created on: 18.02.2023 г.
 *      Author: dida
 */

#ifndef MAIN_TCP_SERVER_H_
#define MAIN_TCP_SERVER_H_

#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
//#include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include <stdlib.h>
#include "pid.h"
#include "interpreter.h"

#define PORT                        3333
#define KEEPALIVE_IDLE              20
#define KEEPALIVE_INTERVAL          1
#define KEEPALIVE_COUNT             15



void tcp_server_task(void *pvParameters);
void comunication_commands(const int sock);


#endif /* MAIN_TCP_SERVER_H_ */