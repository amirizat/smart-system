/*
 * databuffer.h
 *
 *  Created on: 6 Nov 2020
 *      Author: Administrator
 */

#ifndef DATABUFFER_H_
#define DATABUFFER_H_

#include "target.h"

#define TOPDATA_POWERUP 					0x00000001	//pos = 1;
#define TOPDATA_SCHEDULE					0x00000002
#define TOPDATA_SYNCDATE 					0x00000004	//pos = 2;
#define TOPDATA_GETASSIGNMENT 				0x00000008	//pos = 3;
#define TOPDATA_DELETEASSIGNMENT			0x00000010	//pos = 4;
#define TOPDATA_RELOADASSIGNMENT			0x00000020	//pos = 5;
#define TOPDATA_GETMACHINEINFO				0x00000040	//pos = 6;
//#define TOPDATA_GETUHFMODULE				0x00000080	//pos = 7;
#define TOPDATA_GETQTYEMPLOYEE				0x00000100	//pos = 8;
#define TOPDATA_GETOVERALLEMPLOYEE			0x00000200	//pos = 9;
#define TOPDATA_GETPREFIX					0x00000400	//pos = 10;
#define TOPDATA_GETPREFIXDONE				0x00000800
#define TOPDATA_GETBUNDLEINFO				0x00001000	//pos = 11;
#define TOPDATA_GETBUNDLEINFODONE			0x00002000
#define TOPDATA_REGISTERUHFTAG				0x00004000	//pos = 12;
#define TOPDATA_REGISTERUHFTAGDONE  		0x00008000
#define TOPDATA_REGISTERUHFTAGEXPIRED  		0x00010000
#define TOPDATA_GETPRODUCTIONINFO			0x00020000	//pos = 13;
#define TOPDATA_GETINFO_AGAIN				0x00040000
#define TOPDATA_GETLASTCLAIMOP				0x00080000	//pos = 14;
#define TOPDATA_CHECKOPVALIDATION			0x00100000	//pos = 15;
#define TOPDATA_CHECKOPVALIDATION_2ND		0x00200000	//pos = 15;
#define TOPDATA_CHANGEGROUPTAG				0x00400000	//pos = 16;
#define TOPDATA_CHANGEGROUPTAGDONE			0x00800000	//pos = 16;
#define TOPDATA_HAVEMSG						0x01000000
#define TOPDATA_UPDATETERMINFO				0x02000000
#define TOPDATA_CHECKUPDATEFIRM				0x04000000
#define TOPDATA_TERMINALINFOBYMAC			0x08000000
#define TOPDATA_BASICINFOBYGROUP			0x10000000
#define TOPDATA_PRODUCTIONINFOBYGROUP		0x20000000
#define TOPDATA_GETPRODGROUPBYNAME			0x40000000
#define TOPDATA_CHECKINPUTOPCLAIMEDGROUP	0x80000000

#pragma pack(1)
typedef struct LOG_NAME
{
	T8U cmd;
	T8U len;
}LOG_NAME;
#pragma pack()

#pragma pack(1)
typedef struct ERROR_CLAIM
{
	T8U sv_empcode[10];
	T32U tagid;
	T32U oritagid;
	T32U tagdtime;
	T16U op[20];
}ERROR_CLAIM;
#pragma pack()

#pragma pack(1)
typedef struct LOG_ERROR//total 126 bytes
{
	T8U type;
	T8U employeecode[10];
	T8U functionName[20];
	DATETIME eventtime;
	union extra //max 88 bytes
	{
		T8U errorMessage[88];		//type 1
		ERROR_CLAIM error_claim;	//type 2
	}extra;
}LOG_ERROR;
#pragma pack()

#pragma pack(1)
typedef struct LOG_TDT_CLAIM_V2
{
	T8U type;
	T8U tagversion;
	T32U oritagid;
	T32U tagdtime;
	T16U op[20];
	T8U opratio[20];
	T16U qty;
	T8U employeecode[10];
	DATETIME starttime;
	DATETIME endtime;
}LOG_TDT_CLAIM_V2;
#pragma pack()

#pragma pack(1)
typedef struct LOG_TDT_CLAIM_V3
{
	T8U type;
	T8U tagversion;
	T32U oritagid;
	T32U tagdtime;
	T16U op[20];
	T8U opratio[20];
	T16U qty;
	T8U employeecode[10];
	DATETIME starttime;
	DATETIME endtime;
	T16U lineid;
}LOG_TDT_CLAIM_V3;
#pragma pack()

#pragma pack(1)
typedef struct LOG_GROUPTAG_CLAIM_V2
{
	T8U employeecode[10];
	T32U grouptagid;
	T8U grouptagcode[10];
	T32U group_oritagid;
	T8U GUID[4];
	T16U op[20];
	T8U opratio[20];
	DATETIME starttime;
	DATETIME endtime;
}LOG_GROUPTAG_CLAIM_V2;
#pragma pack()



#pragma pack(1)
typedef struct LOG_TDT_CLAIM
{
	T8U type;
	T8U tagversion;
	T32U oritagid;
	T32U tagdtime;
	T16U op[5];
	T8U opratio[5];
	T16U qty;
	T8U employeecode[10];
	DATETIME starttime;
	DATETIME endtime;
}LOG_TDT_CLAIM;
#pragma pack()

#pragma pack(1)
typedef struct LOG_GROUPTAG_REGISTER
{
	T8U type;
	T8U employeecode[10];
	T32U grouptagid;
	T8U grouptagcode[10];
	T32U grouptagoritagid;
	T8U grouptagGUID[4];
	T8U tagversion;
	T32U tagid;
	T32U tagwritecount;
	T32U itemnumber;
	T32U originaltagid;
	T32U guid;
	DATETIME eventtime;
}LOG_GROUPTAG_REGISTER;
#pragma pack()

#pragma pack(1)
typedef struct LOG_GROUPTAG_CLAIM
{
	T8U employeecode[10];
	T32U grouptagid;
	T8U grouptagcode[10];
	T32U group_oritagid;
	T8U GUID[4];
	T16U op[5];
	T8U opratio[5];
	DATETIME starttime;
	DATETIME endtime;
}LOG_GROUPTAG_CLAIM;
#pragma pack()

#pragma pack(1)
typedef struct LOG_BUNDLE_MODIFYQTY
{
	T8U empcode[10];
	T8U tagversion;
	T8U tagid[4];
	T8U itemnumber[4];
	T8U tagwritecount[4];
	T8U oritagid[4];
	T8U guid[4];
	T16U oldqty;
	T16U newqty;
	DATETIME eventtime;
	T8U terminaladdr[4];
	char processstage[10];
	char modireason[10];
}LOG_BUNDLE_MODIFYQTY;
#pragma pack()

#pragma pack(1)
typedef struct LOG_EMPLOYEE
{
	T8U empcode[10];
	DATETIME eventtime;
	T8U inout;
}LOG_EMPLOYEE;
#pragma pack()

#pragma pack(1)
typedef struct LOG_UDF
{
	T8U udfid;
	T8U empcode[10];
	T8U udftext[20];   //10 first use to send code repair key in by technician	//last 7 contain time.
}LOG_UDF;
#pragma pack()

#pragma pack(1)
typedef struct LOG_REQUEST_MO
{
	T8U mo[10];
}LOG_REQUEST_MO;
#pragma pack()

#pragma pack(1)
typedef struct LOG_LOSTTIME
{
	T8U empcode[10];
	DATETIME eventtime;
	T8U inout;
	T16U lostid;
}LOG_LOSTTIME;
#pragma pack()

#pragma pack(1)
typedef struct LOG_POWERUP
{
	T16U losttime_crc;
	T16U udf_crc;
	T16U security_crc;
}LOG_POWERUP;
#pragma pack()

#pragma pack(1)
typedef struct LOG_BUNDLE_CLAIMV3
{
	T8U claimtype;		//0 = claim; 1 = partialclaim
	T32U tagid;
	T32U tagwritecount;
	T32U itemnumber;
	T16U op[5];
	T8U opratio[5];
	T16U Qty;
	T8U employeecode[10];
	DATETIME Starttime;
	DATETIME Endtime;
}LOG_BUNDLE_CLAIMV3;
#pragma pack()

#pragma pack(1)
typedef struct LOG_BUNDLE_CLAIM_V1
{
	T32U tagid;
	T32U tagwritecount;
	T16U op[5];
	T8U opratio[5];
	T16U qty;
	T8U empcode[10];
	DATETIME starttime;
	DATETIME endtime;
}LOG_BUNDLE_CLAIM_V1;
#pragma pack()

#pragma pack(1)
typedef struct LOG_GARMENTREPAIR
{
	T8U type;
	T32U oritagid;
	T32U tagdtime;
	T8U employeecode[10];
	T16U tag_qty;
	T32U ID_eventid;
	T32U ID_datetimeid;
	T32U ID_SN;
	T8U QC_term[4];
	T16U OP_assigned;
	DATETIME starttime;
	DATETIME eventtime;
}LOG_GARMENTREPAIR;
#pragma pack()

#pragma pack(1)
typedef struct LOG_UPDATINGOPMANUALLY
{
	T8U type;	// mo op = 1, fix op =2
	T8U MO[10];
	T8U slot;  //mo op = 0, fix op = which slot
	T8U group_op; //mo op = 0 (no group op), fix op = 1 or 0 (op more then one is group)
	T16U op[15];
	union
	{
		T8U MO[10];
		T16U op[5];
	}MO_OR_OP;
	DATETIME eventtime;
}LOG_UPDATINGOPMANUALLY;
#pragma pack()

#pragma pack(1)
typedef struct LOG_NOTIFYERROR
{
	T8U type; //1 = pre op unclaimed, 2 = op select error, 3 = mo op select error, 4 = tag error, 5 = qty 0
	T32U oritagid;
	T32U tagdtime;
	T8U employeecode[10];
	DATETIME eventtime;
	T16U opNo;
}LOG_NOTIFYERROR;
#pragma pack()

#pragma pack(1)
typedef struct LOG_GROUPTAG_CHANGE
{
	T8U type;
	T8U employeecode[10];
	T32U group_oritagid_old;
	T8U GUID_old[4];
	T32U group_oritagid_new;
	T8U GUID_new[4];
	DATETIME eventtime;
}LOG_GROUPTAG_CHANGE;
#pragma pack()

#pragma pack(1)
typedef struct LOG_ASSET_TRACKING
{
	T8U type;			// = 1
	T32U machinetagid;
	T8U machinecode[20];      // changed from 20 to 10
	T8U empcode[10];
	DATETIME eventtime;
}LOG_ASSET_TRACKING;
#pragma pack()

#pragma pack(1)
typedef struct LOG_TDT_SPLIT_START
{
	T8U type;	//type == 1
	T8U mother_tagversion;
	T32U mother_oritagid;
	T32U mother_tagdtime;
	T8U employeecode[10];
	DATETIME eventtime;
}LOG_TDT_SPLIT_START;
#pragma pack()

#pragma pack(1)
typedef struct LOG_TDT_SPLIT_CHILDITEM
{
	T8U type;	//type == 2
	T8U mother_tagversion;
	T32U mother_oritagid;
	T32U mother_tagdtime;
	T32U child_tagid;
	T32U child_tagdtime;
	T32U child_tagwritecount;
	T16U split_sequence;
	T16U split_qty;
	T8U employeecode[10];
	DATETIME eventtime;
}LOG_TDT_SPLIT_CHILDITEM;
#pragma pack()

#pragma pack(1)
typedef struct LOG_TDT_SPLIT_END
{
	T8U type;	//type == 3
	T8U mother_tagversion;
	T32U mother_oritagid;
	T32U mother_tagdtime;
	T8U employeecode[10];
	DATETIME eventtime;
}LOG_TDT_SPLIT_END;
#pragma pack()

#pragma pack(1)
typedef struct LOG_REGISTEREPC
{
	T8U partial;
	T32U mother_oritagid;
	T32U mother_tagdtime;
	T8U employeecode[10];
	T16U split_qty;
	T8U childEPC[16];
	T8U childEPCLen;
	T16U opList[20];
	T8U opRatio[20];
	DATETIME starttime;
	DATETIME eventtime;
}LOG_REGISTEREPC;
#pragma pack()

extern const LOG_NAME DATA_LOG_ERROR;
extern const LOG_NAME DATA_TDT_CLAIM_V2;
extern const LOG_NAME DATA_TDT_CLAIM_V3;
extern const LOG_NAME DATA_GROUPTAG_CLAIM_V2;

extern const LOG_NAME DATA_TDT_CLAIM;
extern const LOG_NAME DATA_TDT_SPLIT_START;
extern const LOG_NAME DATA_TDT_SPLIT_CHILDITEM;
extern const LOG_NAME DATA_TDT_SPLIT_END;
extern const LOG_NAME DATA_GROUPTAG_REGISTER;
extern const LOG_NAME DATA_GROUPTAG_CLAIM;
extern const LOG_NAME DATA_BUNDLE_MODIFYQTY;
extern const LOG_NAME DATA_EMPLOYEEINOUT;
extern const LOG_NAME DATA_EMPLOYEEREQUEST;
extern const LOG_NAME DATA_UDF;
extern const LOG_NAME DATA_REQUEST_MO;
extern const LOG_NAME DATA_LOSTTIME;
extern const LOG_NAME DATA_POWERUP;
extern const LOG_NAME DATA_BUNDLE_CLAIMV3;
extern const LOG_NAME DATA_BUNDLE_CLAIMV1;
extern const LOG_NAME DATA_TDT_CLAIM_1;
extern const LOG_NAME DATA_TDT_CLAIM_2;
extern const LOG_NAME DATA_TDT_CLAIM_3;
extern const LOG_NAME DATA_GARMENTREPAIR;
extern const LOG_NAME DATA_UPDATINGOPMANUALLY;
extern const LOG_NAME DATA_NOTIFYERROR;
extern const LOG_NAME DATA_GROUPTAG_CHANGE;
extern const LOG_NAME DATA_GROUPTAG_CLAIM_1;
extern const LOG_NAME DATA_GROUPTAG_CLAIM_2;
extern const LOG_NAME DATA_GROUPTAG_CLAIM_3;
extern const LOG_NAME DATA_ASSET_TRACKING;

extern LOG_REGISTEREPC log_registerepc;
extern LOG_GROUPTAG_CHANGE log_grouptag_change;

void save_lorecord(const LOG_NAME *log_name, T8U * data);
void save_hirecord(const LOG_NAME *log_name, T8U * data);
void save_lorecord_2(const LOG_NAME *log_name, T8U * data);
void get_hirecord(T8U * data);
void get_hirecord_web(T8U * data);
void get_lorecord(T8U * data, T8U otherRecord);
void get_lorecord_web(T8U * data);
void save_toprecord(const LOG_NAME *log_name, T8U * data, T8U slotname);

T8U Is_MemoryFull(void);
void Set_MemoryFull(void);
void Clear_Memory(void);
T16U Get_RecordCount(void);
T16U Get_RecordHiCount(void);
T16U Get_RecordLoCount(void);
void flush_record(T8U HIGHLOW);

extern T32U TOP_Message;
//extern T8U topData[2][64];
extern T8U savingInProcess;
#endif /* DATABUFFER_H_ */
