#include "app.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "esp_log.h"
#include "databuffer.h"
#include "keypad.h"
// #include "screen.h"
#include "table.h"
#include "util.h"
#include "display.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rtc_sntp.h"
#include "handleSpiffs.h"
//#include <sys/time.h>

MENUID menuid = MENUID_IDLE;
MENUID MENU_IDLE(void);
MENUID MENU_MENU(void);

T8U MenuIdle_State = 0;