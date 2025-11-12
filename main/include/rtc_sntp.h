/*
 * rtc.h
 *
 *  Created on: 1 Dec 2020
 *      Author: Administrator
 */

#ifndef RTC_SNTP_H_
#define RTC_SNTP_H_

#include "target.h"
#include <time.h>
#include <sys/time.h>

void RTC_Init(DATETIME *inittime);
void RTC_Set(DATETIME *newtime);
void RTC_Set_2(T32U newmilisec);
void RTC_get(DATETIME *gettime);
void RTC_get_ms(time_t *t);
void RTC_convert_to_term_ms(time_t *t, T16S adjustment);
time_t RTC_DATETIME_to_ms(DATETIME *temptime);
void RTC_print(DATETIME *inittime);
void time_sync_notification_cb(struct timeval *tv);
void obtain_time(void);
//esp_err_t example_connect(void);
//esp_err_t example_disconnect(void);

#endif /* RTC_SNTP_H_ */
