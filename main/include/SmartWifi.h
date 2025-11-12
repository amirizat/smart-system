
#include "target.h"

void initialize_wifi(void);
void set_wifi(char *ssid , char *password);
//int get_finish_wifi_flag();
void saveWifiParam(T8U*ssid, T8U*password);
void reSniffWifi(void);
void reConnectWifi(void);
void scanWifi(void);
void reConnectWifi3(void);
extern T8U flag_espnow;
