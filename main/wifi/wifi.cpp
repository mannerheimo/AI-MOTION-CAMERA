#include "wifi.h"



Wifi::Wifi() :  
    // creating eventgroup after evenhandler is run
    TAG("wifi station"), 
    s_retry_num(0)
    {}

esp_err_t Wifi::initWifi() {
    s_wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_netif_init()); //tcp/ipstack
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &Wifi::eventHandler,
                                                        this,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &Wifi::eventHandler,
                                                        this,
                                                        &instance_got_ip));

    wifi_config_t wifi_config{};
    std::memcpy(wifi_config.sta.ssid, WIFI_SSID, sizeof(WIFI_SSID));
    std::memcpy(wifi_config.sta.password, WIFI_PASSWORD, sizeof(WIFI_PASSWORD));
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;


    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");
    return ESP_OK;
}

void Wifi::eventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    Wifi* wifi_obj = static_cast<Wifi*>(arg);
    
    if (wifi_obj != nullptr) {
        wifi_obj->memberEventHandler(event_base, event_id, event_data);
    }
}

void Wifi::memberEventHandler(esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    }
    //if disconnect event happens we try WIFI_MAX_RETRY times to reconnect before fail
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < WIFI_MAX_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the ap");
        } 
        else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG, "connect to ap fail");
    } 
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    } 
}
