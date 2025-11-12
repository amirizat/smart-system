#include "handleSpiffs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "util.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include <errno.h>
#include <sys/stat.h>

static const char* TAG = "handlespiffs";

esp_vfs_spiffs_conf_t conf = {
  .base_path = "/spiffs",
  .partition_label = NULL,
  .max_files = 5,
  .format_if_mount_failed = false//true
};

/////spiffs error reason//////////////can read more under error.h or esp_spiffs.c
//#define	EPERM 		1		/* Not owner */
//#define	ENOENT 		2		/* No such file or directory */
//#define	ESRCH 		3		/* No such process */
//#define	EINTR 		4		/* Interrupted system call */
//#define	EIO 		5		/* I/O error */
//#define	ENXIO 		6		/* No such device or address */
//#define	E2BIG 		7		/* Arg list too long */
//#define	ENOEXEC 	8		/* Exec format error */
//#define	EBADF 		9		/* Bad file number */
//#define	ECHILD 		10		/* No children */
//#define	EAGAIN 		11		/* No more processes */
//#define	ENOMEM 		12		/* Not enough space */
//#define	EACCES 		13		/* Permission denied */
//#define	EFAULT 		14		/* Bad address */
//////////////////////////////////////////////////////////////////////
void checkfiles(void)
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
		sprintf(filePath, "/spiffs/%s.txt", PTR_FILENAME[i]);
		if(i == FILE_SETTING) continue;
		else if (stat(filePath, &st) != 0)
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

void initSpiffs(void)
{
//	T16U i = 0;
//	TBL_VERSION_T tbl_version_t;
    ESP_LOGI(TAG, "Initializing SPIFFS");
    esp_err_t ret = ESP_OK;
    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    memset((char*)&tbl_spiffs_diag, 0, sizeof(tbl_spiffs_diag));
    for(int i = 0; i < MAX_RETRY_SPIFFS; i++)
    {
		ret = esp_vfs_spiffs_register(&conf);

		if (ret != ESP_OK)
		{
			tbl_spiffs_diag.errMount = YES;
			tbl_spiffs_diag.errMountCode[tbl_spiffs_diag.errMountCount] = ret;
			tbl_spiffs_diag.errMountCount++;

			if (ret == ESP_FAIL)
			{
				ESP_LOGE(TAG, "Failed to mount or format filesystem");
			}
			else if (ret == ESP_ERR_NOT_FOUND)
			{
				ESP_LOGE(TAG, "Failed to find SPIFFS partition");
			}
			else
			{
				ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
				sprintf((char*)&tbl_spiffs_diag.errMountMsg, "%s", esp_err_to_name(ret));
			}
		}
		else
		{
			tbl_spiffs_diag.errMount = NO;
			break;
		}

		if(tbl_spiffs_diag.errMountCount == MAX_RETRY_SPIFFS)
		{
			ESP_LOGE(TAG, "Require to format SPIFFS");
			conf.format_if_mount_failed = true;
			ret = esp_vfs_spiffs_register(&conf);

			if (ret != ESP_OK)
			{
				if (ret == ESP_FAIL)
				{
					ESP_LOGE(TAG, "Failed to mount or format filesystem");
				}
				else if (ret == ESP_ERR_NOT_FOUND)
				{
					ESP_LOGE(TAG, "Failed to find SPIFFS partition");
				}
				else
				{
					ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
					sprintf((char*)&tbl_spiffs_diag.errMountMsg, "%s", esp_err_to_name(ret));
				}
				return;
			}
		}
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    }
    else
    {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }

//	// Check if destination file exists before renaming
//	vTaskDelay(1);
//	struct stat st;
//	char filePath[30] = "";// = "/spiffs/";
//	sprintf(filePath, "/spiffs/%s.txt", PTR_FILENAME[3]);
//	if (stat(filePath, &st) != 0)
//	{
//		//not exist, create file
//		// Use POSIX and C standard library functions to work with files.
//		// First create a file.
//		ESP_LOGI(TAG, "Create file: %s", filePath);
////			unlink(filePath);
//		FILE* f = fopen(filePath, "w");
//		if (f == NULL) {
////				continue;
//		}
//		fclose(f);
//	}

//	vTaskDelay(1);//vTaskDelay(100 / portTICK_PERIOD_MS);
//	for(int i = 0; i < 20; i++)//retry read
//	{
//		if(ReadTable2(&T_VERSION_T, (T8U *) &tbl_version_t, 0) == _PASS) break;
//		vTaskDelay(100 / portTICK_PERIOD_MS);
//	}
////	if(tbl_version_t.firstTimeClear == SPECIAL_ON)//first time
////	{
//////		ReadTable(&T_CONTROL, (T8U *)&tbl_control, 0);
//		ReadTable2(&T_CONTROL, (T8U *)&tbl_control, 0);
////	}

}

//void readSpiffsPage(int title, int len, T8U *data)
T8U readSpiffsPage(int title, int len, T8U *data)
{
    // Open file for reading
//    ESP_LOGI(TAG, "Reading file");
	char temp[MAX_BYTEPAGE];
	char filePath[35] = "";
	int i = 0, j = 0;
//	vTaskDelay(1);
	memset(temp, 0, len);
	sprintf(filePath, "/spiffs/%s.txt", PTR_FILENAME[title]);//(int)page->title]);
	ESP_LOGI(TAG, "Reading file: %s", filePath);
	FILE* f = fopen(filePath, "r");
    if (f == NULL)
    {
    	memset(data, 0, len);
        ESP_LOGE(TAG, "Failed to open file for reading(%d) %s\n", errno, strerror(errno));

        if(tbl_spiffs_diag.errReadCount < MAX_RETRY_SPIFFS)
        {
			tbl_spiffs_diag.errRead = YES;
			tbl_spiffs_diag.errReadCode[tbl_spiffs_diag.errReadCount] = errno;
			tbl_spiffs_diag.errReadCount++;
			sprintf((char*)&tbl_spiffs_diag.errReadMsg, "%s", esp_err_to_name(errno));
        }
        return _FAIL;
    }
    fgets(temp, MAX_BYTEPAGE, f);//len, f);
    fclose(f);
//    StrNRep((T8U*)temp, 0xFF, '\0', len);
	for(i = 0; i < MAX_BYTEPAGE; i += 2)
	{
		*(data + j) = hexchar2int(temp[i])*16 +  hexchar2int(temp[i+1]);
		j++;
		if(j == len)break;
	}
	return _PASS;
}

T8U writeSpiffsPage(int title, int len, T8U *data)
{
    // Open file for writing
//    ESP_LOGI(TAG, "Writing file");
	char temp[MAX_BYTEPAGE];
	char hextemp[3];
	char filePath[35] = "";
	int i = 0, j = 0;
//	vTaskDelay(1);
	memset(temp, 0, MAX_BYTEPAGE);
	sprintf(filePath, "/spiffs/%s.txt", PTR_FILENAME[title]);//(int)page->title]);
//	unlink(filePath);
	ESP_LOGI(TAG, "Writing file: %s", filePath);
	FILE* f = fopen(filePath, "w");
//	FILE* f = fopen(filePath, "r+");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing(%d) %s\n", errno, strerror(errno));
        return _FAIL;
    }
//    if (ftruncate(fileno(f), 0) != 0) {
//    	ESP_LOGE(TAG, "Error Clearing file");
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
    for(; j < MAX_BYTEPAGE; j++)
    {
    	temp[j] = 0x30;
    }
//    StrNRep((T8U*)temp, '\0', 0xFF, len);
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

//T8U readSpiffsBuffer(char *name, T8U *data)
//{
//    // Open file for reading
////    ESP_LOGI(TAG, "Reading file");
//	char temp[MAX_BYTEBUFFER];
//	char filePath[35] = "";
//	int i = 0;//, j = 0;
//	memset(temp, 0, MAX_BYTEBUFFER);
//	sprintf(filePath, "/spiffs/%s.txt", name);
//	ESP_LOGI(TAG, "Reading file: %s", filePath);
//	FILE* f = fopen(filePath, "r");
//    if (f == NULL)
//    {
//        ESP_LOGE(TAG, "Failed to open file for reading");
//        return _FAIL;
//    }
//    fgets(temp, MAX_BYTEBUFFER, f);
//    fclose(f);
//    for(i = 0; i < MAX_BYTEBUFFER; i += 2)
//    {
//    	*(data + (i/2)) = hexchar2int(temp[i])*16 +  hexchar2int(temp[i+1]);
//    }
////    ESP_LOGI(TAG, "Data: %s\n", temp);
//    return _PASS;
//}
//
//T8U writeSpiffsBuffer(char *name, T8U *data)
//{
//    // Open file for writing
////    ESP_LOGI(TAG, "Writing file");
//	char temp[MAX_BYTEBUFFER];
//	char hextemp[3];
//	char filePath[35] = "";
//	int i = 0;//, j = 0;
//	memset(temp, 0, MAX_BYTEBUFFER);
//	sprintf(filePath, "/spiffs/%s.txt", name);
//	unlink(filePath);
//	ESP_LOGI(TAG, "Writing file: %s", filePath);
//	FILE* f = fopen(filePath, "w");
//    if (f == NULL)
//    {
//        ESP_LOGE(TAG, "Failed to open file for writing");
//        return _FAIL;
//    }
//    for(i = 0; i < MAX_WRITEBYTEBUFFER; i++)
//    {
//    	sprintf(hextemp, "%02X", *(data + i));
//    	strcat(temp, hextemp);
//    }
////    StrNRep((T8U*)temp, '\0', ' ', MAX_BYTEPAGE);
//    fprintf(f, temp);
//    fclose(f);
////    ESP_LOGI(TAG, "Data: %s", temp);
//    return _PASS;
//}

T8U readSpiffsBuffer2(char *name, T8U *data)
{
    // Open file for reading
//    ESP_LOGI(TAG, "Reading file");
	char temp[MAX_BYTEPAGE];
	char filePath[35] = "";
	int i = 0;//, j = 0;
	memset(temp, 0, MAX_BYTEPAGE);
	sprintf(filePath, "/spiffs/%s.txt", name);
	ESP_LOGI(TAG, "Reading file: %s", filePath);
	FILE* f = fopen(filePath, "r");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for reading(%d) %s\n", errno, strerror(errno));
        return _FAIL;
    }
    fgets(temp, MAX_BYTEPAGE, f);
    fclose(f);
    for(i = 0; i < MAX_BYTEPAGE; i += 2)
    {
    	*(data + (i/2)) = hexchar2int(temp[i])*16 +  hexchar2int(temp[i+1]);
    }
//    ESP_LOGI(TAG, "Data: %s\n", temp);
    return _PASS;
}

T8U writeSpiffsBuffer2(char *name, T8U *data)
{
    // Open file for writing
//    ESP_LOGI(TAG, "Writing file");
	char temp[MAX_BYTEPAGE];
	char hextemp[3];
	char filePath[35] = "";
	int i = 0;//, j = 0;
	memset(temp, 0, MAX_BYTEPAGE);
	sprintf(filePath, "/spiffs/%s.txt", name);
//	unlink(filePath);
	ESP_LOGI(TAG, "Writing file: %s", filePath);
//	ESP_LOGE(TAG, "File Path:%s\n", filePath);
	FILE* f = fopen(filePath, "w");
//	FILE* f = fopen(filePath, "r+");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing(%d) %s\n", errno, strerror(errno));
        return _FAIL;
    }
//    if (ftruncate(fileno(f), 0) != 0) {
//    	ESP_LOGE(TAG, "Error Clearing file");
//        exit(1);
//    }
    for(i = 0; i < MAX_WRITEBYTEPAGE; i++)
    {
    	sprintf(hextemp, "%02X", *(data + i));
    	strcat(temp, hextemp);
    }
//    StrNRep((T8U*)temp, '\0', ' ', MAX_BYTEPAGE);
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

//T8U readSpiffsBufferHexString(char *name, T8U *data)
//{
//    // Open file for reading
////    ESP_LOGI(TAG, "Reading file");
//	char temp[MAX_BYTEPAGE];
//	char filePath[35] = "";
//	int i = 0;//, j = 0;
//	memset(temp, 0, MAX_BYTEPAGE);
//	sprintf(filePath, "/spiffs/%s.txt", name);
//	ESP_LOGI(TAG, "Reading file: %s", filePath);
//	FILE* f = fopen(filePath, "r");
//    if (f == NULL)
//    {
//        ESP_LOGE(TAG, "Failed to open file for reading");
//        return _FAIL;
//    }
//    fgets(temp, MAX_BYTEPAGE, f);
//    fclose(f);
//    for(i = 0; i < MAX_BYTEPAGE; i++)
//    {
//    	*(data + i) = temp[i];//hexchar2int(temp[i])*16 +  hexchar2int(temp[i+1]);
//    }
////    ESP_LOGI(TAG, "Data: %s\n", temp);
//    return _PASS;
//}

void deletePage(char *name)
{
	char filePath[35] = "";
	sprintf(filePath, "/spiffs/%s.txt", name);
	ESP_LOGI(TAG, "Deleting file: %s", filePath);
	unlink(filePath);
}

void endSpiffs(void)
{
    // All done, unmount partition and disable SPIFFS
    esp_vfs_spiffs_unregister(conf.partition_label);
//    ESP_LOGI(TAG, "SPIFFS unmounted");
}
