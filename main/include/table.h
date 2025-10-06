/*
 * table.h
 *
 *  Created on: 5 Nov 2020
 *      Author: Administrator
 */

#ifndef TABLE_H_
#define TABLE_H_

#include "target.h"

#define TABLE_VERSION   	1
#define TABLE_VERSION_2   	2
#define TABLE_VERSION_3   	3
#define TABLE_VERSION_4   	4

#define MAX_SET_FILE	FILE_LASTNOPAGE
#define MAX_TITLE_LEN	15

#define FILE_PRESET				0
#define FILE_INFO 				1
#define FILE_EMP 				2
#define FILE_WIFI 				3
#define FILE_SETTING 			4
#define FILE_EVENT	 			5
#define FILE_LASTTAG			6
#define FILE_PARTIAL			7
#define FILE_MSG				8
#define FILE_MSG2				9
#define FILE_CONDITION 			10
#define FILE_CONDITION2			11
#define FILE_UDF 				12
#define FILE_UDF2 				13
#define FILE_OPSELECT			14
#define FILE_MOOP 				15
#define FILE_MOOP2 				16
#define FILE_MOOP3 				17
#define FILE_MOOP4 				18
#define FILE_MOOP5 				19
#define FILE_MOOP6 				20
#define FILE_MOOP7 				21
#define FILE_MOOP8 				22
#define FILE_MOOP9 				23
#define FILE_FIXOP 				24
#define FILE_FIXOP2				25
#define FILE_FIXOP3				26
#define FILE_POINTER 			27
#define FILE_BKPOINTER 			28
#define FILE_POINTER_FS			29
#define FILE_RUNNING_ID			30
#define FILE_BASICINFO1			31
#define FILE_BASICINFO2			32
#define FILE_BASICINFO3			33
#define FILE_EQTYINFO			34
#define FILE_EOVERALLINFO		35
#define FILE_MACHINEINFO		36
#define FILE_PREFIX				37
#define FILE_SPLIT				38
#define FILE_SPLIT2				39
#define FILE_LASTCHILDTAG		40
#define FILE_WRITEFAILED		41
#define FILE_NEEDWRITE			42
#define FILE_PROCESSSTAGE		43
#define FILE_MODIREASON			44
#define FILE_PRODGROUP			45
#define FILE_TEMPROSTAGE		46
#define FILE_LASTNOPAGE			47///always last

#define LEN_SN					12
#define LEN_ASSET_CODE			20
#define LEN_MO					10
#define LEN_COLOR				10
#define LEN_SIZE				10
#define LEN_PRODGRP				10
#define LEN_EMPCODE				10
//#define LEN_TAGID				5
#define MAX_SECURITY			64
//#define MAX_TBL_EMP			5
#define MAX_LEN_MESSAGE			256

#define MAX_EMPCOUNT			5
//#define MAX_OPCOUNT_SELECTION	5

#define MAX_CONDITION			15 //max condition to show is 15 [0] - [14], but have one extra to save what has been selected in loc [15]
#define MAX_CONDITION_1PAGE		8
#define LEN_CONDITON 			10

#define MAX_UDF					3
#define MAX_LABEL				3
#define MAX_UDF_1PAGE			2
#define LEN_UDFCAPTION			20
#define LEN_UDFLABEL			10

#define MAX_MOASSIGN			25
#define MAX_MOASSIGN_1PAGE		3
#define MAX_OPCOUNT_5			5

#define MAX_FIXOPASSIGN			5
#define MAX_FIXOPASSIGN_1PAGE	2
#define MAX_OPCOUNT_20			20

#define PARTIAL_SCANBUNDLETAG	0
#define	PARTIAL_FIRSTCLAIM		1
#define PARTIAL_TICKCLAIM		2

#define MAX_PREFIX				5
#define MAX_PREFIX_1PAGE		5

#define MAX_PROCESS_STAGE		10
#define LEN_PROCESS_STAGE		10

#define MAX_MODI_REASON			10
#define LEN_MODI_REASON			10

#define SPECIAL_ON 	0x15
#define SPECIAL_OFF	0xCC

//extern const char *PTR_FILENAME[MAX_SET_FILE];//[MAX_TITLE_LEN];
extern const char PTR_FILENAME[MAX_SET_FILE][MAX_TITLE_LEN];

//#pragma pack(1)
typedef const struct P_NAME
{
	char title[0];
	T16U len;
}P_NAME;
//#pragma pack()

//#pragma pack(1)
typedef const struct T_NAME
{
	T8U file;
	T8U key;
	T16U len;
}T_NAME;
//#pragma pack()

#pragma pack(1)
typedef struct TBL_PRESET
{
	T8U from;
}TBL_PRESET;
#pragma pack()

#pragma pack(1)
typedef struct PG_PRESET
{
	TBL_PRESET tbl_preset;
}PG_PRESET;
#pragma pack()

#pragma pack(1)
typedef struct TBL_SN
{
	T8U serialnumber[LEN_SN];
}TBL_SN;
#pragma pack()

#pragma pack(1)
typedef struct TBL_VERSION_T
{
	T8U firstTimeClear;
	T16U versiontable;
}TBL_VERSION_T;
#pragma pack()

#pragma pack(1)
typedef struct TBL_FLASHPROGRAM
{
	T8U flashprogram;
}TBL_FLASHPROGRAM;
#pragma pack()

#pragma pack(1)
typedef struct TBL_ADDR
{
	T32U addr;
}TBL_ADDR;
#pragma pack()

#pragma pack(1)
typedef struct TBL_ASSET
{
	T8U assetcode[LEN_ASSET_CODE];
}TBL_ASSET;
#pragma pack()

#pragma pack(1)
typedef struct TBL_GROUP
{
	T8U group[LEN_PRODGRP];
}TBL_GROUP;
#pragma pack()

#pragma pack(1)
typedef struct PG_INFO
{
	TBL_SN tbl_sn;
	TBL_VERSION_T tbl_version_t;
	TBL_FLASHPROGRAM tbl_flashprogram;
	TBL_ADDR tbl_addr;
	TBL_ASSET tbl_asset;
	TBL_GROUP tbl_group;
}PG_INFO;
#pragma pack()

#pragma pack(1)
typedef struct TBL_SECURITY //size 64
{
	T8U security[MAX_SECURITY];
}TBL_SECURITY;
#pragma pack()

#pragma pack(1)
typedef struct TBL_EMP //size 22
{
	T8U empcode[LEN_EMPCODE];
	T32U tagid;
	T8U securitylevel;
	DATETIME eventtime;
}TBL_EMP;
#pragma pack()

#pragma pack(1)
typedef struct PG_EMP
{
	TBL_SECURITY tbl_security;
	TBL_EMP tbl_emp;
}PG_EMP;
#pragma pack()

#pragma pack(1)
typedef struct TBL_WIFI
{
	T8U hasSet;
	T8U userAck;
	T8U tellUser;
	T8U ssid[33];
	T8U pssw[65];
}TBL_WIFI;
#pragma pack()

#pragma pack(1)
typedef struct TBL_SERVER
{
	T8U hasSet;
	T8U IPv4[16];
	T8U port[5];
}TBL_SERVER;
#pragma pack()

#pragma pack(1)
typedef struct PG_WIFI
{
	TBL_WIFI tbl_wifi;
	TBL_SERVER tbl_server;
}PG_WIFI;
#pragma pack()

#pragma pack(1)
typedef struct TBL_STATUS
{
	T8U sosid;
	T8U use;
	T8U losttime;
	T8U empin;
}TBL_STATUS;
#pragma pack()


#define USE_WIFI 	0
#define USE_485		1

#pragma pack(1)
typedef struct TBL_CONTROL
{
	T8U loginctrl;
	T8U prodgrp;
	T8U schedule;
	T8U antenna;
	T8U partialmode;//onepieceflow
	T8U defaultop;
	T8U moconfirmation;
	T8U recycletag;
	T8U autologout;
	T8U callhelp;
	T8U groupOpstrict;
	T8U	techservcode;
	T8U delay_10s;
	T8U notify_error;
	T8U modiclaim;
	T8U invertscreen;
	T8U rotatescreen;
	T8U uhfpower;
	T8U splitClaim;
	T8U splitCheckOp;
	T16U checkOp;
	T8U byPassSV;
	T8U readTagFull;
	T8U opValidation;
	T8U kickout;
	T16U autologout_minutes;
	T8U split;
	T8U lineproduction;
	T8U inputPoint;//firstTerminal;
	T8U strictMO;
	T8U assetCheck;
}TBL_CONTROL;
#pragma pack()

#pragma pack(1)
typedef struct PG_SETTING
{
	TBL_STATUS tbl_status;
	TBL_CONTROL tbl_control;
}PG_SETTING;
#pragma pack()

#pragma pack(1)
typedef struct TBL_TIME
{
	DATETIME time;
}TBL_TIME;
#pragma pack()

#pragma pack(1)
typedef struct PG_EVENT
{
	TBL_TIME start;
	TBL_TIME lost;
}PG_EVENT;
#pragma pack()

#pragma pack(1)
typedef struct TBL_LASTTAG
{
	T32U tagid;
	T8U MO[LEN_MO];
	T16U qty;
	T8U color[LEN_COLOR];
	T8U size[LEN_SIZE];
	T16U op[MAX_OPCOUNT_20];
	T8U empcode[LEN_EMPCODE];
}TBL_LASTTAG;
#pragma pack()

#pragma pack(1)
typedef struct PG_LASTTAG
{
	TBL_LASTTAG tbl_lasttag;
}PG_LASTTAG;
#pragma pack()

#pragma pack(1)
typedef struct TBL_PARTIAL
{
	T8U cp_state;
	T8U tagversion;
	T32U oritagid;
	T32U tagdtime;
	T16U remain_qty;
	T16U claim_qty;
	T16U claim_op[MAX_OPCOUNT_20];
	T8U claim_opratio[MAX_OPCOUNT_20];
}TBL_PARTIAL;
#pragma pack()

#pragma pack(1)
typedef struct PG_PARTIAL
{
	TBL_PARTIAL tbl_partial;
}PG_PARTIAL;
#pragma pack()

#pragma pack(1)
typedef struct TBL_MSG
{
	T8U message[MAX_LEN_MESSAGE];
}TBL_MSG;
#pragma pack()

#pragma pack(1)
typedef struct PG_MSG //1 and 2 same size
{
	T8U message[MAX_LEN_MESSAGE/2];
}PG_MSG;
#pragma pack()

#pragma pack(1)
typedef struct TBL_CONDITION
{
	T16U id;
	T8U caption[LEN_CONDITON];
}TBL_CONDITION;
#pragma pack()

#pragma pack(1)
typedef struct PG_CONDITION
{
	TBL_CONDITION tbl_condition[MAX_CONDITION_1PAGE];
}PG_CONDITION;
#pragma pack()

#pragma pack(1)
typedef struct LABEL
{
	T8U caption[LEN_UDFLABEL];
	T8U enabled;
}LABEL;
#pragma pack()

#pragma pack(1)
typedef struct TBL_UDF
{
	T8U caption[LEN_UDFCAPTION];
	LABEL label[MAX_LABEL];
}TBL_UDF;
#pragma pack()

#pragma pack(1)
typedef struct PG_UDF
{
	TBL_UDF tbl_udf[MAX_UDF_1PAGE];
}PG_UDF;
#pragma pack()

#pragma pack(1)
typedef struct TBL_MOOP_SELECT
{
	T16U op[MAX_OPCOUNT_5];
	T8U opratio[MAX_OPCOUNT_5];
	T16U previous_op;
}TBL_MOOP_SELECT;
#pragma pack()

#pragma pack(1)
typedef struct TBL_FIXOP_SELECT
{
	T16U op[MAX_OPCOUNT_20];
	T8U opratio[MAX_OPCOUNT_20];
	T16U previous_op;
}TBL_FIXOP_SELECT;
#pragma pack()

#pragma pack(1)
typedef struct PG_OPSELECT
{
	TBL_MOOP_SELECT moopselect;
	TBL_FIXOP_SELECT fixopselect;
}PG_OPSELECT;
#pragma pack()

#pragma pack(1)
typedef struct TBL_MOOP_ASSIGN
{
	T8U MO[LEN_MO];
	T16U OP[MAX_OPCOUNT_5];
	T8U	ratio[MAX_OPCOUNT_5];
	T16U previousOP[MAX_OPCOUNT_5];
	DATETIME recentlyuse;
}TBL_MOOP_ASSIGN;
#pragma pack()

#pragma pack(1)
typedef struct PG_MOOP
{
	TBL_MOOP_ASSIGN moopassign[MAX_MOASSIGN_1PAGE];
}PG_MOOP;
#pragma pack()

#pragma pack(1)
typedef struct TBL_FIXOP_ASSIGN
{
	T8U isgroupop;
	T16U op[MAX_OPCOUNT_20];
	T16U previous_op;
}TBL_FIXOP_ASSIGN;
#pragma pack()

#pragma pack(1)
typedef struct PG_FIXOP
{
	TBL_FIXOP_ASSIGN fixopassign[MAX_FIXOPASSIGN_1PAGE];
}PG_FIXOP;
#pragma pack()

#pragma pack(1)
typedef struct POINTER
{
	T16U front;
	T16U rear;
}POINTER;
#pragma pack()

#pragma pack(1)
typedef struct TBL_BUF_POINTER
{
	POINTER high;
	POINTER low;
}TBL_BUF_POINTER;
#pragma pack()

#pragma pack(1)
typedef struct PG_POINTER
{
	TBL_BUF_POINTER tbl_buf_pointer;
}PG_POINTER;
#pragma pack()

#pragma pack(1)
typedef struct TBL_FS_POINTER
{
	T16U front;
	T16U rear;
}TBL_FS_POINTER;
#pragma pack()

#pragma pack(1)
typedef struct PG_POINTER_FS
{
	TBL_FS_POINTER tbl_fs_pointer;
}PG_POINTER_FS;
#pragma pack()

#pragma pack(1)
typedef struct TBL_EVENTID
{
	T32U id;
}TBL_EVENTID;
#pragma pack()

#pragma pack(1)
typedef struct PG_RUNNING_ID
{
	TBL_EVENTID tbl_eventid;
}PG_RUNNING_ID;
#pragma pack()

#define LEN_OPNAME		100
#define LEN_SO			20
#define LEN_STYLENAME	50
#pragma pack(1)
typedef struct TBL_BASIC_INFO1
{
	T8U mo[LEN_MO];
	T8U soNo[LEN_SO];
	T8U styleName[LEN_STYLENAME];
}TBL_BASIC_INFO1;
#pragma pack()

#pragma pack(1)
typedef struct TBL_BASIC_INFO2 //115
{
	T8U isgroupop;
	T16U opNo;
	T8U opName[LEN_OPNAME];
//	T32U smv[2];
	float smv;
	T16U hourlyTarget;
	T16U TargetQty;
	T32U WorkMinuteSec;
}TBL_BASIC_INFO2;
#pragma pack()

#define MAX_DATA	20
#pragma pack(1)
typedef struct TBL_BASIC_INFO3
{
	T16U opNo[MAX_DATA];
	float smv[MAX_DATA];
}TBL_BASIC_INFO3;
#pragma pack()

#define LEN_TREND	20
#pragma pack(1)
typedef struct TBL_EQTY_INFO
{
	T16U AccOutput;
	T16U CurrentTarget;
	T16U HourlyOutput;
	T32U AccTerminalOutput;
    T8U Trend[LEN_TREND];
}TBL_EQTY_INFO;
#pragma pack()

#pragma pack(1)
typedef struct TBL_EOVERALL_INFO
{
//	T32U Efficiency[2];
	float Efficiency;
    T16U DefectQty;
	T16U RepairQty;
//	T32U DHU[2];
	float DHU;
    float LostTime;
    T32U errorLoqQty;
}TBL_EOVERALL_INFO;
#pragma pack()

#define LEN_MACHINETYPE		50
#pragma pack(1)
typedef struct TBL_MACHINE_INFO
{
    T8U machineType[LEN_MACHINETYPE];
}TBL_MACHINE_INFO;
#pragma pack()

#define LEN_PREFIX		20
#pragma pack(1)
typedef struct TBL_PREFIX
{
	T8U prefix[LEN_PREFIX];
    T8U uhfTagType;
	T32U tagCount;
}TBL_PREFIX;
#pragma pack()

#pragma pack(1)
typedef struct PG_PREFIX
{
	TBL_PREFIX tbl_prefix[MAX_PREFIX_1PAGE];
}PG_PREFIX;
#pragma pack()

#pragma pack(1)
typedef struct TBL_SPLIT
{
	T8U flag;		// 0 = iniitial; 1 = in progress
	T8U HF;
	T16U splitcount;
	T16U MotherRemainQty;
	T8U uid_reference[8];
	T32U mothertagidfordisp;
	T16U splitqty;
	T16U oriqty;
}TBL_SPLIT;
#pragma pack()

#pragma pack(1)
typedef struct TBL_SPLIT2
{
	T8U motheritem[112];
}TBL_SPLIT2;
#pragma pack()

//#pragma pack(1)
//typedef struct TBL_SPLITUHF_STATE
//{
//	T8U flag;		// 0 = iniitial; 1 = in progress
//	T16U splitcount;
//	T16U MotherRemainQty;
//	T8U uid_reference[8];
//	T8U motheritem[112];
//	T32U mothertagidfordisp;
//	T16U splitqty;
//}TBL_SPLITUHF_STATE;
//#pragma pack()

#pragma pack(1)
typedef struct TBL_LASTCHILDTAG
{
	T32U tagid[20];
	T8U pointer;
	T8U mode;
}TBL_LASTCHILDTAG;
#pragma pack()

#pragma pack(1)
typedef struct TBL_EPC
{
	T8U epc[16];
}TBL_EPC;
#pragma pack()

#define MAX_CHILDUHF	50
#pragma pack(1)
typedef struct TBL_LASTCHILDTAG_UHF //currently not store in memory. Exceed, if want to store. rearrange back
{
//	T8U epc[5][16];
	TBL_EPC tbl_epc[MAX_CHILDUHF];
	T8U pointer;
	T8U mode;
}TBL_LASTCHILDTAG_UHF;
#pragma pack()

#pragma pack(1)
typedef struct TBL_TAG
{
	T32U tagid;
	DATETIME eventtime;
}TBL_TAG;
#pragma pack()

#define MAX_WRITEFAILED		10
#pragma pack(1)
typedef struct TBL_WRITEFAILED
{
	T8U datachanged;
	T8U loc_front;
	T8U loc_rear;
	T8U prev_front;
	T8U prev_rear;
	TBL_TAG tbl_tag[MAX_WRITEFAILED];
}TBL_WRITEFAILED;
#pragma pack()

#pragma pack(1)
typedef struct TBL_NEEDWRITE
{
	T8U claimFrom;
	T8U tagUID[8];
	T8U tagItem[112];
}TBL_NEEDWRITE;
#pragma pack()

#define MAX_CLAIMEDTAG		10
#pragma pack(1)
typedef struct TBL_CLAIMEDTAG
{
	T8U loc;
//	T32U globalTagId[MAX_CLAIMEDTAG];
	T8U claimed_tag[MAX_CLAIMEDTAG][8];
	T16U op[MAX_CLAIMEDTAG];
}TBL_CLAIMEDTAG;
#pragma pack()

#define MAX_TEMP_DATA		100
#define MAX_LENPAGE 		128
#pragma pack(1)
typedef struct TBL_DATA
{
	T8U needsave;
//	T8U needsave_backup;
	T8U file;
//	T16U len;
	T8U data[MAX_LENPAGE];
}TBL_DATA;
#pragma pack()

#pragma pack(1)
typedef struct TBL_TEMP
{
//	T8U havechanged;
	TBL_DATA tbl_data[MAX_TEMP_DATA];
}TBL_TEMP;
#pragma pack()

#define MAX_BREAK 10
#pragma pack(1)
typedef struct TBL_BREAK
{
	T32U start_t;	//"BreakTimeStart": "\/Date(1654319700000+0800)\/",
	T32U end_t;		//"BreakTimeEnd": "\/Date(1654323300000+0800)\/"
}TBL_BREAK;
#pragma pack()

#pragma pack(1)
typedef struct TBL_SCREEN
{
	//server
	T16U TotalClaim;				//	TotalClaimOthersOp;
	float TotalClaimEarnMin;		//	TotalClaimEarnMinOthersOp
	T16U AccClaimQty;				//	accuOpOutputThisEmp;
	T16U OpQty[MAX_OPCOUNT_20];		//	accuOpOutputThisEmp;
	float AccQtyEarnMin;			//	AccQtyEarnMin = smv * AccQty
	T16U TerminalAccQty;			//	accuOpOutputAllEmp;
	T32U ScheduleStart;				//  "ScheduleStart": "\/Date(1654300800000+0800)\/",
	T32U ScheduleEnd;				//  "ScheduleEnd": "\/Date(1654344000000+0800)\/",
	T32U FirstTrxTime;				//  "FirstTrxTime": "\/Date(1654302096000+0800)\/",
	T32U targetTotalWorkHr;
	T8U breakCount;
	TBL_BREAK tbl_break[MAX_BREAK];
	//G2 calculate
	T16U HourlyOutput;
	T16U currentTarget;				//  calculate from daily target and hourly target
	T16S catchUpQty;				//	current target - TerminalAccQty
									//	totalEarnMinutes = earnMinuteOthersOp + earnMinuteCurrentOp;
	T16U AvgOutputHr;				//  AvgOutputHr =  Acc Output / (schedule start till current hour [0.1 have to + 1 to hour])
	T32U totalWorkHr;				//	total working hours = from login until now - break time - lost time
	float eff;						//	eff = totalEarnMinutes / total working hours (min)
	//////////////////////////////////
    T8U Trend[LEN_TREND];
    TBL_EOVERALL_INFO tbl_eoverall_info;
}TBL_SCREEN;
#pragma pack()

#define MAX_AP 			10
#define MAX_HTTP_MSG	100
#pragma pack(1)
typedef struct TBL_WIFI_DIAG
{
	T8U ip_lost;
	T8U reason_disconn;
	T8U cur_use;
	T8U bssid[MAX_AP][6];
	T8S rssi[MAX_AP];//close to zero is the best, at least  > -70 to have better data transfer
	T8U	httpSend[MAX_HTTP_MSG];
	T8U havessid;
}TBL_WIFI_DIAG;
#pragma pack()

#define MAX_RETRY_SPIFFS	10
#pragma pack(1)
typedef struct TBL_SPIFFS_DIAG
{
	T8U errMount;
	int errMountCode[MAX_RETRY_SPIFFS];
	T8U errMountMsg[50];
	T8U errMountCount;
	T8U errRead;
	int errReadCode[MAX_RETRY_SPIFFS];
	T8U errReadMsg[50];
	T8U errReadCount;
}TBL_SPIFFS_DIAG;
#pragma pack()

#define MAX_RETRY_LITTLEFS	10
#pragma pack(1)
typedef struct TBL_LITTLEFS_DIAG
{
	T8U errMount;
	int errMountCode[MAX_RETRY_LITTLEFS];
	T8U errMountMsg[50];
	T8U errMountCount;
	T8U errRead;
	int errReadCode[MAX_RETRY_LITTLEFS];
	T8U errReadMsg[50];
	T8U errReadCount;
}TBL_LITTLEFS_DIAG;
#pragma pack()

#pragma pack(1)
typedef struct TBL_CHECK_OP
{
//	T16U currentOP[20];
//	T16U previousOP;
	T16U previousClaimed;
	T8U status;
	T16U claimOP[20];
}TBL_CHECK_OP;
#pragma pack()

#pragma pack(1)
typedef struct TBL_PROCESS_STAGE
{
	char prosesstageccode[LEN_PROCESS_STAGE];
}TBL_PROCESS_STAGE;
#pragma pack()

#pragma pack(1)
typedef struct TBL_TEMP_PRO_STAGE
{
	char temp_pro_stage[LEN_PROCESS_STAGE];
}TBL_TEMP_PRO_STAGE;
#pragma pack()

#pragma pack(1)
typedef struct PG_PROCESS_STAGE
{
	TBL_PROCESS_STAGE tbl_process_stage[MAX_PROCESS_STAGE];
}PG_PROCESS_STAGE;
#pragma pack()

#pragma pack(1)
typedef struct TBL_MODI_REASON
{
	char modireason[LEN_MODI_REASON];
}TBL_MODI_REASON;
#pragma pack()

#pragma pack(1)
typedef struct PG_MODI_REASON
{
	TBL_MODI_REASON tbl_modi_reason[MAX_MODI_REASON];
}PG_MODI_REASON;
#pragma pack()

#pragma pack(1)
typedef struct CONV_BREAKDATA
{
	T32U start;
	T32U end;
}CONV_BREAKDATA;
#pragma pack()
extern CONV_BREAKDATA conv_breakdata[MAX_BREAK];

#pragma pack(1)
typedef struct TBL_PROD_GROUP
{
	T16U prod_group_id;
	T8U group[LEN_PRODGRP];
}TBL_PROD_GROUP;
#pragma pack()

#pragma pack(1)
typedef struct TEMP_PROD_GROUP
{
	T8U status;
	T8U message[30];
	T8U prod_group_name[10];
}TEMP_PROD_GROUP;
#pragma pack()

#pragma pack(1)
typedef struct TEMP_WIFI
{
	T8U hasSet;
	T8U ssid[33];
	T8U pssw[65];
}TEMP_WIFI;
#pragma pack()

extern TBL_SN tbl_sn;
extern TBL_EMP tbl_emp;
extern TBL_ADDR tbl_addr;
extern TBL_WIFI tbl_wifi;
extern TBL_SERVER tbl_server;
//extern TBL_GROUP tbl_group;
extern TBL_STATUS tbl_status;
extern TBL_CONTROL tbl_control;
extern TBL_PARTIAL tbl_partial;
extern TBL_MOOP_SELECT tbl_moop_select;
extern TBL_FIXOP_SELECT tbl_fixop_select;
extern TBL_BUF_POINTER tbl_buf_pointer;
extern TBL_FS_POINTER tbl_fs_pointer;
extern TBL_BASIC_INFO1 tbl_basic_info1;
extern TBL_BASIC_INFO2 tbl_basic_info2;
extern TBL_BASIC_INFO3 tbl_basic_info3;
//extern TBL_EQTY_INFO tbl_eqty_info;
//extern TBL_EOVERALL_INFO tbl_eoverall_info;
extern TBL_WRITEFAILED tbl_writefailed;
//extern TBL_WRITEFAILED tbl_writefailed2;
extern TBL_CLAIMEDTAG tbl_claimedtag;
//extern TBL_TEMP tbl_temp;
extern TBL_SCREEN tbl_screen;
extern TBL_WIFI_DIAG tbl_wifi_diag;
extern TBL_SPIFFS_DIAG tbl_spiffs_diag;
extern TBL_LITTLEFS_DIAG tbl_littlefs_diag;
extern TBL_CHECK_OP tbl_check_op;
//extern T8U TOP_Message;
//extern TBL_PROCESS_STAGE tbl_process_stage;
//extern TBL_MODI_REASON tbl_modi_reason;
extern TBL_PROD_GROUP tbl_prod_group;
extern TEMP_PROD_GROUP temp_prod_group;
extern TEMP_WIFI temp_wifi;
extern TBL_TEMP_PRO_STAGE tbl_temp_pro_stage;

//extern P_NAME P_INFO;
//extern P_NAME P_WIFI;
//extern P_NAME P_TERM_SET;
//extern P_NAME P_EMP;
//extern P_NAME P_ACTIVITY;
//extern P_NAME P_MSG;
//extern P_NAME P_CONDITION;
//extern P_NAME P_MOOP;
//extern P_NAME P_UDF;
//extern P_NAME P_FIXOP;
//extern P_NAME P_POINTER;
//extern PG_WIFI pg_wifi;

extern T_NAME T_PRESET;
extern T_NAME T_SN;
extern T_NAME T_VERSION_T;
extern T_NAME T_FLASHPROGRAM;
extern T_NAME T_ADDR;
extern T_NAME T_ASSET;
extern T_NAME T_GROUP;
extern T_NAME T_SECURITY;
extern T_NAME T_WIFI;
extern T_NAME T_SERVER	;
extern T_NAME T_STATUS;
extern T_NAME T_CONTROL;
extern T_NAME T_EMP;
extern T_NAME T_STARTTIME;
extern T_NAME T_LOSTTIME;
extern T_NAME T_LASTTAG;
extern T_NAME T_PARTIAL;
extern T_NAME T_MSG;
extern T_NAME T_CONDITION;
extern T_NAME T_UDF;
extern T_NAME T_MOOP_SELECT;
extern T_NAME T_FIXOP_SELECT;
extern T_NAME T_MOOP_ASSIGN;
extern T_NAME T_FIXOP_ASSIGN;
extern T_NAME T_BUF_POINTER;
extern T_NAME T_BUF_BKPOINTER;
extern T_NAME T_FS_POINTER;
extern T_NAME T_EVENTID;
extern T_NAME T_BASIC_INFO1;
extern T_NAME T_BASIC_INFO2;
extern T_NAME T_BASIC_INFO3;
extern T_NAME T_EQTY_INFO;
extern T_NAME T_EOVERALL_INFO;
extern T_NAME T_MACHINE_INFO;
extern T_NAME T_PREFIX;
extern T_NAME T_SPLIT;
extern T_NAME T_SPLIT2;
extern T_NAME T_LASTCHILDTAG;
extern T_NAME T_WRITEFAILED;
extern T_NAME T_NEEDWRITE;
extern T_NAME T_PROCESS_STAGE;
extern T_NAME T_MODI_REASON;
extern T_NAME T_PROD_GROUP;
extern T_NAME T_TEM_PRO_STAGE;

#define BYROM	0
#define BYRAM	1

void ReadTable(T_NAME *table, T8U *data, T8U recordid);
void WriteTable(T_NAME *table, T8U *data,  T8U recordid);
void readSpiffstoTemp(void);
//void writeTemptoSpiffs(void);
void readLittleFStoTemp(void);
void writeTemptoLittleFS(void);
T8U ReadTable2(T_NAME *table, T8U *data, T8U recordid);

//==================================================================================================
// MACRO
//==================================================================================================
#define CHK_SOSCALL_CTRL()			(T8U)(tbl_status.sosid == 0x1 ? 1:0)
#define CHK_SOSTECH_MODE_CTRL()		(T8U)(tbl_status.sosid > 0x1 ? 1:0)
#define CHK_SOSTECH_CALL_CTRL()		(T8U)(tbl_status.sosid == 0x2 ? 1:0)
#define CHK_SOSTECH_REPLY_CTRL()	(T8U)(tbl_status.sosid == 0x3 ? 1:0)
#define CHK_SOSTECH_ASSESS_CTRL()	(T8U)(tbl_status.sosid == 0x4 ? 1:0)
#define CHK_USE()					(T8U)(tbl_status.use)
#define CHK_EMPIN()					(T8U)(tbl_status.empin)
#define CHK_LOSTTIME()				(T8U)(tbl_status.losttime)

#define SET_USE()					tbl_status.use = 1;		\
									WriteTable(&T_STATUS, (T8U *)&tbl_status,0)

#define SET_EMPIN()					tbl_status.empin = 1;	\
									WriteTable(&T_STATUS, (T8U *)&tbl_status,0)

#define SET_LOSTTIME()				tbl_status.losttime=1;	\
									WriteTable(&T_STATUS, (T8U *)&tbl_status,0)

#define CLR_USE()					tbl_status.use = 0;		\
									WriteTable(&T_STATUS, (T8U *)&tbl_status,0)

#define CLR_EMPIN()					tbl_status.empin = 0;	\
									WriteTable(&T_STATUS, (T8U *)&tbl_status,0)

#define CLR_LOSTTIME()				tbl_status.losttime=0;	\
									WriteTable(&T_STATUS, (T8U *)&tbl_status,0)

#define CHK_LOGIN_CTRL()			(T8U)(tbl_control.loginctrl)
#define CHK_PRODGRP_CTRL()			(T8U)(tbl_control.prodgrp)
#define CHK_SCHEDULE_CTRL()			(T8U)(tbl_control.schedule)
#define CHK_ANTENNA_CTRL()			(T8U)(tbl_control.antenna)
#define CHK_PARTIALMODE_CTRL()		(T8U)(tbl_control.partialmode)
//#define CHK_CLOCKIN_CTRL()			(T8U)(tbl_control.clockin)
#define CHK_DEFAULTOP_CTRL()		(T8U)(tbl_control.defaultop)


#define SET_LOGIN_CTRL()			{(tbl_control.loginctrl = 1);		\
									WriteTable(&T_CONTROL, (T8U *)&tbl_control, 0);}

#define SET_PRODGRP_CTRL()			{(tbl_control.prodgrp = 1);		\
									WriteTable(&T_CONTROL, (T8U *)&tbl_control, 0);}

#define SET_SCHEDULE_CTRL()			{(tbl_control.schedule = 1);	\
									WriteTable(&T_CONTROL, (T8U *)&tbl_control, 0);}

#define SET_ANTENNA_CTRL()			{(tbl_control.antenna = 1);		\
									WriteTable(&T_CONTROL, (T8U *)&tbl_control, 0);}

#define SET_PARTIALMODE_CTRL()		{(tbl_control.partialmode = 1);	\
									WriteTable(&T_CONTROL, (T8U *)&tbl_control, 0);}

#define SET_CLOCKIN_CTRL()			{tbl_control.clockin = 1;		\
									WriteTable(&T_CONTROL, (T8U *)&tbl_control, 0);}

#define SET_DEFAULTOP_CTRL()		{(tbl_control.defaultop = 1);	\
									WriteTable(&T_CONTROL, (T8U *)&tbl_control, 0);}

#define SET_SOSCALL_CTRL()			{(tbl_status.sosid = 1);		\
									WriteTable(&T_STATUS, (T8U *)&tbl_status, 0);}

#define SET_SOSTECH_CALL_CTRL()		{(tbl_status.sosid = 2);		\
									WriteTable(&T_STATUS, (T8U *)&tbl_status, 0);}

#define SET_SOSTECH_REPLY_CTRL()	{(tbl_status.sosid = 3);		\
									WriteTable(&T_STATUS, (T8U *)&tbl_status, 0);}

#define SET_SOSTECH_ASSESS_CTRL()	{(tbl_status.sosid = 4);		\
									WriteTable(&T_STATUS, (T8U *)&tbl_status, 0);}

#define CLR_LOGIN_CTRL()			{(tbl_control.loginctrl = 0);		\
									WriteTable(&T_CONTROL, (T8U *)&tbl_control, 0);}

#define CLR_PRODGRP_CTRL()			{(tbl_control.prodgrp = 0);		\
									WriteTable(&T_CONTROL, (T8U *)&tbl_control, 0);}

#define CLR_SCHEDULE_CTRL()			{(tbl_control.schedule = 0);	\
									WriteTable(&T_CONTROL, (T8U *)&tbl_control, 0);}

#define CLR_ANTENNA_CTRL()			{(tbl_control.antenna = 0);		\
									WriteTable(&T_CONTROL, (T8U *)&tbl_control, 0);}

#define CLR_PARTIALMODE_CTRL()		{(tbl_control.partialmode = 0);	\
									WriteTable(&T_CONTROL, (T8U *)&tbl_control, 0);}

#define CLR_CLOCKIN_CTRL()			{tbl_control.clockin = 0;		\
									WriteTable(&T_CONTROL, (T8U *)&tbl_control, 0);}

#define CLR_DEFAULTOP_CTRL()		{(tbl_control.defaultop = 0);	\
									WriteTable(&T_CONTROL, (T8U *)&tbl_control, 0);}

#define CLR_SOSCALL_CTRL()			{(tbl_status.sosid = 0);		\
									WriteTable(&T_STATUS, (T8U *)&tbl_status, 0);}

#define CLR_SOSTECH_CTRL()			{(tbl_status.sosid = 0);		\
									WriteTable(&T_STATUS, (T8U *)&tbl_status, 0);}


#endif /* TABLE_H_ */
