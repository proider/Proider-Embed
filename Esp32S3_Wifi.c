#include "wifi_connect.h"

uint16_t wifi_is_initialized = 0;
uint16_t nvs_flash_is_initialized = 0;
uint16_t netif_is_initialized = 0;
uint16_t esp_default_event_loop_created = 0;
uint16_t wifi_event_is_registed = 0;
uint16_t ip_event_is_registed = 0;
uint16_t default_wifi_sta_is_created = 0;

uint8_t wifi_ssid[30] = "Peter";
uint8_t wifi_password[30] = "zhou20050223";

wifi_mode_t wifi_mode = WIFI_MODE_STA;

wifi_config_t wifi_config = {.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK,
                             .sta.pmf_cfg.capable = true,
                             .sta.pmf_cfg.required = false,
                            };

esp_event_handler_instance_t instance_any_id;
esp_event_handler_instance_t instance_got_ip;

#define MAX_Retry 20
uint16_t s_retry_num = 0;

void event_hander(void* event_hander_arg,esp_event_base_t esp_event_base,int32_t event_id,void* event_data);


int wifi_connect(void){
    strcpy((char*)&(wifi_config.sta.ssid),(char*)wifi_ssid);
    strcpy((char*)&(wifi_config.sta.password),(char*)wifi_password);
    if(nvs_flash_is_initialized==0){
        if(nvs_flash_init()!=ESP_OK){
            nvs_flash_erase();
            ESP_ERROR_CHECK(nvs_flash_init());
            nvs_flash_is_initialized = 1;
        }
    }
     if(netif_is_initialized==0){
            ESP_ERROR_CHECK(esp_netif_init());
            netif_is_initialized = 1;
        }
    if(esp_default_event_loop_created==0){
            ESP_ERROR_CHECK(esp_event_loop_create_default());
            esp_default_event_loop_created = 1;
        }
    if(default_wifi_sta_is_created==0){
            esp_netif_create_default_wifi_sta();
            default_wifi_sta_is_created = 1;
        }
    if(wifi_is_initialized==0){
        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));
        wifi_is_initialized = 1;
    }
    if(wifi_event_is_registed==0){
        ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,ESP_EVENT_ANY_ID,&event_hander,NULL,&instance_any_id));
        wifi_event_is_registed = 1;
    }

    if(ip_event_is_registed==0){
        ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,IP_EVENT_STA_GOT_IP,&event_hander,NULL,&instance_got_ip));
        ip_event_is_registed = 1;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(wifi_mode));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA,&wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI("WIFI","WIFI Start");
    return 0;
}
int wifi_disconnect(void){
    esp_wifi_disconnect();
    ESP_LOGI("WIFI","WIFI Disconnect");
    return 0;
}


void event_hander(void* event_hander_arg,esp_event_base_t esp_event_base,int32_t event_id,void* event_data){
    if(esp_event_base==WIFI_EVENT&&event_id==WIFI_EVENT_STA_START){
        ESP_ERROR_CHECK(esp_wifi_connect());
         ESP_LOGI("WIFI","WIFI is connecting");
    }
    else if(esp_event_base==WIFI_EVENT&&event_id==WIFI_EVENT_STA_DISCONNECTED){
        if(s_retry_num<MAX_Retry){
            s_retry_num++;
            ESP_ERROR_CHECK(esp_wifi_connect());
            ESP_LOGI("WIFI","WIFI is connecting");
        }
        else{
             ESP_LOGI("WIFI","WIFI is disconnected");
        }
    }
    else if(esp_event_base==WIFI_EVENT&&event_id==WIFI_EVENT_STA_CONNECTED){
         ESP_LOGI("WIFI","WIFI Successfully Connect");
    }
    else if(esp_event_base==IP_EVENT&&event_id==IP_EVENT_STA_GOT_IP){
        ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
        ESP_LOGI("IP","GOT IP:"IPSTR,IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
    }
}
