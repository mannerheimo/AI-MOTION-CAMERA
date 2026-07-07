#include "wifi.h"

esp_err_t wifi::initWifi() {
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    
    esp_err_t ret = esp_wifi_init(&cfg);
    ESP_ERROR_CHECK(ret);
    
    ret = esp_wifi_set_mode(WIFI_MODE_STA);
    ESP_ERROR_CHECK(ret);

    ret = esp_wifi_start();
    ESP_ERROR_CHECK(ret);
    wifi_config_t wifi_config{};
    //std::strcpy(wifi_config.sta.ssid, )


    //ret = esp_wifi_connect();


}