#pragma once
#include "esp_wifi.h"
#include "esp_wifi_default.h"
#include "cstring"
#include "secrets.h"
#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "nvs_flash.h"

#define WIFI_MAX_RETRY 5
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

class Wifi {
    public:
    Wifi();
    esp_err_t initWifi();
    static void eventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
    
    private:
    EventGroupHandle_t s_wifi_event_group;
    const char *TAG;
    int s_retry_num;
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    void memberEventHandler(esp_event_base_t event_base, int32_t event_id, void* event_data);
    
    
};