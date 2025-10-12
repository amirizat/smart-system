/*
 * ota.h
 *
 *  Created on: Feb 2, 2021
 *      Author: GPROTECH
 */

//#define OTA_G2				0x01
////...
//#define OTA_G2_BROTHER 		0x02	//always put at last

#define MAX_OTAMENU			6

void test_ota(void);
void test_ota_2(void);
void test_ota_3(void);
void test_ota4(void);
void roll_back();

extern T8U FLAG_OTA_CHANGEFIRM;
extern const T8U OTAMENUFUNCTION[MAX_OTAMENU][15];
extern T8U selected_OTA;
