#ifndef UTIL_H_
#define UTIL_H_

#include <string.h>

#include "target.h"

typedef struct GUID
{
	T32U guid_serialid;
	T32U guid_eventid;
	T32U guid_datetimeid;
}GUID;

//str convertToString(unsigned char* a, int size);
void delay(T16U ms);
void delay_tick(T32U _tick);
void ChangeEndian(T8U destination[], T8U source[], T8U byteperfield);
void GetLine(T8U *Message, T8U *Destination, T8U Line);
T8U GetLineCount(T8U *Message);

T16U crc_calc(T16U crc, T8U *buf, T8U nbytes);
T16U Tag_Chksum(T8U *data);
T8U Verify_Chksum(T8U *data);
T16U uhfTag_Chksum(T8U *data);
T8U uhfVerify_Chksum(T8U *data);
T8U StrNCmp(T8U * Str1, T8U * Str2, T16U Len);
void StrNCpy(T8U * Str1, T8U * Str2, T16U Len);
void StrNRep(T8U * Str, T8U toreplace, T8U newdata, T16U Len);
void StrNRep2(T8U * Str, T8U toreplace, T16U Len, T16U maxLen);
T8U hexchar2int(char input);
T8U stringDatetime(DATETIME * time, T8U * result, T8U takenew);
void stringDatetime2(DATETIME * time, T8U * result);
T8U stringDatetime3(DATETIME * time, T8U * result, T8U takenew);
void GUIDConvertStrDate(T8U * tdt, T8U * result);
T8U checkSysTime(DATETIME *t1, DATETIME *t2);
void GUID_Generator(GUID * target);
void Get_TagDTime(T8U * tdt);

#endif /* UTIL_H_ */
