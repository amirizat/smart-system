idf_component_register(SRCS "ledBuzz.c" "screen.c" "ota.c" "myhttprequest.c" "SmartWifi.c" "Wifi_Espnow.c" "rtc_sntp.c" 
                    "main.c"
                    "databuffer.c"
                    "handlelittleFS.c"
                    "handleSpiffs.c"
                    "table.c"
                    "util.c"
                    "display.c"
                    "app.c"
                    "keypad.c"
                    "Wifi_Espnow.c"
                    "SmartWifi.c"
                    "socketServer.c"
                    "myhttprequest.c"
                    "ota.c"
                    "screen.c"
                    "ledBuzz.c"
                    INCLUDE_DIRS include
                    REQUIRES  lvgl_esp32_drivers lvgl nvs_flash esp_netif spiffs wpa_supplicant esp-tls
					fatfs esp_http_client esp_http_server esp_https_ota app_update esp_littlefs
                     EMBED_TXTFILES ${project_dir}/server_certs/ca_cert.pem ) 
