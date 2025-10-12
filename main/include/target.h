#ifndef TARGET_H_
#define TARGET_H_

#include <stdbool.h>

#define _VERSION "0.0.0.1" //max 99.99.99.99

#ifndef DINAISY
#define DINAISY
#endif

#define _PASS	1
#define _FAIL 	0

#define EXPIRED	3

#define YES		1
#define NO		0

#define ON		1
#define OFF		0

#define PROCESS 2
#define READTAG 3

#define TICK50MS	50000
#define TICK100MS	100000

#define F_WIFICONNECT		0x00000001
#define F_WIFICONNECTING	0x00000002
#define F_WIFISNIFFER		0x00000004
#define F_T50MS				0x00000008
#define F_T100MS			0x00000010
#define F_TSYNCSERVER		0x00000020
#define F_TSYNCSNTP			0x00000040
#define F_WIFISNIFFING		0x00000080
#define F_CONNECTSERVER		0x00000100
#define F_OTAFAILED			0x00000200
#define F_UPDATEHEADER		0x00000400
#define F_OTASAMEVERSION	0x00000800
#define F_OTALOWVERSION		0x00001000
#define F_SOCKETFAILED		0x00002000
#define F_SOCKETOPENED		0x00004000
#define F_SENDINGDATA		0x00008000
#define F_RECONNECTWIFI		0x00010000
#define F_OTAPASSED			0x00020000
#define F_OTASTART			0x00040000
#define F_RECONNECTWIFI2	0x00080000
#define F_MEMORYCHECKING	0x00100000
#define F_NEEDUPDATEFIRM	0x00200000
#define F_CANDOWNLOADFIRM	0x00400000
#define F_NEWFIRMDONE		0x00800000
#define F_NEEDWRITEBUNDLE	0x01000000
#define F_NEEDWRITEGROUP	0x02000000
#define F_CHECKLINEPROD		0x04000000
#define F_NEEDWRITECHILDTAG	0x08000000
#define F_GETUHFMODULE		0x10000000
#define F_SERVERTIMEOUT		0x20000000
#define F_CONENCTFAILED		0x40000000
#define F_SCREENABORT		0x80000000

#define NOSENDACTIVITY_MIN	1200

#define MAX_LOGIN_TIMEOUT_MINUTE	180

#define G2_DIAGNOSTIC	0
#define G2				1 //all sdt will go as g2, no need clear memory (groupeffort, split,...)
#define G2_BROTHER		2

//////////////SAVE_DATA////////////////
#define SAVE_HOLD			0x01
#define SAVE_POINTER		0x02
#define SAVE_POINTER_FS		0x04
///////////////////////////////////////

typedef unsigned char 		T8U;
typedef signed char 		T8S;
typedef unsigned short		T16U;
typedef signed short 		T16S;
typedef unsigned int 		T32U;
typedef signed int 			T32S;
typedef unsigned long long	T64U;
typedef signed long long	T64S;

#pragma pack(1)
typedef struct DATETIME
{
	T8U year;
	T8U month;
	T8U day;
	T8U dayofweek;
	T8U hour;
	T8U minute;
	T8U second;
}DATETIME;
#pragma pack(1)

extern DATETIME currenttime;
extern T8U claimed_tag[8];
extern T32U FLAG_STATE;
extern T8U myIP[16];
extern T16U nosendactivity_mincount;
extern T8U one_minute;
extern T8U i2c_init;
extern T8U LED_STRIP_UPDATE;
extern T32U totalClaim;
extern T8U SAVE_DATA;
extern T8U macAddr[6];
extern T8U wifi_hasSet;
extern T8S wifiRssi;

void refresh_count(void);
void clear_writeBundle(void);

#endif /* TARGET_H_ */
