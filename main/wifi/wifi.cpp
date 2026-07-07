#include "wifi.h"

esp_err_t wifi::initWifi() {
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    
    esp_err_t ret = esp_wifi_init(&cfg);
    if (ret != ESP_OK) {return ret;}
    
    ret = esp_wifi_set_mode(WIFI_MODE_STA);
    if (ret != ESP_OK) {return ret;}

}