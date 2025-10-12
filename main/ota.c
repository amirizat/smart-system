/* Advanced HTTPS OTA example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "target.h"
#include "table.h"
#include "ota.h"
#include "app.h"
#include "screen.h"
#include "display.h"
//#include "protocol_examples_common.h"

#if CONFIG_BOOTLOADER_APP_ANTI_ROLLBACK
#include "esp_efuse.h"
#endif

#if CONFIG_EXAMPLE_CONNECT_WIFI
#include "esp_wifi.h"
#endif

static const char *TAG = "ota";

extern const uint8_t server_cert_pem_start[] asm("_binary_ca_cert_pem_start");
//extern const uint8_t server_cert_pem_end[] asm("_binary_ca_cert_pem_end");

#define OTA_URL_SIZE 256
int a = 0;
T8U FLAG_OTA_CHANGEFIRM = 0;
T8U selected_OTA  = 0;
T32U version = 0;

static esp_err_t validate_image_header(esp_app_desc_t *new_app_info)
{
    if (new_app_info == NULL) {
    	ESP_LOGE(TAG, "ESP_ERR_INVALID_ARG");
        return ESP_ERR_INVALID_ARG;
    }

    const esp_partition_t *running = esp_ota_get_running_partition();
    esp_app_desc_t running_app_info;
    if (esp_ota_get_partition_description(running, &running_app_info) == ESP_OK) {
        ESP_LOGI(TAG, "Running firmware version: %s", running_app_info.version);
    }

    ESP_LOGW(TAG, " the new app version is %s",new_app_info->version);
    int oldVersion=0;
    int newVersion=0;
    oldVersion=atoi(running_app_info.version);
    newVersion=atoi(new_app_info->version);
    version = newVersion;

    if (memcmp(new_app_info->version, running_app_info.version, sizeof(new_app_info->version)) == 0) {
        ESP_LOGW(TAG, "Current running version is the same as a new. Not continue the update. the new app version is %s",new_app_info->version);
        FLAG_STATE |= F_OTASAMEVERSION;
        return ESP_FAIL;
    }

    if(newVersion<=oldVersion){
    	ESP_LOGW(TAG, "Current running version is less than the old one. Not continue the update.");
    	FLAG_STATE |= F_OTALOWVERSION;
    	return ESP_FAIL;
    }


/*
#ifdef CONFIG_BOOTLOADER_APP_ANTI_ROLLBACK
    *
     * Secure version check from firmware image header prevents subsequent download and flash write of
     * entire firmware image. However this is optional because it is also taken care in API
     * esp_https_ota_finish at the end of OTA update procedure.

    const uint32_t hw_sec_version = esp_efuse_read_secure_version();
    if (new_app_info->secure_version < hw_sec_version) {
        ESP_LOGW(TAG, "New firmware security version is less than eFuse programmed, %d < %d", new_app_info->secure_version, hw_sec_version);
        return ESP_FAIL;
    }
#endif
*/

    return ESP_OK;
}

static esp_err_t _http_client_init_cb(esp_http_client_handle_t http_client)
{
    esp_err_t err = ESP_OK;
    return err;
}

void advanced_ota_example_task(void *pvParameter)
{
	char temp_url[100] = {0};
    ESP_LOGI(TAG, "Starting Advanced OTA example");

    sprintf(temp_url, "http://%.15s:80/G2Firmware/G2.bin", tbl_server.IPv4);
    esp_err_t ota_finish_err = ESP_OK;
    esp_http_client_config_t config = {
        .url = temp_url,//"http://192.168.1.9:8888/ota/z5.bin",
        .cert_pem = (char *)server_cert_pem_start,
        .timeout_ms = 3000,
    };

#ifdef CONFIG_EXAMPLE_SKIP_COMMON_NAME_CHECK
    config.skip_cert_common_name_check = true;
#endif

    esp_https_ota_config_t ota_config = {
        .http_config = &config,
        .http_client_init_cb = _http_client_init_cb, // Register a callback to be invoked after esp_http_client is initialized
    };

    esp_https_ota_handle_t https_ota_handle = NULL;
    esp_err_t err = esp_https_ota_begin(&ota_config, &https_ota_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "ESP HTTPS OTA Begin failed");
        FLAG_STATE |= F_OTAFAILED;
        vTaskDelete(NULL);
    }

    esp_app_desc_t app_desc;
    err = esp_https_ota_get_img_desc(https_ota_handle, &app_desc);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_https_ota_read_img_desc failed");
        goto ota_end;
    }
    err = validate_image_header(&app_desc);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "image header verification failed");
        goto ota_end;
    }

    int totalSize = esp_https_ota_get_image_size(https_ota_handle);
    int downloadSize = 0;
    int previousloadSize = 0;
    T8U percentSize = 0;
    T8U count = 0;

    ESP_LOGW(TAG, "New Version: %d", version);

    while (1) {
    	count++;
        err = esp_https_ota_perform(https_ota_handle);
//        ESP_LOGW(TAG, "count %d", count);
        if (err != ESP_ERR_HTTPS_OTA_IN_PROGRESS) {
        	downloadSize = esp_https_ota_get_image_len_read(https_ota_handle);
        	percentSize = (T8U)((downloadSize* 100) / totalSize );
        	ESP_LOGW(TAG, "Downloading: %d%%,%d Bytes", percentSize, downloadSize);
        	sprintf((char*)&message, "New Version: %d\nDownloading: %d%%\nChecking Image...", version, percentSize);
        	display_message(message);
        	lv_task_handler();
            break;
        }
        // esp_https_ota_perform returns after every read operation which gives user the ability to
        // monitor the status of OTA upgrade by calling esp_https_ota_get_image_len_read, which gives length of image
        // data read so far.
//        ESP_LOGW(TAG, "Image bytes read: %d", esp_https_ota_get_image_len_read(https_ota_handle));
        if(count >= 100)
        {
        	count = 0;
        	downloadSize = esp_https_ota_get_image_len_read(https_ota_handle);
        	percentSize = (T8U)((downloadSize* 100) / totalSize );
        	ESP_LOGW(TAG, "Downloading: %d%%,%d Bytes", percentSize, downloadSize);
        	sprintf((char*)&message, "New Version: %d\nDownloading: %d%%", version, percentSize);
//        	ESP_LOGW(TAG, "%s", message);
        	display_message(message);
        	lv_task_handler();
        }
        else if(count % 10 == 0)
        {
        	downloadSize = esp_https_ota_get_image_len_read(https_ota_handle);
        	if(previousloadSize == downloadSize) break;
        	else previousloadSize = downloadSize;
        }
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);

    if (esp_https_ota_is_complete_data_received(https_ota_handle) != true) {
        // the OTA image was not completely received and user can customise the response to this situation.
        ESP_LOGE(TAG, "Complete data was not received.");
    }
    else {
        ota_finish_err = esp_https_ota_finish(https_ota_handle);
        if ((err == ESP_OK) && (ota_finish_err == ESP_OK)) {
            ESP_LOGI(TAG, "ESP_HTTPS_OTA upgrade successful. Rebooting ...");

        	sprintf((char*)&message, "Update successful.\nRebooting ...");
        	display_message(message);
        	lv_task_handler();

            vTaskDelay(1000 / portTICK_PERIOD_MS);
            esp_restart();
        } else {
            if (ota_finish_err == ESP_ERR_OTA_VALIDATE_FAILED) {
                ESP_LOGE(TAG, "Image validation failed, image is corrupted");
            }
            ESP_LOGE(TAG, "ESP_HTTPS_OTA upgrade failed 0x%x", ota_finish_err);
            FLAG_STATE |= F_OTAFAILED;
            vTaskDelete(NULL);
        }
    }

ota_end:
    esp_https_ota_abort(https_ota_handle);
    ESP_LOGE(TAG, "ESP_HTTPS_OTA upgrade failed");
    FLAG_STATE |= F_OTAFAILED;
    vTaskDelete(NULL);
/*	ota_finish_err = esp_https_ota_finish(https_ota_handle);
    if ((err == ESP_OK) && (ota_finish_err == ESP_OK)) {
        ESP_LOGI(TAG, "ESP_HTTPS_OTA upgrade successful. Rebooting ...");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        esp_restart();
    } else {
        if (ota_finish_err == ESP_ERR_OTA_VALIDATE_FAILED) {
            ESP_LOGE(TAG, "Image validation failed, image is corrupted");
        }
        ESP_LOGE(TAG, "ESP_HTTPS_OTA upgrade failed %d", ota_finish_err);
        vTaskDelete(NULL);
    }*/
}

void test_ota(void)
{
/*
    // Initialize NVS.
	//vTaskDelay(2000 / portTICK_RATE_MS);
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // 1.OTA app partition table has a smaller NVS partition size than the non-OTA
        // partition table. This size mismatch may cause NVS initialization to fail.
        // 2.NVS partition contains data in new format and cannot be recognized by this version of code.
        // If this happens, we erase NVS partition and initialize NVS again.
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    ESP_ERROR_CHECK(esp_netif_init());
  //  ESP_ERROR_CHECK(esp_event_loop_create_default());
*/
	xTaskCreate(&advanced_ota_example_task, "advanced_ota_example_task", 1024 * 8, NULL, 5, NULL);

}

const T8U OTAMENUFUNCTION[MAX_OTAMENU][15]= //maximum 19 character not include\n
{
	"G2_Diagnostic",
	"G2",
	"G2_groupEffort",
	"G2_init",
	"G2_timeOff",
	"G2_brother"//always last
};

void advanced_ota_search_task(void *pvParameter)
{
	char temp_url[100] = {0};
    ESP_LOGI(TAG, "Starting Advanced OTA search");

    for(int i = 0; i < MAX_OTAMENU; i++)
    {
        sprintf(temp_url, "http://%.15s:80/G2Firmware/%s.bin", tbl_server.IPv4, OTAMENUFUNCTION[i]);
//        esp_err_t ota_finish_err = ESP_OK;
        esp_http_client_config_t config = {
            .url = temp_url,//"http://192.168.1.9:8888/ota/z5.bin",
            .cert_pem = (char *)server_cert_pem_start,
            .timeout_ms = 3000,
        };

    #ifdef CONFIG_EXAMPLE_SKIP_COMMON_NAME_CHECK
        config.skip_cert_common_name_check = true;
    #endif

		esp_https_ota_config_t ota_config = {
			.http_config = &config,
			.http_client_init_cb = _http_client_init_cb, // Register a callback to be invoked after esp_http_client is initialized
		};

		esp_https_ota_handle_t https_ota_handle = NULL;
		esp_err_t err = esp_https_ota_begin(&ota_config, &https_ota_handle);
		if (err != ESP_OK) {
			ESP_LOGE(TAG, "ESP HTTPS OTA Begin failed error: %u", err);
//			FLAG_STATE |= F_OTAFAILED;
//			vTaskDelete(NULL);
//			esp_https_ota_abort(https_ota_handle);
//			continue;
			goto ota_end;
		}

		esp_app_desc_t app_desc;
		err = esp_https_ota_get_img_desc(https_ota_handle, &app_desc);
		if (err != ESP_OK) {
			ESP_LOGE(TAG, "esp_https_ota_read_img_desc failed");
		}
		else
		{
			FLAG_OTA_CHANGEFIRM |= (0x01 << i);
		}

ota_end:
		esp_https_ota_abort(https_ota_handle);

		vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    FLAG_STATE |= F_OTAPASSED;

    ESP_LOGI(TAG, "Advanced OTA search finish %u, %u", FLAG_STATE, FLAG_OTA_CHANGEFIRM);
//
//    vTaskDelay(1000 / portTICK_PERIOD_MS);
    vTaskDelete(NULL);
}

void test_ota_2(void)
{
	xTaskCreate(&advanced_ota_search_task, "advanced_ota_search_task", 1024 * 8, NULL, 5, NULL);
}

void advanced_ota_example_task_2(void *pvParameter)
{
	char temp_url[100] = {0};
    ESP_LOGI(TAG, "Starting Advanced OTA example");
//    ESP_LOGI(TAG, "selected_OTA: %u", selected_OTA);

    sprintf(temp_url, "http://%.15s:80/G2Firmware/%s.bin", tbl_server.IPv4, OTAMENUFUNCTION[selected_OTA]);
    esp_err_t ota_finish_err = ESP_OK;
    esp_http_client_config_t config = {
        .url = temp_url,//"http://192.168.1.9:8888/ota/z5.bin",
        .cert_pem = (char *)server_cert_pem_start,
        .timeout_ms = 3000,
    };

#ifdef CONFIG_EXAMPLE_SKIP_COMMON_NAME_CHECK
    config.skip_cert_common_name_check = true;
#endif

    esp_https_ota_config_t ota_config = {
        .http_config = &config,
        .http_client_init_cb = _http_client_init_cb, // Register a callback to be invoked after esp_http_client is initialized
    };

    esp_https_ota_handle_t https_ota_handle = NULL;
    esp_err_t err = esp_https_ota_begin(&ota_config, &https_ota_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "ESP HTTPS OTA Begin failed");
        FLAG_STATE |= F_OTAFAILED;
        vTaskDelete(NULL);
    }

    esp_app_desc_t app_desc;
    err = esp_https_ota_get_img_desc(https_ota_handle, &app_desc);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_https_ota_read_img_desc failed");
        goto ota_end;
    }
    err = validate_image_header(&app_desc);
//    if (err != ESP_OK) {
//        ESP_LOGE(TAG, "image header verification failed");
//        goto ota_end;
//    }

    int totalSize = esp_https_ota_get_image_size(https_ota_handle);
    int downloadSize = 0;
    int previousloadSize = 0;
    T8U percentSize = 0;
    T8U count = 0;

    ESP_LOGW(TAG, "New Version: %d", version);

    while (1) {
    	count++;
        err = esp_https_ota_perform(https_ota_handle);
        if (err != ESP_ERR_HTTPS_OTA_IN_PROGRESS) {
        	downloadSize = esp_https_ota_get_image_len_read(https_ota_handle);
        	percentSize = (T8U)((downloadSize* 100) / totalSize );
        	ESP_LOGW(TAG, "Downloading: %d%%,%d Bytes", percentSize, downloadSize);
        	sprintf((char*)&message, "New Version: %d\nDownloading: %d%%\nChecking Image...", version, percentSize);
        	display_message(message);
        	lv_task_handler();
            break;
        }
        // esp_https_ota_perform returns after every read operation which gives user the ability to
        // monitor the status of OTA upgrade by calling esp_https_ota_get_image_len_read, which gives length of image
        // data read so far.
//        ESP_LOGW(TAG, "Image bytes read: %d", esp_https_ota_get_image_len_read(https_ota_handle));
        if(count >= 100)
        {
        	count = 0;
        	downloadSize = esp_https_ota_get_image_len_read(https_ota_handle);
        	percentSize = (T8U)((downloadSize* 100) / totalSize );
        	ESP_LOGW(TAG, "Downloading: %d%%,%d Bytes", percentSize, downloadSize);
        	sprintf((char*)&message, "New Version: %d\nDownloading: %d%%", version, percentSize);
//        	ESP_LOGW(TAG, "%s", message);
        	display_message(message);
        	lv_task_handler();
        }
        else if(count % 10 == 0)
        {
        	downloadSize = esp_https_ota_get_image_len_read(https_ota_handle);
        	if(previousloadSize == downloadSize) break;
        	else previousloadSize = downloadSize;
        }
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);

    if (esp_https_ota_is_complete_data_received(https_ota_handle) != true) {
        // the OTA image was not completely received and user can customise the response to this situation.
        ESP_LOGE(TAG, "Complete data was not received.");
    }
    else {
        ota_finish_err = esp_https_ota_finish(https_ota_handle);
        if ((err == ESP_OK) && (ota_finish_err == ESP_OK)) {
            ESP_LOGI(TAG, "ESP_HTTPS_OTA upgrade successful. Rebooting ...");

        	sprintf((char*)&message, "Update successful.\nRebooting ...");
        	display_message(message);
        	lv_task_handler();

            vTaskDelay(1000 / portTICK_PERIOD_MS);
            esp_restart();
        } else {
            if (ota_finish_err == ESP_ERR_OTA_VALIDATE_FAILED) {
                ESP_LOGE(TAG, "Image validation failed, image is corrupted");
            }
            ESP_LOGE(TAG, "ESP_HTTPS_OTA upgrade failed 0x%x", ota_finish_err);
            FLAG_STATE |= F_OTAFAILED;
            vTaskDelete(NULL);
        }
    }

ota_end:
    esp_https_ota_abort(https_ota_handle);
    ESP_LOGE(TAG, "ESP_HTTPS_OTA upgrade failed");
    FLAG_STATE |= F_OTAFAILED;
    vTaskDelete(NULL);
}

void test_ota_3(void)
{
	xTaskCreate(&advanced_ota_example_task_2, "advanced_ota_example_task_2", 1024 * 8, NULL, 5, NULL);
}

void advanced_ota_example_task_3(void *pvParameter)
{
	char temp_url[100] = {0};
    ESP_LOGI(TAG, "Starting Advanced OTA example");

    sprintf(temp_url, "http://%.15s:80/G2Firmware/G2.bin", tbl_server.IPv4);
    esp_err_t ota_finish_err = ESP_OK;
    esp_http_client_config_t config = {
        .url = temp_url,//"http://192.168.1.9:8888/ota/z5.bin",
        .cert_pem = (char *)server_cert_pem_start,
        .timeout_ms = 5000,
    };

#ifdef CONFIG_EXAMPLE_SKIP_COMMON_NAME_CHECK
    config.skip_cert_common_name_check = true;
#endif

    esp_https_ota_config_t ota_config = {
        .http_config = &config,
        .http_client_init_cb = _http_client_init_cb, // Register a callback to be invoked after esp_http_client is initialized
    };

    esp_https_ota_handle_t https_ota_handle = NULL;
    esp_err_t err = esp_https_ota_begin(&ota_config, &https_ota_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "ESP HTTPS OTA Begin failed");
//        FLAG_STATE |= F_OTAFAILED;
//        FLAG_STATE &= ~F_OTASTART;
        FLAG_STATE |= F_NEEDUPDATEFIRM;
//        FLAG_STATE |= F_UPDATEHEADER;
        vTaskDelete(NULL);
    }

    esp_app_desc_t app_desc;
    err = esp_https_ota_get_img_desc(https_ota_handle, &app_desc);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_https_ota_read_img_desc failed");
        goto ota_end;
    }
    err = validate_image_header(&app_desc);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "image header verification failed");
        goto ota_end;
    }

    int totalSize = esp_https_ota_get_image_size(https_ota_handle);
    int downloadSize = 0;
    int previousloadSize = 0;
    T8U percentSize = 0;
    T8U count = 0;

	FLAG_STATE |= F_OTASTART;
	FLAG_STATE |= F_UPDATEHEADER;

    ESP_LOGW(TAG, "New Version: %d", version);

    while (1) {
    	count++;
        err = esp_https_ota_perform(https_ota_handle);
//        ESP_LOGW(TAG, "count %d", count);
        if (err != ESP_ERR_HTTPS_OTA_IN_PROGRESS) {
        	downloadSize = esp_https_ota_get_image_len_read(https_ota_handle);
        	percentSize = (T8U)((downloadSize* 100) / totalSize );
        	ESP_LOGW(TAG, "Downloading: %d%%,%d Bytes", percentSize, downloadSize);
//        	sprintf((char*)&message, "New Version: %d\nDownloading: %d%%\nChecking Image...", version, percentSize);
//        	display_message(message);
//        	lv_task_handler();
            break;
        }
        // esp_https_ota_perform returns after every read operation which gives user the ability to
        // monitor the status of OTA upgrade by calling esp_https_ota_get_image_len_read, which gives length of image
        // data read so far.
//        ESP_LOGW(TAG, "Image bytes read: %d", esp_https_ota_get_image_len_read(https_ota_handle));
        if(count >= 100)
        {
        	count = 0;
        	downloadSize = esp_https_ota_get_image_len_read(https_ota_handle);
        	percentSize = (T8U)((downloadSize* 100) / totalSize );
        	ESP_LOGW(TAG, "Downloading: %d%%,%d Bytes", percentSize, downloadSize);
//        	sprintf((char*)&message, "New Version: %d\nDownloading: %d%%", version, percentSize);
////        	ESP_LOGW(TAG, "%s", message);
//        	display_message(message);
//        	lv_task_handler();
//        	ESP_LOGE(TAG, "err: %d", err);
        }
        else if(count % 10 == 0)
        {
        	downloadSize = esp_https_ota_get_image_len_read(https_ota_handle);
        	if(previousloadSize == downloadSize) break;
        	else previousloadSize = downloadSize;
        }
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);

    if (esp_https_ota_is_complete_data_received(https_ota_handle) != true) {
        // the OTA image was not completely received and user can customise the response to this situation.
        ESP_LOGE(TAG, "Complete data was not received.");
    }
    else {
        ota_finish_err = esp_https_ota_finish(https_ota_handle);
        if ((err == ESP_OK) && (ota_finish_err == ESP_OK)) {
            ESP_LOGI(TAG, "ESP_HTTPS_OTA upgrade successful. Require to Reboot!");

//        	sprintf((char*)&message, "Update successful.\nRebooting ...");
//        	display_message(message);
//        	lv_task_handler();

            vTaskDelay(1000 / portTICK_PERIOD_MS);
//            esp_restart();
            FLAG_STATE &= ~F_OTASTART;
            FLAG_STATE |= F_NEWFIRMDONE;
            FLAG_STATE |= F_UPDATEHEADER;
            vTaskDelete(NULL);
        } else {
            if (ota_finish_err == ESP_ERR_OTA_VALIDATE_FAILED) {
                ESP_LOGE(TAG, "Image validation failed, image is corrupted");
            }
            ESP_LOGE(TAG, "ESP_HTTPS_OTA upgrade failed 0x%x", ota_finish_err);
//            FLAG_STATE |= F_OTAFAILED;
            FLAG_STATE &= ~F_OTASTART;
            FLAG_STATE |= F_NEEDUPDATEFIRM;
            FLAG_STATE |= F_UPDATEHEADER;
            vTaskDelete(NULL);
        }
    }

ota_end:
    esp_https_ota_abort(https_ota_handle);
    ESP_LOGE(TAG, "ESP_HTTPS_OTA upgrade failed");
//    FLAG_STATE |= F_OTAFAILED;
//    FLAG_STATE &= ~F_OTASTART;
    FLAG_STATE |= F_NEEDUPDATEFIRM;
//    FLAG_STATE |= F_UPDATEHEADER;
    vTaskDelete(NULL);
}

void test_ota4(void)
{
	xTaskCreate(&advanced_ota_example_task_3, "advanced_ota_example_task_3", 1024 * 8, NULL, 5, NULL);
}

void roll_back(){
#if defined(CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE) && defined(CONFIG_BOOTLOADER_APP_ANTI_ROLLBACK)
    /**
     * We are treating successful WiFi connection as a checkpoint to cancel rollback
     * process and mark newly updated firmware image as active. For production cases,
     * please tune the checkpoint behavior per end application requirement.
     */
    const esp_partition_t *running = esp_ota_get_running_partition();
    esp_ota_img_states_t ota_state;
    if (esp_ota_get_state_partition(running, &ota_state) == ESP_OK) {
        if (ota_state == ESP_OTA_IMG_PENDING_VERIFY) {
            if (esp_ota_mark_app_valid_cancel_rollback() == ESP_OK) {
                ESP_LOGI(TAG, "App is valid, rollback cancelled successfully");
            } else {
                ESP_LOGE(TAG, "Failed to cancel rollback");
            }
        }
    }
#endif
}
