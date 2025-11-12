/*
 * databuffer.c
 *
 *  Created on: 6 Nov 2020
 *      Author: Administrator
 */
#include "databuffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "handleSpiffs.h"
#include "handlelittleFS.h"
#include "table.h"
#include "util.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "databuffer";
//==============================================================================
//   CONSTANT
//==============================================================================
const LOG_NAME DATA_LOG_ERROR			=	{	0x01,	sizeof(LOG_ERROR)};
const LOG_NAME DATA_TDT_CLAIM_V2		=	{	0x10,	sizeof(LOG_TDT_CLAIM_V2)};
const LOG_NAME DATA_GROUPTAG_CLAIM_V2	=	{	0x11,	sizeof(LOG_GROUPTAG_CLAIM_V2)};
const LOG_NAME DATA_TDT_CLAIM_V3		=	{	0x12,	sizeof(LOG_TDT_CLAIM_V3)};
const LOG_NAME DATA_TDT_CLAIM			=	{	0x72,	sizeof(LOG_TDT_CLAIM)};
const LOG_NAME DATA_TDT_SPLIT_START		=	{	0x76,	sizeof(LOG_TDT_SPLIT_START)};
const LOG_NAME DATA_TDT_SPLIT_CHILDITEM	=	{	0x76,	sizeof(LOG_TDT_SPLIT_CHILDITEM)};
const LOG_NAME DATA_TDT_SPLIT_END		=	{	0x76,	sizeof(LOG_TDT_SPLIT_END)};
const LOG_NAME DATA_GROUPTAG_REGISTER	=	{	0x7A,	sizeof(LOG_GROUPTAG_REGISTER)};
const LOG_NAME DATA_GROUPTAG_CLAIM		=	{	0x7B,	sizeof(LOG_GROUPTAG_CLAIM)};
const LOG_NAME DATA_BUNDLE_MODIFYQTY	=	{	0X7D,	sizeof(LOG_BUNDLE_MODIFYQTY)};
const LOG_NAME DATA_EMPLOYEEINOUT		=	{	0x81,	sizeof(LOG_EMPLOYEE)};
const LOG_NAME DATA_EMPLOYEEREQUEST		=	{	0x82,	sizeof(LOG_EMPLOYEE)-1};
const LOG_NAME DATA_UDF					=	{	0x83,	sizeof(LOG_UDF)};
const LOG_NAME DATA_REQUEST_MO			=	{	0x86,	sizeof(LOG_REQUEST_MO)};
const LOG_NAME DATA_LOSTTIME			=	{	0x87,	sizeof(LOG_LOSTTIME)};
const LOG_NAME DATA_POWERUP				=	{	0x90,	sizeof(LOG_POWERUP)};
const LOG_NAME DATA_BUNDLE_CLAIMV3		=	{	0x9F,	sizeof(LOG_BUNDLE_CLAIMV3)};
const LOG_NAME DATA_BUNDLE_CLAIMV1		= 	{	0xA1,	sizeof(LOG_BUNDLE_CLAIM_V1)};
const LOG_NAME DATA_TDT_CLAIM_1			=	{	0xB5,	sizeof(LOG_TDT_CLAIM)}; 			// 181
const LOG_NAME DATA_TDT_CLAIM_2			=	{	0xB6,	sizeof(LOG_TDT_CLAIM)}; 			// 182
const LOG_NAME DATA_TDT_CLAIM_3			=	{	0xB7,	sizeof(LOG_TDT_CLAIM)}; 			// 183
const LOG_NAME DATA_GARMENTREPAIR		=	{	0xC1,	sizeof(LOG_GARMENTREPAIR)};
const LOG_NAME DATA_UPDATINGOPMANUALLY	=	{	0xC2,	sizeof(LOG_UPDATINGOPMANUALLY)}; 	//high record
const LOG_NAME DATA_NOTIFYERROR			=	{	0xC3,	sizeof(LOG_NOTIFYERROR)}; 			//high record
const LOG_NAME DATA_GROUPTAG_CHANGE		=	{	0xC4,	sizeof(LOG_GROUPTAG_CHANGE)};
const LOG_NAME DATA_GROUPTAG_CLAIM_1	=	{	0xC5,	sizeof(LOG_GROUPTAG_CLAIM)};
const LOG_NAME DATA_GROUPTAG_CLAIM_2	=	{	0xC6,	sizeof(LOG_GROUPTAG_CLAIM)};
const LOG_NAME DATA_GROUPTAG_CLAIM_3	=	{	0xC7,	sizeof(LOG_GROUPTAG_CLAIM)};
const LOG_NAME DATA_ASSET_TRACKING		=	{	0xC9,	sizeof(LOG_ASSET_TRACKING)};  		// CHANGED FROM 0xC8 	20 code

LOG_REGISTEREPC log_registerepc;
LOG_GROUPTAG_CHANGE log_grouptag_change;

T8U savingInProcess = false;

void save_record(const LOG_NAME *log_name, T8U * data, T8U HIGHLOW);

//==============================================================================
//
//	FUNCTION NAME:
//	DESCRIPTION:
//	PARAMETERS:
//	RETURN:
//
//==============================================================================

T8U Is_MemoryFull(void)
{
//	if((MAX_HIGHRECORD - 10) <= Get_RecordHiCount() || ((MAX_LOWRECORD - 100) <= Get_RecordLoCount())) //&& ((MAX_FS_RECORD - 300) <= Get_RecordFSCount())))
	if(((MAX_HIGHRECORD - 10) <= Get_RecordHiCount()) || 
	   ((MAX_LOWRECORD - 100) <= Get_RecordLoCount()))
	{
		return true;
	}
	return false;
}

//==============================================================================
//
//	FUNCTION NAME:
//	DESCRIPTION:
//	PARAMETERS:
//	RETURN:
//
//==============================================================================
void Set_MemoryFull(void)
{
	tbl_buf_pointer.high.front = MAX_HIGHRECORD;
	tbl_buf_pointer.high.rear = 0;
	tbl_buf_pointer.low.front = MAX_LOWRECORD;
	tbl_buf_pointer.low.rear = 0;
	WriteTable(&T_BUF_POINTER, (T8U *)&tbl_buf_pointer,0);

//	Set_FS_MemoryFull();
}

//==============================================================================
//
//	FUNCTION NAME:
//	DESCRIPTION:
//	PARAMETERS:
//	RETURN:
//
//==============================================================================
void Clear_Memory(void)
{
	tbl_buf_pointer.high.front = 0;
	tbl_buf_pointer.high.rear = 0;
	tbl_buf_pointer.low.front = 0;
	tbl_buf_pointer.low.rear = 0;
	WriteTable(&T_BUF_POINTER, (T8U *)&tbl_buf_pointer,0);

	Clear_FS_Memory();
}

//==============================================================================
//
//	FUNCTION NAME:
//	DESCRIPTION:
//	PARAMETERS:
//	RETURN:
//
//==============================================================================
T16U Get_RecordCount(void)
{
	T16U tempcnt;
	tempcnt =  Get_RecordHiCount() + Get_RecordLoCount() + Get_RecordFSCount();
	return tempcnt;
}

//==============================================================================
//
//	FUNCTION NAME:
//	DESCRIPTION:
//	PARAMETERS:
//	RETURN:
//
//==============================================================================
T16U Get_RecordHiCount(void)
{
	T16U tempcnt;
	tempcnt = (tbl_buf_pointer.high.front >= tbl_buf_pointer.high.rear) ? (tbl_buf_pointer.high.front - tbl_buf_pointer.high.rear) : (tbl_buf_pointer.high.front + MAX_HIGHRECORD - tbl_buf_pointer.high.rear);
	return tempcnt;
}

//==============================================================================
//
//	FUNCTION NAME:
//	DESCRIPTION:
//	PARAMETERS:
//	RETURN:
//
//==============================================================================
T16U Get_RecordLoCount(void)
{
	T16U tempcnt;
	tempcnt = (tbl_buf_pointer.low.front >= tbl_buf_pointer.low.rear) ? (tbl_buf_pointer.low.front - tbl_buf_pointer.low.rear) : (tbl_buf_pointer.low.front + MAX_LOWRECORD - tbl_buf_pointer.low.rear);
	return tempcnt;
}

//==============================================================================
//
//	FUNCTION NAME:
//	DESCRIPTION:
//	PARAMETERS:
//	RETURN:
//
//==============================================================================
void save_lorecord(const LOG_NAME *log_name, T8U * data)
{
	save_record(log_name, data, 0);
	one_minute = currenttime.minute;
}

//==============================================================================
//
//	FUNCTION NAME:
//	DESCRIPTION:
//	PARAMETERS:
//	RETURN:
//
//==============================================================================
void save_hirecord(const LOG_NAME *log_name, T8U * data)
{
	save_record(log_name, data, 1);
}

//==============================================================================
//
//	FUNCTION NAME:
//	DESCRIPTION:
//	PARAMETERS:
//	RETURN:
//
//==============================================================================
//void save_record(const LOG_NAME *log_name, T8U * data, T8U HIGHLOW)
//{
//	T8U tempdata[MAX_BUFFER_LEN];
////	T8U pagebyte[MAX_WRITEBYTEBUFFER];
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
//	if(HIGHLOW == 1)
//	{
//		ESP_LOGI(TAG, "write high buffer");
//		if(tbl_buf_pointer.high.front >= MAX_HIGHRECORD)
//		{
//			tbl_buf_pointer.high.front = 0;
//		}
//		filenumber = tbl_buf_pointer.high.front;// / MAX_RECORDPERPAGE;
//		sprintf(tempname, "databufferhigh%u", filenumber);
////		sprintf(tempname, "H%u", filenumber);
////		if(readSpiffsBuffer(tempname, pagebyte) == _FAIL)
////		{
////			memset(pagebyte, 0, MAX_WRITEBYTEPAGE);
////		}
////		int pos = 64 * (tbl_buf_pointer.high.front % MAX_RECORDPERPAGE);
////		for(int i = 0; i < MAX_BUFFER_LEN; i++)
////		{
////			pagebyte[pos + i] = tempdata[i];
////		}
//		if(writeSpiffsBuffer(tempname, tempdata) == _FAIL)
//		{
////			deletePage(tempname);
//			char tempname2[25] = "";
//			T16U filenumber2 = tbl_buf_pointer.high.front + 1;
//			if(filenumber2 >= MAX_HIGHRECORD)
//			{
//				filenumber2 = 0;
//			}
//			sprintf(tempname2, "databufferhigh%u", filenumber2);
//			deletePage(tempname2);
//			(void)writeSpiffsBuffer(tempname, tempdata);
//		}
//		tbl_buf_pointer.high.front++;
//		if(tbl_buf_pointer.high.front >= MAX_HIGHRECORD)
//		{
//			tbl_buf_pointer.high.front = 0;
//		}
//		if(tbl_buf_pointer.high.front == tbl_buf_pointer.high.rear)
//		{
//			tbl_buf_pointer.high.rear++;
//			if(tbl_buf_pointer.high.rear >= MAX_HIGHRECORD)
//			{
//				tbl_buf_pointer.high.rear = 0;
//			}
//		}
////		WriteTable(&T_BUF_POINTER, (T8U *)&tbl_buf_pointer, 0);
//	}
//	else
//	{
//		ESP_LOGI(TAG, "write low buffer");
//		if(tbl_buf_pointer.low.front >= MAX_LOWRECORD)
//		{
//			tbl_buf_pointer.low.front = 0;
//		}
//		filenumber = tbl_buf_pointer.low.front;// / MAX_RECORDPERPAGE;
//		sprintf(tempname, "databufferlow%u", filenumber);
////		sprintf(tempname, "%u", filenumber);
////		if(readSpiffsBuffer(tempname, pagebyte) == _FAIL)
////		{
////			memset(pagebyte, 0, MAX_WRITEBYTEPAGE);
////		}
////		int pos = 64 * (tbl_buf_pointer.low.front % MAX_RECORDPERPAGE);
////		for(int i = 0; i < MAX_BUFFER_LEN; i++)
////		{
////			pagebyte[pos + i] = tempdata[i];
////		}
//		if(writeSpiffsBuffer(tempname, tempdata) == _FAIL)
//		{
////			deletePage(tempname);
//			char tempname2[25] = "";
//			T16U filenumber2 = tbl_buf_pointer.low.front + 1;
//			if(filenumber2 >= MAX_LOWRECORD)
//			{
//				filenumber2 = 0;
//			}
//			sprintf(tempname2, "databufferlow%u", filenumber2);
//			deletePage(tempname2);
//			(void)writeSpiffsBuffer(tempname, tempdata);
//		}
//		tbl_buf_pointer.low.front++;
//		if(tbl_buf_pointer.low.front >= MAX_LOWRECORD)
//		{
//			tbl_buf_pointer.low.front = 0;
//		}
//		if(tbl_buf_pointer.low.front == tbl_buf_pointer.low.rear)
//		{
//			tbl_buf_pointer.low.rear++;
//			if(tbl_buf_pointer.low.rear >= MAX_LOWRECORD)
//			{
//				tbl_buf_pointer.low.rear = 0;
//			}
//		}
////		WriteTable(&T_BUF_POINTER, (T8U *)&tbl_buf_pointer, 0);
//	}
//	WriteTable(&T_BUF_POINTER, (T8U *)&tbl_buf_pointer, 0);
//}

void save_record(const LOG_NAME *log_name, T8U * data, T8U HIGHLOW)
{
	T8U tempdata[MAX_BYTEBUFFER];
	T16U filenumber = 0;
	char tempname[25] = "";

	memset(tempdata, 0, MAX_BYTEBUFFER);
	tempdata[0] = log_name->cmd;
	tempdata[1] = log_name->len;

	ESP_LOGI(TAG, "%u", log_name->cmd);
	ESP_LOGI(TAG, "%u", log_name->len);

	StrNCpy(&tempdata[2], data , tempdata[1]);
	if(HIGHLOW == 1)
	{
		ESP_LOGI(TAG, "write high buffer");
		if(tbl_buf_pointer.high.front >= MAX_HIGHRECORD)
		{
			tbl_buf_pointer.high.front = 0;
		}
		filenumber = tbl_buf_pointer.high.front;// / MAX_RECORDPERPAGE;
		sprintf(tempname, "databufferhigh%u", filenumber);
		if(writeSpiffsBuffer2(tempname, tempdata) == _FAIL)
		{
//			deletePage(tempname);
//			char tempname2[25] = "";
//			T16U filenumber2 = tbl_buf_pointer.high.front + 1;
//			if(filenumber2 >= MAX_HIGHRECORD)
//			{
//				filenumber2 = 0;
//			}
//			sprintf(tempname2, "databufferhigh%u", filenumber2);
//			deletePage(tempname2);
//			(void)writeSpiffsBuffer2(tempname, tempdata);

			////////write until success///////////////////
			T8U result;
			T8U count = 0;
			while(1)
			{
				result = writeSpiffsBuffer2(tempname, tempdata);
				if(result == _FAIL)
				{
					count++;
					if(count > 20)
					{
						FLAG_STATE |= F_MEMORYCHECKING;
						return;
					}
					vTaskDelay(1000/portTICK_RATE_MS);
					continue;
				}
				break;
			}
			//////////////////////////////////////////////////////
		}
		tbl_buf_pointer.high.front++;
		if(tbl_buf_pointer.high.front >= MAX_HIGHRECORD)
		{
			tbl_buf_pointer.high.front = 0;
		}
		if(tbl_buf_pointer.high.front == tbl_buf_pointer.high.rear)
		{
			tbl_buf_pointer.high.rear++;
			if(tbl_buf_pointer.high.rear >= MAX_HIGHRECORD)
			{
				tbl_buf_pointer.high.rear = 0;
			}
		}
	}
	else
	{
		ESP_LOGI(TAG, "write low buffer");
		if(tbl_buf_pointer.low.front >= MAX_LOWRECORD)
		{
			tbl_buf_pointer.low.front = 0;
		}
		filenumber = tbl_buf_pointer.low.front;// / MAX_RECORDPERPAGE;
		sprintf(tempname, "databufferlow%u", filenumber);
		if(writeSpiffsBuffer2(tempname, tempdata) == _FAIL)
		{
//			deletePage(tempname);
//			char tempname2[25] = "";
//			T16U filenumber2 = tbl_buf_pointer.low.front + 1;
//			if(filenumber2 >= MAX_LOWRECORD)
//			{
//				filenumber2 = 0;
//			}
//			sprintf(tempname2, "databufferlow%u", filenumber2);
//			deletePage(tempname2);
//			(void)writeSpiffsBuffer2(tempname, tempdata);

			////////write until success///////////////////
			T8U result;
			T8U count = 0;
			while(1)
			{
				result = writeSpiffsBuffer2(tempname, tempdata);
				if(result == _FAIL)
				{
					count++;
					if(count > 20)
					{
						FLAG_STATE |= F_MEMORYCHECKING;
						return;
					}
					vTaskDelay(1000/portTICK_RATE_MS);
					continue;
				}
				break;
			}
			//////////////////////////////////////////////////////
		}
		tbl_buf_pointer.low.front++;
		if(tbl_buf_pointer.low.front >= MAX_LOWRECORD)
		{
			tbl_buf_pointer.low.front = 0;
		}
		if(tbl_buf_pointer.low.front == tbl_buf_pointer.low.rear)
		{
			tbl_buf_pointer.low.rear++;
			if(tbl_buf_pointer.low.rear >= MAX_LOWRECORD)
			{
				tbl_buf_pointer.low.rear = 0;
			}
		}
	}
	WriteTable(&T_BUF_POINTER, (T8U *)&tbl_buf_pointer, 0);
}

//void save_lorecord_2(const LOG_NAME *log_name, T8U * data)//, T8U HIGHLOW)
//{
//	T8U tempdata[MAX_BUFFER_LEN];
////	T8U pagebyte[MAX_WRITEBYTEBUFFER];
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
////	if(HIGHLOW == 1)
////	{
////		ESP_LOGI(TAG, "write high buffer");
////		if(tbl_buf_pointer.high.front >= MAX_HIGHRECORD)
////		{
////			tbl_buf_pointer.high.front = 0;
////		}
////		filenumber = tbl_buf_pointer.high.front;// / MAX_RECORDPERPAGE;
////		sprintf(tempname, "databufferhigh%u", filenumber);
//////		sprintf(tempname, "H%u", filenumber);
//////		if(readSpiffsBuffer(tempname, pagebyte) == _FAIL)
//////		{
//////			memset(pagebyte, 0, MAX_WRITEBYTEPAGE);
//////		}
//////		int pos = 64 * (tbl_buf_pointer.high.front % MAX_RECORDPERPAGE);
//////		for(int i = 0; i < MAX_BUFFER_LEN; i++)
//////		{
//////			pagebyte[pos + i] = tempdata[i];
//////		}
////		(void) writeSpiffsBuffer(tempname, tempdata);
////		tbl_buf_pointer.high.front++;
////		if(tbl_buf_pointer.high.front >= MAX_HIGHRECORD)
////		{
////			tbl_buf_pointer.high.front = 0;
////		}
////		if(tbl_buf_pointer.high.front == tbl_buf_pointer.high.rear)
////		{
////			tbl_buf_pointer.high.rear++;
////			if(tbl_buf_pointer.high.rear >= MAX_HIGHRECORD)
////			{
////				tbl_buf_pointer.high.rear = 0;
////			}
////		}
//////		WriteTable(&T_BUF_POINTER, (T8U *)&tbl_buf_pointer, 0);
////	}
////	else
////	{
//		ESP_LOGI(TAG, "write low buffer");
//		if(tbl_buf_pointer.low.front >= MAX_LOWRECORD)
//		{
//			tbl_buf_pointer.low.front = 0;
//		}
//		filenumber = tbl_buf_pointer.low.front;// / MAX_RECORDPERPAGE;
//		sprintf(tempname, "databufferlow%u", filenumber);
////		sprintf(tempname, "%u", filenumber);
////		if(readSpiffsBuffer(tempname, pagebyte) == _FAIL)
////		{
////			memset(pagebyte, 0, MAX_WRITEBYTEPAGE);
////		}
////		int pos = 64 * (tbl_buf_pointer.low.front % MAX_RECORDPERPAGE);
////		for(int i = 0; i < MAX_BUFFER_LEN; i++)
////		{
////			pagebyte[pos + i] = tempdata[i];
////		}
//		if(writeSpiffsBuffer(tempname, tempdata) == _FAIL)
//		{
////			deletePage(tempname);
//			char tempname2[25] = "";
//			T16U filenumber2 = tbl_buf_pointer.low.front + 1;
//			if(filenumber2 >= MAX_LOWRECORD)
//			{
//				filenumber2 = 0;
//			}
//			sprintf(tempname2, "databufferlow%u", filenumber2);
//			deletePage(tempname2);
//			(void)writeSpiffsBuffer(tempname, tempdata);
//		}
//		tbl_buf_pointer.low.front++;
//		if(tbl_buf_pointer.low.front >= MAX_LOWRECORD)
//		{
//			tbl_buf_pointer.low.front = 0;
//		}
//		if(tbl_buf_pointer.low.front == tbl_buf_pointer.low.rear)
//		{
//			tbl_buf_pointer.low.rear++;
//			if(tbl_buf_pointer.low.rear >= MAX_LOWRECORD)
//			{
//				tbl_buf_pointer.low.rear = 0;
//			}
//		}
////		WriteTable(&T_BUF_POINTER, (T8U *)&tbl_buf_pointer, 0);
////	}
////	WriteTable(&T_BUF_POINTER, (T8U *)&tbl_buf_pointer, 0);
//		one_minute = currenttime.minute;
//}

//==============================================================================
//
//	FUNCTION NAME:
//	DESCRIPTION:
//	PARAMETERS:
//	RETURN:
//
//==============================================================================
//void get_hirecord(T8U * data)//byte maximum 64 bytes
//{
////	T8U tempdata[MAX_BUFFER_LEN];
//	T8U pagebyte[MAX_WRITEBYTEBUFFER];
//	T16U filenumber = 0;
//	char tempname[25] = "";
//
//	ESP_LOGI(TAG, "read high buffer");
//	filenumber = tbl_buf_pointer.high.rear;// / MAX_RECORDPERPAGE;
//	sprintf(tempname, "databufferhigh%u", filenumber);
////	sprintf(tempname, "H%u", filenumber);
//	(void) readSpiffsBuffer(tempname, pagebyte);
////	int pos = MAX_BUFFER_LEN * (tbl_buf_pointer.high.rear % MAX_RECORDPERPAGE);
////	for(int i = 0; i < MAX_BUFFER_LEN; i++)
////	{
////		tempdata[i] = pagebyte[pos + i];
////	}
//
////	StrNCpy(data, tempdata, MAX_BUFFER_LEN);
//	StrNCpy(data, pagebyte, MAX_BUFFER_LEN);
//}

void get_hirecord(T8U * data)//byte maximum 128 bytes
{
	T8U pagebyte[MAX_WRITEBYTEPAGE];
	T16U filenumber = 0;
	char tempname[25] = "";

	ESP_LOGI(TAG, "read high buffer");
	filenumber = tbl_buf_pointer.high.rear;
	sprintf(tempname, "databufferhigh%u", filenumber);
	(void) readSpiffsBuffer2(tempname, pagebyte);

	StrNCpy(data, pagebyte, MAX_WRITEBYTEPAGE);
}

void get_hirecord_web(T8U * data)//hexa string maximum 128 chars
{

}

//==============================================================================
//
//	FUNCTION NAME:
//	DESCRIPTION:
//	PARAMETERS:
//	RETURN:
//
//==============================================================================
//void get_lorecord(T8U * data, T8U otherRecord)//byte maximum 64 bytes, use otherRecord incase t0 read several record infront
//{
////	T8U tempdata[MAX_BUFFER_LEN];
//	T8U pagebyte[MAX_WRITEBYTEBUFFER];
//	T16U filenumber = 0;
//	char tempname[25] = "";
//
//	ESP_LOGI(TAG, "read low buffer");
//
//	filenumber = tbl_buf_pointer.low.rear;// / MAX_RECORDPERPAGE;
//	if(otherRecord != 0)
//	{
//		for(int i = 0; i < otherRecord; i++)
//		{
//			filenumber++;
//			if(filenumber >= MAX_LOWRECORD)
//			{
//				filenumber = 0;
//			}
//		}
//	}
//	sprintf(tempname, "databufferlow%u", filenumber);
////	sprintf(tempname, "%u", filenumber);
//	(void) readSpiffsBuffer(tempname, pagebyte);
////	int pos = MAX_BUFFER_LEN * ((tbl_buf_pointer.low.rear+otherRecord) % MAX_RECORDPERPAGE);
////	for(int i = 0; i < MAX_BUFFER_LEN; i++)
////	{
////		tempdata[i] = pagebyte[pos + i];
////	}
//
////	StrNCpy(data, tempdata, MAX_BUFFER_LEN);
//	StrNCpy(data, pagebyte, MAX_BUFFER_LEN);
//}

void get_lorecord(T8U * data, T8U otherRecord)//byte maximum 128 bytes
{
	T8U pagebyte[MAX_WRITEBYTEPAGE];
	T16U filenumber = 0;
	char tempname[25] = "";

	ESP_LOGI(TAG, "read low buffer");

	filenumber = tbl_buf_pointer.low.rear;
	if(otherRecord != 0)
	{
		for(int i = 0; i < otherRecord; i++)
		{
			filenumber++;
			if(filenumber >= MAX_LOWRECORD)
			{
				filenumber = 0;
			}
		}
	}
	sprintf(tempname, "databufferlow%u", filenumber);
	(void) readSpiffsBuffer2(tempname, pagebyte);

	StrNCpy(data, pagebyte, MAX_WRITEBYTEPAGE);
}

void get_lorecord_web(T8U * data)//hexa string maximum 128 chars
{

}
//==============================================================================
//
//	FUNCTION NAME:
//	DESCRIPTION:
//	PARAMETERS:
//	RETURN:
//
//==============================================================================
T32U TOP_Message = 0;
//T8U topData[2][64];
void save_toprecord(const LOG_NAME *log_name, T8U * data, T8U slotname)
{
//	for(int i = 0; i < 64; i++)
//	{
//		topData[slotname/2][i] = 0;
//	}
//
//	topData[slotname/2][0] = log_name->cmd;
//	topData[slotname/2][1] = log_name->len;
//
//	for(int i = 0; i < log_name->len; i++)
//	{
//		topData[slotname/2][i+2] = data[i];
//	}

//	TOP_Message |= (1 << slotname);
	TOP_Message |= slotname;
}


void flush_record(T8U HIGHLOW)
{
	if(HIGHLOW == 1)
	{
		if(tbl_buf_pointer.high.rear != tbl_buf_pointer.high.front)
		{
			tbl_buf_pointer.high.rear++;
			if(tbl_buf_pointer.high.rear >= MAX_HIGHRECORD)
			{
				tbl_buf_pointer.high.rear = 0;
			}
			if(tbl_buf_pointer.high.front >= MAX_HIGHRECORD)
			{
				tbl_buf_pointer.high.front = 0;
			}
		}
	}
	else
	{
		if(tbl_buf_pointer.low.rear != tbl_buf_pointer.low.front)
		{
//			char tempname[25] = "";
//			sprintf(tempname, "databufferlow%u", tbl_buf_pointer.low.rear);
//			deletePage(tempname);
			tbl_buf_pointer.low.rear++;
			if(tbl_buf_pointer.low.rear >= MAX_LOWRECORD)
			{
				tbl_buf_pointer.low.rear = 0;
			}
			if(tbl_buf_pointer.low.front >= MAX_LOWRECORD)
			{
				tbl_buf_pointer.low.front = 0;
			}
		}
	}
	WriteTable(&T_BUF_POINTER, (T8U *)&tbl_buf_pointer, 0);
}


