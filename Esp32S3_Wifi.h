#ifndef __WIFI_CONNECT_H_
#define __WIFI_CONNECT_H_

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/inet.h"

//ESP32S3只支持2.4G频段的WIFI,记得手机开热点的时候换成2.4G

extern  uint8_t wifi_ssid[30];
extern uint8_t wifi_password[30];

extern wifi_mode_t wifi_mode;

int wifi_connect(void);
int wifi_disconnect(void);
int wifi_connect_target_wifi(const uint8_t wifi_ssid[],const uint8_t wifi_password[]);


#endif
