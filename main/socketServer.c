/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
//#include "protocol_examples_common.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include "socketServer.h"
#include "util.h"
#include "table.h"
#include "app.h"
#include "target.h"
#include "databuffer.h"

#define PORT 8888

static const char *ack="ok";

static const char *testSocketServer = "socket server";

T8U GOT_SERVERIP = false;

static void do_retransmit(const int sock)
{
	#define ADDRESS	0
	#define SERIAL 1

//	TBL_SN tbl_sn;

    int len;
    char rx_buffer[64];
//	char ipAddress[15]={0};
	char terminalID[7]={0};
//	char serialNumber[12]={0};
	int i = 0;
//	char *p1;
//	char *p2;
    do {
        len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
        if (len < 0) {
            ESP_LOGE(testSocketServer, "Error occurred during receiving: errno %d", errno);
        } else if (len == 0) {
            ESP_LOGW(testSocketServer, "Connection closed");
        } else {
        	int state;
            rx_buffer[len] = 0; // Null-terminate whatever is received and treat it like a string
            ESP_LOGI(testSocketServer, "Received %d bytes: %s", len, rx_buffer);
            if((rx_buffer[0] == 'F') && (rx_buffer[1] == '0'))
            {
            	state = ADDRESS;
            	for(i = 3; i < len; i++)
				{
					if(rx_buffer[i] == ',')
					{
						break;
					}
				}
            	StrNCpy((T8U*)&terminalID, (T8U*)&rx_buffer[3], i-3);
				int a=len-(i+1);
				memset((T8U*)&tbl_server, 0, sizeof(tbl_server));
				StrNCpy((T8U*)&tbl_server.IPv4, (T8U*)&rx_buffer[i+1], a);
            }
            else if((rx_buffer[0] == 'F') && (rx_buffer[1] == '1'))
            {
            	state = SERIAL;
            	GOT_SERVERIP = false;
//            	StrNCpy((T8U*)&serialNumber, (T8U*)&rx_buffer[3], len-3);
            	StrNCpy((T8U*)&tbl_sn.serialnumber[0], (T8U*)&rx_buffer[3], LEN_SN);

            	WriteTable(&T_SN, (T8U *) &tbl_sn, 0);
            	memset(&tbl_sn, 0, sizeof(tbl_sn));
            	ReadTable(&T_SN, (T8U *)&tbl_sn, 0);

            	ESP_LOGI(testSocketServer, "Serial number is %-12s", tbl_sn.serialnumber);
            }
            else
            {
            	state = ADDRESS;
				for(i = 0; i < len; i++)
				{
					if(rx_buffer[i] == ',')
					{
						break;
					}
				}
				StrNCpy((T8U*)&terminalID, (T8U*)&rx_buffer, i);
				int a=len-(i+1);
				memset((T8U*)&tbl_server, 0, sizeof(tbl_server));
				StrNCpy((T8U*)&tbl_server.IPv4, (T8U*)&rx_buffer[i+1], a);
            }
//			p1=ipAddress;
//			p2=rx_buffer;
//			p2+=7;
//			while(a--) *(p1++)=*(p2++);
//	        *(p1++)='\0';
            if(state == ADDRESS)
            {
            	LOG_ERROR log_error;

				tbl_addr.addr = atol(terminalID);
				WriteTable(&T_ADDR, (T8U *) &tbl_addr, 0);
				tbl_server.hasSet = SPECIAL_ON;
	//			sprintf((char*)&tbl_server.IPv4, "10.1.1.29");//137.116.129.169");//cloud server
				sprintf((char*)&tbl_server.port, "80");
				WriteTable(&T_SERVER, (T8U *) &tbl_server, 0);
				MenuIdle_State = 0;

				ESP_LOGI(testSocketServer, "ip address is %s", tbl_server.IPv4);
				ESP_LOGI(testSocketServer, "terminalID is : %u", tbl_addr.addr);

				GOT_SERVERIP = true;

				TOP_Message |= TOPDATA_UPDATETERMINFO;

				memset((char*)&log_error, 0, sizeof(log_error));
				log_error.type = 1;
				StrNCpy(log_error.employeecode , tbl_emp.empcode, sizeof(log_error.employeecode));
				sprintf((char*)&log_error.functionName, "LinkMobileApps");
				sprintf((char*)&log_error.extra.errorMessage, "{NewAddr=%u,NewServerIP=%s}", tbl_addr.addr, tbl_server.IPv4);
				save_lorecord(&DATA_LOG_ERROR, (T8U *)&log_error);
            }


            // send() can return less bytes than supplied length.
            // Walk-around for robust implementation.
            //int to_write = len;
			int to_write = 2;
            while (to_write > 0) {
                int written = send(sock, ack + (0), to_write, 0);
                if (written < 0) {
                    ESP_LOGE(testSocketServer, "Error occurred during sending: errno %d", errno);
                }
                to_write -= written;
            }
        }
    } while (len > 0);
}

static void tcp_server_task(void *pvParameters)
{
    char addr_str[128];
    int addr_family = (int)pvParameters;
    int ip_protocol = 0;
    struct sockaddr_in6 dest_addr;//sockaddr_in dest_addr;//


	struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
	dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
	dest_addr_ip4->sin_family = AF_INET;
	dest_addr_ip4->sin_port = htons(PORT);
	ip_protocol = IPPROTO_IP;


    int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (listen_sock < 0) {
        ESP_LOGE(testSocketServer, "Unable to create socket: errno %d", errno);
        FLAG_STATE |= F_SOCKETFAILED;
        vTaskDelete(NULL);
        return;
    }
/*
#if defined(CONFIG_EXAMPLE_IPV4) && defined(CONFIG_EXAMPLE_IPV6)
    // Note that by default IPV6 binds to both protocols, it is must be disabled
    // if both protocols used at the same time (used in CI)
    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(listen_sock, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt));
#endif
*/

    ESP_LOGI(testSocketServer, "Socket created");

    int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
        ESP_LOGE(testSocketServer, "Socket unable to bind: errno %d", errno);
        ESP_LOGE(testSocketServer, "IPPROTO: %d", addr_family);
        FLAG_STATE |= F_SOCKETFAILED;
        goto CLEAN_UP;
    }
    ESP_LOGI(testSocketServer, "Socket bound, port %d", PORT);

    err = listen(listen_sock, 1);
    if (err != 0) {
        ESP_LOGE(testSocketServer, "Error occurred during listen: errno %d", errno);
        FLAG_STATE |= F_SOCKETFAILED;
        goto CLEAN_UP;
    }

    FLAG_STATE |= F_SOCKETOPENED;

    while (1) {

        ESP_LOGI(testSocketServer, "Socket listening");

        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        uint addr_len = sizeof(source_addr);
        int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (sock < 0) {
            ESP_LOGE(testSocketServer, "Unable to accept connection: errno %d", errno);
            FLAG_STATE |= F_SOCKETFAILED;
            break;
        }

        // Convert ip address to string

        inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);

        ESP_LOGI(testSocketServer, "Socket accepted ip address: %s", addr_str);

        do_retransmit(sock);

        shutdown(sock, 0);
        close(sock);

        if(GOT_SERVERIP) break;
    }

CLEAN_UP:
	FLAG_STATE &= ~F_SOCKETOPENED;
    close(listen_sock);
    vTaskDelete(NULL);
}

void test_socket(void)
{

    xTaskCreate(tcp_server_task, "tcp_server", 8192, (void*)AF_INET, 5, NULL);

}
