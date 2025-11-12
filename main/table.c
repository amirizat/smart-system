/*
 * eepmap.c
 *
 *  Created on: 5 Nov 2020
 *      Author: Administrator
 */
#include "table.h"
#include "databuffer.h"
#include "handleSpiffs.h"
#include "util.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "handlelittleFS.h"

static const char* TAG = "table";

const char PTR_FILENAME[MAX_SET_FILE][MAX_TITLE_LEN] =
{
		"preset",
		"info",
		"emp",
		"wifi",
		"setting",
		"event",
		"lasttag",
		"partial",
		"msg1", "msg2",
		"condition1", "condition2",
		"udf1", "udf2",
		"opselect",
		"moop1", "moop2", "moop3", "moop4", "moop5", "moop6", "moop7", "moop8", "moop9",
		"fixop1", "fixop2", "fixop3",
		"pointer",
		"bkpointer",//back up pointer
		"pointerFS",
		"runningID",
		"BasicInfo1",
		"BasicInfo2",
		"BasicInfo3",
		"EQtyInfo",
		"EOverallInfo",
		"MachineInfo",
		"prefix",
		"split1",
		"split2",
		"lastChild",
		"writefail",
		"needwrite",
		"processstage",
		"modireason",
		"prodgroup",
		"tempprostage",

};

////////////////////////////Table inside page preset////////////////////////////1
T_NAME T_PRESET				=	{FILE_PRESET		,	1	,	sizeof(TBL_PRESET)};
////////////////////////////Table inside page info//////////////////////////////1
T_NAME T_SN					=	{FILE_INFO			,	1	,	sizeof(TBL_SN)};
T_NAME T_VERSION_T			=	{FILE_INFO			,	2	,	sizeof(TBL_VERSION_T)};
T_NAME T_FLASHPROGRAM		=	{FILE_INFO			,	3	,	sizeof(TBL_FLASHPROGRAM)};
T_NAME T_ADDR				=	{FILE_INFO			,	4	,	sizeof(TBL_ADDR)};
T_NAME T_ASSET				=	{FILE_INFO			,	5	,	sizeof(TBL_ASSET)};
T_NAME T_GROUP				=	{FILE_INFO			,	6	,	sizeof(TBL_GROUP)};
////////////////////////////Table inside page emp///////////////////////////////1
T_NAME T_SECURITY			=	{FILE_EMP			,	1	,	sizeof(TBL_SECURITY)};
T_NAME T_EMP				=	{FILE_EMP			,	2	,	sizeof(TBL_EMP)};
////////////////////////////Table inside page wifi//////////////////////////////1
T_NAME T_WIFI				=	{FILE_WIFI			,	1	,	sizeof(TBL_WIFI)};
T_NAME T_SERVER				=	{FILE_WIFI			,	2	,	sizeof(TBL_SERVER)};
////////////////////////////Table inside page setting///////////////////////////1
T_NAME T_STATUS				=	{FILE_SETTING		,	1	,	sizeof(TBL_STATUS)};
T_NAME T_CONTROL			=	{FILE_SETTING		,	2	,	sizeof(TBL_CONTROL)};
////////////////////////////Table inside page event/////////////////////////////1
T_NAME T_STARTTIME			=	{FILE_EVENT			,	1	,	sizeof(TBL_TIME)};
T_NAME T_LOSTTIME			=	{FILE_EVENT			,	2	,	sizeof(TBL_TIME)};
////////////////////////////Table inside page lasttag///////////////////////////1
T_NAME T_LASTTAG			=	{FILE_LASTTAG		,	1	,	sizeof(TBL_LASTTAG)};
////////////////////////////Table inside page partial///////////////////////////1
T_NAME T_PARTIAL			=	{FILE_PARTIAL		,	1	,	sizeof(TBL_PARTIAL)};
////////////////////////////Table inside page message///////////////////////////2
T_NAME T_MSG				=	{FILE_MSG			,	1	,	sizeof(TBL_MSG)};
////////////////////////////Table inside page condition/////////////////////////2
T_NAME T_CONDITION			=	{FILE_CONDITION		,	1	,	sizeof(TBL_CONDITION)};
////////////////////////////Table inside page udf///////////////////////////////2
T_NAME T_UDF				=	{FILE_UDF			,	1	,	sizeof(TBL_UDF)};
////////////////////////////Table inside page opselect//////////////////////////1
T_NAME T_MOOP_SELECT		=	{FILE_OPSELECT		,	1	,	sizeof(TBL_MOOP_SELECT)};
T_NAME T_FIXOP_SELECT		=	{FILE_OPSELECT		,	2	,	sizeof(TBL_FIXOP_SELECT)};
////////////////////////////Table inside page moop//////////////////////////////9
T_NAME T_MOOP_ASSIGN		=	{FILE_MOOP			,	1	,	sizeof(TBL_MOOP_ASSIGN)};
////////////////////////////Table inside page fxop//////////////////////////////2
T_NAME T_FIXOP_ASSIGN		=	{FILE_FIXOP			,	1	,	sizeof(TBL_FIXOP_ASSIGN)};
////////////////////////////Table inside page pointer///////////////////////////1
T_NAME T_BUF_POINTER		=	{FILE_POINTER		,	1	,	sizeof(TBL_BUF_POINTER)};
////////////////////////////Table inside page bkpointer//////////////////////////1
T_NAME T_BUF_BKPOINTER		=	{FILE_BKPOINTER		,	1	,	sizeof(TBL_BUF_POINTER)};
////////////////////////////Table inside page pointerFS//////////////////////////1
T_NAME T_FS_POINTER			=	{FILE_POINTER_FS	,	1	,	sizeof(TBL_FS_POINTER)};
////////////////////////////Table inside page runningID//////////////////////////1
T_NAME T_EVENTID	    	=	{FILE_RUNNING_ID	,	1	,	sizeof(TBL_EVENTID)};
////////////////////////////Table inside page BasicInfo1/////////////////////////1
T_NAME T_BASIC_INFO1		=	{FILE_BASICINFO1	,	1	,	sizeof(TBL_BASIC_INFO1)};
////////////////////////////Table inside page BasicInfo2/////////////////////////1
T_NAME T_BASIC_INFO2		=	{FILE_BASICINFO2	,	1	,	sizeof(TBL_BASIC_INFO2)};
////////////////////////////Table inside page BasicInfo3/////////////////////////1
T_NAME T_BASIC_INFO3		=	{FILE_BASICINFO3	,	1	,	sizeof(TBL_BASIC_INFO3)};
////////////////////////////Table inside page EmpQtyInfo/////////////////////////1
T_NAME T_EQTY_INFO	    	=	{FILE_EQTYINFO		,	1	,	sizeof(TBL_EQTY_INFO)};
////////////////////////////Table inside page EmpOverallInfo/////////////////////1
T_NAME T_EOVERALL_INFO		=	{FILE_EOVERALLINFO  ,	1	,	sizeof(TBL_EOVERALL_INFO)};
////////////////////////////Table inside page MachineInfo////////////////////////1
T_NAME T_MACHINE_INFO		=	{FILE_MACHINEINFO	,	1	,	sizeof(TBL_MACHINE_INFO)};
////////////////////////////Table inside page prefix/////////////////////////////1
T_NAME T_PREFIX				=	{FILE_PREFIX		,	1	,	sizeof(TBL_PREFIX)};
////////////////////////////Table inside page split//////////////////////////////1
T_NAME T_SPLIT				= 	{FILE_SPLIT			,	1	,	sizeof(TBL_SPLIT)};
////////////////////////////Table inside page split2/////////////////////////////1
T_NAME T_SPLIT2				= 	{FILE_SPLIT2		,	1	,	sizeof(TBL_SPLIT2)};
////////////////////////////Table inside page lastChild/////////////////////////////1
T_NAME T_LASTCHILDTAG 		=	{FILE_LASTCHILDTAG  ,	1	,   sizeof(TBL_LASTCHILDTAG)};
////////////////////////////Table inside page writefail/////////////////////////////1
T_NAME T_WRITEFAILED		=	{FILE_WRITEFAILED  	,	1	,   sizeof(TBL_WRITEFAILED)};
////////////////////////////Table inside page needwrite/////////////////////////////1
T_NAME T_NEEDWRITE			=	{FILE_NEEDWRITE  	,	1	,   sizeof(TBL_NEEDWRITE)};
////////////////////////////Table inside page process stage/////////////////////////////1
T_NAME T_PROCESS_STAGE		=	{FILE_PROCESSSTAGE  ,	1	,   sizeof(TBL_PROCESS_STAGE)};
////////////////////////////Table inside page modi reason/////////////////////////////1
T_NAME T_MODI_REASON		=	{FILE_MODIREASON 	,	1	,   sizeof(TBL_MODI_REASON)};
////////////////////////////Table inside page production group////////////////////1
T_NAME T_PROD_GROUP			=	{FILE_PRODGROUP 	,	1	,   sizeof(TBL_PROD_GROUP)};
////////////////////////////Table inside page production group////////////////////1
T_NAME T_TEM_PRO_STAGE		=	{FILE_TEMPROSTAGE 	,	1	,   sizeof(TBL_TEMP_PRO_STAGE)};

TBL_SN tbl_sn;
TBL_EMP tbl_emp;
TBL_ADDR tbl_addr;
TBL_WIFI tbl_wifi;
TBL_SERVER tbl_server;
//TBL_GROUP tbl_group;
TBL_STATUS tbl_status;
TBL_CONTROL tbl_control;
TBL_PARTIAL tbl_partial;
TBL_MOOP_SELECT tbl_moop_select;
TBL_FIXOP_SELECT tbl_fixop_select;
TBL_BUF_POINTER tbl_buf_pointer;
TBL_FS_POINTER tbl_fs_pointer;
TBL_BASIC_INFO1 tbl_basic_info1;
TBL_BASIC_INFO2 tbl_basic_info2;
TBL_BASIC_INFO3 tbl_basic_info3;
//TBL_EQTY_INFO tbl_eqty_info;
//TBL_EOVERALL_INFO tbl_eoverall_info;
TBL_WRITEFAILED tbl_writefailed;
//TBL_WRITEFAILED tbl_writefailed2;
TBL_CLAIMEDTAG tbl_claimedtag;
TBL_TEMP tbl_temp;
TBL_SCREEN tbl_screen;
TBL_WIFI_DIAG tbl_wifi_diag;
TBL_SPIFFS_DIAG tbl_spiffs_diag;
TBL_LITTLEFS_DIAG tbl_littlefs_diag;
TBL_CHECK_OP tbl_check_op;
CONV_BREAKDATA conv_breakdata[MAX_BREAK];
//TBL_PROCESS_STAGE tbl_process_stage;
//TBL_MODI_REASON tbl_modi_reason;
TBL_PROD_GROUP tbl_prod_group;
TEMP_PROD_GROUP temp_prod_group;
TEMP_WIFI temp_wifi;
TBL_TEMP_PRO_STAGE tbl_temp_pro_stage;

void readTemp(int title, int len, T8U *data);
void writeTemp(int title, int len, T8U *data);

T8U ReadTable2(T_NAME *table, T8U *data, T8U recordid)
{
	T8U result = _FAIL;
//	ESP_LOGI(TAG, "ReadTable");
	switch(table->file)
	{
		case FILE_INFO:
		{
			PG_INFO pg_info;
//			result = readSpiffsPage(FILE_INFO, sizeof(pg_info), (T8U*)&pg_info);
			result = readLittleFsPage(FILE_INFO, sizeof(pg_info), (T8U*)&pg_info);
			if(table->key == 1)
			{
				ESP_LOGI(TAG, "RT2 tbl_sn");
				StrNCpy(data, (T8U *)&pg_info.tbl_sn, table->len);
			}
			else if(table->key == 2)
			{
				ESP_LOGI(TAG, "RT2 tbl_version_t");
				StrNCpy(data, (T8U *)&pg_info.tbl_version_t, table->len);
			}
			else if(table->key == 3)
			{
				ESP_LOGI(TAG, "RT2 tbl_flashprogram");
				StrNCpy(data, (T8U *)&pg_info.tbl_flashprogram, table->len);
			}
			else if(table->key == 4)
			{
				ESP_LOGI(TAG, "RT2 tbl_addr");
				StrNCpy(data, (T8U *)&pg_info.tbl_addr, table->len);
			}
			else if(table->key == 5)
			{
				ESP_LOGI(TAG, "RT2 tbl_asset");
				StrNCpy(data, (T8U *)&pg_info.tbl_asset, table->len);
			}
			else if(table->key == 6)
			{
				ESP_LOGI(TAG, "RT2 tbl_group");
				StrNCpy(data, (T8U *)&pg_info.tbl_group, table->len);
			}
			break;
		}
		case FILE_SETTING:
		{
			PG_SETTING pg_setting;
//			result = readSpiffsPage(FILE_SETTING, sizeof(pg_setting), (T8U*)&pg_setting);
			result = readLittleFsPage(FILE_SETTING, sizeof(pg_setting), (T8U*)&pg_setting);
			if(table->key == 1)
			{
				ESP_LOGI(TAG, "tbl_status");
				StrNCpy(data, (T8U *)&pg_setting.tbl_status, table->len);
			}
			else if(table->key == 2)
			{
				ESP_LOGI(TAG, "tbl_control");
				StrNCpy(data, (T8U *)&pg_setting.tbl_control, table->len);
			}
			break;
		}
		default:
		{
			break;
		}
	}
	return result;
}

//void WriteTable(T_NAME *table, T8U *data, T8U recordid)
//{
//	ESP_LOGI(TAG, "WriteTable");
//	switch(table->file)
//	{
//		case FILE_PRESET:
//		{
//			PG_PRESET pg_preset;
//			readSpiffsPage(FILE_PRESET, sizeof(pg_preset), (T8U*)&pg_preset);
//			ESP_LOGI(TAG, "pg_preset");
//			StrNCpy((T8U *)&pg_preset.tbl_preset, data, table->len);
//			writeSpiffsPage(FILE_PRESET, sizeof(pg_preset), (T8U*)&pg_preset);
//			break;
//		}
//		case FILE_INFO:
//		{
//			PG_INFO pg_info;
////			readSpiffsPage(&P_INFO, (T8U *)&pg_info);
//			readSpiffsPage(FILE_INFO, sizeof(pg_info), (T8U*)&pg_info);
//			if(table->key == 1)
//			{
//				ESP_LOGI(TAG, "tbl_sn");
//				StrNCpy((T8U *)&pg_info.tbl_sn, data, table->len);
//			}
//			else if(table->key == 2)
//			{
//				ESP_LOGI(TAG, "tbl_version_t");
//				StrNCpy((T8U *)&pg_info.tbl_version_t, data, table->len);
//			}
//			else if(table->key == 3)
//			{
//				ESP_LOGI(TAG, "tbl_flashprogram");
//				StrNCpy((T8U *)&pg_info.tbl_flashprogram, data, table->len);
//			}
//			else if(table->key == 4)
//			{
//				ESP_LOGI(TAG, "tbl_addr");
//				StrNCpy((T8U *)&pg_info.tbl_addr, data, table->len);
//			}
//			else if(table->key == 5)
//			{
//				ESP_LOGI(TAG, "tbl_asset");
//				StrNCpy((T8U *)&pg_info.tbl_asset, data, table->len);
//			}
//			else if(table->key == 6)
//			{
//				ESP_LOGI(TAG, "tbl_group");
//				StrNCpy((T8U *)&pg_info.tbl_group, data, table->len);
//			}
////			writeSpiffsPage(&P_INFO, (T8U *)&pg_info);
//			writeSpiffsPage(FILE_INFO, sizeof(pg_info), (T8U*)&pg_info);
//			break;
//		}
//		case FILE_EMP:
//		{
//			PG_EMP pg_emp;
////			readSpiffsPage(&P_EMP, (T8U *)&pg_emp);
//			readSpiffsPage(FILE_EMP, sizeof(pg_emp), (T8U*)&pg_emp);
//			if(table->key == 1)
//			{
//				ESP_LOGI(TAG, "tbl_security");
//				StrNCpy((T8U *)&pg_emp.tbl_security, data, table->len);
//			}
//			else if(table->key == 2)
//			{
//				ESP_LOGI(TAG, "tbl_emp");
//				StrNCpy((T8U *)&pg_emp.tbl_emp, data, table->len);
//			}
////			writeSpiffsPage(&P_EMP, (T8U *)&pg_emp);
//			writeSpiffsPage(FILE_EMP, sizeof(pg_emp), (T8U*)&pg_emp);
//			break;
//		}
//		case FILE_WIFI:
//		{
//			PG_WIFI pg_wifi;
////			readSpiffsPage(&P_WIFI, (T8U *)&pg_wifi);
//			readSpiffsPage(FILE_WIFI, sizeof(pg_wifi), (T8U*)&pg_wifi);
//			if(table->key == 1)
//			{
//				ESP_LOGI(TAG, "tbl_wifi");
//				StrNCpy((T8U *)&pg_wifi.tbl_wifi, data, table->len);
//			}
//			else if(table->key == 2)
//			{
//				ESP_LOGI(TAG, "tbl_server");
//				StrNCpy((T8U *)&pg_wifi.tbl_server, data, table->len);
//			}
////			writeSpiffsPage(&P_WIFI, (T8U *)&pg_wifi);
//			writeSpiffsPage(FILE_WIFI, sizeof(pg_wifi), (T8U*)&pg_wifi);
//			break;
//		}
//		case FILE_SETTING:
//		{
//			PG_SETTING pg_setting;
////			readSpiffsPage(&P_TERM_SET, (T8U *)&pg_term_set);
//			readSpiffsPage(FILE_SETTING, sizeof(pg_setting), (T8U*)&pg_setting);
//			if(table->key == 1)
//			{
//				ESP_LOGI(TAG, "tbl_status");
//				StrNCpy((T8U *)&pg_setting.tbl_status, data, table->len);
//			}
//			else if(table->key == 2)
//			{
//				ESP_LOGI(TAG, "tbl_control");
//				StrNCpy((T8U *)&pg_setting.tbl_control, data, table->len);
//			}
////			writeSpiffsPage(&P_TERM_SET, (T8U *)&pg_term_set);
//			writeSpiffsPage(FILE_SETTING, sizeof(pg_setting), (T8U*)&pg_setting);
//			break;
//		}
//		case FILE_EVENT:
//		{
//			PG_EVENT pg_event;
//			readSpiffsPage(FILE_EVENT, sizeof(pg_event), (T8U*)&pg_event);
//			if(table->key == 1)
//			{
//				ESP_LOGI(TAG, "start");
//				StrNCpy((T8U *)&pg_event.start, data, table->len);
//			}
//			else if(table->key == 2)
//			{
//				ESP_LOGI(TAG, "lost");
//				StrNCpy((T8U *)&pg_event.lost, data, table->len);
//			}
//			writeSpiffsPage(FILE_EVENT, sizeof(pg_event), (T8U*)&pg_event);
//			break;
//		}
//		case FILE_LASTTAG:
//		{
//			PG_LASTTAG pg_lasttag;
////			readSpiffsPage(FILE_LASTTAG, sizeof(pg_lasttag), (T8U*)&pg_lasttag);
////			if(table->key == 1)
////			{
//				ESP_LOGI(TAG, "lasttag");
//				StrNCpy((T8U *)&pg_lasttag.tbl_lasttag, data, table->len);
////			}
//			writeSpiffsPage(FILE_LASTTAG, sizeof(pg_lasttag), (T8U*)&pg_lasttag);
//			break;
//		}
//		case FILE_PARTIAL:
//		{
//			PG_PARTIAL pg_partial;
////			readSpiffsPage(FILE_PARTIAL, sizeof(pg_partial), (T8U*)&pg_partial);
////			if(table->key == 1)
////			{
//				ESP_LOGI(TAG, "partial");
//				StrNCpy((T8U *)&pg_partial.tbl_partial, data, table->len);
////			}
//			writeSpiffsPage(FILE_PARTIAL, sizeof(pg_partial), (T8U*)&pg_partial);
//			break;
//		}
//		case FILE_MSG:
//		{
//			PG_MSG pg_msg;
////			PG_MSG pg_msg2;
////			readSpiffsPage(&P_MSG, (T8U *)&pg_msg);
////			readSpiffsPage(FILE_MSG, sizeof(pg_msg), (T8U*)&pg_msg);
////			readSpiffsPage(FILE_MSG2, sizeof(pg_msg2), (T8U*)&pg_msg2);
////			if(table->key == 1)
////			{
//				ESP_LOGI(TAG, "tbl_msg");
//				StrNCpy((T8U *)&pg_msg, data, (table->len / 2));
//				writeSpiffsPage(FILE_MSG, sizeof(pg_msg), (T8U*)&pg_msg);
//				StrNCpy((T8U *)&pg_msg, data + (table->len / 2), (table->len / 2));
//				writeSpiffsPage(FILE_MSG2, sizeof(pg_msg), (T8U*)&pg_msg);
////			}
////			writeSpiffsPage(&P_MSG, (T8U *)&pg_msg);
////			writeSpiffsPage(FILE_MSG, sizeof(pg_msg), (T8U*)&pg_msg);
//			break;
//		}
//		case FILE_CONDITION:
//		{
//			PG_CONDITION pg_condition;
//			T8U temploc = recordid / MAX_CONDITION_1PAGE;
////			readSpiffsPage(&P_CONDITION, (T8U *)&pg_condition);
//			readSpiffsPage(FILE_CONDITION + temploc, sizeof(pg_condition), (T8U*)&pg_condition);
////			if(table->key == 1)
////			{
//				ESP_LOGI(TAG, "tbl_condition");
//				temploc = recordid % MAX_CONDITION_1PAGE;
//				StrNCpy((T8U *)&pg_condition.tbl_condition[temploc], data, table->len);
////			}
////			writeSpiffsPage(&P_CONDITION, (T8U *)&pg_condition);
//			temploc = recordid / MAX_CONDITION_1PAGE;
//			writeSpiffsPage(FILE_CONDITION + temploc, sizeof(pg_condition), (T8U*)&pg_condition);
//			break;
//		}
//		case FILE_UDF:
//		{
//			PG_UDF pg_udf;
//			T8U temploc = recordid / MAX_UDF_1PAGE;
////			readSpiffsPage(&P_UDF, (T8U *)&pg_udf);
//			readSpiffsPage(FILE_UDF + temploc, sizeof(pg_udf), (T8U*)&pg_udf);
////			if(table->key == 1)
////			{
//				ESP_LOGI(TAG, "tbl_udf");
//				temploc = recordid % MAX_UDF_1PAGE;
//				StrNCpy((T8U *)&pg_udf.tbl_udf[temploc], data, table->len);
////			}
////			writeSpiffsPage(&P_UDF, (T8U *)&pg_udf);
//			temploc = recordid / MAX_UDF_1PAGE;
//			writeSpiffsPage(FILE_UDF + temploc, sizeof(pg_udf), (T8U*)&pg_udf);
//			break;
//		}
//		case FILE_OPSELECT:
//		{
//			PG_OPSELECT pg_opselect;
////			readSpiffsPage(&P_OPSELECT, (T8U *)&pg_opselect);
//			readSpiffsPage(FILE_OPSELECT, sizeof(pg_opselect), (T8U*)&pg_opselect);
//			if(table->key == 1)
//			{
//				ESP_LOGI(TAG, "tbl_moopselect");
//				StrNCpy((T8U *)&pg_opselect.moopselect, data, table->len);
//			}
//			else if(table->key == 2)
//			{
//				ESP_LOGI(TAG, "tbl_fixopselect");
//				StrNCpy((T8U *)&pg_opselect.fixopselect, data, table->len);
//			}
//			writeSpiffsPage(FILE_OPSELECT, sizeof(pg_opselect), (T8U*)&pg_opselect);
//			break;
//		}
//		case FILE_MOOP:
//		{
//			PG_MOOP pg_moop;
//			T8U temploc = recordid / MAX_MOASSIGN_1PAGE;
////			readSpiffsPage(&P_MOOP, (T8U *)&pg_moop);
//			readSpiffsPage(FILE_MOOP + temploc, sizeof(pg_moop), (T8U*)&pg_moop);
////			if(table->key == 1)
////			{
//				ESP_LOGI(TAG, "tbl_moopassignt");
//				temploc = recordid % MAX_MOASSIGN_1PAGE;
//				StrNCpy((T8U *)&pg_moop.moopassign[temploc], data, table->len);
////			}
////			writeSpiffsPage(&P_MOOP, (T8U *)&pg_moop);
////				ESP_LOGI(TAG, "len: %u", table->len);
//			temploc = recordid / MAX_MOASSIGN_1PAGE;
//			writeSpiffsPage(FILE_MOOP + temploc, sizeof(pg_moop), (T8U*)&pg_moop);
//			break;
//		}
//		case FILE_FIXOP:
//		{
//			PG_FIXOP pg_fixop;
//			T8U temploc = recordid / MAX_FIXOPASSIGN_1PAGE;
////			readSpiffsPage(&P_FIXOP, (T8U *)&pg_fixop);
//			readSpiffsPage(FILE_FIXOP + temploc, sizeof(pg_fixop), (T8U*)&pg_fixop);
////			if(table->key == 1)
////			{
//				ESP_LOGI(TAG, "fixopassign");
//				temploc = recordid % MAX_FIXOPASSIGN_1PAGE;
//				StrNCpy((T8U *)&pg_fixop.fixopassign[temploc], data, table->len);
////			}
////			writeSpiffsPage(&P_FIXOP, (T8U *)&pg_fixop);
//				temploc = recordid / MAX_FIXOPASSIGN_1PAGE;
//			writeSpiffsPage(FILE_FIXOP + temploc, sizeof(pg_fixop), (T8U*)&pg_fixop);
//			break;
//		}
//		case FILE_POINTER:
//		{
//			PG_POINTER pg_pointer;
////			readSpiffsPage(&P_POINTER, (T8U *)&pg_pointer);
//			readSpiffsPage(FILE_POINTER, sizeof(pg_pointer), (T8U*)&pg_pointer);
////			if(table->key == 1)
////			{
//				ESP_LOGI(TAG, "tbl_buf_pointer");
//				StrNCpy((T8U *)&pg_pointer.tbl_buf_pointer, data, table->len);
////			}
////			writeSpiffsPage(&P_POINTER, (T8U *)&pg_pointer);
//			writeSpiffsPage(FILE_POINTER, sizeof(pg_pointer), (T8U*)&pg_pointer);
////			ESP_LOGI(TAG, "tbl_fs_pointer.front %u", tbl_fs_pointer.front);
////			ESP_LOGI(TAG, "tbl_fs_pointer.rear %u", tbl_fs_pointer.rear);
//			break;
//		}
//		case FILE_POINTER_FS:
//		{
//			PG_POINTER_FS pg_pointer_fs;
//			readSpiffsPage(FILE_POINTER_FS, sizeof(pg_pointer_fs), (T8U*)&pg_pointer_fs);
//			ESP_LOGI(TAG, "tbl_fs_pointer");
//			StrNCpy((T8U *)&pg_pointer_fs.tbl_fs_pointer, data, table->len);
//			writeSpiffsPage(FILE_POINTER_FS, sizeof(pg_pointer_fs), (T8U*)&pg_pointer_fs);
//			break;
//		}
//		case FILE_RUNNING_ID:
//		{
//			PG_RUNNING_ID pg_running_id;
//			readSpiffsPage(FILE_RUNNING_ID, sizeof(pg_running_id), (T8U*)&pg_running_id);
//			ESP_LOGI(TAG, "tbl_eventid");
//			StrNCpy((T8U *)&pg_running_id.tbl_eventid, data, table->len);
//			writeSpiffsPage(FILE_RUNNING_ID, sizeof(pg_running_id), (T8U*)&pg_running_id);
//			break;
//		}
//		case FILE_BASICINFO1:
//		{
//			TBL_BASIC_INFO1 tbl_basic_info1;
//			readSpiffsPage(FILE_BASICINFO1, sizeof(tbl_basic_info1), (T8U*)&tbl_basic_info1);
//			ESP_LOGI(TAG, "tbl_basic_info1");
//			StrNCpy((T8U *)&tbl_basic_info1, data, table->len);
//			writeSpiffsPage(FILE_BASICINFO1, sizeof(tbl_basic_info1), (T8U*)&tbl_basic_info1);
//			break;
//		}
//		case FILE_BASICINFO2:
//		{
//			TBL_BASIC_INFO2 tbl_basic_info2;
//			readSpiffsPage(FILE_BASICINFO2, sizeof(tbl_basic_info2), (T8U*)&tbl_basic_info2);
//			ESP_LOGI(TAG, "tbl_eventid");
//			StrNCpy((T8U *)&tbl_basic_info2, data, table->len);
//			writeSpiffsPage(FILE_BASICINFO2, sizeof(tbl_basic_info2), (T8U*)&tbl_basic_info2);
//			break;
//		}
//		case FILE_EQTYINFO:
//		{
//			TBL_EQTY_INFO tbl_eqty_info;
//			readSpiffsPage(FILE_EQTYINFO, sizeof(tbl_eqty_info), (T8U*)&tbl_eqty_info);
//			ESP_LOGI(TAG, "tbl_eqty_info");
//			StrNCpy((T8U *)&tbl_eqty_info, data, table->len);
//			writeSpiffsPage(FILE_EQTYINFO, sizeof(tbl_eqty_info), (T8U*)&tbl_eqty_info);
//			break;
//		}
//		case FILE_EOVERALLINFO:
//		{
//			TBL_EOVERALL_INFO tbl_eoverall_info;
//			readSpiffsPage(FILE_EOVERALLINFO, sizeof(tbl_eoverall_info), (T8U*)&tbl_eoverall_info);
//			ESP_LOGI(TAG, "tbl_eoverall_info");
//			StrNCpy((T8U *)&tbl_eoverall_info, data, table->len);
//			writeSpiffsPage(FILE_EOVERALLINFO, sizeof(tbl_eoverall_info), (T8U*)&tbl_eoverall_info);
//			break;
//		}
//		case FILE_MACHINEINFO:
//		{
//			TBL_MACHINE_INFO tbl_machine_info;
//			readSpiffsPage(FILE_MACHINEINFO, sizeof(tbl_machine_info), (T8U*)&tbl_machine_info);
//			ESP_LOGI(TAG, "tbl_machine_info");
//			StrNCpy((T8U *)&tbl_machine_info, data, table->len);
//			writeSpiffsPage(FILE_MACHINEINFO, sizeof(tbl_machine_info), (T8U*)&tbl_machine_info);
//			break;
//		}
//		case FILE_PREFIX:
//		{
//			PG_PREFIX pg_prefix;
//			T8U temploc = recordid / MAX_PREFIX_1PAGE;
//			readSpiffsPage(FILE_PREFIX + temploc, sizeof(pg_prefix), (T8U*)&pg_prefix);
//			ESP_LOGI(TAG, "tbl_prefix");
//			temploc = recordid % MAX_PREFIX_1PAGE;
//			StrNCpy((T8U *)&pg_prefix.tbl_prefix[temploc], data, table->len);
//			temploc = recordid / MAX_PREFIX_1PAGE;
//			writeSpiffsPage(FILE_PREFIX + temploc, sizeof(pg_prefix), (T8U*)&pg_prefix);
//			break;
//		}
//		case FILE_SPLIT:
//		{
//			TBL_SPLIT tbl_split;
////			readSpiffsPage(FILE_SPLIT, sizeof(tbl_split), (T8U*)&tbl_split);
//			ESP_LOGI(TAG, "tbl_split");
//			StrNCpy((T8U *)&tbl_split, data, table->len);
//			writeSpiffsPage(FILE_SPLIT, sizeof(tbl_split), (T8U*)&tbl_split);
//			break;
//		}
//		case FILE_SPLIT2:
//		{
//			TBL_SPLIT2 tbl_split2;
////			readSpiffsPage(FILE_SPLIT2, sizeof(tbl_split2), (T8U*)&tbl_split2);
//			ESP_LOGI(TAG, "tbl_split2");
//			StrNCpy((T8U *)&tbl_split2, data, table->len);
//			writeSpiffsPage(FILE_SPLIT2, sizeof(tbl_split2), (T8U*)&tbl_split2);
//			break;
//		}
//		case FILE_WRITEFAILED:
//		{
//			TBL_WRITEFAILED tbl_wf;
////			readSpiffsPage(FILE_WRITEFAILED, sizeof(tbl_wf), (T8U*)&tbl_wf);
//			ESP_LOGI(TAG, "tbl_writefailed");
//			StrNCpy((T8U *)&tbl_wf, data, table->len);
//			writeSpiffsPage(FILE_WRITEFAILED, sizeof(tbl_wf), (T8U*)&tbl_wf);
//			break;
//		}
////		case FILE_WRITEFAILED2:
////		{
////			TBL_WRITEFAILED tbl_wf;
//////			readSpiffsPage(FILE_WRITEFAILED2, sizeof(tbl_wf), (T8U*)&tbl_wf);
////			ESP_LOGI(TAG, "tbl_writefailed2");
////			StrNCpy((T8U *)&tbl_wf, data, table->len);
////			writeSpiffsPage(FILE_WRITEFAILED2, sizeof(tbl_wf), (T8U*)&tbl_wf);
////			break;
////		}
//		default:
//		{
//			break;
//		}
//	}
//}

void ReadTable(T_NAME *table, T8U *data, T8U recordid)
{
//	ESP_LOGI(TAG, "ReadTable");
	switch(table->file)
	{
		case FILE_PRESET:
		{
			PG_PRESET pg_preset;
			readTemp(FILE_PRESET, sizeof(pg_preset), (T8U*)&pg_preset);
			ESP_LOGI(TAG, "RT pg_preset");
			StrNCpy(data, (T8U *)&pg_preset.tbl_preset, table->len);
			break;
		}
		case FILE_INFO:
		{
			PG_INFO pg_info;
			readTemp(FILE_INFO, sizeof(pg_info), (T8U*)&pg_info);
			if(table->key == 1)
			{
				ESP_LOGI(TAG, "RT tbl_sn");
				StrNCpy(data, (T8U *)&pg_info.tbl_sn, table->len);
			}
			else if(table->key == 2)
			{
				ESP_LOGI(TAG, "RT tbl_version_t");
				StrNCpy(data, (T8U *)&pg_info.tbl_version_t, table->len);
			}
			else if(table->key == 3)
			{
				ESP_LOGI(TAG, "RT tbl_flashprogram");
				StrNCpy(data, (T8U *)&pg_info.tbl_flashprogram, table->len);
			}
			else if(table->key == 4)
			{
				ESP_LOGI(TAG, "RT tbl_addr");
				StrNCpy(data, (T8U *)&pg_info.tbl_addr, table->len);
			}
			else if(table->key == 5)
			{
				ESP_LOGI(TAG, "RT tbl_asset");
				StrNCpy(data, (T8U *)&pg_info.tbl_asset, table->len);
			}
			else if(table->key == 6)
			{
				ESP_LOGI(TAG, "RT tbl_group");
				StrNCpy(data, (T8U *)&pg_info.tbl_group, table->len);
			}
			break;
		}
		case FILE_EMP:
		{
			PG_EMP pg_emp;
			readTemp(FILE_EMP, sizeof(pg_emp), (T8U*)&pg_emp);
			if(table->key == 1)
			{
				ESP_LOGI(TAG, "RT tbl_security");
				StrNCpy(data, (T8U *)&pg_emp.tbl_security, table->len);
			}
			 else if(table->key == 2)
			{
				ESP_LOGI(TAG, "RT tbl_emp");
				StrNCpy(data, (T8U *)&pg_emp.tbl_emp, table->len);
			}
			break;
		}
		case FILE_WIFI:
		{
			PG_WIFI pg_wifi;
			readTemp(FILE_WIFI, sizeof(pg_wifi), (T8U*)&pg_wifi);
			if(table->key == 1)
			{
				ESP_LOGI(TAG, "RT tbl_wifi");
				StrNCpy(data, (T8U *)&pg_wifi.tbl_wifi, table->len);
			}
			else if(table->key == 2)
			{
				ESP_LOGI(TAG, "RT tbl_server");
				StrNCpy(data, (T8U *)&pg_wifi.tbl_server, table->len);
			}
			break;
		}
		case FILE_SETTING:
		{
			PG_SETTING pg_setting;
			readTemp(FILE_SETTING, sizeof(pg_setting), (T8U*)&pg_setting);
			if(table->key == 1)
			{
				ESP_LOGI(TAG, "RT tbl_status");
				StrNCpy(data, (T8U *)&pg_setting.tbl_status, table->len);
			}
			else if(table->key == 2)
			{
				ESP_LOGI(TAG, "RT tbl_control");
				StrNCpy(data, (T8U *)&pg_setting.tbl_control, table->len);
			}
			break;
		}
		case FILE_EVENT:
		{
			PG_EVENT pg_event;
			readTemp(FILE_EVENT, sizeof(pg_event), (T8U*)&pg_event);
			if(table->key == 1)
			{
				ESP_LOGI(TAG, "RT start");
				StrNCpy(data, (T8U *)&pg_event.start, table->len);
			}
			else if(table->key == 2)
			{
				ESP_LOGI(TAG, "RT lost");
				StrNCpy(data, (T8U *)&pg_event.lost, table->len);
			}
			break;
		}
		case FILE_LASTTAG:
		{
			PG_LASTTAG pg_lasttag;
			readTemp(FILE_LASTTAG, sizeof(pg_lasttag), (T8U*)&pg_lasttag);
			ESP_LOGI(TAG, "RT lasttag");
			StrNCpy(data, (T8U *)&pg_lasttag.tbl_lasttag, table->len);
			break;
		}
		case FILE_PARTIAL:
		{
			PG_PARTIAL pg_partial;
			readTemp(FILE_PARTIAL, sizeof(pg_partial), (T8U*)&pg_partial);
			ESP_LOGI(TAG, "RT partial");
			StrNCpy(data, (T8U *)&pg_partial.tbl_partial, table->len);
			break;
		}
		case FILE_MSG:
		{
			PG_MSG pg_msg;
			PG_MSG pg_msg2;
			readTemp(FILE_MSG, sizeof(pg_msg), (T8U*)&pg_msg);
			readTemp(FILE_MSG2, sizeof(pg_msg2), (T8U*)&pg_msg2);
			ESP_LOGI(TAG, "RT tbl_msg");
			StrNCpy(data, (T8U *)&pg_msg, (table->len / 2));
			StrNCpy(data + (table->len / 2), (T8U *)&pg_msg2, (table->len / 2));
			break;
		}
		case FILE_CONDITION:
		{
			PG_CONDITION pg_condition;
			T8U temploc = recordid / MAX_CONDITION_1PAGE;
			readTemp(FILE_CONDITION + temploc, sizeof(pg_condition), (T8U*)&pg_condition);
			ESP_LOGI(TAG, "RT tbl_condition");
			temploc = recordid % MAX_CONDITION_1PAGE;
			StrNCpy(data, (T8U *)&pg_condition.tbl_condition[temploc], table->len);
			break;
		}
		case FILE_UDF:
		{
			PG_UDF pg_udf;
			T8U temploc = recordid / MAX_UDF_1PAGE;
			readTemp(FILE_UDF + temploc, sizeof(pg_udf), (T8U*)&pg_udf);
			ESP_LOGI(TAG, "RT tbl_udf");
			temploc = recordid % MAX_UDF_1PAGE;
			StrNCpy(data, (T8U *)&pg_udf.tbl_udf[temploc], table->len);
			break;
		}
		case FILE_OPSELECT:
		{
			PG_OPSELECT pg_opselect;
			readTemp(FILE_OPSELECT, sizeof(pg_opselect), (T8U*)&pg_opselect);
			if(table->key == 1)
			{
				ESP_LOGI(TAG, "RT tbl_moopselect");
				StrNCpy(data, (T8U *)&pg_opselect.moopselect, table->len);
			}
			else if(table->key == 2)
			{
				ESP_LOGI(TAG, "RT tbl_fixopselect");
				StrNCpy(data, (T8U *)&pg_opselect.fixopselect, table->len);
			}
			break;
		}
		case FILE_MOOP:
		{
			PG_MOOP pg_moop;
			T8U temploc = recordid / MAX_MOASSIGN_1PAGE;
			readTemp(FILE_MOOP + temploc, sizeof(pg_moop), (T8U*)&pg_moop);
			ESP_LOGI(TAG, "RT moopassign");
			temploc = recordid % MAX_MOASSIGN_1PAGE;
			StrNCpy(data, (T8U *)&pg_moop.moopassign[temploc], table->len);
			break;
		}
		case FILE_FIXOP:
		{
			PG_FIXOP pg_fixop;
			T8U temploc = recordid / MAX_FIXOPASSIGN_1PAGE;
			readTemp(FILE_FIXOP + temploc, sizeof(pg_fixop), (T8U*)&pg_fixop);
			ESP_LOGI(TAG, "RT fixopassign");
			temploc = recordid % MAX_FIXOPASSIGN_1PAGE;
			StrNCpy(data, (T8U *)&pg_fixop.fixopassign[temploc], table->len);
			break;
		}
		case FILE_POINTER:
		{
			PG_POINTER pg_pointer;
			readTemp(FILE_POINTER, sizeof(pg_pointer), (T8U*)&pg_pointer);
			ESP_LOGI(TAG, "RT tbl_buf_pointer");
			StrNCpy(data, (T8U *)&pg_pointer.tbl_buf_pointer, table->len);
			break;
		}
		case FILE_BKPOINTER:
		{
			PG_POINTER pg_pointer;
			readTemp(FILE_BKPOINTER, sizeof(pg_pointer), (T8U*)&pg_pointer);
			ESP_LOGI(TAG, "RT tbl_buf_bkpointer");
			StrNCpy(data, (T8U *)&pg_pointer.tbl_buf_pointer, table->len);
			break;
		}
		case FILE_POINTER_FS:
		{
			PG_POINTER_FS pg_pointer_fs;
			readTemp(FILE_POINTER_FS, sizeof(pg_pointer_fs), (T8U*)&pg_pointer_fs);
			ESP_LOGI(TAG, "RT tbl_fs_pointer");
			StrNCpy(data, (T8U *)&pg_pointer_fs.tbl_fs_pointer, table->len);
			break;
		}
		case FILE_RUNNING_ID:
		{
			PG_RUNNING_ID pg_running_id;
			readTemp(FILE_RUNNING_ID, sizeof(pg_running_id), (T8U*)&pg_running_id);
			ESP_LOGI(TAG, "RT tbl_eventid");
			StrNCpy(data, (T8U *)&pg_running_id.tbl_eventid, table->len);
			break;
		}
		case FILE_BASICINFO1:
		{
			TBL_BASIC_INFO1 tbl_basic_info1;
			readTemp(FILE_BASICINFO1, sizeof(tbl_basic_info1), (T8U*)&tbl_basic_info1);
			ESP_LOGI(TAG, "RT tbl_basic_info1");
			StrNCpy(data, (T8U *)&tbl_basic_info1, table->len);
			break;
		}
		case FILE_BASICINFO2:
		{
			TBL_BASIC_INFO2 tbl_basic_info2;
			readTemp(FILE_BASICINFO2, sizeof(tbl_basic_info2), (T8U*)&tbl_basic_info2);
			ESP_LOGI(TAG, "RT tbl_basic_info2");
			StrNCpy(data, (T8U *)&tbl_basic_info2, table->len);
			break;
		}
		case FILE_BASICINFO3:
		{
			TBL_BASIC_INFO3 tbl_basic_info3;
			readTemp(FILE_BASICINFO3, sizeof(tbl_basic_info3), (T8U*)&tbl_basic_info3);
			ESP_LOGI(TAG, "RT tbl_basic_info3");
			StrNCpy(data, (T8U *)&tbl_basic_info3, table->len);
			break;
		}
		case FILE_EQTYINFO:
		{
			TBL_EQTY_INFO tbl_eqty_info;
			readTemp(FILE_EQTYINFO, sizeof(tbl_eqty_info), (T8U*)&tbl_eqty_info);
			ESP_LOGI(TAG, "RT tbl_eqty_info");
			StrNCpy(data, (T8U *)&tbl_eqty_info, table->len);
			break;
		}
		case FILE_EOVERALLINFO:
		{
			TBL_EOVERALL_INFO tbl_eoverall_info;
			readTemp(FILE_EOVERALLINFO, sizeof(tbl_eoverall_info), (T8U*)&tbl_eoverall_info);
			ESP_LOGI(TAG, "RT tbl_eoverall_info");
			StrNCpy(data, (T8U *)&tbl_eoverall_info, table->len);
			break;
		}
		case FILE_MACHINEINFO:
		{
			TBL_MACHINE_INFO tbl_machine_info;
			readTemp(FILE_MACHINEINFO, sizeof(tbl_machine_info), (T8U*)&tbl_machine_info);
			ESP_LOGI(TAG, "RT tbl_machine_info");
			StrNCpy(data, (T8U *)&tbl_machine_info, table->len);
			break;
		}
		case FILE_PREFIX:
		{
			PG_PREFIX pg_prefix;
			T8U temploc = recordid / MAX_PREFIX_1PAGE;//incase later want to increase size and page
			readTemp(FILE_PREFIX + temploc, sizeof(pg_prefix), (T8U*)&pg_prefix);
			ESP_LOGI(TAG, "RT tbl_prefix");
			temploc = recordid % MAX_PREFIX_1PAGE;
			StrNCpy(data, (T8U *)&pg_prefix.tbl_prefix[temploc], table->len);
			break;
		}
		case FILE_SPLIT:
		{
			TBL_SPLIT tbl_split;
			readTemp(FILE_SPLIT, sizeof(tbl_split), (T8U*)&tbl_split);
			ESP_LOGI(TAG, "RT tbl_split");
			StrNCpy(data, (T8U *)&tbl_split, table->len);
			break;
		}
		case FILE_SPLIT2:
		{
			TBL_SPLIT2 tbl_split2;
			readTemp(FILE_SPLIT2, sizeof(tbl_split2), (T8U*)&tbl_split2);
			ESP_LOGI(TAG, "RT tbl_split2");
			StrNCpy(data, (T8U *)&tbl_split2, table->len);
			break;
		}
		case FILE_LASTCHILDTAG:
		{
			TBL_LASTCHILDTAG tbl_lastchildtag;
			readTemp(FILE_LASTCHILDTAG, sizeof(tbl_lastchildtag), (T8U*)&tbl_lastchildtag);
			ESP_LOGI(TAG, "RT tbl_lastchildtag");
			StrNCpy(data, (T8U *)&tbl_lastchildtag, table->len);
			break;
		}
		case FILE_WRITEFAILED:
		{
			TBL_WRITEFAILED tbl_wf;
			readTemp(FILE_WRITEFAILED, sizeof(tbl_wf), (T8U*)&tbl_wf);
			ESP_LOGI(TAG, "RT tbl_writefailed");
			StrNCpy(data, (T8U *)&tbl_wf, table->len);
			break;
		}
		case FILE_NEEDWRITE:
		{
			TBL_NEEDWRITE tbl_needwrite;
			readTemp(FILE_NEEDWRITE, sizeof(tbl_needwrite), (T8U*)&tbl_needwrite);
			ESP_LOGI(TAG, "RT tbl_needwrite");
			StrNCpy(data, (T8U *)&tbl_needwrite, table->len);
			break;
		}
		case FILE_PROCESSSTAGE:
		{
//			TBL_PROCESS_STAGE tbl_process_stage;
//			readTemp(FILE_PROCESSSTAGE, sizeof(tbl_process_stage), (T8U*)&tbl_process_stage);
//			ESP_LOGI(TAG, "RT tbl_process_stage");
//			StrNCpy(data, (T8U *)&tbl_process_stage, table->len);
//			break;

			PG_PROCESS_STAGE pg_processstage;
			T8U temploc = recordid / MAX_PROCESS_STAGE;//incase later want to increase size and page
			readTemp(FILE_PROCESSSTAGE + temploc, sizeof(pg_processstage), (T8U*)&pg_processstage);
			ESP_LOGI(TAG, "RT pg_processstage");
			temploc = recordid % MAX_PROCESS_STAGE;
			StrNCpy(data, (T8U *)&pg_processstage.tbl_process_stage[temploc], table->len);
			break;
		}
		case FILE_MODIREASON:
		{
//			TBL_MODI_REASON tbl_modi_reason;
//			readTemp(FILE_MODIREASON, sizeof(tbl_modi_reason), (T8U*)&tbl_modi_reason);
//			ESP_LOGI(TAG, "RT tbl_modi_reason");
//			StrNCpy(data, (T8U *)&tbl_modi_reason, table->len);
//			break;

			PG_MODI_REASON pg_modireason;
			T8U temploc = recordid / MAX_MODI_REASON;//incase later want to increase size and page
			readTemp(FILE_MODIREASON + temploc, sizeof(pg_modireason), (T8U*)&pg_modireason);
			ESP_LOGI(TAG, "RT pg_modireason");
			temploc = recordid % MAX_MODI_REASON;
			StrNCpy(data, (T8U *)&pg_modireason.tbl_modi_reason[temploc], table->len);
			break;
		}
		case FILE_PRODGROUP:
		{
			TBL_PROD_GROUP tbl_prod_group;
			readTemp(FILE_PRODGROUP, sizeof(tbl_prod_group), (T8U*)&tbl_prod_group);
			ESP_LOGI(TAG, "RT tbl_prod_group");
			StrNCpy(data, (T8U *)&tbl_prod_group, table->len);
			break;
		}
		case FILE_TEMPROSTAGE:
		{
			TBL_TEMP_PRO_STAGE tbl_temp_pro_stage;
			readTemp(FILE_TEMPROSTAGE, sizeof(tbl_temp_pro_stage), (T8U*)&tbl_temp_pro_stage);
			ESP_LOGI(TAG, "RT temp_process_stage");
			StrNCpy(data, (T8U *)&tbl_temp_pro_stage, table->len);
			break;
		}
		default:
		{
			break;
		}
	}
}

void WriteTable(T_NAME *table, T8U *data, T8U recordid)
{
//	ESP_LOGI(TAG, "WriteTable");
	switch(table->file)
	{
		case FILE_PRESET:
		{
			PG_PRESET pg_preset;
//			readTemp(FILE_PRESET, sizeof(pg_preset), (T8U*)&pg_preset);
			ESP_LOGI(TAG, "WT pg_preset");
			StrNCpy((T8U *)&pg_preset.tbl_preset, data, table->len);
			writeTemp(FILE_PRESET, sizeof(pg_preset), (T8U*)&pg_preset);
			break;
		}
		case FILE_INFO:
		{
			PG_INFO pg_info;
			readTemp(FILE_INFO, sizeof(pg_info), (T8U*)&pg_info);
			if(table->key == 1)
			{
				ESP_LOGI(TAG, "WT tbl_sn");
				StrNCpy((T8U *)&pg_info.tbl_sn, data, table->len);
			}
			else if(table->key == 2)
			{
				ESP_LOGI(TAG, "WT tbl_version_t");
				StrNCpy((T8U *)&pg_info.tbl_version_t, data, table->len);
			}
			else if(table->key == 3)
			{
				ESP_LOGI(TAG, "WT tbl_flashprogram");
				StrNCpy((T8U *)&pg_info.tbl_flashprogram, data, table->len);
			}
			else if(table->key == 4)
			{
				ESP_LOGI(TAG, "WT tbl_addr");
				StrNCpy((T8U *)&pg_info.tbl_addr, data, table->len);
			}
			else if(table->key == 5)
			{
				ESP_LOGI(TAG, "WT tbl_asset");
				StrNCpy((T8U *)&pg_info.tbl_asset, data, table->len);
			}
			else if(table->key == 6)
			{
				ESP_LOGI(TAG, "WT tbl_group");
				StrNCpy((T8U *)&pg_info.tbl_group, data, table->len);
			}
			writeTemp(FILE_INFO, sizeof(pg_info), (T8U*)&pg_info);
			break;
		}
		case FILE_EMP:
		{
			PG_EMP pg_emp;
			readTemp(FILE_EMP, sizeof(pg_emp), (T8U*)&pg_emp);
			if(table->key == 1)
			{
				ESP_LOGI(TAG, "WT tbl_security");
				StrNCpy((T8U *)&pg_emp.tbl_security, data, table->len);
			}
			else if(table->key == 2)
			{
				ESP_LOGI(TAG, "WT tbl_emp");
				StrNCpy((T8U *)&pg_emp.tbl_emp, data, table->len);
			}
			writeTemp(FILE_EMP, sizeof(pg_emp), (T8U*)&pg_emp);
			break;
		}
		case FILE_WIFI:
		{
			PG_WIFI pg_wifi;
			readTemp(FILE_WIFI, sizeof(pg_wifi), (T8U*)&pg_wifi);
			if(table->key == 1)
			{
				ESP_LOGI(TAG, "WT tbl_wifi");
				StrNCpy((T8U *)&pg_wifi.tbl_wifi, data, table->len);
			}
			else if(table->key == 2)
			{
				ESP_LOGI(TAG, "WT tbl_server");
				StrNCpy((T8U *)&pg_wifi.tbl_server, data, table->len);
			}
			writeTemp(FILE_WIFI, sizeof(pg_wifi), (T8U*)&pg_wifi);
			break;
		}
		case FILE_SETTING:
		{
			PG_SETTING pg_setting;
			readTemp(FILE_SETTING, sizeof(pg_setting), (T8U*)&pg_setting);
			if(table->key == 1)
			{
				ESP_LOGI(TAG, "WT tbl_status");
				StrNCpy((T8U *)&pg_setting.tbl_status, data, table->len);
			}
			else if(table->key == 2)
			{
				ESP_LOGI(TAG, "WT tbl_control");
				StrNCpy((T8U *)&pg_setting.tbl_control, data, table->len);
			}
			writeTemp(FILE_SETTING, sizeof(pg_setting), (T8U*)&pg_setting);
			break;
		}
		case FILE_EVENT:
		{
			PG_EVENT pg_event;
			readTemp(FILE_EVENT, sizeof(pg_event), (T8U*)&pg_event);
			if(table->key == 1)
			{
				ESP_LOGI(TAG, "WT start");
				StrNCpy((T8U *)&pg_event.start, data, table->len);
			}
			else if(table->key == 2)
			{
				ESP_LOGI(TAG, "WT lost");
				StrNCpy((T8U *)&pg_event.lost, data, table->len);
			}
			writeTemp(FILE_EVENT, sizeof(pg_event), (T8U*)&pg_event);
			break;
		}
		case FILE_LASTTAG:
		{
			PG_LASTTAG pg_lasttag;
//			readTemp(FILE_LASTTAG, sizeof(pg_lasttag), (T8U*)&pg_lasttag);
			ESP_LOGI(TAG, "WT lasttag");
			StrNCpy((T8U *)&pg_lasttag.tbl_lasttag, data, table->len);
			writeTemp(FILE_LASTTAG, sizeof(pg_lasttag), (T8U*)&pg_lasttag);
			break;
		}
		case FILE_PARTIAL:
		{
			PG_PARTIAL pg_partial;
//			readTemp(FILE_PARTIAL, sizeof(pg_partial), (T8U*)&pg_partial);
			ESP_LOGI(TAG, "WT partial");
			StrNCpy((T8U *)&pg_partial.tbl_partial, data, table->len);
			writeTemp(FILE_PARTIAL, sizeof(pg_partial), (T8U*)&pg_partial);
			break;
		}
		case FILE_MSG:
		{
			PG_MSG pg_msg;
			ESP_LOGI(TAG, "WT tbl_msg");
			StrNCpy((T8U *)&pg_msg, data, (table->len / 2));
			writeTemp(FILE_MSG, sizeof(pg_msg), (T8U*)&pg_msg);
			StrNCpy((T8U *)&pg_msg, data + (table->len / 2), (table->len / 2));
			writeTemp(FILE_MSG2, sizeof(pg_msg), (T8U*)&pg_msg);
			break;
		}
		case FILE_CONDITION:
		{
			PG_CONDITION pg_condition;
			T8U temploc = recordid / MAX_CONDITION_1PAGE;
			readTemp(FILE_CONDITION + temploc, sizeof(pg_condition), (T8U*)&pg_condition);
				ESP_LOGI(TAG, "WT tbl_condition");
				temploc = recordid % MAX_CONDITION_1PAGE;
				StrNCpy((T8U *)&pg_condition.tbl_condition[temploc], data, table->len);
			temploc = recordid / MAX_CONDITION_1PAGE;
			writeTemp(FILE_CONDITION + temploc, sizeof(pg_condition), (T8U*)&pg_condition);
			break;
		}
		case FILE_UDF:
		{
			PG_UDF pg_udf;
			T8U temploc = recordid / MAX_UDF_1PAGE;
			readTemp(FILE_UDF + temploc, sizeof(pg_udf), (T8U*)&pg_udf);
				ESP_LOGI(TAG, "WT tbl_udf");
				temploc = recordid % MAX_UDF_1PAGE;
				StrNCpy((T8U *)&pg_udf.tbl_udf[temploc], data, table->len);
			temploc = recordid / MAX_UDF_1PAGE;
			writeTemp(FILE_UDF + temploc, sizeof(pg_udf), (T8U*)&pg_udf);
			break;
		}
		case FILE_OPSELECT:
		{
			PG_OPSELECT pg_opselect;
			readTemp(FILE_OPSELECT, sizeof(pg_opselect), (T8U*)&pg_opselect);
			if(table->key == 1)
			{
				ESP_LOGI(TAG, "WT tbl_moopselect");
				StrNCpy((T8U *)&pg_opselect.moopselect, data, table->len);
			}
			else if(table->key == 2)
			{
				ESP_LOGI(TAG, "WT tbl_fixopselect");
				StrNCpy((T8U *)&pg_opselect.fixopselect, data, table->len);
			}
			writeTemp(FILE_OPSELECT, sizeof(pg_opselect), (T8U*)&pg_opselect);
			break;
		}
		case FILE_MOOP:
		{
			PG_MOOP pg_moop;
			T8U temploc = recordid / MAX_MOASSIGN_1PAGE;
			readTemp(FILE_MOOP + temploc, sizeof(pg_moop), (T8U*)&pg_moop);
				ESP_LOGI(TAG, "WT tbl_moopassign");
				temploc = recordid % MAX_MOASSIGN_1PAGE;
				StrNCpy((T8U *)&pg_moop.moopassign[temploc], data, table->len);
			temploc = recordid / MAX_MOASSIGN_1PAGE;
			writeTemp(FILE_MOOP + temploc, sizeof(pg_moop), (T8U*)&pg_moop);
			break;
		}
		case FILE_FIXOP:
		{
			PG_FIXOP pg_fixop;
			T8U temploc = recordid / MAX_FIXOPASSIGN_1PAGE;
			readTemp(FILE_FIXOP + temploc, sizeof(pg_fixop), (T8U*)&pg_fixop);
				ESP_LOGI(TAG, "WT fixopassign");
				temploc = recordid % MAX_FIXOPASSIGN_1PAGE;
				StrNCpy((T8U *)&pg_fixop.fixopassign[temploc], data, table->len);
			temploc = recordid / MAX_FIXOPASSIGN_1PAGE;
			writeTemp(FILE_FIXOP + temploc, sizeof(pg_fixop), (T8U*)&pg_fixop);
			break;
		}
		case FILE_POINTER:
		{
			PG_POINTER pg_pointer;
			readTemp(FILE_POINTER, sizeof(pg_pointer), (T8U*)&pg_pointer);
			ESP_LOGI(TAG, "WT tbl_buf_pointer");
			StrNCpy((T8U *)&pg_pointer.tbl_buf_pointer, data, table->len);
			writeTemp(FILE_POINTER, sizeof(pg_pointer), (T8U*)&pg_pointer);

			vTaskDelay(1/portTICK_RATE_MS);

//			readTemp(FILE_BKPOINTER, sizeof(pg_pointer), (T8U*)&pg_pointer);
			ESP_LOGI(TAG, "WT tbl_buf_bkpointer");
			StrNCpy((T8U *)&pg_pointer.tbl_buf_pointer, data, table->len);
			writeTemp(FILE_BKPOINTER, sizeof(pg_pointer), (T8U*)&pg_pointer);
			break;
		}
		case FILE_POINTER_FS:
		{
			PG_POINTER_FS pg_pointer_fs;
			readTemp(FILE_POINTER_FS, sizeof(pg_pointer_fs), (T8U*)&pg_pointer_fs);
			ESP_LOGI(TAG, "WT tbl_fs_pointer");
			StrNCpy((T8U *)&pg_pointer_fs.tbl_fs_pointer, data, table->len);
			writeTemp(FILE_POINTER_FS, sizeof(pg_pointer_fs), (T8U*)&pg_pointer_fs);
			break;
		}
		case FILE_RUNNING_ID:
		{
			PG_RUNNING_ID pg_running_id;
			readTemp(FILE_RUNNING_ID, sizeof(pg_running_id), (T8U*)&pg_running_id);
			ESP_LOGI(TAG, "WT tbl_eventid");
			StrNCpy((T8U *)&pg_running_id.tbl_eventid, data, table->len);
			writeTemp(FILE_RUNNING_ID, sizeof(pg_running_id), (T8U*)&pg_running_id);
			break;
		}
		case FILE_BASICINFO1:
		{
			TBL_BASIC_INFO1 tbl_basic_info1;
			ESP_LOGI(TAG, "WT tbl_basic_info1");
			StrNCpy((T8U *)&tbl_basic_info1, data, table->len);
			writeTemp(FILE_BASICINFO1, sizeof(tbl_basic_info1), (T8U*)&tbl_basic_info1);
			break;
		}
		case FILE_BASICINFO2:
		{
			TBL_BASIC_INFO2 tbl_basic_info2;
			ESP_LOGI(TAG, "WT tbl_basic_info2");
			StrNCpy((T8U *)&tbl_basic_info2, data, table->len);
			writeTemp(FILE_BASICINFO2, sizeof(tbl_basic_info2), (T8U*)&tbl_basic_info2);
			break;
		}
		case FILE_BASICINFO3:
		{
			TBL_BASIC_INFO3 tbl_basic_info3;
			ESP_LOGI(TAG, "WT tbl_basic_info3");
			StrNCpy((T8U *)&tbl_basic_info3, data, table->len);
			writeTemp(FILE_BASICINFO3, sizeof(tbl_basic_info3), (T8U*)&tbl_basic_info3);
			break;
		}
		case FILE_EQTYINFO:
		{
			TBL_EQTY_INFO tbl_eqty_info;
			ESP_LOGI(TAG, "WT tbl_eqty_info");
			StrNCpy((T8U *)&tbl_eqty_info, data, table->len);
			writeTemp(FILE_EQTYINFO, sizeof(tbl_eqty_info), (T8U*)&tbl_eqty_info);
			break;
		}
		case FILE_EOVERALLINFO:
		{
			TBL_EOVERALL_INFO tbl_eoverall_info;
			ESP_LOGI(TAG, "WT tbl_eoverall_info");
			StrNCpy((T8U *)&tbl_eoverall_info, data, table->len);
			writeTemp(FILE_EOVERALLINFO, sizeof(tbl_eoverall_info), (T8U*)&tbl_eoverall_info);
			break;
		}
		case FILE_MACHINEINFO:
		{
			TBL_MACHINE_INFO tbl_machine_info;
			ESP_LOGI(TAG, "WT tbl_machine_info");
			StrNCpy((T8U *)&tbl_machine_info, data, table->len);
			writeTemp(FILE_MACHINEINFO, sizeof(tbl_machine_info), (T8U*)&tbl_machine_info);
			break;
		}
		case FILE_PREFIX:
		{
			PG_PREFIX pg_prefix;
			T8U temploc = recordid / MAX_PREFIX_1PAGE;
			readTemp(FILE_PREFIX + temploc, sizeof(pg_prefix), (T8U*)&pg_prefix);
			ESP_LOGI(TAG, "WT tbl_prefix");
			temploc = recordid % MAX_PREFIX_1PAGE;
			StrNCpy((T8U *)&pg_prefix.tbl_prefix[temploc], data, table->len);
			temploc = recordid / MAX_PREFIX_1PAGE;
			writeTemp(FILE_PREFIX + temploc, sizeof(pg_prefix), (T8U*)&pg_prefix);
			break;
		}
		case FILE_SPLIT:
		{
			TBL_SPLIT tbl_split;
			ESP_LOGI(TAG, "WT tbl_split");
			StrNCpy((T8U *)&tbl_split, data, table->len);
			writeTemp(FILE_SPLIT, sizeof(tbl_split), (T8U*)&tbl_split);
			break;
		}
		case FILE_SPLIT2:
		{
			TBL_SPLIT2 tbl_split2;
			ESP_LOGI(TAG, "WT tbl_split2");
			StrNCpy((T8U *)&tbl_split2, data, table->len);
			writeTemp(FILE_SPLIT2, sizeof(tbl_split2), (T8U*)&tbl_split2);
			break;
		}
		case FILE_LASTCHILDTAG:
		{
			TBL_LASTCHILDTAG tbl_lastchildtag;
			ESP_LOGI(TAG, "WT tbl_lastchildtag");
			StrNCpy((T8U *)&tbl_lastchildtag, data, table->len);
			writeTemp(FILE_LASTCHILDTAG, sizeof(tbl_lastchildtag), (T8U*)&tbl_lastchildtag);
			break;
		}
		case FILE_WRITEFAILED:
		{
			TBL_WRITEFAILED tbl_wf;
			ESP_LOGI(TAG, "WT tbl_writefailed");
			StrNCpy((T8U *)&tbl_wf, data, table->len);
			writeTemp(FILE_WRITEFAILED, sizeof(tbl_wf), (T8U*)&tbl_wf);
			break;
		}
		case FILE_NEEDWRITE:
		{
			TBL_NEEDWRITE tbl_needwrite;
			ESP_LOGI(TAG, "WT tbl_needwrite");
			StrNCpy((T8U *)&tbl_needwrite, data, table->len);
			writeTemp(FILE_NEEDWRITE, sizeof(tbl_needwrite), (T8U*)&tbl_needwrite);
			break;
		}
		case FILE_PROCESSSTAGE:
		{
//			TBL_PROCESS_STAGE tbl_process_stage;
//			ESP_LOGI(TAG, "WT tbl_process_stage");
//			StrNCpy((T8U *)&tbl_process_stage, data, table->len);
//			writeTemp(FILE_PROCESSSTAGE, sizeof(tbl_process_stage), (T8U*)&tbl_process_stage);

			PG_PROCESS_STAGE pg_process_stage;
			T8U temploc = recordid / MAX_PROCESS_STAGE;
			readTemp(FILE_PROCESSSTAGE + temploc, sizeof(pg_process_stage), (T8U*)&pg_process_stage);
			ESP_LOGI(TAG, "WT tbl_processstage");
			temploc = recordid % MAX_PROCESS_STAGE;
			StrNCpy((T8U *)&pg_process_stage.tbl_process_stage[temploc], data, table->len);
			temploc = recordid / MAX_PROCESS_STAGE;
			writeTemp(FILE_PROCESSSTAGE + temploc, sizeof(pg_process_stage), (T8U*)&pg_process_stage);
			break;
		}
		case FILE_MODIREASON:
		{
//			TBL_MODI_REASON tbl_modi_reason;
//			ESP_LOGI(TAG, "WT tbl_modi_reason");
//			StrNCpy((T8U *)&tbl_modi_reason, data, table->len);
//			writeTemp(FILE_MODIREASON, sizeof(tbl_modi_reason), (T8U*)&tbl_modi_reason);

			PG_MODI_REASON pg_modi_reason;
			T8U temploc = recordid / MAX_MODI_REASON;
			readTemp(FILE_MODIREASON + temploc, sizeof(pg_modi_reason), (T8U*)&pg_modi_reason);
			ESP_LOGI(TAG, "WT tbl_modireason");
			temploc = recordid % MAX_MODI_REASON;
			StrNCpy((T8U *)&pg_modi_reason.tbl_modi_reason[temploc], data, table->len);
			temploc = recordid / MAX_MODI_REASON;
			writeTemp(FILE_MODIREASON + temploc, sizeof(pg_modi_reason), (T8U*)&pg_modi_reason);
			break;
		}
		case FILE_PRODGROUP:
		{
			TBL_PROD_GROUP tbl_prod_group;
			ESP_LOGI(TAG, "WT tbl_prod_group");
			StrNCpy((T8U *)&tbl_prod_group, data, table->len);
			writeTemp(FILE_PRODGROUP, sizeof(tbl_prod_group), (T8U*)&tbl_prod_group);
			break;
		}
		case FILE_TEMPROSTAGE:
		{
			TBL_TEMP_PRO_STAGE tbl_temp_pro_stage;
			ESP_LOGI(TAG, "WT temp_process_stage");
			StrNCpy((T8U *)&tbl_temp_pro_stage, data, table->len);
			writeTemp(FILE_TEMPROSTAGE, sizeof(tbl_temp_pro_stage), (T8U*)&tbl_temp_pro_stage);
			break;
		}
		default:
		{
			break;
		}
	}
}

void readTemp(int title, int len, T8U *data)
{
	StrNCpy(data, (T8U *)&tbl_temp.tbl_data[title].data, len);
}

void writeTemp(int title, int len, T8U *data)
{
	StrNCpy((T8U *)&tbl_temp.tbl_data[title].data, data, len);
//	tbl_temp.tbl_data[title].file = title;
	tbl_temp.tbl_data[title].needsave = YES;
//	tbl_temp.tbl_data[title].needsave_backup = YES;
//	tbl_temp.havechanged = YES;
}

void readSpiffstoTemp(void)
{
	TBL_DATA tbl_data_temp;
	T8U count = 0;
	T8U temp = _PASS;
	for(int i = 0; i < MAX_SET_FILE; i++)
	{
		vTaskDelay(1);//vTaskDelay(100/portTICK_RATE_MS);
//		readSpiffsPage(i, MAX_LENPAGE, (T8U*)&tbl_temp.tbl_data[i].data);
//		tbl_temp.tbl_data[i].file = i;
//		tbl_temp.tbl_data[i].needsave = NO;

		while(1)
		{
			count++;
			temp = readSpiffsPage(i, MAX_LENPAGE, (T8U*)&tbl_temp.tbl_data[i].data);
			vTaskDelay(100/portTICK_RATE_MS);
			temp = readSpiffsPage(i, MAX_LENPAGE, (T8U*)&tbl_data_temp.data);
			if(StrNCmp((T8U*)&tbl_temp.tbl_data[i].data, (T8U*)&tbl_data_temp.data, MAX_LENPAGE) == 0)
			{
				break;
			}
			if(count > 2)
			{
				ESP_LOGE(TAG, "Read file %s NOT MATCHED!!!", PTR_FILENAME[i]);
				break;
			}
			vTaskDelay(100/portTICK_RATE_MS);
		}
		tbl_temp.tbl_data[i].file = i;
//		tbl_temp.tbl_data[i].needsave = NO;
		tbl_temp.tbl_data[i].needsave = YES;
//		tbl_temp.tbl_data[i].needsave_backup = NO;

//		if(temp == _FAIL)
//		{
//			readBackupSpiffs(i, MAX_LENPAGE, (T8U*)&tbl_temp.tbl_data[i].data);
//			tbl_temp.tbl_data[i].needsave = YES;
//		}
	}
}

//void writeTemptoSpiffs(void)
//{
//	if((SAVE_DATA & SAVE_HOLD) == 0)
//	{
//		for(int i = 0; i < MAX_SET_FILE; i++)
//		{
//			vTaskDelay(1/portTICK_RATE_MS);
//			if(SAVE_DATA & SAVE_HOLD) break;
//			if(tbl_temp.tbl_data[i].needsave)
//			{
//				T8U result;
//				T8U count = 0;
//				while(1)
//				{
//					result = writeSpiffsPage(tbl_temp.tbl_data[i].file, MAX_LENPAGE, (T8U*)&tbl_temp.tbl_data[i].data);
//					if(result == _FAIL)
//					{
//						count++;
//						if(count > 20)
//						{
//							count = 0;
//							FLAG_STATE |= F_MEMORYCHECKING;
//						}
//						vTaskDelay(1000/portTICK_RATE_MS);
//						continue;
//					}
//					break;
//				}
////				tbl_temp.tbl_data[i].file = i;
//				tbl_temp.tbl_data[i].needsave = NO;
//			}
////			if(tbl_temp.tbl_data[i].needsave_backup)
////			{
////				writeBackupSpiffs(tbl_temp.tbl_data[i].file, MAX_LENPAGE, (T8U*)&tbl_temp.tbl_data[i].data);
////				tbl_temp.tbl_data[i].needsave_backup = NO;
////			}
//		}
////		for(int i = 0; i < MAX_SET_FILE; i++)
////		{
////			vTaskDelay(1/portTICK_RATE_MS);
////			if(SAVE_DATA & SAVE_HOLD) break;
////			if(tbl_temp.tbl_data[i].needsave) break;
////			if(tbl_temp.tbl_data[i].needsave_backup)
////			{
////				T8U result;
//////				T8U count = 0;
//////				while(1)
//////				{
////					result = writeBackupSpiffs(tbl_temp.tbl_data[i].file, MAX_LENPAGE, (T8U*)&tbl_temp.tbl_data[i].data);
//////					if(result == _FAIL)
//////					{
//////						count++;
//////						if(count > 20)
//////						{
//////							count = 0;
//////							FLAG_STATE |= F_MEMORYCHECKING;
//////						}
//////						vTaskDelay(1000/portTICK_RATE_MS);
//////						continue;
//////					}
//////					break;
//////				}
////////				tbl_temp.tbl_data[i].file = i;
////				tbl_temp.tbl_data[i].needsave_backup = NO;
////			}
////		}
//	}
//}

void readLittleFStoTemp(void)
{
	TBL_DATA tbl_data_temp;
	T8U count = 0;
	for(int i = 0; i < MAX_SET_FILE; i++)
	{
		vTaskDelay(1);//vTaskDelay(100/portTICK_RATE_MS);
//		readSpiffsPage(i, MAX_LENPAGE, (T8U*)&tbl_temp.tbl_data[i].data);
//		tbl_temp.tbl_data[i].file = i;
//		tbl_temp.tbl_data[i].needsave = NO;

		while(1)
		{
			count++;
			readLittleFsPage(i, MAX_LENPAGE, (T8U*)&tbl_temp.tbl_data[i].data);
			vTaskDelay(100/portTICK_RATE_MS);
			readLittleFsPage(i, MAX_LENPAGE, (T8U*)&tbl_data_temp.data);
			if(StrNCmp((T8U*)&tbl_temp.tbl_data[i].data, (T8U*)&tbl_data_temp.data, MAX_LENPAGE) == 0)
			{
				break;
			}
			if(count > 2)
			{
				ESP_LOGE(TAG, "Read file %s NOT MATCHED!!!", PTR_FILENAME[i]);

		        if(tbl_littlefs_diag.errReadCount < MAX_RETRY_SPIFFS)
		        {
		        	tbl_littlefs_diag.errRead = YES;
//		        	tbl_littlefs_diag.errReadCode[tbl_spiffs_diag.errReadCount] = errno;
		        	tbl_littlefs_diag.errReadCount++;
					sprintf((char*)&tbl_littlefs_diag.errReadMsg, "ReadFile %u NotMatched!!!", i);
		        }

				break;
			}
			vTaskDelay(100/portTICK_RATE_MS);
		}
		tbl_temp.tbl_data[i].file = i;
		tbl_temp.tbl_data[i].needsave = NO;
	}
}

void writeTemptoLittleFS(void)
{
	if((SAVE_DATA & SAVE_HOLD) == 0)
	{
		for(int i = 0; i < MAX_SET_FILE; i++)
		{
			vTaskDelay(1/portTICK_RATE_MS);
			if(SAVE_DATA & SAVE_HOLD) break;
			if(tbl_temp.tbl_data[i].needsave)
			{
				T8U result;
				T8U count = 0;
				while(1)
				{
					result = writeLittleFsPage(tbl_temp.tbl_data[i].file, MAX_LENPAGE, (T8U*)&tbl_temp.tbl_data[i].data);
					if(result == _FAIL)
					{
						count++;
						if(count > 20)
						{
							count = 0;
							FLAG_STATE |= F_MEMORYCHECKING;
						}
						vTaskDelay(1000/portTICK_RATE_MS);
						continue;
					}
					break;
				}
//				tbl_temp.tbl_data[i].file = i;
				tbl_temp.tbl_data[i].needsave = NO;
			}
		}
	}
}
