#include "rtc_sntp.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_sntp.h"
#include "esp_system.h"
#include "esp_event.h"

static const char* TAG = "rtc_sntp";
static void initialize_sntp(void);

void RTC_Init(DATETIME *inittime)
{
    struct tm tm;

    tm.tm_year = inittime->year + 2000 - 1900;
    tm.tm_mon = inittime->month - 1;
    tm.tm_mday = inittime->day;
    tm.tm_hour = inittime->hour;
    tm.tm_min = inittime->minute;
    tm.tm_sec = inittime->second;

    time_t t = mktime(&tm);
    ESP_LOGI(TAG, "Init time: %s", asctime(&tm));
    struct timeval now = { .tv_sec = t };
    settimeofday(&now, NULL);
}

void RTC_Set(DATETIME *newtime)
{
	struct tm tm;
    tm.tm_year = newtime->year + 2000 - 1900;
    tm.tm_mon = newtime->month - 1;
    tm.tm_mday = newtime->day;
    tm.tm_hour = newtime->hour;
    tm.tm_min = newtime->minute;
    tm.tm_sec = newtime->second;

    time_t t = mktime(&tm);
    ESP_LOGI(TAG, "Set time: %s", asctime(&tm));
    struct timeval now = { .tv_sec = t };
    settimeofday(&now, NULL);
}

void RTC_Set_2(T32U newmilisec)
{
    time_t t = newmilisec;
    ESP_LOGI(TAG, "Set time: %lu", t);
    struct timeval now = { .tv_sec = t };
    settimeofday(&now, NULL);
}

void RTC_get(DATETIME *gettime)
{
    time_t now = 0;
    struct tm timeinfo = { 0 };
    time(&now);
	localtime_r(&now, &timeinfo);

	int year = timeinfo.tm_year + 1900 - 2000;
	gettime->year = (T8U)year;
	gettime->month = (T8U)timeinfo.tm_mon + 1;
	gettime->day = (T8U)timeinfo.tm_mday;
	gettime->dayofweek = (T8U)timeinfo.tm_wday;
	gettime->hour = (T8U)timeinfo.tm_hour;
	gettime->minute = (T8U)timeinfo.tm_min;
	gettime->second = (T8U)timeinfo.tm_sec;
}

void RTC_get_ms(time_t *t)
{
    struct tm timeinfo = { 0 };
    time(t);
	localtime_r(t, &timeinfo);
}

void RTC_convert_to_term_ms(time_t *t, T16S adjustment)
{
	T8S hour = (T8S) (adjustment / 100);
	T8S minute = (T8S) (adjustment - (hour*100));
	adjustment = ((hour * 60) + minute) * 60; //in seconds

	*t = *t + adjustment;
}

time_t RTC_DATETIME_to_ms(DATETIME *temptime)
{
	struct tm tm;
    tm.tm_year = temptime->year + 2000 - 1900;
    tm.tm_mon = temptime->month - 1;
    tm.tm_mday = temptime->day;
    tm.tm_hour = temptime->hour;
    tm.tm_min = temptime->minute;
    tm.tm_sec = temptime->second;

    time_t t = mktime(&tm);
    return t;
}

void RTC_print(DATETIME *inittime)
{
    struct tm tm;

    tm.tm_year = inittime->year + 2000 - 1900;
    tm.tm_mon = inittime->month - 1;
    tm.tm_mday = inittime->day;
    tm.tm_hour = inittime->hour;
    tm.tm_min = inittime->minute;
    tm.tm_sec = inittime->second;

    time_t t = mktime(&tm);
    ESP_LOGI(TAG, "Print time: %s", asctime(&tm));
}

void time_sync_notification_cb(struct timeval *tv)
{
    time_t now = 0;
    struct tm timeinfo = { 0 };
    setenv("TZ", "CST-8", 1);
	tzset();
	time(&now);
	localtime_r(&now, &timeinfo);
	sntp_stop();
	ESP_LOGI(TAG, "After get snpt: %s", asctime(&timeinfo));
	FLAG_STATE |= F_TSYNCSNTP;
}

void obtain_time(void)
{
    initialize_sntp();
}

static void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
#ifdef CONFIG_SNTP_TIME_SYNC_METHOD_SMOOTH
    sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
#endif
    sntp_init();
}
