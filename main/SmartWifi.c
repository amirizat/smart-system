/* Example of FAT filesystem on external Flash.
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.

   This sample shows how to store files inside a FAT filesystem.
   FAT filesystem is stored in a partition inside SPI flash, using the
   flash wear levelling library.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "esp_flash.h"
#include "esp_flash_spi_init.h"
#include "esp_partition.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_wpa2.h"
#include "esp_event.h"
#include "esp_log.h"
//#include "nvs_flash.h"
#include "tcpip_adapter.h"
#include "esp_smartconfig.h"
#include <sys/param.h>
//#include "smart_config.h"

//#include "file.h"
//#include "socket.h"
#include "SmartWifi.h"
#include "handleSpiffs.h"
#include "handlelittleFS.h"
#include "table.h"
#include "util.h"
#include "socketServer.h"
#include "Wifi_Espnow.h"

static EventGroupHandle_t s_wifi_event_group;
static const int CONNECTED_BIT = BIT0;
static const int ESPTOUCH_DONE_BIT = BIT1;
static const char *TAG = "SmartWifi";
//int reset=0;//can be enable back if want to reboot before used
uint8_t ssid[33] = { 0 };
uint8_t password[65] = { 0 };
//int finish_wifi_flag=0;
uint8_t count_failed = 0;
uint8_t smartConfig = 0;
uint8_t wifiOFF = YES;
T8U wifi_scan = NO;
T8U flag_espnow=0;
T8U changeState=0;

void reConnectWifi2(void);
void delay_scan(void *param);
void espnow_receive(void *param);

TaskHandle_t task_sniff = NULL; // Handle for Task 1
TaskHandle_t task_espnow = NULL; // Handle for Task 2

static void smartconfig_example_task(void * parm)
{
    EventBits_t uxBits;
    ESP_ERROR_CHECK( esp_smartconfig_set_type(SC_TYPE_ESPTOUCH) );
    smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_smartconfig_start(&cfg) );
    while (1)
    {
    	vTaskDelay(1);
        uxBits = xEventGroupWaitBits(s_wifi_event_group, CONNECTED_BIT | ESPTOUCH_DONE_BIT, true, false, portMAX_DELAY);
        if(uxBits & CONNECTED_BIT)
        {
            ESP_LOGI(TAG, "WiFi Connected to ap");
            tbl_wifi_diag.ip_lost = NO;
            flag_espnow=0;
        }
        if(uxBits & ESPTOUCH_DONE_BIT)
        {
        	smartConfig = 0;
        	FLAG_STATE &= ~F_WIFISNIFFING;
            ESP_LOGI(TAG, "smartconfig over");
            esp_smartconfig_stop();
//            if(reset==1){
//            	save_data(password,ssid);
            	saveWifiParam(ssid,password);
//                esp_restart();
//            }
//            reset=1;
//            break;
            	vTaskDelete(NULL);
        }
        else if(smartConfig == 0)
        {
        	FLAG_STATE &= ~F_WIFISNIFFING;
            ESP_LOGI(TAG, "smartconfig over");
            esp_smartconfig_stop();
            vTaskDelete(NULL);
        }
    }
//     vTaskDelete(NULL);
}

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
    	tbl_wifi_diag.ip_lost = YES;
        const char *name;
        int err;
        char hostname[20] = "";
        sprintf(hostname, "G2_%u", tbl_addr.addr);
        // Set the hostname for the default TCP/IP station interface
        err = tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA, hostname);
        if (err != ESP_OK) {
        	ESP_LOGE(TAG, "Set Hostname Err: %s", esp_err_to_name(err));
        } else {
        	err = tcpip_adapter_get_hostname(TCPIP_ADAPTER_IF_STA, &name);
            if (err != ESP_OK) {
            	ESP_LOGE(TAG, "Get Hostname Err: %s", esp_err_to_name(err));
            } else {
            	ESP_LOGI(TAG, "Hostname: %s\n", (name == NULL ? "<None>" : name));
            }
        }

		if((FLAG_STATE & F_WIFISNIFFER) > 0)
		{
			smartConfig = 1;
			FLAG_STATE |= F_WIFISNIFFING;
			xTaskCreate(smartconfig_example_task, "smartconfig_example_task", 8192, NULL, 3, task_sniff);
			xTaskCreate(espnow_receive, "espnow_receive", 8192, NULL, 2, task_espnow);
			wifi_scan = NO;
		}
		else
		{
			wifiOFF = NO;
//			esp_wifi_connect();
			scanWifi();
		}
		ESP_LOGI(TAG, "WIFI_EVENT_STA_START");
    }
//    WIFI_EVENT_STA_CONNECTED
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED)
    {
    	wifi_ap_record_t ap_info;
//    	T8U chck[1]={"-"};
    	ESP_LOGI(TAG, "WIFI_EVENT_STA_CONNECTED");

    	ESP_ERROR_CHECK(esp_wifi_sta_get_ap_info(&ap_info));
    	StrNCpy(&tbl_wifi_diag.bssid[tbl_wifi_diag.cur_use][0], ap_info.bssid, 6);

//    	if(0 != StrNCmp(&myIP[0], &chck[0], sizeof(myIP)))
//    	{
    		tbl_wifi_diag.ip_lost = YES;
    		ESP_LOGI(TAG, "WIFI CONNECTED BUT NO IP");
//    	}
    }
    else if ((event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) ||
    		(event_base == IP_EVENT && event_id ==IP_EVENT_STA_LOST_IP))
    {
    	sprintf((char*)&myIP, "-");
    	FLAG_STATE &= ~F_WIFICONNECTING;
    	tbl_wifi_diag.ip_lost = NO;

    	if(event_base == IP_EVENT && event_id ==IP_EVENT_STA_LOST_IP){
        	ESP_LOGI(TAG, "IP_EVENT_STA_LOST_IP");
        	tbl_wifi_diag.ip_lost = YES;
    	}

        ESP_LOGI(TAG, "WIFI_EVENT_STA_DISCONNECTED");
        wifi_event_sta_disconnected_t *disconnected = (wifi_event_sta_disconnected_t*) event_data;
        ESP_LOGE(TAG, "Disconnected reason:%d", disconnected->reason);
        tbl_wifi_diag.reason_disconn = disconnected->reason;
    //    	////////reason code////////////refer esp_wifi_types.h

//    	typedef enum {
//    	    WIFI_REASON_UNSPECIFIED              = 1,
//    	    WIFI_REASON_AUTH_EXPIRE              = 2,
//    	    WIFI_REASON_AUTH_LEAVE               = 3,
//    	    WIFI_REASON_ASSOC_EXPIRE             = 4,
//    	    WIFI_REASON_ASSOC_TOOMANY            = 5,
//    	    WIFI_REASON_NOT_AUTHED               = 6,
//    	    WIFI_REASON_NOT_ASSOCED              = 7,
//    	    WIFI_REASON_ASSOC_LEAVE              = 8,
//    	    WIFI_REASON_ASSOC_NOT_AUTHED         = 9,
//    	    WIFI_REASON_DISASSOC_PWRCAP_BAD      = 10,
//    	    WIFI_REASON_DISASSOC_SUPCHAN_BAD     = 11,
//    	    WIFI_REASON_BSS_TRANSITION_DISASSOC  = 12,
//    	    WIFI_REASON_IE_INVALID               = 13,
//    	    WIFI_REASON_MIC_FAILURE              = 14,
//    	    WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT   = 15,
//    	    WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT = 16,
//    	    WIFI_REASON_IE_IN_4WAY_DIFFERS       = 17,
//    	    WIFI_REASON_GROUP_CIPHER_INVALID     = 18,
//    	    WIFI_REASON_PAIRWISE_CIPHER_INVALID  = 19,
//    	    WIFI_REASON_AKMP_INVALID             = 20,
//    	    WIFI_REASON_UNSUPP_RSN_IE_VERSION    = 21,
//    	    WIFI_REASON_INVALID_RSN_IE_CAP       = 22,
//    	    WIFI_REASON_802_1X_AUTH_FAILED       = 23,
//    	    WIFI_REASON_CIPHER_SUITE_REJECTED    = 24,
//
//    	    WIFI_REASON_INVALID_PMKID            = 53,
//
//    	    WIFI_REASON_BEACON_TIMEOUT           = 200,
//    	    WIFI_REASON_NO_AP_FOUND              = 201,
//    	    WIFI_REASON_AUTH_FAIL                = 202,
//    	    WIFI_REASON_ASSOC_FAIL               = 203,
//    	    WIFI_REASON_HANDSHAKE_TIMEOUT        = 204,
//    	    WIFI_REASON_CONNECTION_FAIL          = 205,
//    	    WIFI_REASON_AP_TSF_RESET             = 206,
//    	    WIFI_REASON_ROAMING                  = 207,
//    	} wifi_err_reason_t;
//    	///////////////////////////////////////////////////

    	FLAG_STATE &= ~F_WIFICONNECT;
    	FLAG_STATE &= ~F_CONNECTSERVER;
		FLAG_STATE |= F_UPDATEHEADER;

//        if(!wifiOFF) esp_wifi_connect();
//		if(set_wifi_flag==0){
			xEventGroupClearBits(s_wifi_event_group, CONNECTED_BIT);
//		}

//		wifi_hasSet = 0;//tbl_wifi.tellUser = true;
//		FLAG_STATE &= ~F_SENDINGDATA;
////		FLAG_STATE |= F_RECONNECTWIFI2;
//		if(!(FLAG_STATE & F_WIFISNIFFER))
//		{
//			wifi_scan = YES;
//			xTaskCreate(delay_scan, "delay_scan", 4096, NULL, 10, NULL);
//		}

		if(count_failed > 200)
		{
			count_failed++;
			if(count_failed > 215)
			{
				count_failed = 0;
				StrNCpy(tbl_wifi.ssid, ssid, sizeof(tbl_wifi.ssid));
				wifi_hasSet = 2;//tbl_wifi.tellUser = true;
			}
			esp_wifi_connect();
		}
		else// if(count_failed < 100)
		{
			count_failed++;
			if((count_failed % 5) == 0)
			{
				tbl_wifi_diag.cur_use++;
				if((tbl_wifi_diag.cur_use >= MAX_AP) || (tbl_wifi_diag.rssi[tbl_wifi_diag.cur_use] == 0))
				{
					tbl_wifi_diag.cur_use--;
					count_failed = 0;
					FLAG_STATE |= F_RECONNECTWIFI2;
					FLAG_STATE &= ~F_RECONNECTWIFI;
				}
				else
				{
					FLAG_STATE |= F_RECONNECTWIFI;
				}
			}
			else
			{
				FLAG_STATE |= F_WIFICONNECTING;
				esp_wifi_connect();
			}
		}

    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
//		if(set_wifi_flag==1){
			ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
			ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
//			start_socket();
//			finish_wifi_flag=1;
			sprintf((char*)&myIP, IPSTR, IP2STR(&event->ip_info.ip));

	    	wifi_ap_record_t ap;
	    	esp_wifi_sta_get_ap_info(&ap);
//	    	ESP_LOGI(TAG, "Wifi Rssi: %d\n", ap.rssi);
	    	wifiRssi = ap.rssi;

			FLAG_STATE &= ~F_WIFICONNECTING;
			FLAG_STATE |= F_WIFICONNECT;
			FLAG_STATE |= F_UPDATEHEADER;
			count_failed = 0;
			tbl_wifi_diag.reason_disconn = 0 ;
//		}else{
			xEventGroupSetBits(s_wifi_event_group, CONNECTED_BIT);
//		}
		ESP_LOGI(TAG, "IP_EVENT_STA_GOT_IP");

		if(tbl_server.hasSet != SPECIAL_ON)//((FLAG_STATE & F_WIFISNIFFER) || (tbl_server.hasSet != SPECIAL_ON))
		{
			vTaskDelay(1);
//			vTaskDelay(3000/portTICK_RATE_MS);
			test_socket();
		}
		//create task to send databuffer
//		xTaskCreate(	sendingTask,       /* Task function. */
//		           	   	"sendingTask",     /* String with name of task. */
//		                 10000,            /* Stack size in bytes. */
//		                 NULL,             /* Parameter passed as input of the task */
//		                 1,                /* Priority of the task. */
//		                 NULL);            /* Task handle. */
//		//will use http task to do requesting and sending
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE)
    {
        ESP_LOGI(TAG, "Scan done");
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL)
    {
        ESP_LOGI(TAG, "Found channel");
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD)
    {
        ESP_LOGI(TAG, "Got SSID and password");

        smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
        wifi_config_t wifi_config;

        bzero(&wifi_config, sizeof(wifi_config_t));
        memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
        memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));
        wifi_config.sta.bssid_set = evt->bssid_set;
        if (wifi_config.sta.bssid_set == true)
        {
            memcpy(wifi_config.sta.bssid, evt->bssid, sizeof(wifi_config.sta.bssid));
        }

        memcpy(ssid, evt->ssid, sizeof(evt->ssid));
        memcpy(password, evt->password, sizeof(evt->password));
        ESP_LOGI(TAG, "SSID:%s", ssid);
        ESP_LOGI(TAG, "PASSWORD:%s", password);

        wifiOFF = YES;
        ESP_ERROR_CHECK( esp_wifi_disconnect() );
        ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
        wifiOFF = NO;
        FLAG_STATE |= F_WIFICONNECTING;
        ESP_ERROR_CHECK( esp_wifi_connect() );

        count_failed = 201;
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE)
    {
//		int i = eTaskGetState(smartconfig_example_task);
//		ESP_LOGE(TAG, "smartconfig_example_task 1: %u", i);
        xEventGroupSetBits(s_wifi_event_group, ESPTOUCH_DONE_BIT);

//        if(reset==1){
//          save_data(password,ssid);
//        	saveWifiParam(ssid,password);
//          esp_restart();
//        }
//        reset=1;
        ESP_LOGI(TAG, "SC_EVENT_SEND_ACK_DONE");
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_SCAN_DONE)
    {
    	uint16_t apCount = 0;
    	memset(&tbl_wifi_diag, 0, sizeof(tbl_wifi_diag));
    	esp_wifi_scan_get_ap_num(&apCount);
    	wifi_ap_record_t *list = (wifi_ap_record_t *)malloc(sizeof(wifi_ap_record_t) * apCount);
    	ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&apCount, list));
		int i , j = 0;
		printf("======================================================================\n");
		printf("             SSID             |    RSSI    |           AUTH           \n");
		printf("======================================================================\n");
		for (i=0; i<apCount; i++) {
			char *authmode;
			switch(list[i].authmode) {
			case WIFI_AUTH_OPEN:
               authmode = "WIFI_AUTH_OPEN";
               break;
            case WIFI_AUTH_WEP:
               authmode = "WIFI_AUTH_WEP";
               break;
            case WIFI_AUTH_WPA_PSK:
               authmode = "WIFI_AUTH_WPA_PSK";
               break;
            case WIFI_AUTH_WPA2_PSK:
               authmode = "WIFI_AUTH_WPA2_PSK";
               break;
            case WIFI_AUTH_WPA_WPA2_PSK:
               authmode = "WIFI_AUTH_WPA_WPA2_PSK";
               break;
            default:
               authmode = "Unknown";
               break;
			}
			printf("%26.26s    |    % 4d    |    %22.22s\n",list[i].ssid, list[i].rssi, authmode);

			if((StrNCmp(tbl_wifi.ssid, list[i].ssid, strlen((char *)&list[i].ssid)) == 0) && (j < MAX_AP))
			{
				StrNCpy(&tbl_wifi_diag.bssid[j][0], list[i].bssid, 6);
				tbl_wifi_diag.rssi[j] = list[i].rssi;
				j++;
			}
//	        if ((strcmp((char*)list[i].ssid, (char*)tbl_wifi.ssid) == 0)&&(!(FLAG_STATE & F_WIFISNIFFER))) {
//	            printf("Found network %s\n", (char*)tbl_wifi.ssid);
//	    		tbl_wifi_diag.cur_use = 0; //reset to first because first normally have better RSSI
//	    		reConnectWifi2();
//	    		wifi_scan = NO;
//	    		xTaskCreate(delay_scan, "delay_scan", 4096, NULL, 10, NULL);
//	            return;
//	        }
		}
		free(list);

		if((j == 0) && (!(FLAG_STATE & F_WIFISNIFFER)))
		{
			wifi_scan = YES;
			xTaskCreate(delay_scan, "delay_scan", 4096, NULL, 10, NULL);
		}
		else
		{
			tbl_wifi_diag.cur_use = 0; //reset to first because first normally have better RSSI
			reConnectWifi2();
		}
    }
}

void initialize_wifi(void)
{
//	tcpip_adapter_init();
//	s_wifi_event_group = xEventGroupCreate();
//	ESP_ERROR_CHECK(esp_event_loop_create_default());
//	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//	ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK(esp_netif_init());
    s_wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );

    vTaskDelay(1);

//	if(pg_wifi.tbl_wifi.hasSet == SPECIAL_ON)
	if(tbl_wifi.hasSet == SPECIAL_ON)
	{
		ESP_LOGI(TAG, "ready to set wifi");
		//    	read_ssid_password_file_and_set_wifi();

//		ESP_LOGI(TAG, "ssid read: %s", pg_wifi.tbl_wifi.ssid);
//		ESP_LOGI(TAG, "password read: %s", pg_wifi.tbl_wifi.pssw);
//
//		set_wifi((char*)pg_wifi.tbl_wifi.ssid,(char*)pg_wifi.tbl_wifi.pssw);

		ESP_LOGI(TAG, "ssid read: %s", tbl_wifi.ssid);
		ESP_LOGI(TAG, "password read: %s", tbl_wifi.pssw);
//		set_wifi((char*)tbl_wifi.ssid,(char*)tbl_wifi.pssw, (char*)tbl_wifi_diag.bssid);
		set_wifi((char*)tbl_wifi.ssid,(char*)tbl_wifi.pssw);
	}
	else
	{ // The data has not been saved
		FLAG_STATE |= F_WIFISNIFFER;
		ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));
		ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, NULL));
		ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_LOST_IP, &event_handler, NULL, NULL));
		ESP_ERROR_CHECK(esp_event_handler_instance_register(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));
		ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, &event_handler, NULL, NULL));
		//		   ESP_ERROR_CHECK( esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );
		//		   ESP_ERROR_CHECK( esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL) );
		//		   ESP_ERROR_CHECK( esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );
		ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
		ESP_ERROR_CHECK( esp_wifi_start() );
		ESP_LOGE(TAG, "ready to set wifi again");
	}
}

//void set_wifi(char *ssid , char *password, char *bssid)
void set_wifi(char *ssid , char *password)
{
//	set_wifi_flag=1;

	wifi_config_t wifi_config_n;
	bzero(&wifi_config_n, sizeof(wifi_config_t));
	memcpy(wifi_config_n.sta.ssid, ssid, sizeof(wifi_config_n.sta.ssid));
	memcpy(wifi_config_n.sta.password, password, sizeof(wifi_config_n.sta.password));
//	memcpy(wifi_config_n.sta.bssid, bssid, sizeof(wifi_config_n.sta.bssid));

	ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));
	ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, NULL));


	vTaskDelay(1);
	// Initialize default station as network interface instance (esp-netif)
//	esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
//	assert(sta_netif);

	ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
  // ESP_ERROR_CHECK( esp_wifi_disconnect() );
	ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config_n) );
	ESP_ERROR_CHECK( esp_wifi_start() );
 //  ESP_ERROR_CHECK( esp_wifi_connect() );

}

//int get_finish_wifi_flag()
//{
//	return finish_wifi_flag;
//}

void saveWifiParam(T8U*ssid, T8U*password)
{
////	ESP_LOGI(TAG, "saveWifiParam ssid: %s", ssid);
////	ESP_LOGI(TAG, "saveWifiParam password: %s", password);
//	pg_wifi.tbl_wifi.hasSet = SPECIAL_ON;
//	StrNCpy(pg_wifi.tbl_wifi.ssid, ssid, sizeof(pg_wifi.tbl_wifi.ssid));
//	StrNCpy(pg_wifi.tbl_wifi.pssw, password, sizeof(pg_wifi.tbl_wifi.pssw));
//
////	ESP_LOGI(TAG, "saveWifiParam StrNCpy ssid: %s", pg_wifi.tbl_wifi.ssid);
////	ESP_LOGI(TAG, "saveWifiParam StrNCpy password: %s", pg_wifi.tbl_wifi.pssw);
//	writeSpiffsPage(FILE_WIFI, sizeof(pg_wifi), (T8U*)&pg_wifi);
////
////	ESP_ERROR_CHECK( esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler) );
////	ESP_ERROR_CHECK( esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler) );
////	ESP_ERROR_CHECK( esp_event_handler_unregister(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler) );
////	ESP_ERROR_CHECK( esp_wifi_stop() );
////	ESP_ERROR_CHECK( esp_netif_deinit());
////	ESP_ERROR_CHECK( esp_wifi_disconnect());
////
////	set_wifi((char*)ssid, (char*)password);

	memset((T8U *)&tbl_wifi,'\0',sizeof(tbl_wifi));

	tbl_wifi.hasSet = SPECIAL_ON;
	StrNCpy(tbl_wifi.ssid, ssid, sizeof(tbl_wifi.ssid));
	StrNCpy(tbl_wifi.pssw, password, sizeof(tbl_wifi.pssw));
	WriteTable(&T_WIFI, (T8U *) &tbl_wifi, 0);
	if((FLAG_STATE & F_WIFISNIFFER) != 0)
	{
		wifi_hasSet = 1; //tbl_wifi.userAck = true;
	}
}

void reSniffWifi(void)
{
	ESP_ERROR_CHECK(esp_smartconfig_stop());

	if((FLAG_STATE & F_WIFISNIFFER) == 0)
	{
		FLAG_STATE |= F_WIFISNIFFER;
		ESP_ERROR_CHECK(esp_event_handler_instance_register(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));
	}

	wifiOFF = NO;
	ESP_ERROR_CHECK( esp_wifi_stop() );
	vTaskDelay(5000/portTICK_RATE_MS);

	ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
	ESP_ERROR_CHECK( esp_wifi_start() );
	ESP_LOGE(TAG, "ready to set wifi again");
}

//void reConnectWifi(void)
//{
////	wifi_config_t wifi_config_n;
////	ESP_ERROR_CHECK(esp_smartconfig_stop());
////
////	ESP_ERROR_CHECK( esp_wifi_stop() );
////	vTaskDelay(5000/portTICK_RATE_MS);
////
////	bzero(&wifi_config_n, sizeof(wifi_config_t));
////	memcpy(wifi_config_n.sta.ssid, (char*)tbl_wifi.ssid, sizeof(wifi_config_n.sta.ssid));
////	memcpy(wifi_config_n.sta.password, (char*)tbl_wifi.pssw, sizeof(wifi_config_n.sta.password));
////
////	ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
////	ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config_n) );
////	ESP_ERROR_CHECK( esp_wifi_start() );
//
//	esp_wifi_disconnect();
//	vTaskDelay(5000/portTICK_RATE_MS);
//	esp_wifi_connect();
//}

void reConnectWifi(void)
{
	wifi_config_t wifi_config_n;

	ReadTable(&T_WIFI, (T8U *) &tbl_wifi, 0);
	ReadTable(&T_SERVER, (T8U *) &tbl_server, 0);

	if(tbl_wifi.hasSet == SPECIAL_ON)
	{
		bzero(&wifi_config_n, sizeof(wifi_config_t));
		if(tbl_wifi_diag.rssi[tbl_wifi_diag.cur_use] != 0)
		{
			memcpy(wifi_config_n.sta.bssid, (char*)&tbl_wifi_diag.bssid[tbl_wifi_diag.cur_use][0], 6);
			ESP_LOGI(TAG, "bssid: %02X:%02X:%02X:%02X:%02X:%02X", tbl_wifi_diag.bssid[tbl_wifi_diag.cur_use][0],
																  tbl_wifi_diag.bssid[tbl_wifi_diag.cur_use][1],
																  tbl_wifi_diag.bssid[tbl_wifi_diag.cur_use][2],
																  tbl_wifi_diag.bssid[tbl_wifi_diag.cur_use][3],
																  tbl_wifi_diag.bssid[tbl_wifi_diag.cur_use][4],
																  tbl_wifi_diag.bssid[tbl_wifi_diag.cur_use][5]);
		}
		memcpy(wifi_config_n.sta.ssid, (char*)tbl_wifi.ssid, sizeof(wifi_config_n.sta.ssid));
		memcpy(wifi_config_n.sta.password, (char*)tbl_wifi.pssw, sizeof(wifi_config_n.sta.password));
		wifi_config_n.sta.bssid_set = 1;

		smartConfig = NO;
	}
	else
	{
		return;
	}

	wifiOFF = YES;
	ESP_ERROR_CHECK( esp_wifi_disconnect() );
	vTaskDelay(5000/portTICK_RATE_MS);
	ESP_LOGI(TAG, "WIFI: RECONNECT");
	ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config_n) );
	wifiOFF = NO;
	FLAG_STATE |= F_WIFICONNECTING;
//	ESP_ERROR_CHECK( esp_wifi_connect() );
	if(esp_wifi_connect() != ESP_OK) FLAG_STATE |= F_RECONNECTWIFI;
}

void scanWifi(void)
{
	wifi_scan_config_t config;

	ReadTable(&T_WIFI, (T8U *) &tbl_wifi, 0);
	ReadTable(&T_SERVER, (T8U *) &tbl_server, 0);

	config.ssid                 = 0;
	config.bssid                = 0;
	config.channel              = 0;
	config.show_hidden          = false;
	config.scan_type            = WIFI_SCAN_TYPE_ACTIVE;
	config.scan_time.active.min = 100;
	config.scan_time.active.max = 1500;

	wifiOFF = YES;
	ESP_ERROR_CHECK( esp_wifi_disconnect() );
	ESP_ERROR_CHECK(esp_wifi_scan_start(&config, 1));
}

void reConnectWifi2(void)
{
	wifi_config_t wifi_config_n;

	if(tbl_wifi.hasSet == SPECIAL_ON)
	{
		bzero(&wifi_config_n, sizeof(wifi_config_t));
		if(tbl_wifi_diag.rssi[tbl_wifi_diag.cur_use] != 0)
		{
			memcpy(wifi_config_n.sta.bssid, (char*)&tbl_wifi_diag.bssid[tbl_wifi_diag.cur_use][0], 6);
			ESP_LOGI(TAG, "bssid: %02X:%02X:%02X:%02X:%02X:%02X", tbl_wifi_diag.bssid[tbl_wifi_diag.cur_use][0],
																  tbl_wifi_diag.bssid[tbl_wifi_diag.cur_use][1],
																  tbl_wifi_diag.bssid[tbl_wifi_diag.cur_use][2],
																  tbl_wifi_diag.bssid[tbl_wifi_diag.cur_use][3],
																  tbl_wifi_diag.bssid[tbl_wifi_diag.cur_use][4],
																  tbl_wifi_diag.bssid[tbl_wifi_diag.cur_use][5]);
		}
		memcpy(wifi_config_n.sta.ssid, (char*)tbl_wifi.ssid, sizeof(wifi_config_n.sta.ssid));
		memcpy(wifi_config_n.sta.password, (char*)tbl_wifi.pssw, sizeof(wifi_config_n.sta.password));
		wifi_config_n.sta.bssid_set = 1;

		smartConfig = NO;
	}
	else
	{
		return;
	}

	ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config_n) );
	wifiOFF = NO;
	FLAG_STATE |= F_WIFICONNECTING;
	ESP_ERROR_CHECK( esp_wifi_connect() );
}

void reConnectWifi3(void)
{
	wifi_config_t wifi_config_n;
	ReadTable(&T_WIFI, (T8U *) &tbl_wifi, 0);
	ReadTable(&T_SERVER, (T8U *) &tbl_server, 0);

	if(tbl_wifi.hasSet == SPECIAL_ON)
	{
		bzero(&wifi_config_n, sizeof(wifi_config_t));
		memcpy(wifi_config_n.sta.ssid, (char*)tbl_wifi.ssid, sizeof(wifi_config_n.sta.ssid));
		memcpy(wifi_config_n.sta.password, (char*)tbl_wifi.pssw, sizeof(wifi_config_n.sta.password));

		smartConfig = NO;
	}
	else
	{
		return;
	}

	wifiOFF = YES;
	ESP_ERROR_CHECK( esp_wifi_disconnect() );
	vTaskDelay(5000/portTICK_RATE_MS);
	ESP_LOGI(TAG, "WIFI: RECONNECT");
	ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config_n) );
	wifiOFF = NO;
	FLAG_STATE |= F_WIFICONNECTING;
	ESP_ERROR_CHECK( esp_wifi_connect() );
	if(esp_wifi_connect() != ESP_OK) FLAG_STATE |= F_RECONNECTWIFI;

	if(FLAG_STATE & F_WIFICONNECT) wifi_hasSet = 1;
}

void delay_scan(void *param)
{
	while(1)
	{
		vTaskDelay(1);
		if(wifi_scan == YES)
		{
			tbl_wifi_diag.havessid=NO;
			vTaskDelay(pdMS_TO_TICKS(5000));
			FLAG_STATE |= F_RECONNECTWIFI2;
			ESP_LOGW(TAG,"WIFI SCAN YES");
			break;
		}
		else if(wifi_scan ==NO)
		{
			tbl_wifi_diag.havessid=YES;
			ESP_LOGW(TAG,"WIFI SCAN NO");
			break;

		}
	}
	vTaskDelete(NULL);
}

void espnow_receive(void *param)
{
	static T8U updatenumber = 50;
	T8U receive;

	while(1)
	{
		vTaskDelay(1);
		switch(changeState)
		{
			case 0:
			{
				updatenumber=0;
				if(espnow_init()==_PASS)
				{
					changeState = 1;
					flag_espnow=1;
				}
				else
				{
					deinit_espnow();
					ESP_LOGW(TAG, "ERROR INIT");
				}
				break;
			}
			case 1:
			{
				receive_data();
				receive=check_receive_data(tbl_wifi.ssid,tbl_wifi.pssw);
				if(receive==_PASS)
				{
					FLAG_STATE &= ~F_WIFISNIFFER;
					FLAG_STATE &= ~F_WIFISNIFFING;//
					FLAG_STATE &= ~F_WIFICONNECTING;//
					FLAG_STATE |= F_UPDATEHEADER;//
					esp_smartconfig_stop();//
					deinit_espnow();
					changeState = 2;
				}
				else
				{
					changeState = 1;
				}
				break;
			}
			case 2:
			{
				if(updatenumber < 50)
				{
					updatenumber++;
					if(updatenumber >= 50)
					{
						tbl_wifi.hasSet = SPECIAL_ON;
						WriteTable(&T_WIFI, (T8U *) &tbl_wifi, 0);
						changeState = 3;
						receive = _FAIL;
						reConnectWifi();
					}
				}
				break;
			}
			case 3:
			{
				if(FLAG_STATE & F_WIFICONNECT)
				{
					wifi_hasSet = true;
					flag_espnow=0;
					changeState = 0;
//					tbl_wifi.userAck = true;
					vTaskDelete(task_sniff);
					vTaskDelete(task_espnow);
				}
				break;
			}
			default:
			{
				changeState = 0;
				break;
			}
		}
	}
}
