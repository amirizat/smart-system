#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "esp_log.h"
#include "rtc_sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "table.h"

static const char* TAG = "util";

//str convertToString(T8U * a, int size)
//{
//    int i;
//    str s = "";
//    for (i = 0; i < size; i++)
//    {
//        s = s + a[i];
//    }
//    return s;
//}

T16U delayms_tick = 0;

void delay(T16U ms)
{
	delayms_tick = ms * 10;
	while (delayms_tick > 0)
	{
//		uart_StateMachine(&Uart_0_Queue);
	}
}

void delay_tick(T32U _tick)
{
	T32U i;

	for(i=0; i<1000; i++)
	{
		//ASM_NOP();
	}
	for(i=0; i<_tick; i++)
	{
		//ASM_NOP();
	}
}

void ChangeEndian(T8U destination[], T8U source[], T8U byteperfield)
{
	T8U i;
	for(i = 0; i < byteperfield; i++)
	{
		//destination[i] = source[byteperfield -1 - i];
		destination[i] = source[byteperfield -1 - i];
	}
}

void GetLine(T8U *Message, T8U *Destination, T8U Line)
{
//	#define LCD_MAXCHAR 16
	T8U linecount;
	T8U wordcount=0;
	T8U j = 0;
	for(linecount=0; linecount<Line ; linecount++)
	{
		while((*Message !='\0') && (*Message !='\n' ))
		{
			Message++;
		}
		Message++;
	}
	wordcount = 0;
	while((*Message !='\0') && (*Message !='\n' ))
	{
		*Destination=*Message;
		if((*Message == ' ') || (*Message == ':') || (*Message == ';') || (*Message == ',') ||
		   (*Message == '.') || (*Message == '1') || (*Message == '!') || (*Message == 'i') ||
		   (*Message == 'I') || (*Message == '"') || (*Message == '\'')|| (*Message == 'r') ||
		   (*Message == 't') || (*Message == '/') || (*Message == 'l'))
		{
			j++;
		}
		Destination++;
		Message++;
		if(++wordcount > (20+(j/2))-1)//LCD_MAXWIDTH)
		{
			break;
		}
	}
	*Destination = '\0';
//	#undef LCD_MAXCHAR
}

T8U GetLineCount(T8U *Message)
{
	T8U linecount = 0;
	linecount = (*Message=='\0') ?	0: 1;
	while((*Message !='\0'))
	{
 		if(*Message =='\n' )
		{
			linecount++;
		}
		Message++;
	}
	return linecount;
}

const T16U crc_table[256] =

{
	0x0000,0xC0C1,0xC181,0x0140,0xC301,0x03C0,0x0280,0xC241,0xC601,0x06C0,0x0780,
  	0xC741,0x0500,0xC5C1,0xC481,0x0440,0xCC01,0x0CC0,0x0D80,0xCD41,0x0F00,0xCFC1,
  	0xCE81,0x0E40,0x0A00,0xCAC1,0xCB81,0x0B40,0xC901,0x09C0,0x0880,0xC841,0xD801,
  	0x18C0,0x1980,0xD941,0x1B00,0xDBC1,0xDA81,0x1A40,0x1E00,0xDEC1,0xDF81,0x1F40,
  	0xDD01,0x1DC0,0x1C80,0xDC41,0x1400,0xD4C1,0xD581,0x1540,0xD701,0x17C0,0x1680,
  	0xD641,0xD201,0x12C0,0x1380,0xD341,0x1100,0xD1C1,0xD081,0x1040,0xF001,0x30C0,
  	0x3180,0xF141,0x3300,0xF3C1,0xF281,0x3240,0x3600,0xF6C1,0xF781,0x3740,0xF501,
  	0x35C0,0x3480,0xF441,0x3C00,0xFCC1,0xFD81,0x3D40,0xFF01,0x3FC0,0x3E80,0xFE41,
  	0xFA01,0x3AC0,0x3B80,0xFB41,0x3900,0xF9C1,0xF881,0x3840,0x2800,0xE8C1,0xE981,
 	0x2940,0xEB01,0x2BC0,0x2A80,0xEA41,0xEE01,0x2EC0,0x2F80,0xEF41,0x2D00,0xEDC1,
  	0xEC81,0x2C40,0xE401,0x24C0,0x2580,0xE541,0x2700,0xE7C1,0xE681,0x2640,0x2200,
  	0xE2C1,0xE381,0x2340,0xE101,0x21C0,0x2080,0xE041,0xA001,0x60C0,0x6180,0xA141,
  	0x6300,0xA3C1,0xA281,0x6240,0x6600,0xA6C1,0xA781,0x6740,0xA501,0x65C0,0x6480,
  	0xA441,0x6C00,0xACC1,0xAD81,0x6D40,0xAF01,0x6FC0,0x6E80,0xAE41,0xAA01,0x6AC0,
  	0x6B80,0xAB41,0x6900,0xA9C1,0xA881,0x6840,0x7800,0xB8C1,0xB981,0x7940,0xBB01,
  	0x7BC0,0x7A80,0xBA41,0xBE01,0x7EC0,0x7F80,0xBF41,0x7D00,0xBDC1,0xBC81,0x7C40,
  	0xB401,0x74C0,0x7580,0xB541,0x7700,0xB7C1,0xB681,0x7640,0x7200,0xB2C1,0xB381,
  	0x7340,0xB101,0x71C0,0x7080,0xB041,0x5000,0x90C1,0x9181,0x5140,0x9301,0x53C0,
  	0x5280,0x9241,0x9601,0x56C0,0x5780,0x9741,0x5500,0x95C1,0x9481,0x5440,0x9C01,
  	0x5CC0,0x5D80,0x9D41,0x5F00,0x9FC1,0x9E81,0x5E40,0x5A00,0x9AC1,0x9B81,0x5B40,
  	0x9901,0x59C0,0x5880,0x9841,0x8801,0x48C0,0x4980,0x8941,0x4B00,0x8BC1,0x8A81,
  	0x4A40,0x4E00,0x8EC1,0x8F81,0x4F40,0x8D01,0x4DC0,0x4C80,0x8C41,0x4400,0x84C1,
  	0x8581,0x4540,0x8701,0x47C0,0x4680,0x8641,0x8201,0x42C0,0x4380,0x8341,0x4100,
  	0x81C1,0x8081,0x4040
};


T16U crc_calc(T16U crc, T8U *buf, T8U nbytes)
{
      T8U *p, *lim;

      p = (T8U *)buf;
      lim = p + nbytes;
      while (p < lim)
      {
            crc = (crc >> 8 ) ^ crc_table[(crc & 0xFF) ^ *p++];
      }
      return crc;
}

T16U Tag_Chksum(T8U *data)
{
	T16U crc_result=0X00;

	crc_result = crc_calc(crc_result, data, 10);
	crc_result = crc_calc(crc_result, data + 12, 100);
	return crc_result;
}

T8U Verify_Chksum(T8U *data)
{
	T16U crc_result=0;
	crc_result = Tag_Chksum(data);
	if ((*(data + 11) == (T8U)crc_result)  && (*(data + 10) ==(T8U)(crc_result>>8)))
	{
		return _PASS;
	}
	/*
	else
	{
		*(data + 11) = (T8U)crc_result;
		*(data + 10) ==(T8U)(crc_result>>8);
	}
	return PASS; //
	*/
	return _FAIL;
}

T16U uhfTag_Chksum(T8U *data)
{
	T16U crc_result=0X00;

	crc_result = crc_calc(crc_result, data, 2);
	crc_result = crc_calc(crc_result, data + 4, 60);
	return crc_result;
}

T8U uhfVerify_Chksum(T8U *data)
{
	T16U crc_result=0;
	crc_result = uhfTag_Chksum(data);
	if ((*(data + 3) == (T8U)crc_result)  && (*(data + 2) ==(T8U)(crc_result>>8)))
	{
		return _PASS;
	}
	return _FAIL;
}

T8U StrNCmp(T8U * Str1, T8U * Str2, T16U Len)
{
	T8U status = 0;
	T16U i = 0;
	for(i = 0; i < Len; i++)
	{
		if(Str1[i] != Str2[i])
		{
			status = 1;
			break;
		}
	}
	return status;
}

void StrNCpy(T8U * Str1, T8U * Str2, T16U Len)
{
	T16U i = 0;
//	T8U status = 0;

	for(i = 0; i < Len; i++)
	{
		*(Str1 + i) = *(Str2 + i);
	}
	//Str1[i] = '\0';
	return;
}

void StrNRep(T8U * Str, T8U toreplace, T8U newdata, T16U Len)
{
	T16U i = 0;
//	T8U status = 0;

	for(i = 0; i < Len; i++)
	{
		if(*(Str + i) ==  toreplace)
		{
			*(Str + i) = newdata;
		}
	}
	//Str1[i] = '\0';
	return;
}

void StrNRep2(T8U * Str, T8U toreplace, T16U Len, T16U maxLen)
{
	T16U i, j = 0;
	T16U tempLen = Len;

	for(i = 0; i < tempLen; i++)
	{
		if(*(Str + i) ==  toreplace)
		{
			if(tempLen + 3 > maxLen) return;

			for(j=tempLen; j > i; j=j-2)
			{
				*(Str + j + 1) = *(Str + j -1);
				*(Str + j + 2) = *(Str + j);
			}

			*(Str + i + 0) = '%';

			if(toreplace == 0x20)//space
			{
				*(Str + i + 1) = '2';
				*(Str + i + 2) = '0';
			}

			tempLen += 2;
		}
	}
	return;
}

T8U hexchar2int(char input)
{
  if(input >= '0' && input <= '9')
    return input - '0';
  if(input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if(input >= 'a' && input <= 'f')
    return input - 'a' + 10;

  return 0;
}

T8U stringDatetime(DATETIME * time, T8U * result, T8U takenew)//"yyyy-MM-dd-HH-mm-ss"
{
//	char tempstring[30] = {0};
	T16U year = 0;
	T8U correction = NO;

	ESP_LOGI(TAG, "time: %u-%u-%u", time->year, time->month, time->day);
	if((time->year == 0) || (time->month == 0) || (time->day == 0) || (takenew == YES))
	{
//		ESP_LOGI(TAG, "wrong time: %02u-%02u-%02u", time->year, time->month, time->day);
		vTaskDelay(1);
		RTC_get(time);
		correction = YES;

	}
	year = 2000 + time->year;
	sprintf((char*)result, "%u-%02u-%02u-%02u-%02u-%02u", year, time->month, time->day, time->hour, time->minute, time->second);
//	strcpy((char*)&result, tempstring);
	ESP_LOGI(TAG, "%s", (char*)result);

	return correction;
}

void stringDatetime2(DATETIME * time, T8U * result)//"yyyy-MM-dd HH:mm:ss"
{
//	char tempstring[30] = {0};
	T16U year = 0;
//	T8U correction = NO;

//	ESP_LOGI(TAG, "time: %u-%u-%u", time->year, time->month, time->day);
//	if((time->year == 0) || (time->month == 0) || (time->day == 0) || (takenew == YES))
//	{
////		ESP_LOGI(TAG, "wrong time: %02u-%02u-%02u", time->year, time->month, time->day);
//		vTaskDelay(1);
//		RTC_get(time);
//		correction = YES;
//
//	}
	year = 2000 + time->year;
	sprintf((char*)result, "%u-%02u-%02u%%20%02u:%02u:%02u", year, time->month, time->day, time->hour, time->minute, time->second);
//	sprintf((char*)result, "%u-%02u-%02u%%20", year, time->month, time->day);
//	tempstring[0] = '%';
//	tempstring[1] = '2';
//	tempstring[2] = '0';
//	strcpy((char*)&result, tempstring);
//	sprintf(tempstring, "%02u:%02u:%02u", time->hour, time->minute, time->second);
//	strcpy((char*)&result, tempstring);
	ESP_LOGI(TAG, "%s", (char*)result);

//	return correction;
}

T8U stringDatetime3(DATETIME * time, T8U * result, T8U takenew)//"yyyy-MM-dd
{
//	char tempstring[30] = {0};
	T16U year = 0;
	T8U correction = NO;

	ESP_LOGI(TAG, "time: %u-%u-%u", time->year, time->month, time->day);
	if((time->year == 0) || (time->month == 0) || (time->day == 0) || (takenew == YES))
	{
//		ESP_LOGI(TAG, "wrong time: %02u-%02u-%02u", time->year, time->month, time->day);
		vTaskDelay(1);
		RTC_get(time);
		correction = YES;

	}
	year = 2000 + time->year;
	sprintf((char*)result, "%u-%02u-%02u", year, time->month, time->day);
//	strcpy((char*)&result, tempstring);
	ESP_LOGI(TAG, "%s", (char*)result);

	return correction;
}

void GUIDConvertStrDate(T8U * tdt, T8U * result)//"yyMMddHHmmss"
{
	DATETIME temptime;

	temptime.second = ((tdt[3] & 0xFF) & 0x1F) << 1;
	temptime.minute = (((tdt[2] & 0xFF) & 0x07) << 3) + ((tdt[3] & 0xFF) >> 5);
	temptime.hour   = (tdt[2] & 0xFF) >> 3;
	temptime.day	= (tdt[1] & 0xFF) & 0x1F;
	temptime.month  = (((tdt[0] & 0xFF)  & 0x01) << 3) + ((tdt[1] & 0xFF) >> 5);
	temptime.year   = (tdt[0] & 0xFF) >> 1;

	sprintf((char*)result, "%02u%02u%02u%02u%02u%02u", temptime.year, temptime.month, temptime.day, temptime.hour, temptime.minute, temptime.second);
//	strcpy((char*)&result, tempstring);
	ESP_LOGI(TAG, "%s", (char*)result);
}

T8U checkSysTime(DATETIME *t1, DATETIME *t2)
{
	T8U flag_error = _PASS;

	if(t1->year != t2->year)
	{
		flag_error = _FAIL;
	}
	if(t1->month != t2->month)
	{
		flag_error = _FAIL;
	}
	if(t1->day != t2->day)
	{
		flag_error = _FAIL;
	}
	if(t1->hour != t2->hour)
	{
		flag_error = _FAIL;
	}
	if(t1->minute != t2->minute)
	{
		flag_error = _FAIL;
	}
//	if(t1->second != t2->second)
//	{
//		flag_error = _FAIL;
//	}

	return flag_error;
}

void GUID_Generator(GUID * target)
{
//	T8U tempstring[20] = {""};
//	static T8U serialnumber[20] ={""};
	T8U serialnumber[20] ={""};
//	static TBL_EVENTID tbl_eventid;
	TBL_EVENTID tbl_eventid;
//	ReadTable(&T_SN, (T8U *)&tbl_sn, 0);
	ReadTable(&T_EVENTID, (T8U *)&tbl_eventid, 0);
	serialnumber[0] = tbl_sn.serialnumber[2];
	serialnumber[1] = tbl_sn.serialnumber[3];
	serialnumber[2] = tbl_sn.serialnumber[4];
	serialnumber[3] = tbl_sn.serialnumber[5];
	serialnumber[4] = tbl_sn.serialnumber[7];
	serialnumber[5] = tbl_sn.serialnumber[8];
	serialnumber[6] = tbl_sn.serialnumber[9];
	serialnumber[7] = tbl_sn.serialnumber[10];
	serialnumber[8] = tbl_sn.serialnumber[11];
	serialnumber[9] = '\0';
	tbl_eventid.id++;

//	currenttime.year = (T8U)YEAR;
//	currenttime.month = (T8U)MONTH;
//	currenttime.day = (T8U)DOM;
//	currenttime.dayofweek = (T8U)DOW;
//	currenttime.hour = (T8U)HOUR;
//	currenttime.minute = (T8U)MIN;
//	currenttime.second = (T8U)SEC;
	RTC_get(&currenttime);
	WriteTable(&T_EVENTID, (T8U *)&tbl_eventid, 0);
	target->guid_serialid =  atol((char *)&serialnumber);
	target->guid_eventid =  (tbl_eventid.id << 8) + currenttime.year;
//	target->guid_datetimeid = (currenttime.year << 26)  + (currenttime.month << 22) + (currenttime.day << 17) +
//							  (currenttime.hour << 12) + (currenttime.minute << 6) +
//							  (currenttime.second);
	//make it standard with gadblet.It is like tadDtime.
  	target->guid_datetimeid = (currenttime.year << 25)  + (currenttime.month << 21) + (currenttime.day << 16) +
  							  (currenttime.hour << 11) + (currenttime.minute << 5) +
  							  (currenttime.second >> 1);
}

void Get_TagDTime(T8U * tdt)
{
	tdt[0] = (currenttime.month >> 3) 	+  (currenttime.year << 1) ;
	tdt[1] = (currenttime.day & 0x1F) 	+  ((currenttime.month & 0x07) << 5) ;
	tdt[2] = (currenttime.minute >> 3) 	+  (currenttime.hour << 3) ;
	tdt[3] = (currenttime.second >> 1) 	+  (currenttime.minute << 5) ;
}

