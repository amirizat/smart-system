#include "lvgl.h"
#include "lvgl_helpers.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h> // For usleep()
#include "display.h"
#include "keypad.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_heap_caps.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_timer.h"
#include "handleSpiffs.h"
#include "handlelittleFS.h"
#include "esp_wifi.h"
#include "SmartWifi.h"
#include "ledBuzz.h"
#include <rtc_sntp.h>

static const char* TAG = "main";

T32U FLAG_STATE = 0;
T8U temp_prevsec = 0;
T8U temp_calsec = 30;
T8U temp_prevmin = 0;
T8U flag_user_received = 0;
T8U myIP[16] = {"-"};
T8U SAVE_DATA = 0;
T8U macAddr[6] = {};
T16U nosendactivity_mincount = 0;
T8U wifi_hasSet = false;
T8S wifiRssi = 0;
esp_timer_handle_t handler_timer50ms;
esp_timer_handle_t handler_timer1ms;
DATETIME currenttime = {22, 1, 1, 0, 0, 0, 0};
DATETIME rtcexttime = {0, 0, 0, 0, 0, 0, 0};
SemaphoreHandle_t xGuiSemaphore;

void timer_init(void);
static void timer_periodic50ms(void* arg);
void callback_50ms(void);
void callback_100ms(void);

void app_main() {

    ESP_LOGI(TAG, "------------------------------Application----------------------------------------");
	ESP_LOGI(TAG, "Version: %s", _VERSION);
	ESP_LOGI(TAG, "%s", esp_get_idf_version());
	ESP_LOGI(TAG, "main core: %u", xPortGetCoreID());
	ESP_LOGI(TAG, "RAM left %d", esp_get_free_heap_size());
	T8U temp = 0;

	initSpiffs();
	initLittleFs();

	vTaskDelay(1);
	
	display_init();
    img_show_gpro();
    lv_task_handler();

    // Power_Up();

    vTaskDelay(1);
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());


    set_ui_timer();
    vTaskDelay(1);

    initialize_wifi();

    PinOutput_init();
	PinInput_init();
	timer_init();

    RTC_Init(&currenttime);

    // task2init();
	// task3init();
	vTaskDelay(1/portTICK_RATE_MS);

	clear_display();
	header_display();
    border_display();

    Output_AddTask(&led1, OUTPUT_STATE_ON, 10, 0, 0);

    ESP_LOGI(TAG, "RAM left %d", esp_get_free_heap_size());
    RTC_get(&currenttime);
    RTC_print(&currenttime);

    T8U tempstring[50] = {""};
    T8U key;

    while (1) {
        key = Keypad_Scan();
        sprintf((char*)&tempstring, "Keypad %c Pressed",key);
        if(key=='A')
        {
            clear_display();
            text_display((char*)&tempstring,0,10);
        }
        else if(key=='B')
        {
            clear_display();
            text_display((char*)&tempstring,0,10);
        }
        else if(key=='C')
        {
            clear_display();
            text_display((char*)&tempstring,0,10);
        }
        else if(key=='D')
        {
            clear_display();
            img_show_gpro();
        }
        lv_task_handler();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void timer_init(void)
{
	const esp_timer_create_args_t timer_periodic50ms_args = {
	         .callback = &timer_periodic50ms,
	         .name = "timer_periodic50ms"
	};
	ESP_ERROR_CHECK(esp_timer_create(&timer_periodic50ms_args, &handler_timer50ms));
	ESP_ERROR_CHECK(esp_timer_start_periodic(handler_timer50ms, TICK50MS));
}

static void timer_periodic50ms(void* arg)
{
	static T8U count;
    FLAG_STATE |= F_T50MS;
	count++;
	nosendactivity_mincount++;
	if(count > 1)
	{
		FLAG_STATE |= F_T100MS;
		count = 0;
	}
}
