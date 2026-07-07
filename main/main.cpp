#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "OV2640.h"
#include "wifi.h"
static const char *TAG = "MAIN";
void wifi_task(void *pvParameters) {
    
    ESP_LOGI(TAG, "wifi turning on");
    
    Wifi myWifi;
    if (myWifi.initWifi() == ESP_OK) {
        ESP_LOGI(TAG, "wifi init succeeded!, continue");
    } else {
        ESP_LOGI(TAG, "wifi init failed!");
    }
    vTaskDelete(NULL); 
}

extern "C" void app_main(void) {

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    xTaskCreate(wifi_task, "wifi_task", 4096, NULL, 5, NULL);
    
}
