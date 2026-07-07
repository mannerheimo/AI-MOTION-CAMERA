idf_component_register(
    SRCS "camera/OV2640.cpp" "main.cpp" "wifi/wifi.cpp"
    INCLUDE_DIRS "." "wifi" "camera"
    REQUIRES esp_wifi esp_driver_gpio
    
)