#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include "esp_log.h"
#include <esp_now.h>
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "table.h"
#include "util.h"

static const char* TAG = "ESP_NOW";
T8U len_receive;
esp_now_peer_info_t peerInfo;

//static EventGroupHandle_t s_wifi_event_group;

//mac address 34AB95E61670
uint8_t broadcastAddress[] = {0x00, 0xAB, 0x95, 0xE6, 0x16, 0x70};

typedef struct struct_message {
	T8U ssid[33];
	T8U pssw[65];
	bool flag;
} struct_message;

struct_message myData;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
	ESP_LOGI(TAG, "Last Packet Send Status:");
	if(status == ESP_NOW_SEND_SUCCESS)
		ESP_LOGI(TAG, "Sent Success");
	else
		ESP_LOGI(TAG, "Sent Fail");
}

T8U espnow_init(){
	  // Init ESP-NOW
	  if (esp_now_init() != ESP_OK) {
	    ESP_LOGI(TAG, "Error initializing ESP-NOW");
	    return _FAIL;
	  }

	  esp_now_register_send_cb(OnDataSent);

	  // Register peer
	  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
	  peerInfo.channel = 0;
	  peerInfo.encrypt = false;

	  // Add peer
	  if (esp_now_add_peer(&peerInfo) != ESP_OK){
		  ESP_LOGI(TAG, "Failed to add peer");
	    return _FAIL;
	  }
	  ESP_LOGI(TAG, "Espnow init success");
	  return _PASS;
}

void sentData(){
	StrNCpy(myData.ssid, (T8U*)tbl_wifi.ssid,sizeof(myData.ssid));
	StrNCpy(myData.pssw, (T8U*)tbl_wifi.pssw,sizeof(myData.pssw));
	myData.flag = true;

	  // Send message via ESP-NOW
	  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

	  if (result == ESP_OK) {
		  ESP_LOGI(TAG, "Sent Success");
	  }
	  else
	  {
		  ESP_LOGI(TAG, "Sent Fail");
	  }
}

typedef struct struct_message_receive {
	T8U ssid[33];
	T8U pssw[65];
	bool flag;
} struct_message_receive;

struct_message_receive myData_receive;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData_receive, incomingData, sizeof(myData_receive));
  ESP_LOGI(TAG, "SSID:");
  ESP_LOGI(TAG, "%s\n",myData_receive.ssid);
  ESP_LOGI(TAG, "PASS:");
  ESP_LOGI(TAG, "%s\n",myData_receive.pssw);
}

void init_wifi(void)
{

//    ESP_ERROR_CHECK(esp_netif_init());
//    s_wifi_event_group = xEventGroupCreate();
//    ESP_ERROR_CHECK(esp_event_loop_create_default());
//    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
//    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_start());
    ESP_ERROR_CHECK( esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE));


}

void broadcast()
{
	StrNCpy(myData.ssid, tbl_wifi.ssid,sizeof(myData.ssid));
	StrNCpy(myData.pssw, tbl_wifi.pssw,sizeof(myData.pssw));
	myData.flag = true;
  // this will broadcast a message to everyone in range
  uint8_t broadcastAddress1[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  esp_now_peer_info_t peerInfo = {};
  memcpy(&peerInfo.peer_addr, broadcastAddress1, 6);
  if (!esp_now_is_peer_exist(broadcastAddress1))
  {
    esp_now_add_peer(&peerInfo);
  }
//  esp_err_t result = esp_now_send(broadcastAddress, (const uint8_t *)message, message);
  esp_err_t result = esp_now_send(broadcastAddress1, (uint8_t *) &myData, sizeof(myData));
  // and this will send a message to a specific device
  /*uint8_t peerAddress[] = {0x3C, 0x71, 0xBF, 0x47, 0xA5, 0xC0};
  esp_now_peer_info_t peerInfo = {};
  memcpy(&peerInfo.peer_addr, peerAddress, 6);
  if (!esp_now_is_peer_exist(peerAddress))
  {
    esp_now_add_peer(&peerInfo);
  }
  esp_err_t result = esp_now_send(peerAddress, (const uint8_t *)message.c_str(), message.length());*/
  if (result == ESP_OK)
  {
	  ESP_LOGI(TAG, "Broadcast message success");
  }
  else if (result == ESP_ERR_ESPNOW_NOT_INIT)
  {
	  ESP_LOGI(TAG, "ESPNOW not Init");
  }
  else if (result == ESP_ERR_ESPNOW_ARG)
  {
	  ESP_LOGI(TAG, "Invalid Argument");
  }
  else if (result == ESP_ERR_ESPNOW_INTERNAL)
  {
	  ESP_LOGI(TAG, "Internal Error");
  }
  else if (result == ESP_ERR_ESPNOW_NO_MEM)
  {
	  ESP_LOGI(TAG, "ESP_ERR_ESPNOW_NO_MEM");
  }
  else if (result == ESP_ERR_ESPNOW_NOT_FOUND)
  {
	  ESP_LOGI(TAG, "Peer not found");
  }
  else
  {
	  ESP_LOGI(TAG, "Unknown error");
  }
  result = 0;
}

//void app_main(void)
//{
//    // Initialize NVS
//    esp_err_t ret = nvs_flash_init();
//    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
//        ESP_ERROR_CHECK( nvs_flash_erase() );
//        ret = nvs_flash_init();
//    }
//    ESP_ERROR_CHECK( ret );
//	init_wifi();
//	espnow_init();
//    while (true) {
////    	sentData();
//    	esp_now_register_recv_cb(OnDataRecv);
////    	broadcast();
//    	vTaskDelay( 5000 / portTICK_PERIOD_MS );
//    }
//}

void p2p_send(){
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK( nvs_flash_erase() );
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );
	init_wifi();
	espnow_init();
    while (true) {
    	sentData();
    	vTaskDelay( 5000 / portTICK_PERIOD_MS );
    }
}

void receive_data(){
//    esp_err_t ret = nvs_flash_init();
//    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
//        ESP_ERROR_CHECK( nvs_flash_erase() );
//        ret = nvs_flash_init();
//    }
//    ESP_ERROR_CHECK( ret );
//	init_wifi();
//	espnow_init();
//	while(true){
    esp_now_register_recv_cb(OnDataRecv);
//    vTaskDelay( 5000 / portTICK_PERIOD_MS );
//	}
}

void broadcast_data(){
    // Initialize NVS
//    esp_err_t ret = nvs_flash_init();
//    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
//        ESP_ERROR_CHECK( nvs_flash_erase() );
//        ret = nvs_flash_init();
//    }
//    ESP_ERROR_CHECK( ret );
//	init_wifi();
//	espnow_init();
//	while(true){
    broadcast();
//    vTaskDelay( 5000 / portTICK_PERIOD_MS );
//	}
}

void init_espnow(){
    // Initialize NVS
//    esp_err_t ret = nvs_flash_init();
//    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
//        ESP_ERROR_CHECK( nvs_flash_erase() );
//        ret = nvs_flash_init();
//    }
//    ESP_ERROR_CHECK( ret );
//	init_wifi();
//	ESP_ERROR_CHECK( esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE));
	espnow_init();
//	init_wifi();
//	espnow_init();
}

void deinit_espnow(){
	esp_now_deinit();
}

T8U check_receive_data(T8U ssid[33], T8U pass[65])
{

//	strcpy(ssid, myData_receive.ssid);
//	strcpy(pass, myData_receive.pssw);
//
//	sprintf(ssid, "%s", myData_receive.ssid);

	StrNCpy(ssid, (T8U*)myData_receive.ssid,sizeof(myData_receive.ssid));
	StrNCpy(pass, (T8U*)myData_receive.pssw,sizeof(myData_receive.pssw));

	if(myData_receive.flag==true)
	{
//		ESP_LOGI("Receive", "SSID:%s",ssid);
//		ESP_LOGI("Receive", "%s\n",ssid);
//		ESP_LOGI("Receive", "PASS:");
//		ESP_LOGI("Receive", "%s\n",pass);

		myData_receive.flag=false;
//		myData.flag=false;
//		myData_receive.flag=false;
		return _PASS;
	}
	return _FAIL;
}
