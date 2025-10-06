#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include "handlelittleFS.h"
#include "esp_littlefs.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "util.h"
#include "handleSpiffs.h"
#include <errno.h>
#include <sys/stat.h>

static const char *TAG = "handlelittleFS";

//POINTER pointerFS;

//const char PTR_FILENAME2[2][MAX_TITLE_LEN] =
//{
//		"pointerFS",
//};

esp_vfs_littlefs_conf_t conf_littlefs = {
	.base_path = "/littlefs",
	.partition_label = "littleFS",
	.format_if_mount_failed = false,//true,
	.dont_mount = false,
};

void checkfiles_littlefs(void)
{
	T16U i = 0;
//    ESP_LOGI(TAG, "Initializing SPIFFS");

	ESP_LOGI(TAG, "Check files");

    for(i = 0; i < MAX_SET_FILE; i++)
	{
		// Check if destination file exists before renaming
    	vTaskDelay(1);
		struct stat st;
		char filePath[30] = "";// = "/spiffs/";
		sprintf(filePath, "/littlefs/%s.txt", PTR_FILENAME[i]);
//		if(i == FILE_SETTING) continue;
//		else
		if (stat(filePath, &st) != 0)
		{
			//not exist, create file
			// Use POSIX and C standard library functions to work with files.
			// First create a file.
			ESP_LOGI(TAG, "Create file: %s", filePath);
//			unlink(filePath);
			FILE* f = fopen(filePath, "w");
			if (f == NULL) {
				continue;
			}
			fclose(f);
		}
	}

//    ESP_LOGI(TAG, "Read from file: '%s'", line);
}

void initLittleFs(void)
{
//	T16U i = 0;
	TBL_VERSION_T tbl_version_t;
	ESP_LOGI(TAG, "Initializing LittelFS");
	esp_err_t ret = ESP_OK;

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
	memset((char*)&tbl_littlefs_diag, 0, sizeof(tbl_littlefs_diag));
    for(int i = 0; i < MAX_RETRY_LITTLEFS; i++)
    {
		ret = esp_vfs_littlefs_register(&conf_littlefs);

		if (ret != ESP_OK)
		{
			tbl_littlefs_diag.errMount = YES;
			tbl_littlefs_diag.errMountCode[tbl_spiffs_diag.errMountCount] = ret;
			tbl_littlefs_diag.errMountCount++;

			if (ret == ESP_FAIL)
			{
				ESP_LOGE(TAG, "Failed to mount or format filesystem");
			}
			else if (ret == ESP_ERR_NOT_FOUND)
			{
				ESP_LOGE(TAG, "Failed to find LittleFS partition");
			}
			else
			{
				ESP_LOGE(TAG, "Failed to initialize LittleFS (%s)", esp_err_to_name(ret));
				sprintf((char*)&tbl_littlefs_diag.errMountMsg, "%s", esp_err_to_name(ret));
			}
		}
		else
		{
//			esp_littlefs_format(conf_littlefs.partition_label);
			tbl_littlefs_diag.errMount = NO;
			break;
		}

		if(tbl_littlefs_diag.errMountCount == MAX_RETRY_SPIFFS)
		{
			ESP_LOGE(TAG, "Require to format LittleFS");
			conf_littlefs.format_if_mount_failed = true;
			ret = esp_vfs_littlefs_register(&conf_littlefs);

			if (ret != ESP_OK)
			{
				if (ret == ESP_FAIL)
				{
					ESP_LOGE(TAG, "Failed to mount or format filesystem");
				}
				else if (ret == ESP_ERR_NOT_FOUND)
				{
					ESP_LOGE(TAG, "Failed to find LittleFS partition");
				}
				else
				{
					ESP_LOGE(TAG, "Failed to initialize LittleFS (%s)", esp_err_to_name(ret));
					sprintf((char*)&tbl_littlefs_diag.errMountMsg, "%s", esp_err_to_name(ret));
				}
				return;
			}
		}
    }

    size_t total = 0, used = 0;
    ret = esp_littlefs_info(conf_littlefs.partition_label, &total, &used);
    if (ret != ESP_OK)
    {
    	ESP_LOGE(TAG, "Failed to get LittleFS partition information (%s)", esp_err_to_name(ret));
    }
    else
    {
    	ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }

//    for(int k = 0; k < MAX_SET_FILE; k++)
//	{
//		// Check if destination file exists before renaming
//		vTaskDelay(1);
//		struct stat st;
//		char filePath[30] = "";// = "/spiffs/";
//		sprintf(filePath, "/littlefs/%s.txt", PTR_FILENAME[k]);
//		if (stat(filePath, &st) != 0)
//		{
//			//not exist, create file
//			// Use POSIX and C standard library functions to work with files.
//			// First create a file.
//			ESP_LOGW(TAG, "Create file: %s", filePath);
//	//			unlink(filePath);
//			FILE* f = fopen(filePath, "w");
//			if (f == NULL) {
//	//				continue;
//			}
//			fclose(f);
//		}
//	}
    checkfiles_littlefs();
	vTaskDelay(1);//vTaskDelay(100 / portTICK_PERIOD_MS);
	for(int i = 0; i < 20; i++)//retry read
	{
		if(ReadTable2(&T_VERSION_T, (T8U *) &tbl_version_t, 0) == _PASS) break;
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
//	if(tbl_version_t.firstTimeClear == SPECIAL_ON)//first time
//	{
////		ReadTable(&T_CONTROL, (T8U *)&tbl_control, 0);
		ReadTable2(&T_CONTROL, (T8U *)&tbl_control, 0);
//	}

//	init_littlefs = 1;

}

//void checkfilesBackupSpiffs(void)
//{
////	T16U i = 0;
////
////	ESP_LOGI(TAG, "Check files Backup");
////
////    for(i = 0; i < MAX_SET_FILE; i++)
////	{
////		// Check if destination file exists before renaming
//////    	ESP_LOGI(TAG,"i: %u", i);
////    	vTaskDelay(1);
//////    	ESP_LOGI(TAG,"i: %u", i);
////		struct stat st;
////		char filePath[30] = "";// = "/spiffs/";
////		sprintf(filePath, "/littlefs/%s.txt", PTR_FILENAME[i]);
////		if(i == FILE_SETTING) continue;
////		else if (stat(filePath, &st) != 0)
////		{
////			//not exist, create file
////			// Use POSIX and C standard library functions to work with files.
////			// First create a file.
////			ESP_LOGI(TAG, "Create file backup: %s", filePath);
//////			unlink(filePath);
////			FILE* f = fopen(filePath, "w");
////			if (f == NULL) {
////				continue;
////			}
////			fclose(f);
////		}
////	}
//}
//
//T8U readBackupSpiffs(int title, int len, T8U *data)
//{
//    // Open file for reading
////    ESP_LOGI(TAG, "Reading file");
//	char temp[MAX_BYTEPAGE];
//	char filePath[35] = "";
//	int i = 0, j = 0;
////	if(!init_littlefs)return _FAIL;
//	memset(temp, 0, MAX_BYTEPAGE);
//	sprintf(filePath, "/littlefs/%s.txt", PTR_FILENAME[title]);
//	ESP_LOGI(TAG, "Reading file: %s", filePath);
//	FILE* f = fopen(filePath, "r");
//    if (f == NULL)
//    {
//    	memset(data, 0, len);
//        ESP_LOGE(TAG, "Failed to open file for reading(%d) %s\n", errno, strerror(errno));
//
//        if(tbl_littlefs_diag.errReadCount < MAX_RETRY_SPIFFS)
//        {
//        	tbl_littlefs_diag.errRead = YES;
//        	tbl_littlefs_diag.errReadCode[tbl_spiffs_diag.errReadCount] = errno;
//        	tbl_littlefs_diag.errReadCount++;
//			sprintf((char*)&tbl_littlefs_diag.errReadMsg, "%s", esp_err_to_name(errno));
//        }
//        return _FAIL;
//    }
//    fgets(temp, MAX_BYTEPAGE, f);
//    fclose(f);
//	for(i = 0; i < MAX_BYTEPAGE; i += 2)
//	{
//		*(data + j) = hexchar2int(temp[i])*16 +  hexchar2int(temp[i+1]);
//		j++;
//		if(j == len)break;
//	}
////    ESP_LOGI(TAG, "Data: %s\n", temp);
//    return _PASS;
//}
//
//T8U writeBackupSpiffs(int title, int len, T8U *data)
//{
//    // Open file for writing
////    ESP_LOGI(TAG, "Writing file");
//	char temp[MAX_BYTEPAGE];
//	char hextemp[3];
//	char filePath[35] = "";
//	int i = 0, j = 0;
////	ESP_LOGI(TAG, "RAM left %d", esp_get_free_heap_size());
////	ESP_LOGI(TAG, "MAX_BYTEWRITEABLE: %u", MAX_BYTEWRITEABLE);
////	ESP_LOGI(TAG, "MAX_WRITEBYTEBLOCK: %u", MAX_WRITEBYTEBLOCK);
////	if(!init_littlefs)return _FAIL;
//	memset(temp, 0, MAX_BYTEPAGE);
//	sprintf(filePath, "/littlefs/%s.txt", PTR_FILENAME[title]);
////	unlink(filePath);
//	ESP_LOGI(TAG, "Writing file: %s", filePath);
//	FILE* f = fopen(filePath, "w");
//    if (f == NULL)
//    {
//        ESP_LOGE(TAG, "Failed to open file for writing(%d) %s\n", errno, strerror(errno));
//        return _FAIL;
//    }
//    for(i = 0; i < len; i++)
//    {
//    	sprintf(hextemp, "%02X", *(data + i));
////    	ESP_LOGI(TAG, "%02X", *(data + i));
//    	strcat(temp, hextemp);
//    	j += 2;
//    }
//    for(; j < MAX_BYTEPAGE; j++)
//    {
//    	temp[j] = 0x30;
//    }
////    vTaskDelay(1);
//    fprintf(f, temp);
//    fclose(f);
////    ESP_LOGI(TAG, "Data: %s", temp);
//    return _PASS;
//}

//void checkfilesLittleFs(void)
//{
//	// Check if destination file exists before renaming
////  ESP_LOGI(TAG,"i: %u", i);
////  vTaskDelay(1);
////  ESP_LOGI(TAG,"i: %u", i);
////	BLOCK_POINTER block_pointer;
//	struct stat st;
//	T8U len = sizeof(BLOCK_POINTER)*2;
//	char temp[len];//times 2 size pointerFS
//	char filePath[35] = "";
//	int i = 0, j = 0;
//	T8U data[sizeof(BLOCK_POINTER)+1] = {0,0,0,0,0};
//
//	sprintf(filePath, "/littlefs/%s.txt", PTR_FILENAME2[FILE_POINTERFS]);
//	if (stat(filePath, &st) != 0)
//	{
//		//not exist, create file
//		// Use POSIX and C standard library functions to work with files.
//		// First create a file.
//		ESP_LOGI(TAG, "Create file: %s", filePath);
////		unlink(filePath);
//		FILE* f = fopen(filePath, "w");
//		if (f == NULL) {
//			return;
//		}
//		fclose(f);
//	}
//
////	readLittleFsBlock(FILE_POINTERFS, sizeof(block_pointer), (T8U*)&block_pointer);
//	///read pointerFS
//
//	memset(temp, 0, len);
//	sprintf(filePath, "/littlefs/%s.txt", PTR_FILENAME2[FILE_POINTERFS]);//(int)page->title]);
//	ESP_LOGI(TAG, "Reading file: %s", filePath);
//	FILE* f = fopen(filePath, "r");
//    if (f == NULL)
//    {
//        ESP_LOGE(TAG, "Failed to open file for reading");
//        return;
//    }
//    fgets(temp, len, f);//len, f);
//    fclose(f);
////    StrNRep((T8U*)temp, 0xFF, '\0', len);
//	for(i = 0; i < len; i += 2)
//	{
//		data[j] = hexchar2int(temp[i])*16 +  hexchar2int(temp[i+1]);
//		j++;
//		if(j == sizeof(BLOCK_POINTER))break;
//	}
//
//	StrNCpy((T8U *)&pointerFS, (T8U *)&data, sizeof(pointerFS));
//}

//void readLittleFsBlock(int title, int len, T8U *data)
//{
//    // Open file for reading
////    ESP_LOGI(TAG, "Reading file");
//	char temp[MAX_BYTEWRITEABLE];
//	char filePath[35] = "";
//	int i = 0, j = 0;
////	vTaskDelay(1);
////	if(!init_spiffs)return;
//	memset(temp, 0, MAX_BYTEWRITEABLE);
//	sprintf(filePath, "/littlefs/%s.txt", PTR_FILENAME2[title]);//(int)page->title]);
//	ESP_LOGI(TAG, "Reading file: %s", filePath);
//	FILE* f = fopen(filePath, "r");
//    if (f == NULL)
//    {
//        ESP_LOGE(TAG, "Failed to open file for reading");
//        return;
//    }
//    fgets(temp, MAX_BYTEWRITEABLE, f);//len, f);
//    fclose(f);
////    StrNRep((T8U*)temp, 0xFF, '\0', len);
//	for(i = 0; i < MAX_BYTEWRITEABLE; i += 2)
//	{
//		*(data + j) = hexchar2int(temp[i])*16 +  hexchar2int(temp[i+1]);
//		j++;
//		if(j == len)break;
//	}
////    ESP_LOGI(TAG, "i: %u", i);
////    ESP_LOGI(TAG, "size data: %u", sizeof(data));
////    ESP_LOGI(TAG, "Data: %s", temp);
//}
//
//void writeLittleFsBlock(int title, int len, T8U *data)
//{
//    // Open file for writing
////    ESP_LOGI(TAG, "Writing file");
//	char temp[MAX_BYTEWRITEABLE];
//	char hextemp[3];
//	char filePath[35] = "";
//	int i = 0, j = 0;
////	vTaskDelay(1);
////	if(!init_littlefs)return;
//	memset(temp, 0, MAX_BYTEWRITEABLE);
//	sprintf(filePath, "/littlefs/%s.txt", PTR_FILENAME2[title]);//(int)page->title]);
//	ESP_LOGI(TAG, "Writing file: %s", filePath);
//	FILE* f = fopen(filePath, "w");
//    if (f == NULL)
//    {
//        ESP_LOGE(TAG, "Failed to open file for writing");
//        return;
//    }
////    ESP_LOGI(TAG, "len: %u", len);
//    for(i = 0; i < len; i++)
//    {
//    	sprintf(hextemp, "%02X", *(data + i));
////    	ESP_LOGI(TAG, "%02X", *(data + i));
//    	strcat(temp, hextemp);
//    	j += 2;
//    }
//    for(; j < MAX_BYTEWRITEABLE; j++)
//    {
//    	temp[j] = 0x30;
//    }
//    fprintf(f, temp);
//    fclose(f);
////    ESP_LOGI(TAG, "Data: %s", temp);
//}

T8U readLittleFsPage(int title, int len, T8U *data)//follow spiffs page but reduce to minimum read write of littleFs if possible
{
    // Open file for reading
//    ESP_LOGI(TAG, "Reading file");
//	char temp[MAX_BYTEWRITEABLE];
	int tempLen = 0;
	if(len < (MIN_READWRITE/2)) tempLen = MIN_READWRITE/2;
	else tempLen = len;
	char temp[tempLen*2];//128(minimum littleFS read write as in SDK configuratin) to 256(spiffs page size);
	char filePath[35] = "";
	int i = 0, j = 0;
//	vTaskDelay(1);
//	if(!init_spiffs)return;
	memset(temp, 0, sizeof(temp));
	sprintf(filePath, "/littlefs/%s.txt", PTR_FILENAME[title]);//(int)page->title]);
	ESP_LOGI(TAG, "Reading file: %s", filePath);
	FILE* f = fopen(filePath, "r");
    if (f == NULL)
    {
    	memset(data, 0, len);
        ESP_LOGE(TAG, "Failed to open file for reading(%d) %s\n", errno, strerror(errno));

        if(tbl_littlefs_diag.errReadCount < MAX_RETRY_SPIFFS)
        {
        	tbl_littlefs_diag.errRead = YES;
        	tbl_littlefs_diag.errReadCode[tbl_spiffs_diag.errReadCount] = errno;
        	tbl_littlefs_diag.errReadCount++;
			sprintf((char*)&tbl_littlefs_diag.errReadMsg, "%s", esp_err_to_name(errno));
        }
        return _FAIL;
    }
    fgets(temp, sizeof(temp), f);//len, f);
    fclose(f);
//    StrNRep((T8U*)temp, 0xFF, '\0', len);
	for(i = 0; i < sizeof(temp); i += 2)
	{
		*(data + j) = hexchar2int(temp[i])*16 +  hexchar2int(temp[i+1]);
		j++;
		if(j == len)break;
	}
//    ESP_LOGI(TAG, "i: %u", i);
//    ESP_LOGI(TAG, "size data: %u", sizeof(data));
//    ESP_LOGI(TAG, "Data: %s", temp);
	return _PASS;
}

T8U writeLittleFsPage(int title, int len, T8U *data)//follow spiffs page but reduce to minimum read write of littleFs if possible
{
    // Open file for writing
//    ESP_LOGI(TAG, "Writing file");
//	char temp[MAX_BYTEWRITEABLE];
	int tempLen = 0;
	if(len < (MIN_READWRITE/2)) tempLen = MIN_READWRITE/2;
	else tempLen = len;
	char temp[tempLen*2];//128(minimum littleFS read write as in SDK configuratin) to 256(spiffs page size)
	char hextemp[3];
	char filePath[35] = "";
	int i = 0, j = 0;
//	vTaskDelay(1);
//	if(!init_littlefs)return;
	memset(temp, 0, sizeof(temp));
	sprintf(filePath, "/littlefs/%s.txt", PTR_FILENAME[title]);//(int)page->title]);
	ESP_LOGI(TAG, "Writing file: %s", filePath);
//	FILE* f = fopen(filePath, "w");
	FILE* f = fopen(filePath, "r+");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing(%d) %s\n", errno, strerror(errno));
        return _FAIL;
    }
//    if (ftruncate(fileno(f), 0) != 0) {
//        printf("Error clearing file\n");
//        exit(1);
//    }
//    ESP_LOGI(TAG, "len: %u", len);
    for(i = 0; i < len; i++)
    {
    	sprintf(hextemp, "%02X", *(data + i));
//    	ESP_LOGI(TAG, "%02X", *(data + i));
    	strcat(temp, hextemp);
    	j += 2;
    }
    for(; j < sizeof(temp); j++)
    {
    	temp[j] = 0x30;
    }
    errno = fprintf(f, temp);
    fclose(f);
//    ESP_LOGI(TAG, "Data: %s", temp);

    if(errno == ESP_FAIL)
    {
    	ESP_LOGE(TAG, "Failed writing(%d) %s\n", errno, strerror(errno));
    	return _FAIL;
    }

    return _PASS;
}

T8U readLittleFsBuffer(char *name, T8U *data)
{
    // Open file for reading
//    ESP_LOGI(TAG, "Reading file");
	char temp[MAX_BYTEWRITEABLE];
	char filePath[35] = "";
	int i = 0;//, j = 0;
//	if(!init_littlefs)return _FAIL;
	memset(temp, 0, MAX_BYTEWRITEABLE);
	sprintf(filePath, "/littlefs/%s.txt", name);
	ESP_LOGI(TAG, "Reading file: %s", filePath);
	FILE* f = fopen(filePath, "r");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for reading(%d) %s\n", errno, strerror(errno));
        return _FAIL;
    }
    fgets(temp, MAX_BYTEWRITEABLE, f);
    fclose(f);
    for(i = 0; i < MAX_BYTEWRITEABLE; i += 2)
    {
    	*(data + (i/2)) = hexchar2int(temp[i])*16 +  hexchar2int(temp[i+1]);
    }
//    ESP_LOGI(TAG, "Data: %s\n", temp);
    return _PASS;
}

T8U writeLittleFsBuffer(char *name, T8U *data)
{
    // Open file for writing
//    ESP_LOGI(TAG, "Writing file");
	char temp[MAX_BYTEWRITEABLE];
	char hextemp[3];
	char filePath[35] = "";
	int i = 0;//, j = 0;
//	ESP_LOGI(TAG, "RAM left %d", esp_get_free_heap_size());
//	ESP_LOGI(TAG, "MAX_BYTEWRITEABLE: %u", MAX_BYTEWRITEABLE);
//	ESP_LOGI(TAG, "MAX_WRITEBYTEBLOCK: %u", MAX_WRITEBYTEBLOCK);
//	if(!init_littlefs)return _FAIL;
	memset(temp, 0, MAX_BYTEWRITEABLE);
	sprintf(filePath, "/littlefs/%s.txt", name);
//	unlink(filePath);
	ESP_LOGI(TAG, "Writing file: %s", filePath);
//	ESP_LOGE(TAG, "File Path:%s\n", filePath);
	FILE* f = fopen(filePath, "w");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing(%d) %s\n", errno, strerror(errno));
        return _FAIL;
    }
//    if (ftruncate(fileno(f), 0) != 0) {
//    	ESP_LOGE(TAG, "Error Clearing file");
//        exit(1);
//    }
    for(i = 0; i < MAX_WRITEBYTEBLOCK; i++)
    {
    	sprintf(hextemp, "%02X", *(data + i));
    	strcat(temp, hextemp);

//    	if(i == MAX_WRITEBYTEBLOCK - 1)
//    	{
//    		ESP_LOGE(TAG, "temp len: %u, i: %u", strlen((char*)&temp), i);
//    	}
    }
//    vTaskDelay(1);
    errno = fprintf(f, temp);
    fclose(f);

    if(errno == ESP_FAIL)
    {
    	ESP_LOGE(TAG, "Failed writing(%d) %s\n", errno, strerror(errno));
    	return _FAIL;
    }
//    ESP_LOGI(TAG, "Data: %s", temp);
    return _PASS;
}

/////////////////////////////////////below function like databuffer///////////////////////////////////////////////////////
//void save_FS_pointer(void)
//{
////	BLOCK_POINTER block_pointer;
//	T8U len = sizeof(BLOCK_POINTER)*2;
//	char temp[len];//times 2 pointerFS size
//	char hextemp[3];
//	char filePath[35] = "";
//	int i = 0, j = 0;
//	T8U data[sizeof(BLOCK_POINTER)+1] = {0,0,0,0,0};
//
//	StrNCpy((T8U *)&data, (T8U *)&pointerFS, sizeof(pointerFS));
//
//	memset(temp, 0, len);
//	sprintf(filePath, "/littlefs/%s.txt", PTR_FILENAME2[FILE_POINTERFS]);//(int)page->title]);
//	ESP_LOGI(TAG, "Writing file: %s", filePath);
//	FILE* f = fopen(filePath, "w");
//    if (f == NULL)
//    {
//        ESP_LOGE(TAG, "Failed to open file for writing");
//        return;
//    }
////    ESP_LOGI(TAG, "len: %u", len);
//    for(i = 0; i < sizeof(BLOCK_POINTER); i++)
//    {
//    	sprintf(hextemp, "%02X", data[i]);
////    	ESP_LOGI(TAG, "%02X", *(data + i));
//    	strcat(temp, hextemp);
//    	j += 2;
//    }
//    fprintf(f, temp);
//    fclose(f);
//}

void Set_FS_MemoryFull(void)
{
	tbl_fs_pointer.front = MAX_FS_RECORD;
	tbl_fs_pointer.rear = 0;
//	save_FS_pointer();
	WriteTable(&T_FS_POINTER, (T8U *)&tbl_fs_pointer, 0);
}

void Clear_FS_Memory(void)
{
	tbl_fs_pointer.front = 0;
	tbl_fs_pointer.rear = 0;
//	save_FS_pointer();
	WriteTable(&T_FS_POINTER, (T8U *)&tbl_fs_pointer, 0);
}

T16U Get_RecordFSCount(void)
{
	T16U tempcnt;
	tempcnt = (tbl_fs_pointer.front >= tbl_fs_pointer.rear) ? (tbl_fs_pointer.front - tbl_fs_pointer.rear) : (tbl_fs_pointer.front + MAX_FS_RECORD - tbl_fs_pointer.rear);
	return tempcnt;
}

void save_record_FS(const LOG_NAME *log_name, T8U * data)
{
//	T8U tempdata[MAX_BUFFER_LEN];
//	T8U pagebyte[MAX_WRITEBYTEBLOCK];
//	T16U filenumber = 0;
//	char tempname[25] = "";
//
//	memset(tempdata, 0, MAX_BUFFER_LEN);
//	tempdata[0] = log_name->cmd;
//	tempdata[1] = log_name->len;
//
//	ESP_LOGI(TAG, "%u", log_name->cmd);
//	ESP_LOGI(TAG, "%u", log_name->len);
//
//	StrNCpy(&tempdata[2], data , tempdata[1]);
//
//	ESP_LOGI(TAG, "write bufferFS");
//	if(tbl_fs_pointer.front >= MAX_FS_RECORD)
//	{
//		tbl_fs_pointer.front = 0;
//	}
//	filenumber = tbl_fs_pointer.front / MAX_RECORDPERBLOCK;
//	sprintf(tempname, "bufferFS%u", filenumber);
//	if(readLittleFsBuffer(tempname, pagebyte) == _FAIL)
//	{
//		memset(pagebyte, 0, MAX_WRITEBYTEBLOCK);
//	}
//	int pos = 64 * (tbl_fs_pointer.front % MAX_RECORDPERBLOCK);
//	for(int i = 0; i < MAX_BUFFER_LEN; i++)
//	{
//		pagebyte[pos + i] = tempdata[i];
//	}
//	(void) writeLittleFsBuffer(tempname, pagebyte);
//	tbl_fs_pointer.front++;
//	if(tbl_fs_pointer.front >= MAX_FS_RECORD)
//	{
//		tbl_fs_pointer.front = 0;
//	}
//	if(tbl_fs_pointer.front == tbl_fs_pointer.rear)
//	{
//		tbl_fs_pointer.rear++;
//		if(tbl_fs_pointer.rear >= MAX_FS_RECORD)
//		{
//			tbl_fs_pointer.rear = 0;
//		}
//	}
//
////	save_FS_pointer();
//	WriteTable(&T_FS_POINTER, (T8U *)&tbl_fs_pointer, 0);
}

//void save_record_FS_2(T8U * data)
//{
//	T8U tempdata[MAX_BUFFER_LEN];
//	T8U pagebyte[MAX_WRITEBYTEBLOCK];
//	T16U filenumber = 0;
//	char tempname[25] = "";
//
//	StrNCpy(&tempdata[0], data , MAX_BUFFER_LEN);
//
//	ESP_LOGI(TAG, "write bufferFS");
//	if(pointerFS.front >= MAX_FS_RECORD)
//	{
//		pointerFS.front = 0;
//	}
//	filenumber = pointerFS.front / MAX_RECORDPERBLOCK;
//	sprintf(tempname, "bufferFS%u", filenumber);
//	if(readLittleFsBuffer(tempname, pagebyte) == _FAIL)
//	{
//		memset(pagebyte, 0, MAX_WRITEBYTEBLOCK);
//	}
//	int pos = 64 * (pointerFS.front % MAX_RECORDPERBLOCK);
//	for(int i = 0; i < MAX_BUFFER_LEN; i++)
//	{
//		pagebyte[pos + i] = tempdata[i];
//	}
//	(void) writeLittleFsBuffer(tempname, pagebyte);
//	pointerFS.front++;
//	if(pointerFS.front >= MAX_FS_RECORD)
//	{
//		pointerFS.front = 0;
//	}
//	if(pointerFS.front == pointerFS.rear)
//	{
//		pointerFS.rear++;
//		if(pointerFS.rear >= MAX_FS_RECORD)
//		{
//			pointerFS.rear = 0;
//		}
//	}
//}

void get_FSrecord(T8U * data, T8U otherRecord)//byte maximum 64 bytes, use otherRecord incase to read several record infront
{
//	T8U tempdata[MAX_BUFFER_LEN];
//	T8U pagebyte[MAX_WRITEBYTEBLOCK];
//	T16U filenumber = 0;
//	char tempname[25] = "";
//
//	ESP_LOGI(TAG, "read FS buffer");
//	filenumber = tbl_fs_pointer.rear;
//	if(otherRecord != 0)
//	{
//		for(int i = 0; i < otherRecord; i++)
//		{
//			filenumber++;
//			if(filenumber >= MAX_FS_RECORD)
//			{
//				filenumber = 0;
//			}
//		}
//	}
//	filenumber /= MAX_RECORDPERBLOCK;
//	sprintf(tempname, "bufferFS%u", filenumber);
////	sprintf(tempname, "%u", filenumber);
//	memset(pagebyte, 0, sizeof(pagebyte));
//	(void) readLittleFsBuffer(tempname, pagebyte);
//	int pos = MAX_BUFFER_LEN * ((tbl_fs_pointer.rear+otherRecord) % MAX_RECORDPERBLOCK);
//	for(int i = 0; i < MAX_BUFFER_LEN; i++)
//	{
//		tempdata[i] = pagebyte[pos + i];
//	}
//
//	StrNCpy(data, tempdata, MAX_BUFFER_LEN);
}

void flush_FS_record(T8U record)
{
	if(tbl_fs_pointer.rear != tbl_fs_pointer.front)
	{
		for(int i = 0; i < record; i++)
		{
			tbl_fs_pointer.rear++;
			if(tbl_fs_pointer.rear >= MAX_FS_RECORD)
			{
				tbl_fs_pointer.rear = 0;
			}
		}
		if(tbl_fs_pointer.front >= MAX_FS_RECORD)
		{
			tbl_fs_pointer.front = 0;
		}
	}
//	save_FS_pointer();
	WriteTable(&T_FS_POINTER, (T8U *)&tbl_fs_pointer, 0);
}

T8U one_minute = 0;
//void spiffsToLittleFS(void)
//{
//	if(savingInProcess)return;
//	if(Get_RecordFSCount() >= (MAX_FS_RECORD-90))return;
//	if((one_minute == currenttime.minute) && (Get_RecordFSCount() > 0) && (Get_RecordLoCount() < MAX_RECORDPERBLOCK))return;
////	if((Get_RecordLoCount() >= MAX_RECORDPERBLOCK) ||
////	  ((Get_RecordLoCount() > 0) && (one_minute != currenttime.minute)))
//	if(Get_RecordLoCount() > 0)
//	{
////		T8U tempdata[MAX_WRITEBYTEBUFFER];
//		T8U tempdata[MAX_BUFFER_LEN];
//		T8U tempdata2[MAX_BUFFER_LEN];
//		T8U pagebyte[MAX_WRITEBYTEBLOCK];
//		T16U filenumber = 0;
//		T8U skip = false;
//		T8U data_continue = false;
//		char tempname[25] = "";
//		T16U tempRear = tbl_buf_pointer.low.rear;
//
//		//////////////////read from littleFS/////////////////////////////////////////
//		if(tbl_fs_pointer.front >= MAX_FS_RECORD)
//		{
//			tbl_fs_pointer.front = 0;
//		}
//		filenumber = tbl_fs_pointer.front / MAX_RECORDPERBLOCK;
//		sprintf(tempname, "bufferFS%u", filenumber);
//		if(readLittleFsBuffer(tempname, pagebyte) == _FAIL)
//		{
//			memset(pagebyte, 0, MAX_WRITEBYTEBLOCK);
//		}
//
//		for(int i = 0; i < MAX_RECORDPERBLOCK; i++)
//		{
//			vTaskDelay(1);
//			if((savingInProcess) && (data_continue == false))break;
//
////			ESP_LOGI(TAG, "tbl_fs_pointer.rear %u", tbl_fs_pointer.rear);
////			ESP_LOGI(TAG, "tbl_fs_pointer.front %u", tbl_fs_pointer.front);
////			ESP_LOGI(TAG, "tbl_buf_pointer.low.rear %u", tbl_buf_pointer.low.rear);
////			ESP_LOGI(TAG, "tbl_buf_pointer.low.front %u", tbl_buf_pointer.low.front);
//
//			T16U loRecord = Get_RecordLoCount();
//			ESP_LOGI(TAG, "loRecord %u", loRecord);
//			ESP_LOGI(TAG, "tbl_fs_pointer.front %u", tbl_fs_pointer.front);
//			ESP_LOGI(TAG, "tbl_fs_pointer.rear %u", tbl_fs_pointer.rear);
//
//			memset(tempdata, 0, MAX_BUFFER_LEN);
//			get_lorecord(tempdata, 0);
//
//			//check record is more than 5 ops and enough buffer or not
//			if((tempdata[0] == DATA_TDT_CLAIM.cmd) && (loRecord > 1))//&& ((tbl_fs_pointer.front % MAX_RECORDPERBLOCK) > 26))
//			{
//				memset(tempdata2, 0, MAX_BUFFER_LEN);
//				get_lorecord(tempdata2, 1);
//
//				if(tempdata2[0] == DATA_TDT_CLAIM_1.cmd) //&& ((tbl_fs_pointer.front % MAX_RECORDPERBLOCK) > 27))
//				{
//					if((tbl_fs_pointer.front % MAX_RECORDPERBLOCK) > 26)//27)
//					{
//						memset(tempdata, 0, MAX_BUFFER_LEN);
//						skip = true;
//						data_continue = false;
//					}
//					else
//					{
//						data_continue = true;
//					}
//				}
//				else
//				{
//					data_continue = false;
////					memset(tempdata, 0, MAX_BUFFER_LEN);
////					get_lorecord(tempdata, 0);
//				}
//			}
//			else if(tempdata[0] == DATA_TDT_CLAIM_1.cmd) //&& ((tbl_fs_pointer.front % MAX_RECORDPERBLOCK) > 26))
//			{
//				memset(tempdata2, 0, MAX_BUFFER_LEN);
//				get_lorecord(tempdata2, 1);
//
//				if(tempdata2[0] == DATA_TDT_CLAIM_2.cmd) //&& ((tbl_fs_pointer.front % MAX_RECORDPERBLOCK) > 27))
//				{
//					data_continue = true;
//				}
//			}
//			else if(tempdata[0] == DATA_TDT_CLAIM_2.cmd) //&& ((tbl_fs_pointer.front % MAX_RECORDPERBLOCK) > 26))
//			{
//				memset(tempdata2, 0, MAX_BUFFER_LEN);
//				get_lorecord(tempdata2, 1);
//
//				if(tempdata2[0] == DATA_TDT_CLAIM_3.cmd) //&& ((tbl_fs_pointer.front % MAX_RECORDPERBLOCK) > 27))
//				{
//					data_continue = true;
//				}
//			}
//			else if((tempdata[0] == DATA_GROUPTAG_CLAIM.cmd)  && (loRecord > 1))//&& ((tbl_fs_pointer.front % MAX_RECORDPERBLOCK) > 26))
//			{
//				memset(tempdata2, 0, MAX_BUFFER_LEN);
//				get_lorecord(tempdata2, 1);
//
//				if(tempdata2[0] == DATA_GROUPTAG_CLAIM_1.cmd) //&& ((tbl_fs_pointer.front % MAX_RECORDPERBLOCK) > 27))
//				{
//					if((tbl_fs_pointer.front % MAX_RECORDPERBLOCK) > 26)//27)
//					{
//						memset(tempdata, 0, MAX_BUFFER_LEN);
//						skip = true;
//						data_continue = false;
//					}
//					else
//					{
//						data_continue = true;
//					}
//				}
//				else
//				{
//					data_continue = false;
////					memset(tempdata, 0, MAX_BUFFER_LEN);
////					get_lorecord(tempdata, 0);
//				}
//			}
//			else if(tempdata[0] == DATA_GROUPTAG_CLAIM_1.cmd) //&& ((tbl_fs_pointer.front % MAX_RECORDPERBLOCK) > 26))
//			{
//				memset(tempdata2, 0, MAX_BUFFER_LEN);
//				get_lorecord(tempdata2, 1);
//
//				if(tempdata2[0] == DATA_GROUPTAG_CLAIM_2.cmd) //&& ((tbl_fs_pointer.front % MAX_RECORDPERBLOCK) > 27))
//				{
//					data_continue = true;
//				}
//			}
//			else if(tempdata[0] == DATA_GROUPTAG_CLAIM_2.cmd) //&& ((tbl_fs_pointer.front % MAX_RECORDPERBLOCK) > 26))
//			{
//				memset(tempdata2, 0, MAX_BUFFER_LEN);
//				get_lorecord(tempdata2, 1);
//
//				if(tempdata2[0] == DATA_GROUPTAG_CLAIM_3.cmd) //&& ((tbl_fs_pointer.front % MAX_RECORDPERBLOCK) > 27))
//				{
//					data_continue = true;
//				}
//			}
//			else
//			{
//				data_continue = false;
//			}
//
//			//////////////////copy to littleFS array///////////////////////////////////
//			int pos = 64 * (tbl_fs_pointer.front % MAX_RECORDPERBLOCK);
//			for(int j = 0; j < MAX_BUFFER_LEN; j++)
//			{
//				pagebyte[pos + j] = tempdata[j];
//			}
//			tbl_fs_pointer.front++;
//			if(tbl_fs_pointer.front >= MAX_FS_RECORD)
//			{
//				tbl_fs_pointer.front = 0;
//			}
//			if(tbl_fs_pointer.front == tbl_fs_pointer.rear)
//			{
//				tbl_fs_pointer.rear++;
//				if(tbl_fs_pointer.rear >= MAX_FS_RECORD)
//				{
//					tbl_fs_pointer.rear = 0;
//				}
//			}
//			//////////////////////////////////////////////////////////////////////////
//			if((tbl_fs_pointer.front % MAX_RECORDPERBLOCK) == 0)break;
//
//			if(skip)continue;
//
//			tbl_buf_pointer.low.rear++;
//			if(tbl_buf_pointer.low.rear == tbl_buf_pointer.low.front)//||((tbl_fs_pointer.front % MAX_RECORDPERBLOCK) == 0))
//			{
////				if(tbl_buf_pointer.low.rear == tbl_buf_pointer.low.front)
//				one_minute = currenttime.minute;
//				break;
//			}
//			if(tbl_buf_pointer.low.rear >= MAX_LOWRECORD)
//			{
//				tbl_buf_pointer.low.rear = 0;
//			}
////			if(tbl_buf_pointer.low.front >= MAX_LOWRECORD)
////			{
////				tbl_buf_pointer.low.front = 0;
////			}
//		}
//		(void) writeLittleFsBuffer(tempname, pagebyte);
//
//		WriteTable(&T_BUF_POINTER, (T8U *)&tbl_buf_pointer, 0);
////		save_FS_pointer();
//		WriteTable(&T_FS_POINTER, (T8U *)&tbl_fs_pointer, 0);
//
//		for(; tempRear != tbl_buf_pointer.low.rear; tempRear++)
//		{
//			sprintf(tempname, "databufferlow%u", tempRear);
//			deletePage(tempname);
//
//			tempRear++;
//			if(tempRear >= MAX_LOWRECORD)
//			{
//				tempRear = 0;
//			}
//			if(tempRear == tbl_buf_pointer.low.rear)
//			{
//				break;
//			}
//		}
//	}
//}

void deletePageFS(char *name)
{
	char filePath[35] = "";
	sprintf(filePath, "/littlefs/%s.txt", name);
	ESP_LOGI(TAG, "Deleting file: %s", filePath);
	unlink(filePath);
}

void formatLittleFs(void)
{
    // All done, unmount partition and disable LittleFS
//	if(!init_spiffs)return;
	esp_littlefs_format(conf_littlefs.partition_label);
//    ESP_LOGI(TAG, "LittleFs formatted");
	checkfiles_littlefs();
}

void endLittleFs(void)
{
    // All done, unmount partition and disable LittleFS
//	if(!init_spiffs)return;
	esp_vfs_littlefs_unregister(conf_littlefs.partition_label);
//    ESP_LOGI(TAG, "LittleFs unmounted");
}
