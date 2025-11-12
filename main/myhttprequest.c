#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "target.h"
#include "table.h"
#include "databuffer.h"
#include "util.h"
#include "handleSpiffs.h"
#include "app.h"
#include "myhttprequest.h"
#include "rtc_sntp.h"
#include "SmartWifi.h"
#include "handlelittleFS.h"
#include "screen.h"

T8U openstr = '[';
T8U closestr = ']';
T8U openbracesstr = '{';
T8U closebracesstr = '}';
T8U next1str = ',';
T8U next2str = '\n';
T8U plusstr = '+';
T8U minusstr = '-';
T8U closebracketstr = ')';
T8U startstopstr = '"';
T8U ServerTimestr[11] = "ServerTime\"";
T8U	ServerCurrentTimestr[18] = "ServerCurrentTime\"";
T8U	Datestr[5] = "Date("; 										
T8U LostTypestr[9] = "LostType\"";
T8U LostTypeIDstr[11] =	"LostTypeID\""; 							
T8U LostTypeCodestr[13] = "LostTypeCode\""; 						
T8U UserDefinedFunctionColstr[23] = "UserDefinedFunctionCol\"";
T8U Titlestr[6] = "Title\""; 										
T8U SlotNostr[7] = "SlotNo\""; 										
T8U ParameterCollectionstr[20] = "ParameterCollection\"";
T8U ParameterFieldNamestr[19] =	"ParameterFieldName\""; 		
T8U ParameterValuestr[15] =	"ParameterValue\""; 					
T8U ParameterEnabledFlagstr[21] = "ParameterEnabledFlag\""; 		
T8U MenuitemSecurityLevelstr[22] = "MenuitemSecurityLevel\""; 		
T8U	VisibleToTerminalstr[18] = "VisibleToTerminal\""; 			
T8U ProcessStageCodestr[17] = "ProcessStageCode\"";
T8U ProcessStagestr[13] = "ProcessStage\"";
T8U ModiReasonstr[11] = "ModiReason\"";
T8U ProdGroupstr[10] = "ProdGroup\"";

T8U IDstr[3] = "ID\"";									
T8U	AssignmentNamestr[15] = "AssignmentName\"";
T8U MoOpAssignmentstr[15] = "MoOpAssignment\"";
T8U MoOpAssignmentBySequenceStr[25] = "MoOpAssignmentBySequence\"";
T8U FixOpAssignmentstr[16] = "FixOpAssignment\"";
T8U DeleteMoOpAssignmentstr[21] = "DeleteMoOpAssignment\"";
T8U DeleteFixOpOneAssignmentstr[25] = "DeleteFixOpOneAssignment\"";
T8U	MOstr[3] = "MO\"";									
T8U	OPstr[3] = "OP\"";									
T8U	SMVstr[4] = "SMV\"";
T8U	OpRatiostr[8] = "OpRatio\"";
T8U	PreviousOp[11] = "PreviousOp\"";
T8U	IsGroupOp[10] = "IsGroupOp\"";		 	
T8U truestr[4] = "true";
T8U falsestr[5] = "false";

#define ASSIGN_MOOP		1
#define ASSIGN_FIXOP	2
#define DELETE_MOOP		3
#define DELETE_FIXOP	4

T8U Successstr[8] = "Success\"";
T8U Truestr[5] = "True\"";
T8U Falsestr[6] = "False\"";
T8U Messagestr[8] = "Message\"";
T8U MachineTypestr[12] = "MachineType\"";
T8U dotstr = '.';
T8U AccOutputstr[10] = "AccOutput\"";
T8U CurrentTargetstr[14] = "CurrentTarget\"";
T8U HourlyOutputstr[13] = "HourlyOutput\"";
T8U Trendstr[6] = "Trend\"";
T8U AccTerminalOutput[18] = "AccTerminalOutput\"";
T8U Efficiencystr[11] = "Efficiency\"";
T8U DefectQtystr[10] = "DefectQty\"";
T8U RepairQtystr[10] =  "RepairQty\"";
T8U DHUstr[4] = "DHU\"";
T8U LostTimestr[9] =  "LostTime\"";
T8U NoOfErrorstr[10] = "NoOfError\"";
T8U SerialNo[9] = "SerialNo\"";

T8U Prefixstr[7] = "Prefix\"";
T8U UHFTagTypestr [11] = "UHFTagType\"";
T8U TagCountstr[9] = "TagCount\"";
T8U CurrentQtystr[11] = "CurrentQty\"";

T8U tblProductionstr[14] = "tblProduction\"";
T8U ScheduleStartstr[14] = "ScheduleStart\"";
T8U ScheduleEndstr[12] = "ScheduleEnd\"";
T8U FirstTrxTimestr[13] = "FirstTrxTime\"";
T8U TotalClaimstr[11] = "TotalClaim\"";
T8U TotalClaimEarnMinstr[18] = "TotalClaimEarnMin\"";
T8U AccQtystr[7] = "AccQty\"";
T8U AccQtyEarnMinstr[14] = "AccQtyEarnMin\"";
T8U TerminalAccQtystr[15] = "TerminalAccQty\"";
T8U tblBreakTimestr[13] = "tblBreakTime\"";
T8U BreakTimeStartstr[15] = "BreakTimeStart\"";
T8U BreakTimeEndstr[13] = "BreakTimeEnd\"";
T8U ProdGroupIDstr[12] = "ProdGroupID\"";
T8U ProdGroupNamestr[14] = "ProdGroupName\"";
T8U Messagesstrstr[8] = "Message\"";

#define WEB_PORT "80"

T8U httpstr[4] = "HTTP";

#define LEN_RX_BUFFER 8192
static const char *TAG = "myhttprequest";

T8U myhttp_rx_buffer[LEN_RX_BUFFER] = {0};
T8U myhttp_tx_buffer[512] = {0};
T16U len_return = 0;
T32U ID_assignment[10] = {0};
T8U ID_loc = 0;
T32U pos = 0;
T8U dataPos = 0;
T8U record = 0;
T8U countConnectFailed = 0;
T8U callHelpData = NO;
T8U longTiemOut = NO;
T8U tempTimeOut = 0;
//static const char *REQUEST = "GET " WEB_PATH " HTTP/1.0\r\n"
//	"Host: "WEB_SERVER":"WEB_PORT"\r\n"
////    "User-Agent: esp-idf/1.0 esp32\r\n"
//    "\r\n";

const struct addrinfo hints = {
    .ai_family = AF_INET,
    .ai_socktype = SOCK_STREAM,
};
struct addrinfo *res;
struct in_addr *addr;
int s, r;
char recv_buf[64];

void connectFailed(void);
void get_submitstr(void);
void process_received(void);
void clearUrgentFlag(void);
void parseStartup(void);
void parseGetTime(void);
void clearAssignment(void);
void parseAssignment(void);
void parseMachineInfo(void);
void parseBasicInfo(void);
void parseBasicInfobyGroup(void);
void parseGetQtyEmp(void);
void parseGetOverAllEmp(void);
void parseGetTerminalInfoByMAC(void);
void parsePrefix(void);
void parseGetBundle();
T8U parseRegisterMessage(void);
void parseCheckOp(void);
void parseGetProduction(void);
void parseProductionInfobyGroup(void);
void GetProdGroupNameByProdGroupID(void);
void CheckInputOpClaimedGroup(void);
T8U parseRequestMessage(void);
T8U parseSuccessMessage(void);
T8U parsetrueStr(void);
T8U checkstring(T16U * i, T8U * tempstring, T8U size);
T8U checkSendString(T16U * i, T8U * tempstring, T8U size);

void http_send(void)
{
	if(FLAG_STATE & F_RECONNECTWIFI2)
	{
		FLAG_STATE &= ~F_RECONNECTWIFI2;
		FLAG_STATE &= ~F_RECONNECTWIFI;
		scanWifi();
	}
	if(FLAG_STATE & F_RECONNECTWIFI)
	{
		FLAG_STATE &= ~F_RECONNECTWIFI;
		reConnectWifi();
	}	
	if(!(FLAG_STATE & F_WIFICONNECT))return;
	if(tbl_server.hasSet != SPECIAL_ON)return;
	if(FLAG_STATE & F_OTASTART)return;
	if(savingInProcess)return;

	get_submitstr();

	if(myhttp_tx_buffer[0] == 0)
	{
		if(nosendactivity_mincount > NOSENDACTIVITY_MIN)
		{
////			nosendactivity_mincount = 0;
//			TOP_Message |= TOPDATA_SYNCDATE;
		}
		return;
	}
	ESP_LOGI(TAG, "%s", myhttp_tx_buffer);

	if((FLAG_STATE & F_SENDINGDATA) == 0)
	{
		FLAG_STATE |= F_SENDINGDATA;
		FLAG_STATE |= F_UPDATEHEADER;
	}

    int err = getaddrinfo((char*)&tbl_server.IPv4, WEB_PORT, &hints, &res);

	if(err != 0 || res == NULL) {
		ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);
		sprintf((char*)&tbl_wifi_diag.httpSend, "\nhttp_send\nDNS lookup\nfailed err=%d\nres=%p", err, res);
		connectFailed();
		clearUrgentFlag();
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		return;
	}

	addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
	ESP_LOGI(TAG, "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));

	s = socket(res->ai_family, res->ai_socktype, 0);
	if(s < 0) {
		ESP_LOGE(TAG, "... Failed to allocate socket.");
		sprintf((char*)&tbl_wifi_diag.httpSend, "\nhttp_send\nFailed to\nallocate\nsocket.");
		connectFailed();
		clearUrgentFlag();
		freeaddrinfo(res);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		return;
	}
	ESP_LOGI(TAG, "... allocated socket");

	if(connect(s, res->ai_addr, res->ai_addrlen) != 0) {
		FLAG_STATE |= F_CONENCTFAILED;
		ESP_LOGE(TAG, "... socket connect failed errno=%d", errno);
		sprintf((char*)&tbl_wifi_diag.httpSend, "\nhttp_send\nsocket connect\nfailed errno=%d", errno);
		connectFailed();
		clearUrgentFlag();
		close(s);
		freeaddrinfo(res);
		vTaskDelay(4000 / portTICK_PERIOD_MS);
		return;
	}

	ESP_LOGI(TAG, "... connected");
	freeaddrinfo(res);

	if(FLAG_STATE & F_SCREENABORT)
	{
		FLAG_STATE &= ~F_SCREENABORT;
		goto close;
	}

	if (write(s, (char*)&myhttp_tx_buffer, strlen((char*)&myhttp_tx_buffer)) < 0) {
		ESP_LOGE(TAG, "... socket send failed");
		sprintf((char*)&tbl_wifi_diag.httpSend, "\nhttp_send\nsocket send\nfailed");
		connectFailed();
		clearUrgentFlag();
		close(s);
		vTaskDelay(4000 / portTICK_PERIOD_MS);
		return;
	}
	ESP_LOGI(TAG, "... socket send success");

	struct timeval receiving_timeout;

	if(longTiemOut)
	{
		receiving_timeout.tv_sec = 180;
	}
	else
	{
		receiving_timeout.tv_sec = tempTimeOut;
	}
	receiving_timeout.tv_usec = 0;
	if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout,
			sizeof(receiving_timeout)) < 0) {
		ESP_LOGE(TAG, "... failed to set socket receiving timeout");
		sprintf((char*)&tbl_wifi_diag.httpSend, "\nhttp_send\nfailed to set\nsocket receiving\ntimeout");
		connectFailed();
		clearUrgentFlag();
		close(s);
		vTaskDelay(4000 / portTICK_PERIOD_MS);
		return;
	}
	ESP_LOGI(TAG, "... set socket receiving timeout success");

	/* Read HTTP response */
	do {
		bzero(recv_buf, sizeof(recv_buf));
		r = read(s, recv_buf, sizeof(recv_buf)-1);
		for(int i = 0; i < r; i++) {
			putchar(recv_buf[i]);
			if(len_return >= LEN_RX_BUFFER)
			{
				r = 0;
				break;
			}
			else
			{
				myhttp_rx_buffer[len_return++] = recv_buf[i];
			}
		}
	} while(r > 0);

	if(len_return > 0)
	{
		process_received();
		countConnectFailed = 0;
		nosendactivity_mincount = 0;

		if(FLAG_STATE & F_SENDINGDATA)
		{
			FLAG_STATE &= ~F_SENDINGDATA;
			FLAG_STATE |= F_UPDATEHEADER;
		}
	}
	else
	{
		if(TOP_Message & TOPDATA_GETPREFIX)
		{
			TOP_Message &= ~TOPDATA_GETPREFIX;
			FLAG_STATE |= F_SERVERTIMEOUT;
		}
		else if(TOP_Message & TOPDATA_GETBUNDLEINFO)
		{
			TOP_Message &= ~TOPDATA_GETBUNDLEINFO;
			FLAG_STATE |= F_SERVERTIMEOUT;
		}
		else if(TOP_Message & TOPDATA_REGISTERUHFTAG)
		{
			TOP_Message &= ~TOPDATA_REGISTERUHFTAG;
			FLAG_STATE |= F_SERVERTIMEOUT;
		}
	}

	close:
	ESP_LOGI(TAG, "... done reading from socket. Last read return=%d errno=%d.", r, errno);
	close(s);
	vTaskDelay(100 / portTICK_PERIOD_MS);
	ESP_LOGI(TAG, "Starting again!");
	memset(myhttp_rx_buffer, '\0', sizeof(myhttp_rx_buffer));
	len_return = 0;
}

void myhttprequestinit(void)
{
//    xTaskCreate(&http_get_task2, "http_get_task", 4096, NULL, 5, NULL);
//    xTaskCreatePinnedToCore(
//    		http_get_task2,     /* Function to implement the task */
//                    "http_get_task",   /* Name of the task */
//                    4096,      /* Stack size in words */
//                    NULL,      /* Task input parameter */
//                    5,         /* Priority of the task */
//                    NULL,      /* Task handle. */
//                    1);
}

void connectFailed(void)
{
	countConnectFailed++;
	if(countConnectFailed > 3)
	{
		countConnectFailed = 0;
		FLAG_STATE |= F_RECONNECTWIFI;
	}
	if(countConnectFailed > 2)
	{
		if(FLAG_STATE & F_CONNECTSERVER)
		{
			FLAG_STATE &= ~F_CONNECTSERVER;
			FLAG_STATE |= F_UPDATEHEADER;
		}
	}
	if(FLAG_STATE & F_SENDINGDATA)
	{
		FLAG_STATE &= ~F_SENDINGDATA;
		FLAG_STATE |= F_UPDATEHEADER;
	}
	FLAG_STATE |= F_SERVERTIMEOUT;
}

void get_submitstr(void)
{
	T16U tempv = 0;
	T32U tempx = 0;
	T16U tempy = 0;
	char tempstring[256] = {0};
	longTiemOut = NO;
	tempTimeOut = 30;
	pos = 0;
	dataPos = 0;
	sprintf((char*)&myhttp_tx_buffer, "GET /Mobile/handler/");
	if(TOP_Message & TOPDATA_SYNCDATE)
	{
		pos = TOPDATA_SYNCDATE;
		strcat((char*)&myhttp_tx_buffer, "GetServerDateTime.ashx");
	}
	else if(TOP_Message & TOPDATA_POWERUP)
	{
		longTiemOut = YES;
		pos = TOPDATA_POWERUP;
		sprintf(tempstring, "StartUp.ashx?GabletID=%u", tbl_addr.addr);
		strcat((char*)&myhttp_tx_buffer, tempstring);
	}
	else if(TOP_Message & TOPDATA_GETMACHINEINFO)
	{
		pos = TOPDATA_GETMACHINEINFO;
		sprintf(tempstring, "GetMachineType.ashx?TerminalNo=%u", tbl_addr.addr);
		strcat((char*)&myhttp_tx_buffer, tempstring);
	}
	else if(TOP_Message & TOPDATA_CHECKUPDATEFIRM)
	{
		pos = TOPDATA_CHECKUPDATEFIRM;
		sprintf(tempstring, "GetUpdateFlagByTerminal.ashx?TerminalNo=%u", tbl_addr.addr);
		strcat((char*)&myhttp_tx_buffer, tempstring);
	}
	else
	{
		pos = 0;
		if(Get_RecordHiCount() > 0)
		{
			dataPos = 20;
			get_hirecord((T8U*)&tempstring);
			if(*tempstring == DATA_UDF.cmd)
			{
				LOG_UDF log_udf;
				DATETIME time;
				StrNCpy((T8U*)&log_udf, (T8U*)&tempstring[2], sizeof(log_udf));
				StrNCpy((T8U*)&time, (T8U*)&log_udf.udftext[13], sizeof(time));
				if(log_udf.udfid > 10) callHelpData = YES;
				dataPos = 21;
				StrNRep((T8U*)&log_udf.empcode, 0x20, 0 , sizeof(log_udf.empcode));
				sprintf(tempstring, "RequestMessage.ashx?SlotNo=%u&TerminalNo=%u&EmployeeNo=%.10s&EventTime=", log_udf.udfid, tbl_addr.addr, log_udf.empcode);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				memset(tempstring, 0, 50);
				stringDatetime(&time, (T8U*)&tempstring, 0);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				if(log_udf.udfid == 24)
				{
					T8U a[5] = "";
					T32U b = 0;
					StrNCpy((T8U*)&a, (T8U*)&log_udf.udftext[0], 5);
					b = atol((char*)&a);
					sprintf(tempstring, "%u", b);
//					memset(tempstring, 0, 50);
//					StrNCpy((T8U*)&tempstring, (T8U*)&log_udf.udftext[0], 10);
//					ESP_LOGI(TAG, "%s", tempstring);
//					ESP_LOGI(TAG, "%s", tempstring);
					strcat((char*)&myhttp_tx_buffer, "&RepairCode=");
					strcat((char*)&myhttp_tx_buffer, tempstring);
				}
				else
				{
					strcat((char*)&myhttp_tx_buffer, "&RepairCode=0");
				}
			}
//			else if((*tempstring == 0) || (*tempstring == DATA_REQUEST_MO.cmd) || (*tempstring == DATA_UPDATINGOPMANUALLY.cmd) ||
//					(*tempstring == DATA_NOTIFYERROR.cmd))
//			{
//				myhttp_tx_buffer[0] = 0;
//				if(tbl_buf_pointer.high.rear != tbl_buf_pointer.high.front)
//				{
//					tbl_buf_pointer.high.rear++;
//					if(tbl_buf_pointer.high.rear >= MAX_HIGHRECORD)
//					{
//						tbl_buf_pointer.high.rear = 0;
//					}
//				}
//				WriteTable(&T_BUF_POINTER, (T8U *)&tbl_buf_pointer, 0);
//				return;
//			}
			else
			{
				myhttp_tx_buffer[0] = 0;
//				if(tbl_buf_pointer.high.rear != tbl_buf_pointer.high.front)
//				{
//					tbl_buf_pointer.high.rear++;
//					if(tbl_buf_pointer.high.rear >= MAX_HIGHRECORD)
//					{
//						tbl_buf_pointer.high.rear = 0;
//					}
//					if(tbl_buf_pointer.high.front >= MAX_HIGHRECORD)
//					{
//						tbl_buf_pointer.high.front = 0;
//					}
//				}
//				WriteTable(&T_BUF_POINTER, (T8U *)&tbl_buf_pointer, 0);
				flush_record(1);
				return;
			}
//			else
//			{
//				sprintf(tempstring, "ProcessZ5DataPollingTerminalNo=%u&Data=", tbl_addr.addr);
//				strcat((char*)&myhttp_tx_buffer, tempstring);
//
//				sprintf(tempstring, "%02X%02X%02X%02X", PROTOCOL_0_HEADERH, PROTOCOL_0_HEADERL, (T8U)(tbl_addr.addr>>8), (T8U)tbl_addr.addr);
//				strcat((char*)&myhttp_tx_buffer, tempstring);
//
//				get_hirecord_web((T8U*)&tempstring);
//				strcat((char*)&myhttp_tx_buffer, tempstring);
//			}

		}
		else if(Get_RecordFSCount() > 0)
		{
			dataPos = 30;
//			T16U tempv = 0;
//			T32U tempx = 0;
			T16U fsRecord = Get_RecordFSCount();
			record = 0;
			get_FSrecord((T8U*)&tempstring, 0);
			if(*tempstring == DATA_TDT_CLAIM.cmd)
			{
				dataPos = 31;
				LOG_TDT_CLAIM log_tdt_claim[4];
				T8U tempstring2[10] = {0};
				T8U ret = 0;
				StrNCpy((T8U*)&log_tdt_claim[0], (T8U*)&tempstring[2], sizeof(LOG_TDT_CLAIM));
				record++;
				vTaskDelay(1);
				get_FSrecord((T8U*)&tempstring, record);
				if((*tempstring == DATA_TDT_CLAIM_1.cmd) && (fsRecord > 1))
				{
					StrNCpy((T8U*)&log_tdt_claim[1], (T8U*)&tempstring[2], sizeof(LOG_TDT_CLAIM));
					record++;
					vTaskDelay(1);
					get_FSrecord((T8U*)&tempstring, record);
					if((*tempstring == DATA_TDT_CLAIM_2.cmd) && (fsRecord > 2))
					{
						StrNCpy((T8U*)&log_tdt_claim[2], (T8U*)&tempstring[2], sizeof(LOG_TDT_CLAIM));
						record++;
						vTaskDelay(1);
						get_FSrecord((T8U*)&tempstring, record);
						if((*tempstring == DATA_TDT_CLAIM_3.cmd) && (fsRecord > 3))
						{
							StrNCpy((T8U*)&log_tdt_claim[3], (T8U*)&tempstring[2], sizeof(LOG_TDT_CLAIM));
							record++;
							vTaskDelay(1);
						}
					}
				}
//				sprintf(tempstring, "WebClaimWithGlobalID.ashx?OpNo=");
//				strcat((char*)&myhttp_tx_buffer, tempstring);
				strcat((char*)&myhttp_tx_buffer, "WebClaimWithGlobalID.ashx?OpNo=");
				for(T8U i = 0; i < record; i++)
				{
					for(T8U j = 0; j < 5; j++)
					{
						if(log_tdt_claim[i].op[j] == 0)break;
						else
						{
							ChangeEndian((T8U *)&tempv, (T8U *)&log_tdt_claim[i].op[j], 2);
							if((i != 0) || (j != 0))
							{
								sprintf((char*)&tempstring2, "-%u", tempv);
								strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
							}
							else
							{
								sprintf((char*)&tempstring2, "%u", tempv);
								strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
							}
						}
					}
				}
//				sprintf((char*)&tempstring2, "&OpRatio=");
//				strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
				strcat((char*)&myhttp_tx_buffer, "&OpRatio=");
				for(T8U i = 0; i < record; i++)
				{
					for(T8U j = 0; j < 5; j++)
					{
						if(log_tdt_claim[i].opratio[j] == 0)break;
						else
						{
							if((i != 0) || (j != 0))
							{
								sprintf((char*)&tempstring2, "-%u", log_tdt_claim[i].opratio[j]);
								strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
							}
							else
							{
								sprintf((char*)&tempstring2, "%u", log_tdt_claim[i].opratio[j]);
								strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
							}
						}
					}
				}
				StrNRep((T8U*)&log_tdt_claim[0].employeecode, 0x20, 0 , sizeof(log_tdt_claim[0].employeecode));
				sprintf(tempstring, "&PartialClaim=%u&TerminalNo=%u&EmployeeNo=%.10s", log_tdt_claim[0].type, tbl_addr.addr, log_tdt_claim[0].employeecode);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempv, (T8U *)&log_tdt_claim[0].qty, 2);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_tdt_claim[0].oritagid, 4);
				sprintf(tempstring, "&Quantity=%u&OriginalTagID=%u", tempv, tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_tdt_claim[0].tagdtime, 4);
				sprintf(tempstring, "&DOB=%u&StartTime=", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				memset(tempstring, 0, 50);
				ret = stringDatetime(&log_tdt_claim[0].starttime, (T8U*)&tempstring, 0);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				strcat((char*)&myhttp_tx_buffer, "&EndTime=");
				memset(tempstring, 0, 50);
				stringDatetime(&log_tdt_claim[0].endtime, (T8U*)&tempstring, ret);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				strcat((char*)&myhttp_tx_buffer, "&TagType=1&EPC=");
			}
			else if(*tempstring == DATA_GROUPTAG_CLAIM.cmd)
			{
				dataPos = 31;
				LOG_GROUPTAG_CLAIM log_group_claim[4];
				T8U tempstring2[10] = {0};
				T8U ret = 0;
				StrNCpy((T8U*)&log_group_claim[0], (T8U*)&tempstring[2], sizeof(LOG_GROUPTAG_CLAIM));
				record++;
				vTaskDelay(1);
				get_FSrecord((T8U*)&tempstring, record);
				if((*tempstring == DATA_GROUPTAG_CLAIM_1.cmd) && (fsRecord > 1))
				{
					StrNCpy((T8U*)&log_group_claim[1], (T8U*)&tempstring[2], sizeof(LOG_GROUPTAG_CLAIM));
					record++;
					vTaskDelay(1);
					get_FSrecord((T8U*)&tempstring, record);
					if((*tempstring == DATA_GROUPTAG_CLAIM_2.cmd) && (fsRecord > 2))
					{
						StrNCpy((T8U*)&log_group_claim[2], (T8U*)&tempstring[2], sizeof(LOG_GROUPTAG_CLAIM));
						record++;
						vTaskDelay(1);
						get_FSrecord((T8U*)&tempstring, record);
						if((*tempstring == DATA_GROUPTAG_CLAIM_3.cmd) && (fsRecord > 3))
						{
							StrNCpy((T8U*)&log_group_claim[3], (T8U*)&tempstring[2], sizeof(LOG_GROUPTAG_CLAIM));
							record++;
							vTaskDelay(1);
						}
					}
				}
				strcat((char*)&myhttp_tx_buffer, "ProcessGroupTagClaimWithMultipleOp.ashx?");
				ChangeEndian((T8U *)&tempx, (T8U *)&log_group_claim[0].group_oritagid, 4);
				sprintf(tempstring, "GroupGlobalTagID=%u&GroupGlobalTagDOB=", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				memset(tempstring, 0, 50);
				GUIDConvertStrDate((T8U*)&log_group_claim[0].GUID, (T8U*)&tempstring);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				StrNRep((T8U*)&log_group_claim[0].grouptagcode, 0x20, 0 , sizeof(log_group_claim[0].grouptagcode));
				sprintf(tempstring, "&GroupTagCode=%.10s", log_group_claim[0].grouptagcode);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_group_claim[0].grouptagid, 4);
				sprintf(tempstring, "&GroupTagNo=%u&OpNoList=", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);

				for(T8U i = 0; i < record; i++)
				{
					for(T8U j = 0; j < 5; j++)
					{
						if(log_group_claim[i].op[j] == 0)break;
						else
						{
							ChangeEndian((T8U *)&tempv, (T8U *)&log_group_claim[i].op[j], 2);
							if((i != 0) || (j != 0))
							{
								sprintf((char*)&tempstring2, "-%u", tempv);
								strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
							}
							else
							{
								sprintf((char*)&tempstring2, "%u", tempv);
								strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
							}
						}
					}
				}
				strcat((char*)&myhttp_tx_buffer, "&OpRatioList=");
				for(T8U i = 0; i < record; i++)
				{
					for(T8U j = 0; j < 5; j++)
					{
						if(log_group_claim[i].opratio[j] == 0)break;
						else
						{
							if((i != 0) || (j != 0))
							{
								sprintf((char*)&tempstring2, "-%u", log_group_claim[i].opratio[j]);
								strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
							}
							else
							{
								sprintf((char*)&tempstring2, "%u", log_group_claim[i].opratio[j]);
								strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
							}
						}
					}
				}

				strcat((char*)&myhttp_tx_buffer, "&StartTime=");
				memset(tempstring, 0, 50);
				ret = stringDatetime(&log_group_claim[0].starttime, (T8U*)&tempstring, 0);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				strcat((char*)&myhttp_tx_buffer, "&EndTime=");
				memset(tempstring, 0, 50);
				stringDatetime(&log_group_claim[0].endtime, (T8U*)&tempstring, ret);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				StrNRep((T8U*)&log_group_claim[0].employeecode, 0x20, 0 , sizeof(log_group_claim[0].employeecode));
				sprintf(tempstring, "&TerminalNo=%u&EmployeeNo=%.10s", tbl_addr.addr, log_group_claim[0].employeecode);
				strcat((char*)&myhttp_tx_buffer, tempstring);
			}
		}
		else if(Get_RecordLoCount() > 0)
		{
			dataPos = 40;
			T16U loRecord = Get_RecordLoCount();
			record = 0;
			get_lorecord((T8U*)&tempstring, 0);
			if(*tempstring == DATA_TDT_CLAIM.cmd)
			{
				dataPos = 41;
				LOG_TDT_CLAIM log_tdt_claim[4];
				T8U tempstring2[10] = {0};
				T8U ret = 0;
				StrNCpy((T8U*)&log_tdt_claim[0], (T8U*)&tempstring[2], sizeof(LOG_TDT_CLAIM));
				record++;
				vTaskDelay(1);
				get_lorecord((T8U*)&tempstring, record);
				if((*tempstring == DATA_TDT_CLAIM_1.cmd) && (loRecord > 1))
				{
					StrNCpy((T8U*)&log_tdt_claim[1], (T8U*)&tempstring[2], sizeof(LOG_TDT_CLAIM));
					record++;
					vTaskDelay(1);
					get_lorecord((T8U*)&tempstring, record);
					if((*tempstring == DATA_TDT_CLAIM_2.cmd) && (loRecord > 2))
					{
						StrNCpy((T8U*)&log_tdt_claim[2], (T8U*)&tempstring[2], sizeof(LOG_TDT_CLAIM));
						record++;
						vTaskDelay(1);
						get_lorecord((T8U*)&tempstring, record);
						if((*tempstring == DATA_TDT_CLAIM_3.cmd) && (loRecord > 3))
						{
							StrNCpy((T8U*)&log_tdt_claim[3], (T8U*)&tempstring[2], sizeof(LOG_TDT_CLAIM));
							record++;
							vTaskDelay(1);
						}
					}
				}
				strcat((char*)&myhttp_tx_buffer, "WebClaimWithGlobalID.ashx?OpNo=");
				for(T8U i = 0; i < record; i++)
				{
					for(T8U j = 0; j < 5; j++)
					{
						if(log_tdt_claim[i].op[j] == 0)break;
						else
						{
							ChangeEndian((T8U *)&tempv, (T8U *)&log_tdt_claim[i].op[j], 2);
							if((i != 0) || (j != 0))
							{
								sprintf((char*)&tempstring2, "-%u", tempv);
								strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
							}
							else
							{
								sprintf((char*)&tempstring2, "%u", tempv);
								strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
							}
						}
					}
				}
				strcat((char*)&myhttp_tx_buffer, "&OpRatio=");
				for(T8U i = 0; i < record; i++)
				{
					for(T8U j = 0; j < 5; j++)
					{
						if(log_tdt_claim[i].opratio[j] == 0)break;
						else
						{
							if((i != 0) || (j != 0))
							{
								sprintf((char*)&tempstring2, "-%u", log_tdt_claim[i].opratio[j]);
								strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
							}
							else
							{
								sprintf((char*)&tempstring2, "%u", log_tdt_claim[i].opratio[j]);
								strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
							}
						}
					}
				}
				StrNRep((T8U*)&log_tdt_claim[0].employeecode, 0x20, 0 , sizeof(log_tdt_claim[0].employeecode));
				sprintf(tempstring, "&PartialClaim=%u&TerminalNo=%u&EmployeeNo=%.10s", log_tdt_claim[0].type, tbl_addr.addr, log_tdt_claim[0].employeecode);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempv, (T8U *)&log_tdt_claim[0].qty, 2);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_tdt_claim[0].oritagid, 4);
				sprintf(tempstring, "&Quantity=%u&OriginalTagID=%u", tempv, tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_tdt_claim[0].tagdtime, 4);
				sprintf(tempstring, "&DOB=%u&StartTime=", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				memset(tempstring, 0, 50);
				ret = stringDatetime(&log_tdt_claim[0].starttime, (T8U*)&tempstring, 0);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				strcat((char*)&myhttp_tx_buffer, "&EndTime=");
				memset(tempstring, 0, 50);
				stringDatetime(&log_tdt_claim[0].endtime, (T8U*)&tempstring, ret);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				strcat((char*)&myhttp_tx_buffer, "&TagType=1&EPC=");
			}
			else if(*tempstring == DATA_GROUPTAG_CLAIM.cmd)
			{
				dataPos = 41;
				LOG_GROUPTAG_CLAIM log_group_claim[4];
				T8U tempstring2[10] = {0};
				T8U ret = 0;
				StrNCpy((T8U*)&log_group_claim[0], (T8U*)&tempstring[2], sizeof(LOG_GROUPTAG_CLAIM));
				record++;
				vTaskDelay(1);
				get_lorecord((T8U*)&tempstring, record);
				if((*tempstring == DATA_GROUPTAG_CLAIM_1.cmd) && (loRecord > 1))
				{
					StrNCpy((T8U*)&log_group_claim[1], (T8U*)&tempstring[2], sizeof(LOG_GROUPTAG_CLAIM));
					record++;
					vTaskDelay(1);
					get_lorecord((T8U*)&tempstring, record);
					if((*tempstring == DATA_GROUPTAG_CLAIM_2.cmd) && (loRecord > 2))
					{
						StrNCpy((T8U*)&log_group_claim[2], (T8U*)&tempstring[2], sizeof(LOG_GROUPTAG_CLAIM));
						record++;
						vTaskDelay(1);
						get_lorecord((T8U*)&tempstring, record);
						if((*tempstring == DATA_GROUPTAG_CLAIM_3.cmd) && (loRecord > 3))
						{
							StrNCpy((T8U*)&log_group_claim[3], (T8U*)&tempstring[2], sizeof(LOG_GROUPTAG_CLAIM));
							record++;
							vTaskDelay(1);
						}
					}
				}
				strcat((char*)&myhttp_tx_buffer, "ProcessGroupTagClaimWithMultipleOp.ashx?");
				ChangeEndian((T8U *)&tempx, (T8U *)&log_group_claim[0].group_oritagid, 4);
				sprintf(tempstring, "GroupGlobalTagID=%u&GroupGlobalTagDOB=", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				memset(tempstring, 0, 50);

				GUIDConvertStrDate((T8U*)&log_group_claim[0].GUID, (T8U*)&tempstring);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				StrNRep((T8U*)&log_group_claim[0].grouptagcode, 0x20, 0 , sizeof(log_group_claim[0].grouptagcode));
				sprintf(tempstring, "&GroupTagCode=%.10s", log_group_claim[0].grouptagcode);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_group_claim[0].grouptagid, 4);
				sprintf(tempstring, "&GroupTagNo=%u&OpNoList=", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);

				for(T8U i = 0; i < record; i++)
				{
					for(T8U j = 0; j < 5; j++)
					{
						if(log_group_claim[i].op[j] == 0)break;
						else
						{
							ChangeEndian((T8U *)&tempv, (T8U *)&log_group_claim[i].op[j], 2);
							if((i != 0) || (j != 0))
							{
								sprintf((char*)&tempstring2, "-%u", tempv);
								strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
							}
							else
							{
								sprintf((char*)&tempstring2, "%u", tempv);
								strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
							}
						}
					}
				}
				strcat((char*)&myhttp_tx_buffer, "&OpRatioList=");
				for(T8U i = 0; i < record; i++)
				{
					for(T8U j = 0; j < 5; j++)
					{
						if(log_group_claim[i].opratio[j] == 0)break;
						else
						{
							if((i != 0) || (j != 0))
							{
								sprintf((char*)&tempstring2, "-%u", log_group_claim[i].opratio[j]);
								strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
							}
							else
							{
								sprintf((char*)&tempstring2, "%u", log_group_claim[i].opratio[j]);
								strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
							}
						}
					}
				}

				strcat((char*)&myhttp_tx_buffer, "&StartTime=");
				memset(tempstring, 0, 50);
				ret = stringDatetime(&log_group_claim[0].starttime, (T8U*)&tempstring, 0);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				strcat((char*)&myhttp_tx_buffer, "&EndTime=");
				memset(tempstring, 0, 50);
				stringDatetime(&log_group_claim[0].endtime, (T8U*)&tempstring, ret);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				StrNRep((T8U*)&log_group_claim[0].employeecode, 0x20, 0 , sizeof(log_group_claim[0].employeecode));
				sprintf(tempstring, "&TerminalNo=%u&EmployeeNo=%.10s", tbl_addr.addr, log_group_claim[0].employeecode);
				strcat((char*)&myhttp_tx_buffer, tempstring);
			}
		}
		else
		{
			if(TOP_Message & TOPDATA_GETINFO_AGAIN)
			{
				TOP_Message &= ~TOPDATA_GETINFO_AGAIN;
				TOP_Message |= TOPDATA_GETOVERALLEMPLOYEE;
				TOP_Message |= TOPDATA_PRODUCTIONINFOBYGROUP;
			}
			myhttp_tx_buffer[0] = 0;
			return;
		}

		if((dataPos == 40) || (dataPos == 30))
		{
			if(*tempstring == DATA_TDT_CLAIM_V3.cmd)
			{
				LOG_TDT_CLAIM_V3 log_tdt_claim_v3;
				T8U tempstring2[10] = {0};
				T8U ret = 0;
				StrNCpy((T8U*)&log_tdt_claim_v3, (T8U*)&tempstring[2], sizeof(LOG_TDT_CLAIM_V3));

				strcat((char*)&myhttp_tx_buffer, "WebClaimWithGlobalID.ashx?OpNo=");

				for(T8U j = 0; j < 20; j++)
				{
					if(log_tdt_claim_v3.op[j] == 0)break;
					else
					{
						ChangeEndian((T8U *)&tempv, (T8U *)&log_tdt_claim_v3.op[j], 2);
						if(j != 0)
						{
							sprintf((char*)&tempstring2, "-%u", tempv);
							strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
						}
						else
						{
							sprintf((char*)&tempstring2, "%u", tempv);
							strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
						}
					}
				}

				strcat((char*)&myhttp_tx_buffer, "&OpRatio=");
				for(T8U j = 0; j < 20; j++)
				{
					if(log_tdt_claim_v3.opratio[j] == 0)break;
					else
					{
						if(j != 0)
						{
							sprintf((char*)&tempstring2, "-%u", log_tdt_claim_v3.opratio[j]);
							strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
						}
						else
						{
							sprintf((char*)&tempstring2, "%u", log_tdt_claim_v3.opratio[j]);
							strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
						}
					}
				}

				StrNRep((T8U*)&log_tdt_claim_v3.employeecode, 0x20, 0 , sizeof(log_tdt_claim_v3.employeecode));
				sprintf(tempstring, "&PartialClaim=%u&TerminalNo=%u&EmployeeNo=%.10s", log_tdt_claim_v3.type, tbl_addr.addr, log_tdt_claim_v3.employeecode);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempv, (T8U *)&log_tdt_claim_v3.qty, 2);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_tdt_claim_v3.oritagid, 4);
				sprintf(tempstring, "&Quantity=%u&OriginalTagID=%u", tempv, tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_tdt_claim_v3.tagdtime, 4);
				sprintf(tempstring, "&DOB=%u&StartTime=", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				memset(tempstring, 0, 50);
				ret = stringDatetime(&log_tdt_claim_v3.starttime, (T8U*)&tempstring, 0);
				strcat((char*)&myhttp_tx_buffer, tempstring);
//				sprintf(tempstring, "&EndTime=");&EndTime=&TagType=&EPC="
//				strcat((char*)&myhttp_tx_buffer, tempstring);
				strcat((char*)&myhttp_tx_buffer, "&EndTime=");
				memset(tempstring, 0, 50);
				stringDatetime(&log_tdt_claim_v3.endtime, (T8U*)&tempstring, ret);
				strcat((char*)&myhttp_tx_buffer, tempstring);
//				sprintf(tempstring, "&TagType=1&EPC=");
//				strcat((char*)&myhttp_tx_buffer, tempstring);
				strcat((char*)&myhttp_tx_buffer, "&TagType=1&EPC=");
				strcat((char*)&myhttp_tx_buffer, "&InputLineID=");
				memset(tempstring, 0, 50);
				sprintf((char*)&tempstring2, "%u", log_tdt_claim_v3.lineid);
				strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
			}
			else if(*tempstring == DATA_TDT_CLAIM_V2.cmd)
			{
				LOG_TDT_CLAIM_V2 log_tdt_claim_v2;
				T8U tempstring2[10] = {0};
				T8U ret = 0;
				StrNCpy((T8U*)&log_tdt_claim_v2, (T8U*)&tempstring[2], sizeof(LOG_TDT_CLAIM_V2));

				strcat((char*)&myhttp_tx_buffer, "WebClaimWithGlobalID.ashx?OpNo=");

				for(T8U j = 0; j < 20; j++)
				{
					if(log_tdt_claim_v2.op[j] == 0)break;
					else
					{
						ChangeEndian((T8U *)&tempv, (T8U *)&log_tdt_claim_v2.op[j], 2);
						if(j != 0)
						{
							sprintf((char*)&tempstring2, "-%u", tempv);
							strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
						}
						else
						{
							sprintf((char*)&tempstring2, "%u", tempv);
							strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
						}
					}
				}

				strcat((char*)&myhttp_tx_buffer, "&OpRatio=");
				for(T8U j = 0; j < 20; j++)
				{
					if(log_tdt_claim_v2.opratio[j] == 0)break;
					else
					{
						if(j != 0)
						{
							sprintf((char*)&tempstring2, "-%u", log_tdt_claim_v2.opratio[j]);
							strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
						}
						else
						{
							sprintf((char*)&tempstring2, "%u", log_tdt_claim_v2.opratio[j]);
							strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
						}
					}
				}

				StrNRep((T8U*)&log_tdt_claim_v2.employeecode, 0x20, 0 , sizeof(log_tdt_claim_v2.employeecode));
				sprintf(tempstring, "&PartialClaim=%u&TerminalNo=%u&EmployeeNo=%.10s", log_tdt_claim_v2.type, tbl_addr.addr, log_tdt_claim_v2.employeecode);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempv, (T8U *)&log_tdt_claim_v2.qty, 2);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_tdt_claim_v2.oritagid, 4);
				sprintf(tempstring, "&Quantity=%u&OriginalTagID=%u", tempv, tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_tdt_claim_v2.tagdtime, 4);
				sprintf(tempstring, "&DOB=%u&StartTime=", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				memset(tempstring, 0, 50);
				ret = stringDatetime(&log_tdt_claim_v2.starttime, (T8U*)&tempstring, 0);
				strcat((char*)&myhttp_tx_buffer, tempstring);
//				sprintf(tempstring, "&EndTime=");&EndTime=&TagType=&EPC="
//				strcat((char*)&myhttp_tx_buffer, tempstring);
				strcat((char*)&myhttp_tx_buffer, "&EndTime=");
				memset(tempstring, 0, 50);
				stringDatetime(&log_tdt_claim_v2.endtime, (T8U*)&tempstring, ret);
				strcat((char*)&myhttp_tx_buffer, tempstring);
//				sprintf(tempstring, "&TagType=1&EPC=");
//				strcat((char*)&myhttp_tx_buffer, tempstring);
				strcat((char*)&myhttp_tx_buffer, "&TagType=1&EPC=");
			}
			else if(*tempstring == DATA_GROUPTAG_CLAIM_V2.cmd)
			{
				LOG_GROUPTAG_CLAIM_V2 log_group_claim_v2;
				T8U tempstring2[10] = {0};
				T8U ret = 0;
				StrNCpy((T8U*)&log_group_claim_v2, (T8U*)&tempstring[2], sizeof(LOG_GROUPTAG_CLAIM_V2));

				strcat((char*)&myhttp_tx_buffer, "ProcessGroupTagClaimWithMultipleOp.ashx?");
				ChangeEndian((T8U *)&tempx, (T8U *)&log_group_claim_v2.group_oritagid, 4);
				sprintf(tempstring, "GroupGlobalTagID=%u&GroupGlobalTagDOB=", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				memset(tempstring, 0, 50);
//				ChangeEndian((T8U *)&tempx, (T8U *)&log_group_claim_v2.group_oritagid, 4);
//				ESP_LOGI(TAG, "GUID: %X %X %X %X", log_group_claim_v2.GUID[0],
//						log_group_claim_v2.GUID[1],log_group_claim_v2.GUID[2],log_group_claim_v2.GUID[3]);

				GUIDConvertStrDate((T8U*)&log_group_claim_v2.GUID, (T8U*)&tempstring);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				StrNRep((T8U*)&log_group_claim_v2.grouptagcode, 0x20, 0 , sizeof(log_group_claim_v2.grouptagcode));
				sprintf(tempstring, "&GroupTagCode=%.10s", log_group_claim_v2.grouptagcode);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_group_claim_v2.grouptagid, 4);
				sprintf(tempstring, "&GroupTagNo=%u&OpNoList=", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);

				for(T8U j = 0; j < 20; j++)
				{
//					ESP_LOGI(TAG, "op %u: %u", j, log_group_claim_v2.op[j]);
					if(log_group_claim_v2.op[j] == 0)break;
					else
					{
						ChangeEndian((T8U *)&tempv, (T8U *)&log_group_claim_v2.op[j], 2);
						if(j != 0)
						{
							sprintf((char*)&tempstring2, "-%u", tempv);
							strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
						}
						else
						{
							sprintf((char*)&tempstring2, "%u", tempv);
							strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
						}
					}
				}

				strcat((char*)&myhttp_tx_buffer, "&OpRatioList=");
				for(T8U j = 0; j < 20; j++)
				{
//					ESP_LOGI(TAG, "op %u: %u", j, log_group_claim_v2.opratio[j]);
					if(log_group_claim_v2.opratio[j] == 0)break;
					else
					{
						if(j != 0)
						{
							sprintf((char*)&tempstring2, "-%u", log_group_claim_v2.opratio[j]);
							strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
						}
						else
						{
							sprintf((char*)&tempstring2, "%u", log_group_claim_v2.opratio[j]);
							strcat((char*)&myhttp_tx_buffer, (char*)&tempstring2);
						}
					}
				}

				strcat((char*)&myhttp_tx_buffer, "&StartTime=");
				memset(tempstring, 0, 50);
				ret = stringDatetime(&log_group_claim_v2.starttime, (T8U*)&tempstring, 0);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				strcat((char*)&myhttp_tx_buffer, "&EndTime=");
				memset(tempstring, 0, 50);
				stringDatetime(&log_group_claim_v2.endtime, (T8U*)&tempstring, ret);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				StrNRep((T8U*)&log_group_claim_v2.employeecode, 0x20, 0 , sizeof(log_group_claim_v2.employeecode));
				sprintf(tempstring, "&TerminalNo=%u&EmployeeNo=%.10s", tbl_addr.addr, log_group_claim_v2.employeecode);
				strcat((char*)&myhttp_tx_buffer, tempstring);
			}
			else if(*tempstring == DATA_EMPLOYEEINOUT.cmd)
			{
				LOG_EMPLOYEE log_employee;
				StrNCpy((T8U*)&log_employee, (T8U*)&tempstring[2], sizeof(log_employee));
//				sprintf(tempstring, "EmployeeTrx.ashx?EventDate=");
//				strcat((char*)&myhttp_tx_buffer, tempstring);
				strcat((char*)&myhttp_tx_buffer, "EmployeeTrx.ashx?EventDate=");
				memset(tempstring, 0, 50);
				stringDatetime(&log_employee.eventtime, (T8U*)&tempstring, 0);
//				ESP_LOGI(TAG, "%s", tempstring);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				StrNRep((T8U*)&log_employee.empcode, 0x20, 0 , sizeof(log_employee.empcode));
				sprintf(tempstring, "&EmployeeNo=%.10s&LogMode=%u&TerminalNo=%u", log_employee.empcode, log_employee.inout, tbl_addr.addr);
				strcat((char*)&myhttp_tx_buffer, tempstring);
			}
			else if(*tempstring == DATA_LOSTTIME.cmd)
			{
				LOG_LOSTTIME log_losttime;
				StrNCpy((T8U*)&log_losttime, (T8U*)&tempstring[2], sizeof(log_losttime));
//				sprintf(tempstring, "ProcessLostTimeTrx.ashx?EventTime=");
//				strcat((char*)&myhttp_tx_buffer, tempstring);
				strcat((char*)&myhttp_tx_buffer, "ProcessLostTimeTrx.ashx?EventTime=");
				memset(tempstring, 0, 50);
				stringDatetime(&log_losttime.eventtime, (T8U*)&tempstring, 0);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				StrNRep((T8U*)&log_losttime.empcode, 0x20, 0 , sizeof(log_losttime.empcode));
				sprintf(tempstring, "&LostID=%u&TerminalNo=%u&EmployeeNo=%.10s&LogMode=%u", log_losttime.lostid, tbl_addr.addr, log_losttime.empcode, log_losttime.inout);
				strcat((char*)&myhttp_tx_buffer, tempstring);
			}
			else if(*tempstring == DATA_ASSET_TRACKING.cmd)
			{
				LOG_ASSET_TRACKING log_asset_tracking;
				StrNCpy((T8U*)&log_asset_tracking, (T8U*)&tempstring[2], sizeof(log_asset_tracking));
				StrNRep((T8U*)&log_asset_tracking.machinecode, 0x20, 0 , sizeof(log_asset_tracking.machinecode));
				sprintf(tempstring, "ProcessMachineTransaction.ashx?MachineCode=%.20s", log_asset_tracking.machinecode);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				StrNRep((T8U*)&log_asset_tracking.empcode, 0x20, 0 , sizeof(log_asset_tracking.empcode));
				sprintf(tempstring, "&EmployeeNo=%.10s&GabletID=%u&EventTime=", log_asset_tracking.empcode, tbl_addr.addr);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				memset(tempstring, 0, 50);
				stringDatetime(&log_asset_tracking.eventtime, (T8U*)&tempstring, 0);
				strcat((char*)&myhttp_tx_buffer, tempstring);
			}
			else if(*tempstring == DATA_BUNDLE_MODIFYQTY.cmd)
			{
				LOG_BUNDLE_MODIFYQTY log_bundle_modifyqty;
				StrNCpy((T8U*)&log_bundle_modifyqty, (T8U*)&tempstring[2], sizeof(log_bundle_modifyqty));
				StrNRep((T8U*)&log_bundle_modifyqty.empcode, 0x20, 0 , sizeof(log_bundle_modifyqty.empcode));
				sprintf(tempstring, "ModifyBundleQuantityWithReason.ashx?EmployeeNo=%.10s&TagVersion=%u", log_bundle_modifyqty.empcode, log_bundle_modifyqty.tagversion);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_bundle_modifyqty.tagid, 4);
				sprintf(tempstring, "&TagID=%u", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_bundle_modifyqty.itemnumber, 4);
				sprintf(tempstring, "&ItemNo=%u", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
//				ChangeEndian((T8U *)&tempx, (T8U *)&log_bundle_modifyqty.tagwritecount, 4);
//				sprintf(tempstring, "&TagWriteCount=%u", tempx);
				sprintf(tempstring, "&TagWriteCount=0");
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_bundle_modifyqty.oritagid, 4);
				sprintf(tempstring, "&OriginalTagID=%u", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_bundle_modifyqty.guid, 4);
				sprintf(tempstring, "&GlobalTagDOB=%u", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempv, (T8U *)&log_bundle_modifyqty.oldqty, 2);
				sprintf(tempstring, "&OldQty=%u", tempv);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempv, (T8U *)&log_bundle_modifyqty.newqty, 2);
				sprintf(tempstring, "&NewQty=%u", tempv);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				sprintf(tempstring, "&TerminalNo=%u&EventTime=", tbl_addr.addr);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				memset(tempstring, 0, 50);
				stringDatetime(&log_bundle_modifyqty.eventtime, (T8U*)&tempstring, 0);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				StrNRep((T8U*)&log_bundle_modifyqty.processstage, 0x20, 0 , sizeof(log_bundle_modifyqty.processstage));
				sprintf(tempstring, "&ProcessStage=%s", log_bundle_modifyqty.processstage);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				sprintf(tempstring, "&ModiReason=%s", log_bundle_modifyqty.modireason);
//				StrNRep((T8U*)&tempstring, 0x20, strlen(tempstring) , sizeof(tempstring));
				StrNRep2((T8U*)&tempstring, 0x20, strlen(tempstring), sizeof(tempstring));
				strcat((char*)&myhttp_tx_buffer, tempstring);
			}
			else if(*tempstring == DATA_GARMENTREPAIR.cmd)
			{
				LOG_GARMENTREPAIR log_garmentrepair;
				T8U ret = 0;
				StrNCpy((T8U*)&log_garmentrepair, (T8U*)&tempstring[2], sizeof(log_garmentrepair));
				ChangeEndian((T8U *)&tempx, (T8U *)&log_garmentrepair.oritagid, 4);
				sprintf(tempstring, "ProcessQCM_SplitRepair.ashx?ProcessType=%u&GlobalTagID=%u", log_garmentrepair.type, tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_garmentrepair.tagdtime, 4);
				sprintf(tempstring, "&GlobalTagDOB=%u&TerminalNo=%u", tempx, tbl_addr.addr);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				StrNRep((T8U*)&log_garmentrepair.employeecode, 0x20, 0 , sizeof(log_garmentrepair.employeecode));
				sprintf(tempstring, "&EmployeeNo=%.10s", log_garmentrepair.employeecode);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempv, (T8U *)&log_garmentrepair.tag_qty, 2);
				sprintf(tempstring, "&BundleQty=%u&StartTime=", tempv);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				memset(tempstring, 0, 50);
				ret = stringDatetime(&log_garmentrepair.starttime, (T8U*)&tempstring, 0);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				sprintf(tempstring, "&EndTime=");
				strcat((char*)&myhttp_tx_buffer, tempstring);
				memset(tempstring, 0, 50);
				stringDatetime(&log_garmentrepair.eventtime, (T8U*)&tempstring, ret);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_garmentrepair.ID_eventid, 4);
				sprintf(tempstring, "&EventID=%u", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_garmentrepair.ID_datetimeid, 4);
				sprintf(tempstring, "&DateTimeID=%u", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_garmentrepair.ID_SN, 4);
				sprintf(tempstring, "&SerialNo=%u", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_garmentrepair.QC_term, 4);
				sprintf(tempstring, "&QCTerminal=%u", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempv, (T8U *)&log_garmentrepair.OP_assigned, 2);
				sprintf(tempstring, "&OPAssigned=%u", tempv);
				strcat((char*)&myhttp_tx_buffer, tempstring);
			}
			else if(*tempstring == DATA_GROUPTAG_REGISTER.cmd)
			{
				LOG_GROUPTAG_REGISTER log_group_register;
				StrNCpy((T8U*)&log_group_register, (T8U*)&tempstring[2], sizeof(log_group_register));
				if(log_group_register.type == 0)
				{//"http://{SERVERNAME}/Mobile/handler/RemoveBundleFromBasket.ashx?BundleGlobalTagID=&BundleGlobalTagDOB=";
					ChangeEndian((T8U *)&tempx, (T8U *)&log_group_register.originaltagid, 4);
					sprintf(tempstring, "RemoveBundleFromBasket.ashx?BundleGlobalTagID=%u&BundleGlobalTagDOB=", tempx);
					strcat((char*)&myhttp_tx_buffer, tempstring);
					memset(tempstring, 0, 50);
					GUIDConvertStrDate((T8U*)&log_group_register.guid, (T8U*)&tempstring);
					strcat((char*)&myhttp_tx_buffer, tempstring);
				}
				else if(log_group_register.type == 1)
				{//"http://{SERVERNAME}/Mobile/handler/ProcessGroupTagRegister.ashx?
				 //TerminalNo=&EmployeeNo=&GroupTagNo=&GroupTagDOB=&BundleTagNo=&GlobalTagID=&GlobalTagDOB=&EventTime=";
					if(dataPos == 30) dataPos = 32;
					else if(dataPos == 40) dataPos = 42;

					StrNRep((T8U*)&log_group_register.employeecode, 0x20, 0 , sizeof(log_group_register.employeecode));
					sprintf(tempstring, "ProcessGroupTagRegister.ashx?TerminalNo=%u&EmployeeNo=%.10s", tbl_addr.addr, log_group_register.employeecode);
					strcat((char*)&myhttp_tx_buffer, tempstring);
					ChangeEndian((T8U *)&tempx, (T8U *)&log_group_register.grouptagoritagid, 4);
					sprintf(tempstring, "&GroupTagNo=%u&GroupTagDOB=", tempx);
					strcat((char*)&myhttp_tx_buffer, tempstring);
					memset(tempstring, 0, 50);
					GUIDConvertStrDate((T8U*)&log_group_register.grouptagGUID, (T8U*)&tempstring);
					strcat((char*)&myhttp_tx_buffer, tempstring);
					ChangeEndian((T8U *)&tempx, (T8U *)&log_group_register.tagid, 4);
					sprintf(tempstring, "&BundleTagNo=%u", tempx);
					strcat((char*)&myhttp_tx_buffer, tempstring);
					ChangeEndian((T8U *)&tempx, (T8U *)&log_group_register.originaltagid, 4);
					sprintf(tempstring, "&GlobalTagID=%u", tempx);//sprintf(tempstring, "&GlobalTagID=%u&GlobalTagDOB=", tempx);
					strcat((char*)&myhttp_tx_buffer, tempstring);
//					memset(tempstring, 0, 50);
//					GUIDConvertStrDate((T8U*)&log_group_register.guid, (T8U*)&tempstring);
//					strcat((char*)&myhttp_tx_buffer, tempstring);
					ChangeEndian((T8U *)&tempx, (T8U *)&log_group_register.guid, 4);
					sprintf(tempstring, "&GlobalTagDOB=%u", tempx);//sprintf(tempstring, "&GlobalTagID=%u&GlobalTagDOB=", tempx);
					strcat((char*)&myhttp_tx_buffer, tempstring);
					sprintf(tempstring, "&EventTime=");
					strcat((char*)&myhttp_tx_buffer, tempstring);
					memset(tempstring, 0, 50);
					stringDatetime2(&log_group_register.eventtime, (T8U*)&tempstring);
					strcat((char*)&myhttp_tx_buffer, tempstring);
				}
			}
//			else if(*tempstring == DATA_GROUPTAG_CHANGE.cmd)
//			{//"http://{SERVERNAME}/Mobile/handler/ChangeBasket.ashx?
//			 //OldGroupGlobalTagID=&OldGroupGlobalTagDOB=&NewGroupGlobalTagID=&NewGroupGlobalTagDOB=";
//
//				LOG_GROUPTAG_CHANGE log_grouptag_change;
//				StrNCpy((T8U*)&log_grouptag_change, (T8U*)&tempstring[2], sizeof(log_grouptag_change));
////				T32U x = 0;
//////				StrNCpy((T8U *)&x, (T8U *)&log_grouptag_change.group_oritagid_old, 4);
////				ChangeEndian((T8U *)&x, (T8U *)&log_grouptag_change.group_oritagid_old, 4);
////				ESP_LOGI(TAG, "old tag Id: %u", x);
//////				StrNCpy((T8U *)&x, (T8U *)&log_grouptag_change.GUID_old, 4);
////				ChangeEndian((T8U *)&x, (T8U *)&log_grouptag_change.GUID_old, 4);
////				ESP_LOGI(TAG, "old guid: %u", x);
//////				StrNCpy((T8U *)&x, (T8U *)&log_grouptag_change.group_oritagid_new, 4);
////				ChangeEndian((T8U *)&x, (T8U *)&log_grouptag_change.group_oritagid_new, 4);
////				ESP_LOGI(TAG, "new tag Id: %u", x);
//////				StrNCpy((T8U *)&x, (T8U *)&log_grouptag_change.GUID_new, 4);
////				ChangeEndian((T8U *)&x, (T8U *)&log_grouptag_change.GUID_new, 4);
////				ESP_LOGI(TAG, "new guid: %u", x);
//
//				sprintf(tempstring, "ChangeBasket.ashx?");
//				strcat((char*)&myhttp_tx_buffer, tempstring);
//				ChangeEndian((T8U *)&tempx, (T8U *)&log_grouptag_change.group_oritagid_old, 4);
////				StrNCpy((T8U *)&tempx, (T8U *)&log_grouptag_change.group_oritagid_old, 4);
//				sprintf(tempstring, "OldGroupGlobalTagID=%u", tempx);
//				strcat((char*)&myhttp_tx_buffer, tempstring);
//				ChangeEndian((T8U *)&tempx, (T8U *)&log_grouptag_change.GUID_old, 4);
////				StrNCpy((T8U *)&tempx, (T8U *)&log_grouptag_change.GUID_old, 4);
//				sprintf(tempstring, "&OldGroupGlobalTagDOB=%u", tempx);
//				strcat((char*)&myhttp_tx_buffer, tempstring);
//				ChangeEndian((T8U *)&tempx, (T8U *)&log_grouptag_change.group_oritagid_new, 4);
////				StrNCpy((T8U *)&tempx, (T8U *)&log_grouptag_change.group_oritagid_new, 4);
//				sprintf(tempstring, "&NewGroupGlobalTagID=%u", tempx);
//				strcat((char*)&myhttp_tx_buffer, tempstring);
//				ChangeEndian((T8U *)&tempx, (T8U *)&log_grouptag_change.GUID_new, 4);
////				StrNCpy((T8U *)&tempx, (T8U *)&log_grouptag_change.GUID_new, 4);
//				sprintf(tempstring, "&NewGroupGlobalTagDOB=%u", tempx);
//				strcat((char*)&myhttp_tx_buffer, tempstring);
//			}
			else if((*tempstring == DATA_TDT_SPLIT_START.cmd) && (*(tempstring+2) == 1))
			{
				//"http://{SERVERNAME}/Mobile/handler/OFFLineSplitBundleStart.ashx?
				//EventDate=&Type=&TagVersion=&OriginalTagId=&TagDTime=&EmployeeNo=&TerminalNo=";

				LOG_TDT_SPLIT_START log_tdt_split_start;
				StrNCpy((T8U*)&log_tdt_split_start, (T8U*)&tempstring[2], sizeof(log_tdt_split_start));

				sprintf(tempstring, "OFFLineSplitBundleStart.ashx?EventDate=");
				strcat((char*)&myhttp_tx_buffer, tempstring);
				memset(tempstring, 0, 50);
				stringDatetime(&log_tdt_split_start.eventtime, (T8U*)&tempstring, 0);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				sprintf(tempstring, "&Type=%u&TagVersion=%u", log_tdt_split_start.type, log_tdt_split_start.mother_tagversion);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_tdt_split_start.mother_oritagid, 4);
				sprintf(tempstring, "&OriginalTagId=%u", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_tdt_split_start.mother_tagdtime, 4);
				sprintf(tempstring, "&TagDTime=%u", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				StrNRep((T8U*)&log_tdt_split_start.employeecode, 0x20, 0 , sizeof(log_tdt_split_start.employeecode));
				sprintf(tempstring, "&EmployeeNo=%.10s&TerminalNo=%u", log_tdt_split_start.employeecode, tbl_addr.addr);
				strcat((char*)&myhttp_tx_buffer, tempstring);
			}
			else if((*tempstring == DATA_TDT_SPLIT_CHILDITEM.cmd) && (*(tempstring+2) == 2))
			{
				//"http://{SERVERNAME}/Mobile/handler/OFFLineSplitItemTransaction.ashx?
				//EventDate=&Type=&TagVersion=&OriginalTagId=&TagDTime=&EmployeeNo=&TerminalNo=
				//&ChildTagDTime=&ChildTagID=&ChildTagWriteCount=&SplitSequence=&SplitQuantity=";

				LOG_TDT_SPLIT_CHILDITEM log_tdt_split_childitem;
				StrNCpy((T8U*)&log_tdt_split_childitem, (T8U*)&tempstring[2], sizeof(log_tdt_split_childitem));

				sprintf(tempstring, "OFFLineSplitItemTransaction.ashx?EventDate=");
				strcat((char*)&myhttp_tx_buffer, tempstring);
				memset(tempstring, 0, 50);
				stringDatetime(&log_tdt_split_childitem.eventtime, (T8U*)&tempstring, 0);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				sprintf(tempstring, "&Type=%u&TagVersion=%u", log_tdt_split_childitem.type, log_tdt_split_childitem.mother_tagversion);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_tdt_split_childitem.mother_oritagid, 4);
				sprintf(tempstring, "&OriginalTagId=%u", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_tdt_split_childitem.mother_tagdtime, 4);
				sprintf(tempstring, "&TagDTime=%u", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				StrNRep((T8U*)&log_tdt_split_childitem.employeecode, 0x20, 0 , sizeof(log_tdt_split_childitem.employeecode));
				sprintf(tempstring, "&EmployeeNo=%.10s&TerminalNo=%u", log_tdt_split_childitem.employeecode, tbl_addr.addr);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_tdt_split_childitem.child_tagdtime, 4);
				sprintf(tempstring, "&ChildTagDTime=%u", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_tdt_split_childitem.child_tagid, 4);
				sprintf(tempstring, "&ChildTagID=%u", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_tdt_split_childitem.child_tagwritecount, 4);
				sprintf(tempstring, "&ChildTagWriteCount=%u", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempv, (T8U *)&log_tdt_split_childitem.split_sequence, 2);
				sprintf(tempstring, "&SplitSequence=%u", tempv);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempv, (T8U *)&log_tdt_split_childitem.split_qty, 2);
				sprintf(tempstring, "&SplitQuantity=%u", tempv);
				strcat((char*)&myhttp_tx_buffer, tempstring);
			}
			else if((*tempstring == DATA_TDT_SPLIT_END.cmd) && (*(tempstring+2) == 3))
			{
				//"http://{SERVERNAME}/Mobile/handler/OFFLineSplitBundleEnd.ashx?
				//EventDate=&Type=&TagVersion=&OriginalTagId=&TagDTime=&EmployeeNo=&TerminalNo=";

				LOG_TDT_SPLIT_END log_tdt_split_end;
				StrNCpy((T8U*)&log_tdt_split_end, (T8U*)&tempstring[2], sizeof(log_tdt_split_end));

				sprintf(tempstring, "OFFLineSplitBundleEnd.ashx?EventDate=");
				strcat((char*)&myhttp_tx_buffer, tempstring);
				memset(tempstring, 0, 50);
				stringDatetime(&log_tdt_split_end.eventtime, (T8U*)&tempstring, 0);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				sprintf(tempstring, "&Type=%u&TagVersion=%u", log_tdt_split_end.type, log_tdt_split_end.mother_tagversion);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_tdt_split_end.mother_oritagid, 4);
				sprintf(tempstring, "&OriginalTagId=%u", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				ChangeEndian((T8U *)&tempx, (T8U *)&log_tdt_split_end.mother_tagdtime, 4);
				sprintf(tempstring, "&TagDTime=%u", tempx);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				StrNRep((T8U*)&log_tdt_split_end.employeecode, 0x20, 0 , sizeof(log_tdt_split_end.employeecode));
				sprintf(tempstring, "&EmployeeNo=%.10s&TerminalNo=%u", log_tdt_split_end.employeecode, tbl_addr.addr);
				strcat((char*)&myhttp_tx_buffer, tempstring);
			}
			else if(*tempstring == DATA_LOG_ERROR.cmd)
			{
				//InsertErrorDeviceLog.ashx?TerminalNo=&EmployeeNo=&FunctionName=&ErrorMessage=&EventTime=

				LOG_ERROR log_error;
				StrNCpy((T8U*)&log_error, (T8U*)&tempstring[2], sizeof(log_error));

				sprintf(tempstring, "InsertErrorDeviceLog.ashx?TerminalNo=%u", tbl_addr.addr);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				StrNRep((T8U*)&log_error.employeecode, 0x20, 0 , sizeof(log_error.employeecode));
				StrNRep((T8U*)&log_error.functionName, 0x20, 0 , sizeof(log_error.functionName));
				sprintf(tempstring, "&EmployeeNo=%.10s&FunctionName=%.20s", log_error.employeecode, log_error.functionName);
				strcat((char*)&myhttp_tx_buffer, tempstring);
				sprintf(tempstring, "&ErrorMessage=");
				strcat((char*)&myhttp_tx_buffer, tempstring);

				if(log_error.type == 1)
				{
					StrNRep((T8U*)&log_error.extra.errorMessage, 0x20, 0 , sizeof(log_error.extra.errorMessage));
					sprintf(tempstring, "%.80s", log_error.extra.errorMessage);
					strcat((char*)&myhttp_tx_buffer, tempstring);
				}
				else if(log_error.type == 2)
				{
					StrNRep((T8U*)&log_error.extra.error_claim.sv_empcode, 0x20, 0 , sizeof(log_error.extra.error_claim.sv_empcode));
					sprintf(tempstring, "{SV=%.10s", log_error.extra.error_claim.sv_empcode);
					strcat((char*)&myhttp_tx_buffer, tempstring);
					sprintf(tempstring, ",TagID=%u,OriTagID=%u,TagDOB=", log_error.extra.error_claim.tagid, log_error.extra.error_claim.oritagid);
					strcat((char*)&myhttp_tx_buffer, tempstring);
					memset(tempstring, 0, 50);
					ChangeEndian((T8U *)&tempx, (T8U *)&log_error.extra.error_claim.tagdtime, 4);
					GUIDConvertStrDate((T8U*)&tempx, (T8U*)&tempstring);
					strcat((char*)&myhttp_tx_buffer, tempstring);
					ChangeEndian((T8U *)&tempv, (T8U *)&log_error.extra.error_claim.op[0], 2);
					sprintf(tempstring, ",OP=%u", tempv);
					strcat((char*)&myhttp_tx_buffer, tempstring);

					for(int tempcount = 1; tempcount < 20; tempcount++)
					{
						if(log_error.extra.error_claim.op[tempcount] == 0) break;
						ChangeEndian((T8U *)&tempv, (T8U *)&log_error.extra.error_claim.op[tempcount], 2);
						sprintf(tempstring, "-%u", tempv);
						strcat((char*)&myhttp_tx_buffer, tempstring);
					}

					sprintf(tempstring, "}");
					strcat((char*)&myhttp_tx_buffer, tempstring);
				}
				sprintf(tempstring, "&EventTime=");
				strcat((char*)&myhttp_tx_buffer, tempstring);
				memset(tempstring, 0, 50);
				stringDatetime(&log_error.eventtime, (T8U*)&tempstring, 0);
				strcat((char*)&myhttp_tx_buffer, tempstring);
			}
			else
			{
				myhttp_tx_buffer[0] = 0;

				if(dataPos == 40)
				{
					flush_record(0);
				}
				else if(dataPos == 30)
				{
					flush_FS_record(1);
				}
				return;
			}
//			sprintf((char*)&myhttp_tx_buffer, "%s", myhttp_tx_buffer_wrong);
//			ESP_LOGW(TAG, "trywrong");
		}
	}
//	else
//	{
//		TOP_Message = 0;
//		myhttp_tx_buffer[0] = 0;
//		return;
//	}
	strcat((char*)&myhttp_tx_buffer, " HTTP/1.0\r\nHost: ");
	sprintf(tempstring, "%s:80\r\n\r\n", tbl_server.IPv4);
//	sprintf(tempstring, "%s:80\r\n\r\n", WEB_SERVER);
	strcat((char*)&myhttp_tx_buffer, tempstring);

//	sprintf((char*)&myhttp_tx_buffer, "%s", myhttp_tx_buffer_wrong);
//	ESP_LOGW(TAG, "trywrong");
}

void process_received(void)
{
	T16U i = 0;
	T8U state = 0;
	T8U result = 2;
	T8U tempv[4] = {};
	T16U httpV = 0;

	while(i < 20)
	{
		vTaskDelay(1 / portTICK_PERIOD_MS);
		switch(state)
		{
			case 0:
			{
				if(checkstring(&i, (T8U*)&httpstr, sizeof(httpstr)) == _PASS)
				{
					state = 10;
				}
				break;
			}
			case 10:
			{
				if(myhttp_rx_buffer[i] == ' ')
				{
					tempv[0] = myhttp_rx_buffer[i+1];
					tempv[1] = myhttp_rx_buffer[i+2];
					tempv[2] = myhttp_rx_buffer[i+3];
					httpV = atoi((char*)&tempv);
					sprintf((char*)&tbl_wifi_diag.httpSend, "\nhttp_send\nSent & Reply\n%u", httpV);
					if(httpV < 400) result = _PASS;//if((httpV > 199) & (httpV < 300)) result = _PASS;
					else result = _FAIL;
				}
				break;
			}
			default:
			{
				state = 0;
				break;
			}
		}

		if(result < 2) break;
		i++;
	}

	ESP_LOGI(TAG, "result:%u, %u", result, httpV);
	if(result == _FAIL)
	{
		if(FLAG_STATE & F_CONNECTSERVER)
		{
			FLAG_STATE &= ~F_CONNECTSERVER;
			FLAG_STATE |= F_UPDATEHEADER;
		}

		clearUrgentFlag();

		return;
	}

	SAVE_DATA |= SAVE_HOLD;

	if(pos == TOPDATA_SYNCDATE)
	{
		parseGetTime();
		TOP_Message &= ~TOPDATA_SYNCDATE;
	}
	else if(pos == TOPDATA_POWERUP)
	{
		parseStartup();
		TOP_Message &= ~TOPDATA_POWERUP;
	}

//	else if(pos == TOPDATA_GETBASICINFO)
//	{
//		parseBasicInfo();
//		TOP_Message &= ~TOPDATA_GETBASICINFO;
//		MenuIdle_State = 0;
//	}
	else if(pos == TOPDATA_BASICINFOBYGROUP)
	{
		parseBasicInfobyGroup();
		TOP_Message &= ~TOPDATA_BASICINFOBYGROUP;
		MenuIdle_State = 0;
	}
	else if(pos == TOPDATA_GETQTYEMPLOYEE)
	{
		parseGetQtyEmp();
		TOP_Message &= ~TOPDATA_GETQTYEMPLOYEE;
		MenuIdle_State = 0;
	}
	else if(pos == TOPDATA_GETOVERALLEMPLOYEE)
	{
		parseGetOverAllEmp();
		TOP_Message &= ~TOPDATA_GETOVERALLEMPLOYEE;
		MenuIdle_State = 0;
	}
	else if(pos == TOPDATA_GETPRODUCTIONINFO)
	{
		parseGetProduction();
		TOP_Message &= ~TOPDATA_GETPRODUCTIONINFO;
		refresh_count();
		MenuIdle_State = 0;
	}
	else if(pos == TOPDATA_PRODUCTIONINFOBYGROUP)
	{
		parseProductionInfobyGroup();
		TOP_Message &= ~TOPDATA_PRODUCTIONINFOBYGROUP;
		refresh_count();
		MenuIdle_State = 0;
	}
	else if(pos == TOPDATA_TERMINALINFOBYMAC)
	{
		parseGetTerminalInfoByMAC();
		TOP_Message &= ~TOPDATA_TERMINALINFOBYMAC;
	}
	else if(pos == TOPDATA_UPDATETERMINFO)
	{
		if(parseSuccessMessage() == _PASS)
		{
			TOP_Message &= ~TOPDATA_UPDATETERMINFO;
		}
	}
	else if(pos == TOPDATA_CHECKUPDATEFIRM)
	{
		if(parsetrueStr() == _PASS)
		{
			FLAG_STATE |= F_NEEDUPDATEFIRM;
		}
		TOP_Message &= ~TOPDATA_CHECKUPDATEFIRM;
	}
	else if(pos == TOPDATA_GETPRODGROUPBYNAME)
	{
		GetProdGroupNameByProdGroupID();
		TOP_Message &= ~TOPDATA_GETPRODGROUPBYNAME;
	}
	else if(pos == TOPDATA_CHECKINPUTOPCLAIMEDGROUP)
	{
		CheckInputOpClaimedGroup();
		TOP_Message &= ~TOPDATA_CHECKINPUTOPCLAIMEDGROUP;
	}
	else if(dataPos == 20)
	{
		if(parseSuccessMessage() == _PASS)
		{
//			if(tbl_buf_pointer.high.rear != tbl_buf_pointer.high.front)
//			{
//				tbl_buf_pointer.high.rear++;
//				if(tbl_buf_pointer.high.rear >= MAX_HIGHRECORD)
//				{
//					tbl_buf_pointer.high.rear = 0;
//				}
//				if(tbl_buf_pointer.high.front >= MAX_HIGHRECORD)
//				{
//					tbl_buf_pointer.high.front = 0;
//				}
//			}
//			WriteTable(&T_BUF_POINTER, (T8U *)&tbl_buf_pointer, 0);
			flush_record(1);
		}
	}
	else if(dataPos == 21)
	{
		if(parseRequestMessage() == _PASS)
		{
//			if(tbl_buf_pointer.high.rear != tbl_buf_pointer.high.front)
//			{
//				tbl_buf_pointer.high.rear++;
//				if(tbl_buf_pointer.high.rear >= MAX_HIGHRECORD)
//				{
//					tbl_buf_pointer.high.rear = 0;
//				}
//				if(tbl_buf_pointer.high.front >= MAX_HIGHRECORD)
//				{
//					tbl_buf_pointer.high.front = 0;
//				}
//			}
//			WriteTable(&T_BUF_POINTER, (T8U *)&tbl_buf_pointer, 0);
			flush_record(1);
		}
	}
	else if(dataPos == 30)
	{
		if(parseSuccessMessage() == _PASS)
		{
			flush_FS_record(1);
		}
	}
	else if(dataPos == 31)
	{
		if(parseSuccessMessage() == _PASS)
		{
//			for(T8U i = 0; i < record; i++)
//			{
				flush_FS_record(record);
//			}
		}
		record = 0;
	}
	else if(dataPos == 32)
	{
		if(parsetrueStr() == _PASS)
		{
			flush_FS_record(1);
		}
	}
	else if(dataPos == 40)
	{
		if(parseSuccessMessage() == _PASS)
		{
			flush_record(0);
		}
	}
	else if(dataPos == 41)
	{
		if(parseSuccessMessage() == _PASS)
		{
			for(T8U i = 0; i < record; i++)
			{
				flush_record(0);
			}
		}
		record = 0;
	}
	else if(dataPos == 42)
	{
		if(parsetrueStr() == _PASS)
		{
			flush_record(0);
		}
	}

	if((FLAG_STATE & F_CONNECTSERVER) == 0)
	{
		FLAG_STATE |= F_CONNECTSERVER;
		FLAG_STATE |= F_UPDATEHEADER;
	}

	SAVE_DATA &= ~SAVE_HOLD;
}

void clearUrgentFlag(void)
{
	if(pos == TOPDATA_GETPREFIX)
	{
		TOP_Message &= ~TOPDATA_GETPREFIX;
	}
	else if(pos == TOPDATA_GETBUNDLEINFO)
	{
		TOP_Message &= ~TOPDATA_GETBUNDLEINFO;
	}
	else if(pos == TOPDATA_REGISTERUHFTAG)
	{
		TOP_Message &= ~TOPDATA_REGISTERUHFTAG;
	}
	else if(pos == TOPDATA_GETLASTCLAIMOP)
	{
		TOP_Message &= ~TOPDATA_GETLASTCLAIMOP;
	}
	if(pos == TOPDATA_CHECKOPVALIDATION)
	{
		TOP_Message &= ~TOPDATA_CHECKOPVALIDATION_2ND;
	}
	if(pos == TOPDATA_CHANGEGROUPTAG)
	{
		TOP_Message &= ~TOPDATA_CHANGEGROUPTAG;
	}
}

void parseStartup(void)
{
	T16U i = 0;
	T16U pre_i = 0;
	T8U slot = 0;
	T8U state = 0;
	T8U tempcount = 0;
	T8U tempdata[20] = {0};
	TBL_CONDITION tbl_condition;
	TBL_UDF tbl_udf;
	TBL_PROCESS_STAGE tbl_process_stage;
	TBL_MODI_REASON tbl_modi_reason;
	TBL_PROD_GROUP tbl_prod_group_temp;
//	T8U count=0;
	while(i < len_return)
	{
		switch(state)
		{
			case 0:
			{
				if(myhttp_rx_buffer[i] == ServerTimestr[tempcount])
				{
					pre_i = i;
					i++;
					for(tempcount = 1; tempcount < sizeof(ServerTimestr); tempcount++)
					{
						if(myhttp_rx_buffer[i] == ServerTimestr[tempcount])
						{
							i++;
						}
						else
						{
							tempcount = 0;
							i = pre_i;
							break;
						}
					}

					if(tempcount == sizeof(ServerTimestr))
					{
						i--;
						slot = 0;
						tempcount = 0;
						state = 10;//20;//10; converting in case 11 not working. SO ignore this time temporary
						break;
					}
				}
				if(myhttp_rx_buffer[i] == LostTypestr[tempcount])
				{
					pre_i = i;
					i++;
					for(tempcount = 1; tempcount < sizeof(LostTypestr); tempcount++)
					{
						if(myhttp_rx_buffer[i] == LostTypestr[tempcount])
						{
							i++;
						}
						else
						{
							tempcount = 0;
							i = pre_i;
							break;
						}
					}

					if(tempcount == sizeof(LostTypestr))
					{
						i--;
						memset(&tbl_condition, '\0', sizeof(tbl_condition));
						for(slot = 0; slot < MAX_CONDITION ; slot++)
						{
							WriteTable(&T_CONDITION, (T8U *) &tbl_condition, slot);
						}
						slot = 0;
						tempcount = 0;
						state = 20;
						break;
					}
				}
				if(myhttp_rx_buffer[i] == UserDefinedFunctionColstr[tempcount])
				{
					pre_i = i;
					i++;
					for(tempcount = 1; tempcount < sizeof(UserDefinedFunctionColstr); tempcount++)
					{
						if(myhttp_rx_buffer[i] == UserDefinedFunctionColstr[tempcount])
						{
							i++;
						}
						else
						{
							tempcount = 0;
							i = pre_i;
							break;
						}
					}

					if(tempcount == sizeof(UserDefinedFunctionColstr))
					{
						i--;
						slot = 0;
						tempcount = 0;
						state = 30;
						break;
					}
				}
				if(myhttp_rx_buffer[i] == ProcessStagestr[tempcount])
				{
					pre_i = i;
					i++;
					for(tempcount = 1; tempcount < sizeof(ProcessStagestr); tempcount++)
					{
						if(myhttp_rx_buffer[i] == ProcessStagestr[tempcount])
						{
							i++;
						}
						else
						{
							tempcount = 0;
							i = pre_i;
							break;
						}
					}

					if(tempcount == sizeof(ProcessStagestr))
					{
						i--;
						memset(&tbl_process_stage, '\0', sizeof(tbl_process_stage));
						for(slot = 0; slot < 10 ; slot++)
						{
							WriteTable(&T_PROCESS_STAGE, (T8U *) &tbl_process_stage, slot);
						}
						slot = 0;
						tempcount = 0;
						state = 40;
						break;
					}

//					if(tempcount == sizeof(ProcessStagestr))
//					{
//						i--;
//						slot = 0;
//						tempcount = 0;
//						state = 40;
//						break;
//					}
				}
				if(myhttp_rx_buffer[i] == ModiReasonstr[tempcount])
				{
					pre_i = i;
					i++;
					for(tempcount = 1; tempcount < sizeof(ModiReasonstr); tempcount++)
					{
						if(myhttp_rx_buffer[i] == ModiReasonstr[tempcount])
						{
							i++;
						}
						else
						{
							tempcount = 0;
							i = pre_i;
							break;
						}
					}

					if(tempcount == sizeof(ModiReasonstr))
					{
						i--;
						memset(&tbl_modi_reason, '\0', sizeof(tbl_modi_reason));
						for(slot = 0; slot < 10 ; slot++)
						{
							WriteTable(&T_MODI_REASON, (T8U *) &tbl_modi_reason, slot);
						}
						slot = 0;
						tempcount = 0;
						state = 50;
						break;
					}
//					if(tempcount == sizeof(ModiReasonstr))
//					{
//						i--;
//						slot = 0;
//						tempcount = 0;
//						state = 50;
//						break;
//					}
				}
				if(myhttp_rx_buffer[i] == ProdGroupstr[tempcount])
				{
					pre_i = i;
					i++;
					for(tempcount = 1; tempcount < sizeof(ProdGroupstr); tempcount++)
					{
						if(myhttp_rx_buffer[i] == ProdGroupstr[tempcount])
						{
							i++;
						}
						else
						{
							tempcount = 0;
							i = pre_i;
							break;
						}
					}

					if(tempcount == sizeof(ProdGroupstr))
					{
						i--;
						memset(&tbl_prod_group_temp, '\0', sizeof(tbl_prod_group_temp));
//						WriteTable(&T_PROD_GROUP, (T8U *) &tbl_prod_group, 0);
						slot = 0;
						tempcount = 0;
						state = 60;
						break;
					}
				}
				break;
			}
			case 10:
			{
				if(myhttp_rx_buffer[i] == ServerCurrentTimestr[tempcount])
				{
					pre_i = i;
					i++;
					for(tempcount = 1; tempcount < sizeof(ServerCurrentTimestr); tempcount++)
					{
						if(myhttp_rx_buffer[i] == ServerCurrentTimestr[tempcount])
						{
							i++;
						}
						else
						{
							tempcount = 0;
							i = pre_i;
							break;
						}
					}

					if(tempcount == sizeof(ServerCurrentTimestr))
					{
						i--;
						tempcount = 0;
						state = 11;
						break;
					}
				}
				break;
			}
			case 11:
			{
				if(myhttp_rx_buffer[i] == Datestr[tempcount])
				{
					pre_i = i;
					i++;
					for(tempcount = 1; tempcount < sizeof(Datestr); tempcount++)
					{
						if(myhttp_rx_buffer[i] == Datestr[tempcount])
						{
							i++;
						}
						else
						{
							tempcount = 0;
							i = pre_i;
							break;
						}
					}

					if(tempcount == sizeof(Datestr))
					{
						memset(tempdata, '\0', sizeof(tempdata));
						for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
						{
							tempdata[tempcount] = myhttp_rx_buffer[i++];
							if((myhttp_rx_buffer[i] == plusstr) || (myhttp_rx_buffer[i] == minusstr))
							{
								break;
							}
						}
						//temporary disable
						ESP_LOGI(TAG, "Rec time:%s", tempdata);
						T64U millisecond = atoll((char*)&tempdata);//this atoll or atol can't be used to convert long

						long t_millisecond = (long) (millisecond / 1000);
						ESP_LOGI(TAG, "Rec time:%lu", t_millisecond);

						memset(tempdata, '\0', sizeof(tempdata));
						for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
						{
							tempdata[tempcount] = myhttp_rx_buffer[i++];
							if(myhttp_rx_buffer[i] == closebracketstr)
							{
								break;
							}
						}

						T16S region = atoi((char*)&tempdata);
						ESP_LOGI(TAG, "Rec time rg:%u", region);

//						T8S hour = (T8S) (region / 100);
//						T8S minute = (T8S) (region - (hour*100));
//						region = ((hour * 60) + minute) * 60; //in seconds
//
//						ESP_LOGI(TAG, "Rec time hr:%u", hour);
//						ESP_LOGI(TAG, "Rec time mn:%u", minute);
//						ESP_LOGI(TAG, "Rec time rg:%u", region);

//						millisecond = millisecond + region;

						RTC_convert_to_term_ms(&t_millisecond, region);

						ESP_LOGI(TAG, "Rec time:%lu", t_millisecond);

						long t = 0;
						RTC_get_ms(&t);
						ESP_LOGI(TAG, "Term time:%lu", t);

//						ESP_LOGI(TAG, "size t:%u", sizeof(t));

//						T32U a = 0;
//
//						ESP_LOGI(TAG, "size a:%u", sizeof(a));

//						float b = 0;
//
//						ESP_LOGI(TAG, "size b:%u", sizeof(b));
//
//						double c = 0;
//
//						ESP_LOGI(TAG, "size c:%u", sizeof(c));

//						RTC_Set_2(millisecond);
						vTaskDelay(1);
						tempcount = 0;
						state = 0;
						break;
					}
				}
				break;
			}
			case 20:
			{
				if(myhttp_rx_buffer[i] == LostTypeIDstr[tempcount])
				{
					pre_i = i;
					i++;
					for(tempcount = 1; tempcount < sizeof(LostTypeIDstr); tempcount++)
					{
						if(myhttp_rx_buffer[i] == LostTypeIDstr[tempcount])
						{
							i++;
						}
						else
						{
							tempcount = 0;
							i = pre_i;
							break;
						}
					}

					if(tempcount == sizeof(LostTypeIDstr))
					{
						i--;
						tempcount = 0;
						state = 21;
						break;
					}
				}
				if(myhttp_rx_buffer[i] == LostTypeCodestr[tempcount])
				{
					pre_i = i;
					i++;
					for(tempcount = 1; tempcount < sizeof(LostTypeCodestr); tempcount++)
					{
						if(myhttp_rx_buffer[i] == LostTypeCodestr[tempcount])
						{
							i++;
						}
						else
						{
							tempcount = 0;
							i = pre_i;
							break;
						}
					}

					if(tempcount == sizeof(LostTypeCodestr))
					{
						i--;
						tempcount = 0;
						state = 22;
						break;
					}
				}
				break;
			}
			case 21:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					pre_i = i;
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == next1str)
						{
							break;
						}
					}
					tbl_condition.id = atol((char*)&tempdata);
//					vTaskDelay(1);
					tempcount = 0;
					state = 20;
//					break;
				}
				break;
			}
			case 22:
			{
				if(myhttp_rx_buffer[i] == startstopstr)
				{
					pre_i = i;
					i++;
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < LEN_CONDITON; tempcount++)
					{
						tbl_condition.caption[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == startstopstr)
						{
							break;
						}
					}
					vTaskDelay(1);
					if(slot < 15)
					{
						WriteTable(&T_CONDITION, (T8U *) &tbl_condition, slot);
					}
					memset(&tbl_condition, '\0', sizeof(tbl_condition));
					slot++;
					vTaskDelay(1);
					tempcount = 0;
					state = 23;
//					break;
				}
				break;
			}
			case 23:
			{
				if(myhttp_rx_buffer[i] == next1str)
				{
					state = 20;
				}
				else if(myhttp_rx_buffer[i] == closestr)
				{
					state = 0;
				}
				break;
			}
			case 30:
			{
				if(myhttp_rx_buffer[i] == Titlestr[tempcount])
				{
					pre_i = i;
					i++;
					for(tempcount = 1; tempcount < sizeof(Titlestr); tempcount++)
					{
						if(myhttp_rx_buffer[i] == Titlestr[tempcount])
						{
							i++;
						}
						else
						{
							tempcount = 0;
							i = pre_i;
							break;
						}
					}

					if(tempcount == sizeof(Titlestr))
					{
						i--;
						tempcount = 0;
						state = 31;
						break;
					}
				}
				if(myhttp_rx_buffer[i] == SlotNostr[tempcount])
				{
					pre_i = i;
					i++;
					for(tempcount = 1; tempcount < sizeof(SlotNostr); tempcount++)
					{
						if(myhttp_rx_buffer[i] == SlotNostr[tempcount])
						{
							i++;
						}
						else
						{
							tempcount = 0;
							i = pre_i;
							break;
						}
					}

					if(tempcount == sizeof(SlotNostr))
					{
						i--;
						tempcount = 0;
						state = 32;
						break;
					}
				}
				else if(myhttp_rx_buffer[i] == closestr)
				{
					state = 0;

				}
				break;
			}
			case 31:
			{
				if(myhttp_rx_buffer[i] == startstopstr)
				{
					pre_i = i;
					i++;
					memset(tempdata, '\0', sizeof(tempdata));
					memset(tbl_udf.caption, '\0', sizeof(tbl_udf.caption));
//					ESP_LOGI(TAG, "t0:%s", tbl_udf.caption);
					for(tempcount = 0; tempcount < LEN_UDFCAPTION; tempcount++)
					{
						tbl_udf.caption[tempcount] = myhttp_rx_buffer[i++];

						if(myhttp_rx_buffer[i] == startstopstr)
						{
//							ESP_LOGI(TAG, "t:meet \"");
							break;
						}
					}
//					ESP_LOGI(TAG, "t1:%s", tbl_udf.caption);
					tempcount = 0;
					state = 30;
//					break;
				}
				break;
			}
			case 32:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					pre_i = i;
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == next1str)
						{
							break;
						}
					}
					slot = atol((char*)&tempdata);
					vTaskDelay(1);
					WriteTable(&T_UDF, (T8U *) &tbl_udf, slot);
//					ESP_LOGI(TAG, "t2:%s", tbl_udf.caption);
					memset(&tbl_udf, '\0', sizeof(tbl_udf));
					vTaskDelay(1);
					tempcount = 0;
					state = 33;
//					break;
				}
				break;
			}
			case 33:
			{
				if(myhttp_rx_buffer[i] == next1str)
				{
					state = 30;
				}
				else if(myhttp_rx_buffer[i] == closestr)
				{
					state = 0;
				}
				break;
			}
			case 40:
			{
				if(myhttp_rx_buffer[i] == ProcessStageCodestr[tempcount])
				{
					pre_i = i;
					i++;
					for(tempcount = 1; tempcount < sizeof(ProcessStageCodestr); tempcount++)
					{
						if(myhttp_rx_buffer[i] == ProcessStageCodestr[tempcount])
						{
							i++;
						}
						else
						{
							tempcount = 0;
							i = pre_i;
							break;
						}
					}

					if(tempcount == sizeof(ProcessStageCodestr))
					{
						i--;
						tempcount = 0;
						state = 41;
						break;
					}
				}
				else if(myhttp_rx_buffer[i] == closestr)
				{
					state = 0;

				}
				break;
			}
			case 41:
			{
//				if(myhttp_rx_buffer[i] == startstopstr)
//				{
//					pre_i = i;
//					i++;
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
//					{
//						tempdata[tempcount] = myhttp_rx_buffer[i++];
//						if(myhttp_rx_buffer[i] == next1str)
//						{
//							break;
//						}
//					}
//					sprintf((char*)&tbl_process_stage.prosesstageccode[count], "%s", tempdata);
////					ESP_LOGI(TAG, "tbl_basic_info2_temp.smv:%f", tbl_basic_info2_temp.smv);
//					tempcount = 0;
//					state = 42;
//					count++;
//				}

				if(myhttp_rx_buffer[i] == startstopstr)
				{
					pre_i = i;
					i++;
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < 10; tempcount++)
					{
						tbl_process_stage.prosesstageccode[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == startstopstr)
						{
							break;
						}
					}
					vTaskDelay(1);
					if(slot < 10)
					{
						WriteTable(&T_PROCESS_STAGE, (T8U *) &tbl_process_stage, slot);
					}
					memset(&tbl_process_stage, '\0', sizeof(tbl_process_stage));
					slot++;
					vTaskDelay(1);
					tempcount = 0;
					state = 42;
//					break;
				}
				break;
			}
			case 42:
			{
				if(myhttp_rx_buffer[i] == next1str)
				{
					state = 40;
				}
				else if(myhttp_rx_buffer[i] == closestr)
				{
					state = 0;
//					count=0;
				}
				break;
			}
			case 50:
			{
				if(myhttp_rx_buffer[i] == ModiReasonstr[tempcount])
				{
					pre_i = i;
					i++;
					for(tempcount = 1; tempcount < sizeof(ModiReasonstr); tempcount++)
					{
						if(myhttp_rx_buffer[i] == ModiReasonstr[tempcount])
						{
							i++;
						}
						else
						{
							tempcount = 0;
							i = pre_i;
							break;
						}
					}

					if(tempcount == sizeof(ModiReasonstr))
					{
						i--;
						tempcount = 0;
						state = 51;
						break;
					}
				}
				else if(myhttp_rx_buffer[i] == closestr)
				{
					state = 0;

				}
				break;
			}
			case 51:
			{
//				if(myhttp_rx_buffer[i] == startstopstr)
//				{
//					pre_i = i;
//					i++;
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
//					{
//						tempdata[tempcount] = myhttp_rx_buffer[i++];
//						if(myhttp_rx_buffer[i] == next1str)
//						{
//							break;
//						}
//					}
//					sprintf((char*)&tbl_modi_reason.modireason[count], "%s", tempdata);
//					tempcount = 0;
//					state = 52;
//					count++;
//				}

				if(myhttp_rx_buffer[i] == startstopstr)
				{
					pre_i = i;
					i++;
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < 10; tempcount++)
					{
						tbl_modi_reason.modireason[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == startstopstr)
						{
							break;
						}
					}
					vTaskDelay(1);
					if(slot < 10)
					{
						WriteTable(&T_MODI_REASON, (T8U *) &tbl_modi_reason, slot);
					}
					memset(&tbl_modi_reason, '\0', sizeof(tbl_modi_reason));
					slot++;
					vTaskDelay(1);
					tempcount = 0;
					state = 52;
//					break;
				}
				break;
			}
			case 52:
			{
				if(myhttp_rx_buffer[i] == next1str)
				{
					state = 50;
				}
				else if(myhttp_rx_buffer[i] == closestr)
				{
					state = 0;
				}
				break;
			}
			case 60:
			{
//				if(myhttp_rx_buffer[i] == ProdGroupIDstr[tempcount])
//				{
//					pre_i = i;
//					i++;
//					for(tempcount = 1; tempcount < sizeof(ProdGroupIDstr); tempcount++)
//					{
//						if(myhttp_rx_buffer[i] == ProdGroupIDstr[tempcount])
//						{
//							i++;
//						}
//						else
//						{
//							tempcount = 0;
//							i = pre_i;
//							break;
//						}
//					}
//
//					if(tempcount == sizeof(ProdGroupIDstr))
//					{
//						i--;
//						tempcount = 0;
//						state = 61;
//						break;
//					}
//				}
//				if(myhttp_rx_buffer[i] == ProdGroupNamestr[tempcount])
//				{
//					pre_i = i;
//					i++;
//					for(tempcount = 1; tempcount < sizeof(ProdGroupNamestr); tempcount++)
//					{
//						if(myhttp_rx_buffer[i] == ProdGroupNamestr[tempcount])
//						{
//							i++;
//						}
//						else
//						{
//							tempcount = 0;
//							i = pre_i;
//							break;
//						}
//					}
//
//					if(tempcount == sizeof(ProdGroupNamestr))
//					{
//						i--;
//						tempcount = 0;
//						state = 62;
//						break;
//					}
//				}
//				if(myhttp_rx_buffer[i] == closestr)
//				{
//					state = 0;
//				}

				if(checkstring(&i, (T8U*)&ProdGroupIDstr, sizeof(ProdGroupIDstr)) == _PASS)
				{
					state = 61;
				}
				if(checkstring(&i, (T8U*)&ProdGroupNamestr, sizeof(ProdGroupNamestr)) == _PASS)
				{
					state = 62;
				}
				break;
			}
			case 61:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					pre_i = i;
					memset(tempdata, '\0', sizeof(tempdata));
//					memset((char*)&tbl_prod_group, '\0', sizeof(tbl_prod_group));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == next1str)
						{
							break;
						}
					}
					tbl_prod_group_temp.prod_group_id = atol((char*)&tempdata);
					tempcount = 0;
					state = 60;
//					break;
					FLAG_STATE |= F_CHECKLINEPROD;
				}
				break;
			}
			case 62:
			{
				if(myhttp_rx_buffer[i] == startstopstr)
				{
					pre_i = i;
					i++;
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < LEN_PRODGRP; tempcount++)
					{
						tbl_prod_group_temp.group[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == startstopstr)
						{
							break;
						}
					}
					vTaskDelay(1);
					StrNCpy((T8U*)&tbl_prod_group, (T8U *) &tbl_prod_group_temp, sizeof(tbl_prod_group));
					WriteTable(&T_PROD_GROUP, (T8U *) &tbl_prod_group, 0);
					tempcount = 0;
					state = 0;
//					break;
				}
				break;
			}
			default:
			{
				state = 0;
				break;
			}
		}
		i++;
	}
}

void parseGetTime(void)
{
	T8U tempring[2] = {0};
	T8U j = 0;
	T8U tempring2[7] = {0};
	T8U tempring3[14] = {0};
//	ESP_LOGI(TAG, "%s", myhttp_rx_buffer);
	T16U i = 0;
	DATETIME temptime;
	while(i < len_return)
	{
		if((myhttp_rx_buffer[i] > 0x2F) && (myhttp_rx_buffer[i]) < 0x3A)
		{
			tempring3[j] = myhttp_rx_buffer[i];
			j++;
		}
		else
		{
			j = 0;
		}

		if(j == 14)
		{
			vTaskDelay(1);
			for(i = 0; i < 3; i++)
			{
				tempring[0] = tempring3[i*2+2];
				tempring[1] = tempring3[i*2+3];
				tempring2[i] = (T8U)atol((char*)&tempring);
//				ESP_LOGI(TAG, "%d", tempring2[i]);
			}
			for(; i < 6; i++)
			{
				tempring[0] = tempring3[i*2+2];
				tempring[1] = tempring3[i*2+3];
				tempring2[i+1] = (T8U)atol((char*)&tempring);
//				ESP_LOGI(TAG, "%d", tempring2[i+1]);
			}
			StrNCpy((T8U*)&temptime, (T8U*)&tempring2, sizeof(temptime));
//			ESP_LOGI(TAG, "%d %d %d : %d %d %d", temptime.year, temptime.month, temptime.day,
//					temptime.hour, temptime.minute, temptime.second);
			if((checkSysTime(&temptime, &currenttime) == _FAIL) || ((FLAG_STATE & F_TSYNCSERVER) == 0))
			{
				RTC_Set(&temptime);
				FLAG_STATE |= F_TSYNCSERVER;
			}
			break;
		}
		i++;
	}
}

void clearAssignment(void)
{
	TBL_MOOP_ASSIGN tbl_moop_assign;
	TBL_FIXOP_ASSIGN tbl_fixop_assign;
	memset((T8U *)&tbl_moop_assign,'\0',sizeof(tbl_moop_assign));
	for(int i = 0; i < 25; i++)
	{
		WriteTable(&T_MOOP_ASSIGN, (T8U *)&tbl_moop_assign, i);
	}
	vTaskDelay(1);
	memset((T8U *)&tbl_fixop_assign, '\0', sizeof(tbl_fixop_assign));
	for(int i = 0; i < 5; i++)
	{
		WriteTable(&T_FIXOP_ASSIGN, (T8U *)&tbl_fixop_assign, i);
	}
	vTaskDelay(1);
}

void parseAssignment(void)
{
	T16U i = 0;
	T8U state = 0;
	T8U loc = 0;
	T8U slot = 0;
//	T8U result = _FAIL;
	T32U tempID = 0;
	T8U tempdata[20] = {0};
	T8U tempcount = 0;
	T8U assignment = 0;
	TBL_MOOP_ASSIGN tbl_moop_assign;
	TBL_FIXOP_ASSIGN tbl_fixop_assign;
	memset(&tbl_moop_assign, '\0', sizeof(tbl_moop_assign));
	memset(&tbl_fixop_assign, '\0', sizeof(tbl_fixop_assign));
	while(i < len_return)
	{
		vTaskDelay(1 / portTICK_PERIOD_MS);
		switch(state)
		{
			case 0:
			{
				if(ID_loc >= 10)return;
				if(checkstring(&i, (T8U*)&IDstr, sizeof(IDstr)) == _PASS)
				{
					state = 10;
				}
				if(checkstring(&i, (T8U*)&AssignmentNamestr, sizeof(AssignmentNamestr)) == _PASS)
				{
					state = 20;
				}
				if(checkstring(&i, (T8U*)&MOstr, sizeof(MOstr)) == _PASS)
				{
					if((assignment == ASSIGN_MOOP) || (assignment == DELETE_MOOP))
					{
						state = 30;
					}
					else
					{
						break;
					}
				}
				if(checkstring(&i, (T8U*)&SlotNostr, sizeof(SlotNostr)) == _PASS)
				{
					state = 40;
				}
				if(checkstring(&i, (T8U*)&OPstr, sizeof(OPstr)) == _PASS)
				{
					if((assignment == ASSIGN_MOOP) || (assignment == ASSIGN_FIXOP))
					{
						state = 50;
					}
					else
					{
						break;
					}
				}
//				if(checkstring(&i, (T8U*)&SMVstr, sizeof(SMVstr)) == _PASS)
//				{
////					state = 60;
//					break;//temporary no process
//				}
				if(checkstring(&i, (T8U*)&OpRatiostr, sizeof(OpRatiostr)) == _PASS)
				{
					if(assignment == ASSIGN_MOOP)// || (assignment == ASSIGN_FIXOP))
					{
						state = 70;
					}
					else
					{
						break;
					}
				}
				if(checkstring(&i, (T8U*)&PreviousOp, sizeof(PreviousOp)) == _PASS)
				{
					if((assignment == ASSIGN_MOOP) || (assignment == ASSIGN_FIXOP))
					{
						state = 80;
					}
					else
					{
						break;
					}
				}
				if(checkstring(&i, (T8U*)&IsGroupOp, sizeof(IsGroupOp)) == _PASS)
				{
					state = 90;
				}
				tempcount = 0;
				break;
			}
			case 10:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
//					pre_i = i;
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == next1str)
						{
							break;
						}
					}
					tempID = atol((char*)&tempdata);
//					vTaskDelay(1);
//					WriteTable(&T_UDF, (T8U *) &tbl_udf, loc);
//					vTaskDelay(1);
					tempcount = 0;
					state = 0;
				}
				break;
			}
			case 20:
			{
				if(checkstring(&i, (T8U*)&MoOpAssignmentstr, sizeof(MoOpAssignmentstr)) == _PASS)
				{
					assignment = ASSIGN_MOOP;
					state = 0;
				}
				if(checkstring(&i, (T8U*)&MoOpAssignmentBySequenceStr, sizeof(MoOpAssignmentBySequenceStr)) == _PASS)
				{
					assignment = ASSIGN_MOOP;
					state = 0;
				}
				if(checkstring(&i, (T8U*)&FixOpAssignmentstr, sizeof(FixOpAssignmentstr)) == _PASS)
				{
					assignment = ASSIGN_FIXOP;
					state = 0;
				}
				if(checkstring(&i, (T8U*)&DeleteMoOpAssignmentstr, sizeof(DeleteMoOpAssignmentstr)) == _PASS)
				{
					assignment = DELETE_MOOP;
					state = 0;
				}
				if(checkstring(&i, (T8U*)&DeleteFixOpOneAssignmentstr, sizeof(DeleteFixOpOneAssignmentstr)) == _PASS)
				{
					assignment = DELETE_FIXOP;
					state = 0;
				}
				break;
			}
			case 30:
			{
				if(myhttp_rx_buffer[i] == startstopstr)
				{
					i++;
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < LEN_MO; tempcount++)
					{
						tbl_moop_assign.MO[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == startstopstr)
						{
							break;
						}
					}
					for(tempcount += 1; tempcount < LEN_MO; tempcount++)
					{
						tbl_moop_assign.MO[tempcount] = 0x20;
					}
					tempcount = 0;
					state = 0;
				}
				break;
			}
			case 40:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == next1str)
						{
							break;
						}
					}
					slot = atol((char*)&tempdata);
					tempcount = 0;
					state = 0;
				}
				break;
			}
			case 50:
			{
				loc = 0;
//				if(myhttp_rx_buffer[i] == startstopstr)
//				{
//					tempcount++;
//					if(tempcount == 2)
//					{
//						assignment = 0;
//						state = 0;
//						break;
//					}
//				}
//				else if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
//				{
					for(;i < len_return;)
					{
//						if(myhttp_rx_buffer[i] == startstopstr)
//						{
//							tempcount++;
//							if(tempcount == 2)
//							{
//								assignment = 0;
////								state = 0;
//								break;
//							}
//						}
//						else
						if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
						{
							memset(tempdata, '\0', sizeof(tempdata));
							for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
							{
								tempdata[tempcount] = myhttp_rx_buffer[i++];
	//							if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == startstopstr))
								if(!((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9')))
								{
									break;
								}
							}
							if(assignment == ASSIGN_MOOP)
							{
								tbl_moop_assign.OP[loc] = (T16U)atol((char*)&tempdata);
							}
							else if(assignment == ASSIGN_FIXOP)
							{
								tbl_fixop_assign.op[loc] = (T16U)atol((char*)&tempdata);
							}
							loc++;
							tempcount = 0;
							if((myhttp_rx_buffer[i] == startstopstr) ||
							  ((assignment == ASSIGN_MOOP)&&(loc>=MAX_OPCOUNT_5)) ||
							  ((assignment == ASSIGN_FIXOP)&&(loc>=MAX_OPCOUNT_20)))
							{
								break;
							}
						}
						else
						{
							if(myhttp_rx_buffer[i] == startstopstr)
							{
								tempcount++;
								if(tempcount >= 2)
								{
									assignment = 0;
	//								state = 0;
									break;
								}
							}
							i++;
						}
					}
					state = 0;
//				}
				break;
			}
			case 60:
			{
				state = 0;
				break;
			}
			case 70:
			{
				loc = 0;
//				if(myhttp_rx_buffer[i] == startstopstr)
//				{
//					tempcount++;
//					if(tempcount == 2)
//					{
//						state = 0;
//						break;
//					}
//				}
//				else if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
//				{
					for(;i < len_return;)
					{
						if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
						{
							memset(tempdata, '\0', sizeof(tempdata));
							for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
							{
								tempdata[tempcount] = myhttp_rx_buffer[i++];
	//							if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == startstopstr))
								if(!((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9')))
								{
									break;
								}
							}
	//						if(assignment == ASSIGN_MOOP)
	//						{
								tbl_moop_assign.ratio[loc] = (T16U)atol((char*)&tempdata);
//								ESP_LOGI(TAG, "ratio%u: %.5s\n", loc, tempdata);
	//						}
	//						else if(assignment == ASSIGN_FIXOP)
	//						{
	//							tbl_fixop_assign.[loc] = (T16U)atol((char*)&tempdata);
	//						}
							loc++;
							tempcount = 0;
							if((myhttp_rx_buffer[i] == startstopstr) ||
							  ((assignment == ASSIGN_MOOP)&&(loc>=MAX_OPCOUNT_5)))// ||
	//						  ((assignment == ASSIGN_FIXOP)&&(loc>=MAX_OPCOUNT_20)))
							{
								break;
							}
						}
						else
						{
							if(myhttp_rx_buffer[i] == startstopstr)
							{
								tempcount++;
								if(tempcount >= 2)
								{
	//								state = 0;
									break;
								}
							}
							i++;
						}
					}
					state = 0;
//				}
				break;
			}
			case 80:
			{
				loc = 0;
//
//				else if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
//				{
					for(;i < len_return;)
					{
//						if(myhttp_rx_buffer[i] == startstopstr)
//						{
//							tempcount++;
//							if(tempcount >= 2)
//							{
////								state = 0;
//								break;
//							}
//						}
//						else
						if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
						{
							memset(tempdata, '\0', sizeof(tempdata));
							for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
							{
								tempdata[tempcount] = myhttp_rx_buffer[i++];
	//							if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == startstopstr))
								if(!((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9')))
								{
									break;
								}
							}
							if(assignment == ASSIGN_MOOP)
							{
								tbl_moop_assign.previousOP[loc] = (T16U)atol((char*)&tempdata);
//								ESP_LOGI(TAG, "previousOP: %u", tbl_moop_assign.previousOP[loc]);
							}
							else if(assignment == ASSIGN_FIXOP)
							{
								tbl_fixop_assign.previous_op = (T16U)atol((char*)&tempdata);
							}
							loc++;
							tempcount = 0;
							if((myhttp_rx_buffer[i] == startstopstr) ||
							  ((assignment == ASSIGN_MOOP)&&(loc>=MAX_OPCOUNT_5)) ||
							  ((assignment == ASSIGN_FIXOP)&&(loc>=1)))//fixop only one previous op, unless want to add group op with each op have prev op
							{
								break;
							}
						}
						else
						{
							if(myhttp_rx_buffer[i] == startstopstr)
							{
								tempcount++;
								if(tempcount >= 2)
								{
	//								state = 0;
									break;
								}
							}
							i++;
						}
					}
					state = 0;
//				}
				break;
			}
			case 90:
			{
				if(checkstring(&i, (T8U*)&truestr, sizeof(truestr)) == _PASS)
				{
					if(assignment == ASSIGN_FIXOP)
					{
						tbl_fixop_assign.isgroupop = true;
					}
					state = 100;
				}
				if(checkstring(&i, (T8U*)&falsestr, sizeof(falsestr)) == _PASS)
				{
					if(assignment == ASSIGN_FIXOP)
					{
						tbl_fixop_assign.isgroupop = false;
					}
					state = 100;
				}
				break;
			}
			case 100:
			{
				if((tempID == 0) && (TOP_Message & TOPDATA_GETASSIGNMENT))
				{
					memset(&tbl_moop_assign, '\0', sizeof(tbl_moop_assign));
					memset(&tbl_fixop_assign, '\0', sizeof(tbl_fixop_assign));
				}
				else if(assignment == ASSIGN_MOOP)
				{
					add_MO(tbl_moop_assign, tempdata);
//					WriteTable(&T_MOOP_ASSIGN, (T8U *) &tbl_moop_assign, slot);
//					ESP_LOGI(TAG, "%.10s\n%u\n%u\n%u\n%u\n%u\n", tbl_moop_assign.MO,
//							tbl_moop_assign.OP[0],tbl_moop_assign.OP[1],tbl_moop_assign.OP[2],tbl_moop_assign.OP[3],
//							tbl_moop_assign.OP[4]);
//					ESP_LOGI(TAG, "\n%u\n%u\n%u\n%u\n%u\n",
//							tbl_moop_assign.ratio[0],tbl_moop_assign.ratio[1],tbl_moop_assign.ratio[2],
//							tbl_moop_assign.ratio[3],tbl_moop_assign.ratio[4]);
//					ESP_LOGI(TAG, "\n%u\n%u\n%u\n%u\n%u\n",
//							tbl_moop_assign.previousOP[0],tbl_moop_assign.previousOP[1],tbl_moop_assign.previousOP[2],
//							tbl_moop_assign.previousOP[3],tbl_moop_assign.previousOP[4]);
					memset(&tbl_moop_assign, '\0', sizeof(tbl_moop_assign));
				}
				else if(assignment == ASSIGN_FIXOP)
				{
					WriteTable(&T_FIXOP_ASSIGN, (T8U *) &tbl_fixop_assign, slot-1);
					memset(&tbl_fixop_assign, '\0', sizeof(tbl_fixop_assign));
				}
				else if(assignment == DELETE_MOOP)
				{
					del_MO(tbl_moop_assign.MO);
					memset(&tbl_moop_assign, '\0', sizeof(tbl_moop_assign));
				}
				else if(assignment == DELETE_FIXOP)
				{
					memset(&tbl_fixop_assign, '\0', sizeof(tbl_fixop_assign));
					WriteTable(&T_FIXOP_ASSIGN, (T8U *) &tbl_fixop_assign, slot-1);
				}


				assignment = 0;
				if(tempID != 0)ID_assignment[ID_loc++] = tempID;
				tempID = 0;
				slot = 0;
				state = 0;

//				if(myhttp_rx_buffer[i] == next1str)
//				{
//					state = 0;
//				}
//				else if(myhttp_rx_buffer[i] == closestr)
//				{
//					state = 0;
//				}
				break;
			}
			default:
			{
				state = 0;
				break;
			}
		}
		i++;
	}
}

void parseMachineInfo(void)
{
	T16U i = 0;
	T8U state = 0;
	T8U tempcount = 0;

	TBL_MACHINE_INFO tbl_machine_info;
	memset(&tbl_machine_info, '\0', sizeof(tbl_machine_info));

	while(i < len_return)
	{
		switch(state)
		{
			case 0:
			{
				if(checkstring(&i, (T8U*)&MachineTypestr, sizeof(MachineTypestr)) == _PASS)
				{
					state = 10;
				}
				break;
			}
			case 10:
			{
				if(myhttp_rx_buffer[i] == startstopstr)
				{
					i++;
					if(myhttp_rx_buffer[i] == startstopstr)
					{
						WriteTable(&T_MACHINE_INFO, (T8U *) &tbl_machine_info, 0);
						tempcount = 0;
						state = 0;
						break;
					}
					for(tempcount = 0; tempcount < LEN_MACHINETYPE; tempcount++)
					{
						tbl_machine_info.machineType[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == startstopstr)
						{
							WriteTable(&T_MACHINE_INFO, (T8U *) &tbl_machine_info, 0);
							break;
						}
					}
					tempcount = 0;
					state = 0;
				}
				break;
			}
			default:
			{
				state = 0;
				break;
			}
		}
		i++;
	}
}

void parseBasicInfo(void)
{
	T8U OpNamestr[7] = "OpName\"";
	T8U TargetQtystr[10] = "TargetQty\"";
	T8U HourTargetstr[11] = "HourTarget\"";
	T8U SONOstr[5] = "SONO\"";
	T8U StyleNamestr[10] ="StyleName\"";
//	T8U ProdGroupstr[10] = "ProdGroup\"";
	T8U WorkMinutestr[11] = "WorkMinute\"";

	T16U i = 0;
	T8U state = 0;
	T8U tempdata[20] = {0};
	T8U tempcount = 0;

	TBL_BASIC_INFO1 tbl_basic_info1_temp;
	TBL_BASIC_INFO2 tbl_basic_info2_temp;
//	TBL_GROUP tbl_group;

	ReadTable(&T_BASIC_INFO1, (T8U *) &tbl_basic_info1_temp, 0);
	ReadTable(&T_BASIC_INFO2, (T8U *) &tbl_basic_info2_temp, 0);

	memset(&tbl_basic_info1_temp.soNo, '\0', sizeof(tbl_basic_info1_temp.soNo));
	memset(&tbl_basic_info1_temp.styleName, '\0', sizeof(tbl_basic_info1_temp.styleName));

	memset(&tbl_basic_info2_temp.opName, '\0', sizeof(tbl_basic_info2_temp.opName));
	memset(&tbl_basic_info2_temp.smv, '\0', sizeof(tbl_basic_info2_temp.smv));
	tbl_basic_info2_temp.TargetQty = 0;
	tbl_basic_info2_temp.hourlyTarget = 0;

//	memset(&tbl_group, '\0', sizeof(tbl_group));

//	sprintf((char*)&myhttp_rx_buffer, 	"HTTP/1.1 200 OK"
//										"Cache-Control: private"
//										"Content-Type: application/json; charset=utf-8"
//										"Server: Microsoft-IIS/10.0"
//										"X-AspNet-Version: 4.0.30319"
//										"X-Powered-By: ASP.NET"
//										"Date: Tue, 24 May 2022 05:36:07 GMT"
//										"Connection: close"
//										"Content-Length: 290"
//										"{"
//										  "\"Table\": ["
//											"{"
//											  "\"OpName\": \"Shoulder Join With Match\","
//											  "\"SMV\": 0.850000,"
//											  "\"TargetQty\": 700.0,"
//											  "\"HourTarget\": 70,"
//											  "\"SONO\": \"GTL-1118 Test File   \","
//											  "\"StyleName\": \"TEST STYLE\","
//											  "\"ProdGroup\": \"Sew403    \","
//											  "\"WorkMinute\": 480.0"
//											"}"
//										  "]"
//										"}");
//	ESP_LOGI(TAG, "%s", myhttp_rx_buffer);
//	len_return = 1000;
	while(i < len_return)
	{
		switch(state)
		{
			case 0:
			{
				if(checkstring(&i, (T8U*)&OpNamestr, sizeof(OpNamestr)) == _PASS)
				{
					state = 10;
//					ESP_LOGI(TAG, "here 3");
				}
				if(checkstring(&i, (T8U*)&SMVstr, sizeof(SMVstr)) == _PASS)
				{
					state = 20;
//					ESP_LOGI(TAG, "here 4");
				}
				if(checkstring(&i, (T8U*)&TargetQtystr, sizeof(TargetQtystr)) == _PASS)
				{
					state = 30;
//					ESP_LOGI(TAG, "here 5");
				}
				if(checkstring(&i, (T8U*)&HourTargetstr, sizeof(HourTargetstr)) == _PASS)
				{
					state = 40;
//					ESP_LOGI(TAG, "here 6");
				}
				if(checkstring(&i, (T8U*)&SONOstr, sizeof(SONOstr)) == _PASS)
				{
					state = 50;
//					ESP_LOGI(TAG, "here 7");
				}
				if(checkstring(&i, (T8U*)&StyleNamestr, sizeof(StyleNamestr)) == _PASS)
				{
					state = 60;
//					ESP_LOGI(TAG, "here 8");
				}
//				if(checkstring(&i, (T8U*)&ProdGroupstr, sizeof(ProdGroupstr)) == _PASS)
//				{
////					ESP_LOGE(TAG, "here");
//					state = 70;
//				}
				if(checkstring(&i, (T8U*)&WorkMinutestr, sizeof(WorkMinutestr)) == _PASS)
				{
//					ESP_LOGE(TAG, "here");
					state = 80;
				}
				tempcount = 0;
				break;
			}
			case 10:
			{
				if(myhttp_rx_buffer[i] == startstopstr)
				{
					i++;
					if(myhttp_rx_buffer[i] == startstopstr)
					{
						tempcount = 0;
						state = 0;
//						ESP_LOGI(TAG, "test");
						break;
					}
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < LEN_OPNAME; tempcount++)
					{
						tbl_basic_info2_temp.opName[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == startstopstr)
						{
//							ESP_LOGI(TAG, "test 1");
							break;
						}
					}
					tempcount = 0;
					state = 0;
//					ESP_LOGI(TAG, "test 2");
				}
				break;
			}
//			case 20:
//			{
//				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
//				{
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
//					{
//						tempdata[tempcount] = myhttp_rx_buffer[i++];
//						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
//						{
//							break;
//						}
//					}
//					tbl_basic_info2_temp.smv[0] = atol((char*)&tempdata);
//					tempcount = 0;
//					state = 21;
//				}
//				break;
//			}
//			case 21:
//			{
//				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
//				{
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
//					{
//						tempdata[tempcount] = myhttp_rx_buffer[i++];
//						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
//						{
//							break;
//						}
//					}
//					tbl_basic_info2_temp.smv[1] = atol((char*)&tempdata);
//					tempcount = 0;
//					state = 0;
//				}
//				break;
//			}
			case 20:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == next1str)
						{
							break;
						}
					}
					tbl_basic_info2_temp.smv = atof((char*)&tempdata);
//					ESP_LOGI(TAG, "tbl_basic_info2_temp.smv:%f", tbl_basic_info2_temp.smv);

					tempcount = 0;
					state = 0;
				}
				break;
			}
			case 30:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
						{
							break;
						}
					}
					tbl_basic_info2_temp.TargetQty = atol((char*)&tempdata);
					tempcount = 0;
					state = 0;
				}
				break;
			}
			case 40:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == next1str)
						{
							break;
						}
					}
					tbl_basic_info2_temp.hourlyTarget = atol((char*)&tempdata);
					tempcount = 0;
					state = 0;
				}
				break;
			}
			case 50:
			{
				if(myhttp_rx_buffer[i] == startstopstr)
				{
					i++;
					if(myhttp_rx_buffer[i] == startstopstr)
					{
						tempcount = 0;
						state = 0;
						break;
					}
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < LEN_SO; tempcount++)
					{
						tbl_basic_info1_temp.soNo[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == startstopstr)
						{
							break;
						}
					}
					for(tempcount += 1; tempcount < LEN_SO; tempcount++)
					{
						tbl_basic_info1_temp.soNo[tempcount] = 0x20;
					}
					tempcount = 0;
					state = 0;
					ESP_LOGI(TAG, "here 2");
				}
				break;
			}
			case 60:
			{
				if(myhttp_rx_buffer[i] == startstopstr)
				{
					i++;
					if(myhttp_rx_buffer[i] == startstopstr)
					{
//						ESP_LOGI(TAG, "here 1");
						StrNCpy((T8U *) &tbl_basic_info1, (T8U *) &tbl_basic_info1_temp, sizeof(tbl_basic_info1));
						StrNCpy((T8U *) &tbl_basic_info2, (T8U *) &tbl_basic_info2_temp, sizeof(tbl_basic_info2));
						WriteTable(&T_BASIC_INFO1, (T8U *) &tbl_basic_info1, 0);
						WriteTable(&T_BASIC_INFO2, (T8U *) &tbl_basic_info2, 0);
						tempcount = 0;
						state = 0;
						break;
					}
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < LEN_STYLENAME; tempcount++)
					{
						tbl_basic_info1_temp.styleName[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == startstopstr)
						{
//							ESP_LOGI(TAG, "tbl_basic_info1_temp.styleName: %s", tbl_basic_info1_temp.styleName);
							break;
						}
					}
					StrNCpy((T8U *) &tbl_basic_info1, (T8U *) &tbl_basic_info1_temp, sizeof(tbl_basic_info1));
					StrNCpy((T8U *) &tbl_basic_info2, (T8U *) &tbl_basic_info2_temp, sizeof(tbl_basic_info2));
					WriteTable(&T_BASIC_INFO1, (T8U *) &tbl_basic_info1, 0);
					WriteTable(&T_BASIC_INFO2, (T8U *) &tbl_basic_info2, 0);
					tempcount = 0;
					state = 0;
				}
				break;
			}
//			case 70:
//			{
//				if(myhttp_rx_buffer[i] == startstopstr)
//				{
//					i++;
//					if(myhttp_rx_buffer[i] == startstopstr)
//					{
//						tempcount = 0;
//						state = 0;
//						break;
//					}
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < LEN_PRODGRP; tempcount++)
//					{
//						tbl_group.group[tempcount] = myhttp_rx_buffer[i++];
//						if(myhttp_rx_buffer[i] == startstopstr)
//						{
//							break;
//						}
//					}
//					for(tempcount += 1; tempcount < LEN_PRODGRP; tempcount++)
//					{
//						tbl_group.group[tempcount] = 0x20;
//					}
////					ESP_LOGE(TAG, "here 1 %s", tbl_group.group);
//					WriteTable(&T_GROUP, (T8U *) &tbl_group, 0);
//					tempcount = 0;
//					state = 0;
//				}
//				break;
//			}
			case 80:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
						{
							break;
						}
					}
					tbl_basic_info2_temp.WorkMinuteSec = atol((char*)&tempdata) * 60;
//					ESP_LOGE(TAG, "WorkMinuteMillSec %u", tbl_basic_info2_temp.WorkMinuteSec);
					StrNCpy((T8U *) &tbl_basic_info1, (T8U *) &tbl_basic_info1_temp, sizeof(tbl_basic_info1));
					StrNCpy((T8U *) &tbl_basic_info2, (T8U *) &tbl_basic_info2_temp, sizeof(tbl_basic_info2));
					WriteTable(&T_BASIC_INFO1, (T8U *) &tbl_basic_info1, 0);
					WriteTable(&T_BASIC_INFO2, (T8U *) &tbl_basic_info2, 0);
					tempcount = 0;
					state = 0;
					return;
				}
				break;
			}
			default:
			{
				state = 0;
				break;
			}
		}
		i++;
	}
}

void parseBasicInfobyGroup(void){
	T8U OpNo[5] = "OpNo\"";
	T8U OpNamestr[7] = "OpName\"";
	T8U TargetQtystr[10] = "TargetQty\"";
	T8U HourTargetstr[11] = "HourTarget\"";
	T8U SONOstr[5] = "SONO\"";
	T8U StyleNamestr[10] ="StyleName\"";
	T8U ProdGroupstr[10] = "ProdGroup\"";
	T8U WorkMinutestr[11] = "WorkMinute\"";

	T16U i = 0;
	T8U state = 0;
	T8U tempdata[20] = {0};
	T8U tempcount = 0;
	T8U count=0;
	T8U camefrom=0;

	TBL_BASIC_INFO1 tbl_basic_info1_temp;
	TBL_BASIC_INFO2 tbl_basic_info2_temp;
	TBL_BASIC_INFO3 tbl_basic_info3_temp;
	TBL_GROUP tbl_group;

	ReadTable(&T_BASIC_INFO1, (T8U *) &tbl_basic_info1_temp, 0);
	ReadTable(&T_BASIC_INFO2, (T8U *) &tbl_basic_info2_temp, 0);
	ReadTable(&T_BASIC_INFO3, (T8U *) &tbl_basic_info3_temp, 0);

	memset(&tbl_basic_info1_temp.soNo, '\0', sizeof(tbl_basic_info1_temp.soNo));
	memset(&tbl_basic_info1_temp.styleName, '\0', sizeof(tbl_basic_info1_temp.styleName));
	memset(&tbl_basic_info2_temp.opName, '\0', sizeof(tbl_basic_info2_temp.opName));
	memset(&tbl_basic_info3_temp.smv, '\0', sizeof(tbl_basic_info3_temp.smv));
	memset(&tbl_basic_info3_temp.opNo, '\0', sizeof(tbl_basic_info3_temp.opNo));
	tbl_basic_info2_temp.TargetQty = 0;
	tbl_basic_info2_temp.hourlyTarget = 0;

	memset(&tbl_group, '\0', sizeof(tbl_group));

//	sprintf((char*)&myhttp_rx_buffer, 	"HTTP/1.1 200 OK"
//										"Cache-Control: private"
//										"Content-Type: application/json; charset=utf-8"
//										"Server: Microsoft-IIS/10.0"
//										"X-AspNet-Version: 4.0.30319"
//										"X-Powered-By: ASP.NET"
//										"Date: Tue, 24 May 2022 05:36:07 GMT"
//										"Connection: close"
//										"Content-Length: 290"
//										"{"
//											  "\"Table\": ["
//												"{"
//												  "\"OpNo\": 846,"
//												  "\"OpName\": null,"
//												  "\"SMV\": null,"
//												  "\"TargetQty\": 750.0,"
//												  "\"HourTarget\": 75.0,"
//												  "\"SONO\": \"CIPL-03824          \","
//												  "\"StyleName\": \"M S FLANNEL PJ SET (TOP)\","
//												  "\"ProdGroup\": \"Sew212    \","
//												  "\"WorkMinute\": 600.0"
//												"},"
//												"{"
//												  "\"OpNo\": 847,"
//												  "\"OpName\": null,"
//												  "\"SMV\": null,"
//												  "\"TargetQty\": 750.0,"
//												  "\"HourTarget\": 75.0,"
//												  "\"SONO\": \"CIPL-03824          \","
//												  "\"StyleName\": \"M S FLANNEL PJ SET (TOP)\","
//												  "\"ProdGroup\": \"Sew212    \","
//												  "\"WorkMinute\": 600.0"
//												"}"
//											  "]"
//										"}");
//	ESP_LOGI(TAG, "%s", myhttp_rx_buffer);
//	len_return = 1000;
	while(i < len_return)
	{
		switch(state)
		{
			case 0:
			{
				if(checkstring(&i, (T8U*)&OpNo, sizeof(OpNo)) == _PASS)
				{
					state = 9;
//					ESP_LOGI(TAG, "Op No");
				}
				if(checkstring(&i, (T8U*)&OpNamestr, sizeof(OpNamestr)) == _PASS)
				{
					state = 10;
//					ESP_LOGI(TAG, "here OpNamestr");
				}
				if(checkstring(&i, (T8U*)&SMVstr, sizeof(SMVstr)) == _PASS)
				{
					state = 20;
//					ESP_LOGI(TAG, "here SMVstr");
				}
				if(checkstring(&i, (T8U*)&TargetQtystr, sizeof(TargetQtystr)) == _PASS)
				{
					state = 30;
//					ESP_LOGI(TAG, "here TargetQtystr");
				}
				if(checkstring(&i, (T8U*)&HourTargetstr, sizeof(HourTargetstr)) == _PASS)
				{
					state = 40;
//					ESP_LOGI(TAG, "here HourTargetstr");
				}
				if(checkstring(&i, (T8U*)&SONOstr, sizeof(SONOstr)) == _PASS)
				{
					state = 50;
//					ESP_LOGI(TAG, "here SONOstr");
				}
				if(checkstring(&i, (T8U*)&StyleNamestr, sizeof(StyleNamestr)) == _PASS)
				{
					state = 60;
//					ESP_LOGI(TAG, "here StyleNamestr");
				}
				if(checkstring(&i, (T8U*)&ProdGroupstr, sizeof(ProdGroupstr)) == _PASS)
				{
//					ESP_LOGE(TAG, "here ProdGroupstr");
					state = 70;
				}
				if(checkstring(&i, (T8U*)&WorkMinutestr, sizeof(WorkMinutestr)) == _PASS)
				{
//					ESP_LOGE(TAG, "here WorkMinutestr");
					state = 80;
				}
				tempcount = 0;
				break;
			}
			case 8:
			{
				if(checkstring(&i, (T8U*)&OpNo, sizeof(OpNo)) == _PASS)
				{
					state = 9;
//					ESP_LOGI(TAG, "here OpNo2");
				}
				if(checkstring(&i, (T8U*)&SMVstr, sizeof(SMVstr)) == _PASS)
				{
					state = 20;
//					ESP_LOGI(TAG, "here SMVstr2");
				}
				else if(myhttp_rx_buffer[i] == closestr)
				{
					StrNCpy((T8U *) &tbl_basic_info3, (T8U *) &tbl_basic_info3_temp, sizeof(tbl_basic_info3));
					WriteTable(&T_BASIC_INFO3, (T8U *) &tbl_basic_info3, 0);
	//				ESP_LOGI(TAG, "tbl_basic_info3_temp.smv:%f", tbl_basic_info3_temp.smv);
					camefrom = 0;
					count=0;
					state = 0;

				}
				tempcount = 0;
				break;
			}
			case 9:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == next1str)
						{
							break;
						}
					}
					tbl_basic_info3_temp.opNo[count] = atoi((char*)&tempdata);
//					ESP_LOGI(TAG, "tbl_basic_info2_temp.smv:%f", tbl_basic_info2_temp.smv);

					tempcount = 0;
					if(camefrom==100)
					{
						state=8;
					}
					else
					{
						state = 0;
					}
				}
				break;
			}
			case 10:
			{
				if(myhttp_rx_buffer[i] == next1str)
				{
					tempcount = 0;
					state = 0;
					break;
				}
				else if(myhttp_rx_buffer[i] == startstopstr)
				{
					i++;
					if(myhttp_rx_buffer[i] == startstopstr)
					{
						tempcount = 0;
						state = 0;
//						ESP_LOGI(TAG, "test");
						break;
					}
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < LEN_OPNAME; tempcount++)
					{
						tbl_basic_info2_temp.opName[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == startstopstr)
						{
//							ESP_LOGI(TAG, "test 1");
							break;
						}
					}
					tempcount = 0;
					state = 0;
//					ESP_LOGI(TAG, "test 2");
				}
				break;
			}
//			case 20:
//			{
//				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
//				{
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
//					{
//						tempdata[tempcount] = myhttp_rx_buffer[i++];
//						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
//						{
//							break;
//						}
//					}
//					tbl_basic_info2_temp.smv[0] = atol((char*)&tempdata);
//					tempcount = 0;
//					state = 21;
//				}
//				break;
//			}
//			case 21:
//			{
//				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
//				{
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
//					{
//						tempdata[tempcount] = myhttp_rx_buffer[i++];
//						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
//						{
//							break;
//						}
//					}
//					tbl_basic_info2_temp.smv[1] = atol((char*)&tempdata);
//					tempcount = 0;
//					state = 0;
//				}
//				break;
//			}
			case 20:
			{
				if(myhttp_rx_buffer[i] == next1str)
				{
					tempcount = 0;
					state = 0;
					break;
				}
				else if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == next1str)
						{
							break;
						}
					}
					tbl_basic_info3_temp.smv[count] = atof((char*)&tempdata);
//					ESP_LOGI(TAG, "tbl_basic_info3_temp.smv:%f", tbl_basic_info3_temp.smv);

					tempcount = 0;
					if(camefrom==100)
					{
						state=90;
					}
					else
					{
						state = 0;
					}
				}
				break;
			}
			case 30:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
						{
							break;
						}
					}
					tbl_basic_info2_temp.TargetQty = atol((char*)&tempdata);
					tempcount = 0;
					state = 0;
				}
				break;
			}
			case 40:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == next1str)
						{
							break;
						}
					}
					tbl_basic_info2_temp.hourlyTarget = atol((char*)&tempdata);
					tempcount = 0;
					state = 0;
				}
				break;
			}
			case 50:
			{
				if(myhttp_rx_buffer[i] == startstopstr)
				{
					i++;
					if(myhttp_rx_buffer[i] == startstopstr)
					{
						tempcount = 0;
						state = 0;
						break;
					}
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < LEN_SO; tempcount++)
					{
						tbl_basic_info1_temp.soNo[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == startstopstr)
						{
							break;
						}
					}
					for(tempcount += 1; tempcount < LEN_SO; tempcount++)
					{
						tbl_basic_info1_temp.soNo[tempcount] = 0x20;
					}
					tempcount = 0;
					state = 0;
				}
				break;
			}
			case 60:
			{
				if(myhttp_rx_buffer[i] == startstopstr)
				{
					i++;
					if(myhttp_rx_buffer[i] == startstopstr)
					{
//						ESP_LOGI(TAG, "here 1");
						StrNCpy((T8U *) &tbl_basic_info1, (T8U *) &tbl_basic_info1_temp, sizeof(tbl_basic_info1));
						StrNCpy((T8U *) &tbl_basic_info2, (T8U *) &tbl_basic_info2_temp, sizeof(tbl_basic_info2));
						WriteTable(&T_BASIC_INFO1, (T8U *) &tbl_basic_info1, 0);
						WriteTable(&T_BASIC_INFO2, (T8U *) &tbl_basic_info2, 0);
						tempcount = 0;
						state = 0;
						break;
					}
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < LEN_STYLENAME; tempcount++)
					{
						tbl_basic_info1_temp.styleName[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == startstopstr)
						{
//							ESP_LOGI(TAG, "tbl_basic_info1_temp.styleName: %s", tbl_basic_info1_temp.styleName);
							break;
						}
					}
					StrNCpy((T8U *) &tbl_basic_info1, (T8U *) &tbl_basic_info1_temp, sizeof(tbl_basic_info1));
					StrNCpy((T8U *) &tbl_basic_info2, (T8U *) &tbl_basic_info2_temp, sizeof(tbl_basic_info2));
					WriteTable(&T_BASIC_INFO1, (T8U *) &tbl_basic_info1, 0);
					WriteTable(&T_BASIC_INFO2, (T8U *) &tbl_basic_info2, 0);
					tempcount = 0;
					state = 0;
				}
				break;
			}
			case 70:
			{
				if(myhttp_rx_buffer[i] == startstopstr)
				{
					i++;
					if(myhttp_rx_buffer[i] == startstopstr)
					{
						tempcount = 0;
						state = 0;
						break;
					}
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < LEN_PRODGRP; tempcount++)
					{
						tbl_group.group[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == startstopstr)
						{
							break;
						}
					}
					for(tempcount += 1; tempcount < LEN_PRODGRP; tempcount++)
					{
						tbl_group.group[tempcount] = 0x20;
					}
//					ESP_LOGE(TAG, "here 1 %s", tbl_group.group);
					WriteTable(&T_GROUP, (T8U *) &tbl_group, 0);
					tempcount = 0;
					state = 0;
				}
				break;
			}
			case 80:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
						{
							break;
						}
					}
					tbl_basic_info2_temp.WorkMinuteSec = atol((char*)&tempdata) * 60;
//					ESP_LOGE(TAG, "WorkMinuteMillSec %u", tbl_basic_info2_temp.WorkMinuteSec);
					StrNCpy((T8U *) &tbl_basic_info1, (T8U *) &tbl_basic_info1_temp, sizeof(tbl_basic_info1));
					StrNCpy((T8U *) &tbl_basic_info2, (T8U *) &tbl_basic_info2_temp, sizeof(tbl_basic_info2));

					WriteTable(&T_BASIC_INFO1, (T8U *) &tbl_basic_info1, 0);
					WriteTable(&T_BASIC_INFO2, (T8U *) &tbl_basic_info2, 0);

					tempcount = 0;
					state = 90;
				}
				break;
			}
			case 90:
			{
				count++;
				state = 100;
				break;
			}
			case 100:
			{
				if(myhttp_rx_buffer[i] == next1str)
				{
					camefrom = 100;
					state = 8;
				}
				else if(myhttp_rx_buffer[i] == closestr)
				{
					StrNCpy((T8U *) &tbl_basic_info3, (T8U *) &tbl_basic_info3_temp, sizeof(tbl_basic_info3));
					WriteTable(&T_BASIC_INFO3, (T8U *) &tbl_basic_info3, 0);
					camefrom = 0;
					count=0;
					state = 0;

				}
				break;
			}
			default:
			{
				state = 0;
				break;
			}
		}
		i++;
	}
}

//void parseGetQtyEmp(void)ori
//{
//	T16U i = 0;
//	T8U state = 0;
//	T8U tempdata[20] = {0};
//	T8U tempcount = 0;
//
////	TBL_EQTY_INFO tbl_eqty_info;
//	memset(&tbl_eqty_info, '\0', sizeof(tbl_eqty_info));
//
////	sprintf((char*)&myhttp_rx_buffer, 	"HTTP/1.1 200 OK"
////										"Cache-Control: private"
////										"Content-Type: application/json; charset=utf-8"
////										"Server: Microsoft-IIS/10.0"
////										"X-AspNet-Version: 4.0.30319"
////										"X-Powered-By: ASP.NET"
////										"Date: Tue, 24 May 2022 05:36:07 GMT"
////										"Connection: close"
////										"Content-Length: 290"
////										"{"
////										  "\"Table\": ["
////											"{"
////												  "\"AccOutput\": 10,"
////												  "\"CurrentTarget\": 490,"
////												  "\"HourlyOutput\": 0,"
////												  "\"Trend\": \"\""
////											"}"
////										  "]"
////										"}");
////	ESP_LOGI(TAG, "%s", myhttp_rx_buffer);
////	len_return = 1000;
//	while(i < len_return)
//	{
//		switch(state)
//		{
//			case 0:
//			{
//				if(checkstring(&i, (T8U*)&AccOutputstr, sizeof(AccOutputstr)) == _PASS)
//				{
//					state = 10;
//				}
//				if(checkstring(&i, (T8U*)&CurrentTargetstr, sizeof(CurrentTargetstr)) == _PASS)
//				{
//					state = 20;
//				}
//				if(checkstring(&i, (T8U*)&HourlyOutputstr, sizeof(HourlyOutputstr)) == _PASS)
//				{
//					state = 30;
//				}
//				if(checkstring(&i, (T8U*)&Trendstr, sizeof(Trendstr)) == _PASS)
//				{
//					state = 40;
//				}
//				tempcount = 0;
//				break;
//			}
//			case 10:
//			{
//				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
//				{
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
//					{
//						tempdata[tempcount] = myhttp_rx_buffer[i++];
//						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
//						{
//							break;
//						}
//					}
//					tbl_eqty_info.AccOutput = atol((char*)&tempdata);
//					tempcount = 0;
//					state = 0;
//				}
//				break;
//			}
//			case 20:
//			{
//				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
//				{
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
//					{
//						tempdata[tempcount] = myhttp_rx_buffer[i++];
//						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
//						{
//							break;
//						}
//					}
//					tbl_eqty_info.CurrentTarget = atol((char*)&tempdata);
//					tempcount = 0;
//					state = 0;
//				}
//				break;
//			}
//			case 30:
//			{
//				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
//				{
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
//					{
//						tempdata[tempcount] = myhttp_rx_buffer[i++];
//						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
//						{
//							break;
//						}
//					}
//					tbl_eqty_info.HourlyOutput = atol((char*)&tempdata);
//					tempcount = 0;
//					state = 0;
//				}
//				break;
//			}
//			case 40:
//			{
//				if(myhttp_rx_buffer[i] == startstopstr)
//				{
//					i++;
//					if(myhttp_rx_buffer[i] == startstopstr)
//					{
////						WriteTable(&T_EQTY_INFO, (T8U *) &tbl_eqty_info, 0);
//						tempcount = 0;
//						state = 0;//41;
//						break;
//					}
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < LEN_TREND; tempcount++)
//					{
//						tbl_eqty_info.Trend[tempcount] = myhttp_rx_buffer[i++];
//						if(myhttp_rx_buffer[i] == startstopstr)
//						{
//							break;
//						}
//					}
////					WriteTable(&T_EQTY_INFO, (T8U *) &tbl_eqty_info, 0);
//					tempcount = 0;
//					state = 0;//41;
//				}
//				break;
//			}
////			case 41:
////			{
////				TBL_EQTY_INFO tbl_eqty_info2;
////				ReadTable(&T_EQTY_INFO, (T8U *) &tbl_eqty_info2, 0);
////
////				if(StrNCmp((T8U *) &tbl_eqty_info, (T8U *) &tbl_eqty_info2, sizeof(tbl_eqty_info)) != 0)
////				{
////					WriteTable(&T_EQTY_INFO, (T8U *) &tbl_eqty_info, 0);
////				}
////				state = 0;
////				break;
////			}
//			default:
//			{
//				state = 0;
//				break;
//			}
//		}
//		i++;
//	}
//}

void parseGetQtyEmp(void)
{
//	T16U i = 0;
//	T8U state = 0;
//	T8U tempdata[20] = {0};
//	T8U tempcount = 0;
//
//	TBL_EQTY_INFO tbl_eqty_info_temp;
//	memset(&tbl_eqty_info_temp, '\0', sizeof(tbl_eqty_info_temp));
//
////	sprintf((char*)&myhttp_rx_buffer, 	"HTTP/1.1 200 OK"
////										"Cache-Control: private"
////										"Content-Type: application/json; charset=utf-8"
////										"Server: Microsoft-IIS/10.0"
////										"X-AspNet-Version: 4.0.30319"
////										"X-Powered-By: ASP.NET"
////										"Date: Tue, 24 May 2022 05:36:07 GMT"
////										"Connection: close"
////										"Content-Length: 290"
////										"{"
////										  "\"Table\": ["
////											"{"
////												  "\"AccOutput\": 10,"
////												  "\"CurrentTarget\": 490,"
////												  "\"HourlyOutput\": 0,"
////												  "\"Trend\": \"\""
////											"}"
////										  "]"
////										"}");
////	ESP_LOGI(TAG, "%s", myhttp_rx_buffer);
////	len_return = 1000;
//	while(i < len_return)
//	{
//		switch(state)
//		{
//			case 0:
//			{
//				if(checkstring(&i, (T8U*)&AccOutputstr, sizeof(AccOutputstr)) == _PASS)
//				{
//					state = 10;
//				}
//				if(checkstring(&i, (T8U*)&CurrentTargetstr, sizeof(CurrentTargetstr)) == _PASS)
//				{
//					state = 20;
//				}
//				if(checkstring(&i, (T8U*)&HourlyOutputstr, sizeof(HourlyOutputstr)) == _PASS)
//				{
//					state = 30;
//				}
//				if(checkstring(&i, (T8U*)&Trendstr, sizeof(Trendstr)) == _PASS)
//				{
//					state = 40;
//				}
//				if(checkstring(&i, (T8U*)&AccTerminalOutput, sizeof(AccTerminalOutput)) == _PASS)
//				{
//					state = 50;
//				}
//				break;
//			}
//			case 10:
//			{
//				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
//				{
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
//					{
//						tempdata[tempcount] = myhttp_rx_buffer[i++];
//						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
//						{
//							break;
//						}
//					}
//					tbl_eqty_info_temp.AccOutput = atol((char*)&tempdata);
//
////					if(tbl_eqty_info_temp.AccOutput > tbl_eqty_info.AccOutput)
////					{
//						tbl_eqty_info.AccOutput = tbl_eqty_info_temp.AccOutput;
////					}
//					tempcount = 0;
//					state = 0;
//				}
//				break;
//			}
//			case 20:
//			{
//				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
//				{
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
//					{
//						tempdata[tempcount] = myhttp_rx_buffer[i++];
//						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
//						{
//							break;
//						}
//					}
//					tbl_eqty_info_temp.CurrentTarget = atol((char*)&tempdata);
//
//					tbl_eqty_info.CurrentTarget = tbl_eqty_info_temp.CurrentTarget;
//					tempcount = 0;
//					state = 0;
//				}
//				break;
//			}
//			case 30:
//			{
//				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
//				{
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
//					{
//						tempdata[tempcount] = myhttp_rx_buffer[i++];
//						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
//						{
//							break;
//						}
//					}
//					tbl_eqty_info_temp.HourlyOutput = atol((char*)&tempdata);
//
//					if(currenttime.minute < 5)
//					{
//
//					}
//					else //if(tbl_eqty_info_temp.HourlyOutput > tbl_eqty_info.HourlyOutput)
//					{
//						tbl_eqty_info.HourlyOutput = tbl_eqty_info_temp.HourlyOutput;
//					}
//					tempcount = 0;
//					state = 0;
//				}
//				break;
//			}
//			case 40:
//			{
//				if(myhttp_rx_buffer[i] == startstopstr)
//				{
//					i++;
//					if(myhttp_rx_buffer[i] == startstopstr)
//					{
////						WriteTable(&T_EQTY_INFO, (T8U *) &tbl_eqty_info_temp, 0);
//						tempcount = 0;
//						state = 0;//41;
//						break;
//					}
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < LEN_TREND; tempcount++)
//					{
//						tbl_eqty_info_temp.Trend[tempcount] = myhttp_rx_buffer[i++];
//						if(myhttp_rx_buffer[i] == startstopstr)
//						{
//							break;
//						}
//					}
////					WriteTable(&T_EQTY_INFO, (T8U *) &tbl_eqty_info_temp, 0);
//					tempcount = 0;
//					state = 0;//41;
//				}
//				break;
//			}
////			case 41:
////			{
////				TBL_EQTY_INFO tbl_eqty_info2;
////				ReadTable(&T_EQTY_INFO, (T8U *) &tbl_eqty_info2, 0);
////
////				if(StrNCmp((T8U *) &tbl_eqty_info, (T8U *) &tbl_eqty_info2, sizeof(tbl_eqty_info)) != 0)
////				{
////					WriteTable(&T_EQTY_INFO, (T8U *) &tbl_eqty_info, 0);
////				}
////				state = 0;
////				break;
////			}
//			case 50:
//			{
//				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
//				{
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
//					{
//						tempdata[tempcount] = myhttp_rx_buffer[i++];
//						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr) || (myhttp_rx_buffer[i] == ' '))
//						{
//							break;
//						}
//					}
//					tbl_eqty_info_temp.AccTerminalOutput = atol((char*)&tempdata);
//
//					if(tbl_eqty_info_temp.AccTerminalOutput > tbl_eqty_info.AccTerminalOutput)
//					{
//						tbl_eqty_info.AccTerminalOutput = tbl_eqty_info_temp.AccTerminalOutput;
//					}
//					tempcount = 0;
//					state = 0;
//				}
//				break;
//			}
//			default:
//			{
//				state = 0;
//				break;
//			}
//		}
//		i++;
//	}
}

//void parseGetOverAllEmp(void)
//{
//	T16U i = 0;
//	T8U state = 0;
//	T8U tempdata[20] = {0};
//	T8U tempcount = 0;
//
////	TBL_EOVERALL_INFO tbl_eoverall_info;
//	memset(&tbl_eoverall_info, '\0', sizeof(tbl_eoverall_info));
//
//	while(i < len_return)
//	{
//		switch(state)
//		{
//			case 0:
//			{
//				if(checkstring(&i, (T8U*)&Efficiencystr, sizeof(Efficiencystr)) == _PASS)
//				{
//					state = 10;
//				}
//				if(checkstring(&i, (T8U*)&DefectQtystr, sizeof(DefectQtystr)) == _PASS)
//				{
//					state = 20;
//				}
//				if(checkstring(&i, (T8U*)&RepairQtystr, sizeof(RepairQtystr)) == _PASS)
//				{
//					state = 30;
//				}
//				if(checkstring(&i, (T8U*)&DHUstr, sizeof(DHUstr)) == _PASS)
//				{
//					state = 40;
//				}
//				if(checkstring(&i, (T8U*)&LostTimestr, sizeof(LostTimestr)) == _PASS)
//				{
//					state = 50;
//				}
//				tempcount = 0;
//				break;
//			}
////			case 10:
////			{
////				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
////				{
////					memset(tempdata, '\0', sizeof(tempdata));
////					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
////					{
////						tempdata[tempcount] = myhttp_rx_buffer[i++];
////						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
////						{
////							break;
////						}
////					}
////					tbl_eoverall_info.Efficiency[0] = atol((char*)&tempdata);
////					tempcount = 0;
////					state = 11;
////				}
////				break;
////			}
////			case 11:
////			{
////				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
////				{
////					memset(tempdata, '\0', sizeof(tempdata));
////					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
////					{
////						tempdata[tempcount] = myhttp_rx_buffer[i++];
////						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
////						{
////							break;
////						}
////					}
////					tbl_eoverall_info.Efficiency[1] = atol((char*)&tempdata);
////					tempcount = 0;
////					state = 0;
////				}
////				break;
////			}
//			case 10:
//			{
//				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
//				{
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
//					{
//						tempdata[tempcount] = myhttp_rx_buffer[i++];
//						if(myhttp_rx_buffer[i] == next1str)
//						{
//							break;
//						}
//					}
//					tbl_eoverall_info.Efficiency = atof((char*)&tempdata);
////					ESP_LOGI(TAG, "tbl_eoverall_info.Efficiency:%f", tbl_eoverall_info.Efficiency);
//
//					tempcount = 0;
//					state = 0;
//				}
//				break;
//			}
//			case 20:
//			{
//				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
//				{
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
//					{
//						tempdata[tempcount] = myhttp_rx_buffer[i++];
//						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
//						{
//							break;
//						}
//					}
//					tbl_eoverall_info.DefectQty = atol((char*)&tempdata);
//					tempcount = 0;
//					state = 0;
//				}
//				break;
//			}
//			case 30:
//			{
//				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
//				{
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
//					{
//						tempdata[tempcount] = myhttp_rx_buffer[i++];
//						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
//						{
//							break;
//						}
//					}
//					tbl_eoverall_info.RepairQty = atol((char*)&tempdata);
//					tempcount = 0;
//					state = 0;
//				}
//				break;
//			}
////			case 40:
////			{
////				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
////				{
////					memset(tempdata, '\0', sizeof(tempdata));
////					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
////					{
////						tempdata[tempcount] = myhttp_rx_buffer[i++];
////						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
////						{
////							break;
////						}
////					}
////					tbl_eoverall_info.DHU[0] = atol((char*)&tempdata);
////					tempcount = 0;
////					state = 41;
////				}
////				break;
////			}
////			case 41:
////			{
////				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
////				{
////					memset(tempdata, '\0', sizeof(tempdata));
////					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
////					{
////						tempdata[tempcount] = myhttp_rx_buffer[i++];
////						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
////						{
////							break;
////						}
////					}
////					tbl_eoverall_info.DHU[1] = atol((char*)&tempdata);
////					tempcount = 0;
////					state = 0;
////				}
////				break;
////			}
//			case 40:
//			{
//				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
//				{
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
//					{
//						tempdata[tempcount] = myhttp_rx_buffer[i++];
//						if(myhttp_rx_buffer[i] == next1str)
//						{
//							break;
//						}
//					}
//					tbl_eoverall_info.DHU = atof((char*)&tempdata);
////					ESP_LOGI(TAG, "tbl_eoverall_info.DHU:%f", tbl_eoverall_info.DHU);
//
//					tempcount = 0;
//					state = 0;
//				}
//				break;
//			}
//			case 50:
//			{
//				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
//				{
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
//					{
//						tempdata[tempcount] = myhttp_rx_buffer[i++];
//						if(myhttp_rx_buffer[i] == next1str)
//						{
//							break;
//						}
//					}
//					tbl_eoverall_info.LostTime = atol((char*)&tempdata);
////					WriteTable(&T_EOVERALL_INFO, (T8U *) &tbl_eoverall_info, 0);
//					tempcount = 0;
//					state = 0;//51;
//				}
//				break;
//			}
////			case 51:
////			{
////				TBL_EOVERALL_INFO tbl_eoverall_info2;
////				ReadTable(&T_EOVERALL_INFO, (T8U *) &tbl_eoverall_info2, 0);
////
////				if(StrNCmp((T8U *) &tbl_eoverall_info, (T8U *) &tbl_eoverall_info2, sizeof(tbl_eoverall_info)) != 0)
////				{
////					WriteTable(&T_EOVERALL_INFO, (T8U *) &tbl_eoverall_info, 0);
////				}
////				state = 0;
////				break;
////			}
//			default:
//			{
//				state = 0;
//				break;
//			}
//		}
//		i++;
//	}
//}

void parseGetOverAllEmp(void)
{
	T16U i = 0;
	T8U state = 0;
	T8U tempdata[20] = {0};
	T8U tempcount = 0;

	while(i < len_return)
	{
		switch(state)
		{
			case 0:
			{
				if(checkstring(&i, (T8U*)&Efficiencystr, sizeof(Efficiencystr)) == _PASS)
				{
					state = 10;
				}
				if(checkstring(&i, (T8U*)&DefectQtystr, sizeof(DefectQtystr)) == _PASS)
				{
					state = 20;
				}
				if(checkstring(&i, (T8U*)&RepairQtystr, sizeof(RepairQtystr)) == _PASS)
				{
					state = 30;
				}
				if(checkstring(&i, (T8U*)&DHUstr, sizeof(DHUstr)) == _PASS)
				{
					state = 40;
				}
				if(checkstring(&i, (T8U*)&LostTimestr, sizeof(LostTimestr)) == _PASS)
				{
					state = 50;
				}
				if(checkstring(&i, (T8U*)&NoOfErrorstr, sizeof(NoOfErrorstr)) == _PASS)
				{
					state = 60;
				}
				tempcount = 0;
				break;
			}
			case 10:
			{
				if((myhttp_rx_buffer[i] >= '0') && (myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == next1str)
						{
							break;
						}
					}
//					tbl_screen.tbl_eoverall_info.Efficiency = atof((char*)&tempdata);	//don't take this value first. Wrong value
////					ESP_LOGI(TAG, "tbl_eoverall_info.Efficiency:%f", tbl_eoverall_info.Efficiency);

					tempcount = 0;
					state = 0;
				}
				break;
			}
			case 20:
			{
				if((myhttp_rx_buffer[i] >= '0') && (myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
						{
							break;
						}
					}
					tbl_screen.tbl_eoverall_info.DefectQty = atol((char*)&tempdata);
					tempcount = 0;
					state = 0;
				}
				break;
			}
			case 30:
			{
				if((myhttp_rx_buffer[i] >= '0') && (myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
						{
							break;
						}
					}
					tbl_screen.tbl_eoverall_info.RepairQty = atol((char*)&tempdata);
					tempcount = 0;
					state = 0;
				}
				break;
			}
			case 40:
			{
				if((myhttp_rx_buffer[i] >= '0') && (myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == next1str)
						{
							break;
						}
					}
					tbl_screen.tbl_eoverall_info.DHU = atof((char*)&tempdata);
//					ESP_LOGI(TAG, "tbl_eoverall_info.DHU:%f", tbl_eoverall_info.DHU);

					tempcount = 0;
					state = 0;
				}
				break;
			}
			case 50:
			{
				if((myhttp_rx_buffer[i] >= '0') && (myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == next1str)
						{
							break;
						}
					}
//					tbl_screen.tbl_eoverall_info.LostTime = atof((char*)&tempdata);		//don't take this value first as other method get already
//					WriteTable(&T_EOVERALL_INFO, (T8U *) &tbl_eoverall_info, 0);
					tempcount = 0;
					state = 0;//51;
				}
				break;
			}
			case 60:
			{
				if((myhttp_rx_buffer[i] >= '0') && (myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == next1str) || !((myhttp_rx_buffer[i] >= '0') && (myhttp_rx_buffer[i] <= '9')))
						{
							break;
						}
					}
					tbl_screen.tbl_eoverall_info.errorLoqQty = atoi((char*)&tempdata);
//					ESP_LOGI(TAG, "tbl_eoverall_info.errorLoqQty:%u", tbl_eoverall_info.errorLoqQty);

					tempcount = 0;
					state = 0;
				}
				break;
			}
			default:
			{
				state = 0;
				break;
			}
		}
		i++;
	}
}

void parseGetTerminalInfoByMAC(void)
{
	T16U i = 0;
	T8U state = 0;
	T8U tempdata[20] = {0};
	T8U tempcount = 0;

	while(i < len_return)
	{
		switch(state)
		{
			case 0:
			{
				if(checkstring(&i, (T8U*)&SerialNo, sizeof(SerialNo)) == _PASS)
				{
					state = 10;
				}
				tempcount = 0;
				break;
			}
			case 10:
			{
				if(myhttp_rx_buffer[i] == startstopstr)
				{
					i++;
					if(myhttp_rx_buffer[i] == startstopstr)
					{
						tempcount = 0;
						state = 0;
						break;
					}
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == startstopstr)
						{
							break;
						}
					}
					StrNCpy((T8U*)&tbl_sn.serialnumber[0], (T8U*)&tempdata, LEN_SN);
	            	WriteTable(&T_SN, (T8U *) &tbl_sn, 0);
	            	memset(&tbl_sn, 0, sizeof(tbl_sn));
	            	ReadTable(&T_SN, (T8U *)&tbl_sn, 0);

	            	ESP_LOGW(TAG, "Serial number is %-12s", tbl_sn.serialnumber);
					tempcount = 0;
					state = 0;
				}
				break;
			}
			default:
			{
				state = 0;
				break;
			}
		}
		i++;
	}
}

void parsePrefix(void)
{
	T16U i = 0;
	T8U state = 0;
	T8U tempdata[20] = {0};
	T8U tempcount = 0;
	T8U slot = 0;
	TBL_PREFIX tbl_prefix;

	while(i < len_return)
	{
		switch(state)
		{
			case 0:
			{
				if(checkstring(&i, (T8U*)&Prefixstr, sizeof(Prefixstr)) == _PASS)
				{
					memset(&tbl_prefix, '\0', sizeof(tbl_prefix));
					state = 10;
				}
				if(checkstring(&i, (T8U*)&UHFTagTypestr, sizeof(UHFTagTypestr)) == _PASS)
				{
					state = 20;
				}
				if(checkstring(&i, (T8U*)&TagCountstr, sizeof(TagCountstr)) == _PASS)
				{
					state = 30;
				}
				tempcount = 0;
				break;
			}
			case 10:
			{
				if(myhttp_rx_buffer[i] == startstopstr)
				{
					i++;
					if(myhttp_rx_buffer[i] == startstopstr)
					{
						tempcount = 0;
						state = 0;
						break;
					}
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < LEN_PREFIX; tempcount++)
					{
						tbl_prefix.prefix[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == startstopstr)
						{
							break;
						}
					}
					tempcount = 0;
					state = 0;
				}
				break;
			}
			case 20:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
						{
							break;
						}
					}
					tbl_prefix.uhfTagType = atol((char*)&tempdata);
					tempcount = 0;
					state = 0;
				}
				break;
			}
			case 30:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
						{
							break;
						}
					}
					tbl_prefix.tagCount = atol((char*)&tempdata);
					if(slot < 5)
					{
						WriteTable(&T_PREFIX, (T8U *) &tbl_prefix, slot);
					}
					tempcount = 0;
					state = 0;
					slot++;
				}
				break;
			}
			default:
			{
				state = 0;
				break;
			}
		}
		i++;
	}
}

void parseGetBundle()
{
	T16U i = 0;
//	T16U temp_start = 0;
//	T16U temp_end = 0;
	T16U temp1 = 0;
	T8U state = 0;

	while(i < len_return)
	{
		switch(state)
		{
			case 0:
			{
				if(myhttp_rx_buffer[i] == openbracesstr)
				{
					state = 1;
//					temp_start = i;
					memset(message, 0, sizeof((char*)&message));
				}
				break;
			}
			case 1:
			{
//				if(myhttp_rx_buffer[i] == closebracesstr)
//				{
//					temp_end = i;
//					if((temp_end - temp_start) < 5) return; //detect empty
//					else
//					{
//						memset(message, 0, sizeof((char*)&message));
//						StrNCpy(&message[0], &myhttp_rx_buffer[temp_start+1], ((temp_end-1) - (temp_start+1)));//exclude temp_end & temp_start
//					}
//					state = 10;
//					i = temp_start;
//				}


				if((myhttp_rx_buffer[i] == ' ') || (myhttp_rx_buffer[i] == '\r') || (myhttp_rx_buffer[i] == '"') || (myhttp_rx_buffer[i] == ',')) break;
				else if((myhttp_rx_buffer[i] == '\n') && (temp1 == 0)) break;
				else if((myhttp_rx_buffer[i] == closebracesstr) || (temp1 >= 550)) //sizeof(message)
				{
					if(temp1 > 0) TOP_Message |= TOPDATA_GETBUNDLEINFODONE;
					return;
				}
				else
				{
					message[temp1++] = myhttp_rx_buffer[i];

//					if((myhttp_rx_buffer[i] == ':') && (myhttp_rx_buffer[i+1] != ','))
					if(myhttp_rx_buffer[i] == ':')
					{
						message[temp1++] = '\n';
					}
				}
				break;
			}
//			case 10:
//			{
//				if(checkstring(&i, (T8U*)&CurrentQtystr, sizeof(CurrentQtystr)) == _PASS)
//				{
//					TOP_Message |= TOPDATA_GETBUNDLEINFODONE;
//					return;
//				}
//				break;
//			}
			default:
			{
				state = 0;
				break;
			}
		}
		i++;
	}
}

T8U parseRegisterMessage(void)
{
	T16U i = 0;
	T8U state = 0;
//	T8U tempdata[256] = {0};
//	T16U tempcount = 0;
	T8U result = _FAIL;
//	TBL_MSG tbl_msg;
	while(i < len_return)
	{
		switch(state)
		{
			case 0:
			{
				if(checkstring(&i, (T8U*)&Successstr, sizeof(Successstr)) == _PASS)
				{
					state = 10;
				}
//				if(checkstring(&i, (T8U*)&Messagestr, sizeof(Messagestr)) == _PASS)
//				{
//					state = 20;
//				}
				break;
			}
			case 10:
			{
				if(myhttp_rx_buffer[i] == '1')//sucsess
				{
					result = _PASS;
					state = 0;
					return result;
				}
				else if(myhttp_rx_buffer[i] == '2')//tag expired
				{
					result = EXPIRED;
					state = 0;
					return result;
				}
//				else //failed
//				{
//					result = _FAIL;
//					state = 0;
//				}
				break;
			}
//			case 20:
//			{
////				if(myhttp_rx_buffer[i] == startstopstr)
////				{
////					i++;
////					memset(tempdata, '\0', sizeof(tempdata));
////					memset((char*)&tbl_msg, '\0', sizeof(tbl_msg));
////					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
////					{
////						if((tempcount == 1)&&(tbl_msg.message[0] == ' '))tempcount = 0;
////						tbl_msg.message[tempcount] = myhttp_rx_buffer[i++];
////						if(myhttp_rx_buffer[i] == startstopstr)
////						{
////							break;
////						}
////						if(tbl_msg.message[tempcount] == ' ')
////						{
////							T8U addEnter = 0;
////							if(myhttp_rx_buffer[i] == '$')
////							{
////								addEnter = 1;
////							}
////							else if((myhttp_rx_buffer[i] == 'M') &&
////								    (myhttp_rx_buffer[i + 1] == 'O'))
////							{
////								addEnter = 1;
////							}
////							else if((myhttp_rx_buffer[i] == 'S') &&
////									(myhttp_rx_buffer[i + 1] == 'O'))
////							{
////								addEnter = 1;
////							}
////							else if((myhttp_rx_buffer[i] == 'O') &&
////									(myhttp_rx_buffer[i + 1] == 'p'))
////							{
////								addEnter = 1;
////							}
////							else if((myhttp_rx_buffer[i] == 'F') &&
////								    (myhttp_rx_buffer[i + 1] == 'o') &&
////								    (myhttp_rx_buffer[i + 2] == 'u') &&
////								    (myhttp_rx_buffer[i + 3] == 'n') &&
////								    (myhttp_rx_buffer[i + 4] == 'd'))
////							{
////								addEnter = 1;
////							}
////
////							if(addEnter == 1)
////							{
////								tbl_msg.message[tempcount] = '\n';
////							}
////						}
////					}
////					vTaskDelay(1);
////					WriteTable(&T_MSG, (T8U *) &tbl_msg, 0);
////					memset(&tbl_msg, '\0', sizeof(tbl_msg));
////					vTaskDelay(1);
////					tempcount = 0;
////					state = 23;
////				}
//				break;
//			}
			default:
			{
				state = 0;
				break;
			}
		}
		i++;
	}
	return result;
}

void parseCheckOp(void)
{
	#define PREVIOUS 	1
	#define CURRENT 	2

	T16U i = 0;
	T8U state = 0;
	T8U tempdata[20] = {0};
	T8U tempcount = 0;
	T8U slot = 0;
	T8U opType = 0;
	T16U opNumber = 0;
	T8U haveClaim = NO;

	T8U Tablestr[5] = "Table";
	T8U OpTypestr[6] = "OpType";
	T8U Previousstr[8] = "Previous";
	T8U Currentstr[7] = "Current";
	T8U OpNostr[4] = "OpNo";
	T8U ClaimStr[5] = "Claim";

//	sprintf((char *)&myhttp_rx_buffer, 	"HTTP/1.1 200 OK\n"
//										"Cache-Control: private\n"
//										"Content-Type: application/json; charset=utf-8\n"
//										"Server: Microsoft-IIS/10.0\n"
//										"X-AspNet-Version: 4.0.30319\n"
//										"X-Powered-By: ASP.NET\n"
//										"Date: Mon, 22 Aug 2022 05:03:54 GMT\n"
//										"Connection: close\n"
//										"Content-Length: 285\n"
//										"{\n"
//										  "\"Table1\": [\n"
//											"{\n"
//											  "\"OpType\": \"Previous\",\n"
//											  "\"OpNo\": \"0\",\n"
//											  "\"Claim\": \"False\"\n"
//											"},\n"
//											"{\n"
//											  "\"OpType\": \"Current\",\n"
//											  "\"OpNo\": \"325\",\n"
//											  "\"Claim\": \"False\"\n"
//											"}\n"
//										  "],\n"
//										  "\"Table2\": [\n"
//											"{\n"
//											  "\"Success\": \"True\",\n"
//											  "\"Message\": \"301\"\n"
//											"}\n"
//										  "]\n"
//										"}");
//
//	ESP_LOGI(TAG, "%s", myhttp_rx_buffer);

	while(i < len_return)
	{
		switch(state)
		{
			case 0:
			{
				if(checkstring(&i, (T8U*)&Tablestr, sizeof(Tablestr)) == _PASS)
				{
					if(myhttp_rx_buffer[i+1] == '1')state = 10;
					if(myhttp_rx_buffer[i+1] == '2')state = 50;
				}
				tempcount = 0;
				break;
			}
			case 10:
			{
				if(checkstring(&i, (T8U*)&OpTypestr, sizeof(OpTypestr)) == _PASS)
				{
					state = 11;
				}
				if(checkstring(&i, (T8U*)&Tablestr, sizeof(Tablestr)) == _PASS)
				{
					state = 50;
				}
				tempcount = 0;
				break;
			}
			case 11:
			{
				if(checkstring(&i, (T8U*)&Previousstr, sizeof(Previousstr)) == _PASS)
				{
					opType = PREVIOUS;
					state = 20;
//					ESP_LOGE(TAG, "case 11 opType = PREVIOUS");
				}
				if(checkstring(&i, (T8U*)&Currentstr, sizeof(Currentstr)) == _PASS)
				{
					opType = CURRENT;
					state = 20;
//					ESP_LOGE(TAG, "case 11 opType = CURRENT");
				}
				tempcount = 0;
				break;
			}
			case 20:
			{
				if(checkstring(&i, (T8U*)&OpNostr, sizeof(OpNostr)) == _PASS)
				{
					state = 21;
				}
				tempcount = 0;
				break;
			}
			case 21:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr) || (myhttp_rx_buffer[i] == startstopstr))
						{
							break;
						}
					}
					opNumber = atol((char*)&tempdata);
//					ESP_LOGE(TAG, "case 21 opNumber %u", opNumber);
					tempcount = 0;
					state = 30;
				}
				break;
			}
			case 30:
			{
				if(checkstring(&i, (T8U*)&ClaimStr, sizeof(ClaimStr)) == _PASS)
				{
					state = 40;
				}
				tempcount = 0;
				break;
			}
			case 40:
			{
				if(checkstring(&i, (T8U*)&Truestr, sizeof(Truestr)) == _PASS)
				{
//					ESP_LOGE(TAG, "case 40 true");
					state = 10;

					if(opType == CURRENT)
					{
						haveClaim = YES;
						vTaskDelay(1);
						for(tempcount = 0; tempcount < MAX_OPCOUNT_20 ;tempcount++)
						{
							if((tbl_check_op.claimOP[tempcount] > 0) && (tbl_check_op.claimOP[tempcount] == opNumber))
							{
								tbl_check_op.claimOP[tempcount] = 0;
//								ESP_LOGE(TAG, "tbl_check_op.claimOP[%u] = %u", tempcount, tbl_check_op.claimOP[tempcount]);
//								ToBeClaimRatio[tempcount] = 0;
								break;
							}
			    		}
//						ESP_LOGE(TAG, "case 40 haveClaim = YES");
					}
					opNumber = 0;
				}
				if(checkstring(&i, (T8U*)&Falsestr, sizeof(Falsestr)) == _PASS)
				{
//					ESP_LOGE(TAG, "case 40 false");
					state = 10;

					if((opType == PREVIOUS) && (opNumber > 0))
					{
						tbl_check_op.status = PREOPUNCLAIMED;
						state = 0;
//						ESP_LOGE(TAG, "tbl_check_op.status = PREOPUNCLAIMED");
					}
					if(opType == CURRENT)
					{
//						ToBeClaimOP[slot++] = opNumber;
						slot++;
//						ESP_LOGE(TAG, "slot = %u", slot);
					}
				}
				tempcount = 0;
				break;
			}
			case 50:
			{
				if(checkstring(&i, (T8U*)&Successstr, sizeof(Successstr)) == _PASS)
				{
					state = 60;
				}
				if(checkstring(&i, (T8U*)&Messagestr, sizeof(Messagestr)) == _PASS)
				{
					state = 70;
				}
				tempcount = 0;
				break;
			}
			case 60:
			{
				if(checkstring(&i, (T8U*)&Truestr, sizeof(Truestr)) == _PASS)
				{
					state = 50;
				}
				if(checkstring(&i, (T8U*)&Falsestr, sizeof(Falsestr)) == _PASS)
				{
					state = 80;
				}
				tempcount = 0;
				break;
			}
			case 70:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr) || (myhttp_rx_buffer[i] == startstopstr))
						{
							break;
						}
					}
					tbl_check_op.previousClaimed = atol((char*)&tempdata);
//					ESP_LOGE(TAG, "tbl_check_op.previousClaimed:%u", tbl_check_op.previousClaimed);
					tempcount = 0;
					state = 80;
				}
				break;
			}
			case 80:
			{
				if(slot > 0)
				{
					tbl_check_op.status = YES;
//					ESP_LOGE(TAG, "here 1");
				}
				else if(haveClaim)
				{
					tbl_check_op.status = CLAIMED;
//					ESP_LOGE(TAG, "here 2");
				}
				else if(tbl_check_op.status == PREOPUNCLAIMED)
				{
//					ESP_LOGE(TAG, "here 3");
				}
				else
				{
					tbl_check_op.status = OPNOTAVAILABLE;
//					ESP_LOGE(TAG, "here 4");
				}
				return;
				break;
			}
			default:
			{
				state = 0;
				break;
			}
		}
		i++;
	}
}

void parseGetProduction(void)
{
	T16U i = 0;
	T8U state = 0;
	T8U tempdata[20] = {0};
	T8U tempcount = 0;
//	T8U slot = 0;

	tbl_screen.breakCount = 0;
//	HTTP/1.1 200 OK
//	Cache-Control: private
//	Content-Type: application/json; charset=utf-8
//	Server: Microsoft-IIS/8.5
//	X-AspNet-Version: 4.0.30319
//	X-Powered-By: ASP.NET
//	Date: Mon, 20 Jun 2022 07:43:24 GMT
//	Connection: close
//	Content-Length: 679
//
//	{
//	  "tblProduction": [
//	    {
//	      "ScheduleStart": "\/Date(1655683200000+0800)\/",
//	      "ScheduleEnd": "\/Date(1655737200000+0800)\/",
//	      "FirstTrxTime": "\/Date(1655683200000+0800)\/",
//	      "TotalClaim": 0,
//	      "TotalClaimEarnMin": 0.000000,
//	      "AccQty": 0,
//	      "AccQtyEarnMin": 0.000000,
//	      "TerminalAccQty": 0,
//	      "HourlyOutput": 0,
//	      "LostTime": 0
//	    }
//	  ],
//	  "tblBreakTime": [
//	    {
//	      "BreakTimeStart": "\/Date(1655701200000+0800)\/",
//	      "BreakTimeEnd": "\/Date(1655703900000+0800)\/"
//	    },
//	    {
//	      "BreakTimeStart": "\/Date(1655717400000+0800)\/",
//	      "BreakTimeEnd": "\/Date(1655719200000+0800)\/"
//	    }
//	  ]
//	}



//	T8U BreakTimeStartstr[15] = "BreakTimeStart\"";
//	T8U BreakTimeEndstr[13] = "BreakTimeEnd\"";

	while(i < len_return)
	{
		switch(state)
		{
			case 0:
			{
				if(checkstring(&i, (T8U*)&tblProductionstr, sizeof(tblProductionstr)) == _PASS)
				{
//					ESP_LOGI(TAG, "tblProductionstr PASS");
					state = 10;
				}
				if(checkstring(&i, (T8U*)&tblBreakTimestr, sizeof(tblBreakTimestr)) == _PASS)
				{
					state = 120;
				}
				break;
			}
			case 10:
			{
				if(checkstring(&i, (T8U*)&ScheduleStartstr, sizeof(ScheduleStartstr)) == _PASS)
				{
//					ESP_LOGI(TAG, "ScheduleStartstr PASS");
					state = 20;
				}
				if(checkstring(&i, (T8U*)&ScheduleEndstr, sizeof(ScheduleEndstr)) == _PASS)
				{
					state = 30;
				}
				if(checkstring(&i, (T8U*)&FirstTrxTimestr, sizeof(FirstTrxTimestr)) == _PASS)
				{
					state = 40;
				}
				if(checkstring(&i, (T8U*)&TotalClaimstr, sizeof(TotalClaimstr)) == _PASS)
				{
					state = 50;
				}
				if(checkstring(&i, (T8U*)&TotalClaimEarnMinstr, sizeof(TotalClaimEarnMinstr)) == _PASS)
				{
					state = 60;
				}
				if(checkstring(&i, (T8U*)&AccQtystr, sizeof(AccQtystr)) == _PASS)
				{
					state = 70;
				}
				if(checkstring(&i, (T8U*)&AccQtyEarnMinstr, sizeof(AccQtyEarnMinstr)) == _PASS)
				{
					state = 80;
				}
				if(checkstring(&i, (T8U*)&TerminalAccQtystr, sizeof(TerminalAccQtystr)) == _PASS)
				{
					state = 90;
				}
				if(checkstring(&i, (T8U*)&HourlyOutputstr, sizeof(HourlyOutputstr)) == _PASS)
				{
					state = 100;
				}
				if(checkstring(&i, (T8U*)&LostTimestr, sizeof(LostTimestr)) == _PASS)
				{
					state = 110;
				}
				break;
			}
			case 20:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == plusstr) || (myhttp_rx_buffer[i] == minusstr))
						{
							break;
						}
					}
//					ESP_LOGI(TAG, "Rec time:%s", tempdata);
					T64U millisecond = atoll((char*)&tempdata);//this atoll or atol can't be used to convert long

					long t_millisecond = (long) (millisecond / 1000);
//					ESP_LOGI(TAG, "Rec time:%lu", t_millisecond);

					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == closebracketstr)
						{
							break;
						}
					}

					T16S region = atoi((char*)&tempdata);
//					ESP_LOGI(TAG, "Rec time rg:%u", region);

					RTC_convert_to_term_ms(&t_millisecond, region);

					tbl_screen.ScheduleStart = (T32U) t_millisecond;
//					ESP_LOGI(TAG, "tbl_screen.ScheduleStart:%u", tbl_screen.ScheduleStart);

					tempcount = 0;
					state = 10;
				}
				break;
			}
			case 30:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == plusstr) || (myhttp_rx_buffer[i] == minusstr))
						{
							break;
						}
					}
//					ESP_LOGI(TAG, "Rec time:%s", tempdata);
					T64U millisecond = atoll((char*)&tempdata);//this atoll or atol can't be used to convert long

					long t_millisecond = (long) (millisecond / 1000);
//					ESP_LOGI(TAG, "Rec time:%lu", t_millisecond);

					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == closebracketstr)
						{
							break;
						}
					}

					T16S region = atoi((char*)&tempdata);
//					ESP_LOGI(TAG, "Rec time rg:%u", region);

					RTC_convert_to_term_ms(&t_millisecond, region);

					tbl_screen.ScheduleEnd = (T32U) t_millisecond;
//					ESP_LOGI(TAG, "tbl_screen.ScheduleEnd:%u", tbl_screen.ScheduleEnd);

					tempcount = 0;
					state = 10;
				}
				break;
			}
			case 40:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == plusstr) || (myhttp_rx_buffer[i] == minusstr))
						{
							break;
						}
					}
//					ESP_LOGI(TAG, "Rec time:%s", tempdata);
					T64U millisecond = atoll((char*)&tempdata);//this atoll or atol can't be used to convert long

					long t_millisecond = (long) (millisecond / 1000);
//					ESP_LOGI(TAG, "Rec time:%lu", t_millisecond);

					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == closebracketstr)
						{
							break;
						}
					}

					T16S region = atoi((char*)&tempdata);
//					ESP_LOGI(TAG, "Rec time rg:%u", region);

					RTC_convert_to_term_ms(&t_millisecond, region);

					tbl_screen.FirstTrxTime = (T32U) t_millisecond;
//					ESP_LOGI(TAG, "tbl_screen.FirstTrxTime:%u", tbl_screen.FirstTrxTime);

					tempcount = 0;
					state = 10;
				}
				break;
			}
			case 50:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
						{
							break;
						}
					}
					tbl_screen.TotalClaim = atol((char*)&tempdata);
//					ESP_LOGI(TAG, "tbl_screen.TotalClaim:%u", tbl_screen.TotalClaim);

					tempcount = 0;
					state = 10;
				}
				break;
			}
			case 60:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == next1str)
						{
							break;
						}
					}
					tbl_screen.TotalClaimEarnMin = atof((char*)&tempdata);
//					ESP_LOGI(TAG, "tbl_screen.TotalClaimEarnMin:%f", tbl_screen.TotalClaimEarnMin);

					tempcount = 0;
					state = 10;
				}
				break;
			}
			case 70:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
						{
							break;
						}
					}
					T16U t_AccQty = atol((char*)&tempdata);
					if(t_AccQty > tbl_screen.AccClaimQty)
					{
						tbl_screen.AccClaimQty = t_AccQty;
					}
//					ESP_LOGI(TAG, "tbl_screen.AccQty:%u", tbl_screen.AccQty);

					tempcount = 0;
					state = 10;
				}
				break;
			}
			case 80:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == next1str)
						{
							break;
						}
					}
					float t_AccQtyEarnMin = atof((char*)&tempdata);
					if(t_AccQtyEarnMin > tbl_screen.AccQtyEarnMin)
					{
						tbl_screen.AccQtyEarnMin = t_AccQtyEarnMin;
					}
//					ESP_LOGI(TAG, "tbl_screen.AccQtyEarnMin:%f", tbl_screen.AccQtyEarnMin);

					tempcount = 0;
					state = 10;
				}
				break;
			}
			case 90:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
						{
							break;
						}
					}
					T16U t_TerminalAccQty = atol((char*)&tempdata);
					if(t_TerminalAccQty > tbl_screen.TerminalAccQty)
					{
						tbl_screen.TerminalAccQty = t_TerminalAccQty;
					}
//					ESP_LOGI(TAG, "tbl_screen.TerminalAccQty:%u", tbl_screen.TerminalAccQty);

					tempcount = 0;
					state = 10;
				}
				break;
			}
			case 100:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
						{
							break;
						}
					}
					T16U t_HourlyOutput = atol((char*)&tempdata);
//					if(t_HourlyOutput > tbl_screen.HourlyOutput)
//					{
//						if(currenttime.minute > 4)
							tbl_screen.HourlyOutput = t_HourlyOutput;
//					}
//					ESP_LOGI(TAG, "tbl_screen.HourlyOutput:%u", tbl_screen.HourlyOutput);

					tempcount = 0;
					state = 10;
				}
				break;
			}
			case 110:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(!(((myhttp_rx_buffer[i] >= '0') && (myhttp_rx_buffer[i] <= '9')) || (myhttp_rx_buffer[i] == dotstr)))
						{
							break;
						}
					}

					float t_LostTime = atof((char*)&tempdata);
					if(t_LostTime > tbl_screen.tbl_eoverall_info.LostTime)
					{
						tbl_screen.tbl_eoverall_info.LostTime = t_LostTime;
					}
//					ESP_LOGI(TAG, "tbl_screen.LostTime:%u", tbl_screen.LostTime);

					tempcount = 0;
					state = 0;
				}
				break;
			}
			case 120:
			{
				if(checkstring(&i, (T8U*)&BreakTimeStartstr, sizeof(BreakTimeStartstr)) == _PASS)
				{
					state = 130;
				}
				if(checkstring(&i, (T8U*)&BreakTimeEndstr, sizeof(BreakTimeEndstr)) == _PASS)
				{
					state = 140;
				}
				break;
			}
			case 130:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == plusstr) || (myhttp_rx_buffer[i] == minusstr))
						{
							break;
						}
					}
//					ESP_LOGI(TAG, "Rec time:%s", tempdata);
					T64U millisecond = atoll((char*)&tempdata);//this atoll or atol can't be used to convert long

					long t_millisecond = (long) (millisecond / 1000);
//					ESP_LOGI(TAG, "Rec time:%lu", t_millisecond);

					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == closebracketstr)
						{
							break;
						}
					}

					T16S region = atoi((char*)&tempdata);
//					ESP_LOGI(TAG, "Rec time rg:%u", region);

					RTC_convert_to_term_ms(&t_millisecond, region);

					tbl_screen.tbl_break[tbl_screen.breakCount].start_t = (T32U) t_millisecond;
//					ESP_LOGI(TAG, "tbl_screen.tbl_break[%u].start_t:%u", tbl_screen.breakCount, tbl_screen.tbl_break[tbl_screen.breakCount].start_t);

					tempcount = 0;
					state = 120;
				}
				break;
			}
			case 140:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == plusstr) || (myhttp_rx_buffer[i] == minusstr))
						{
							break;
						}
					}
//					ESP_LOGI(TAG, "Rec time:%s", tempdata);
					T64U millisecond = atoll((char*)&tempdata);//this atoll or atol can't be used to convert long

					long t_millisecond = (long) (millisecond / 1000);
//					ESP_LOGI(TAG, "Rec time:%lu", t_millisecond);

					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == closebracketstr)
						{
							break;
						}
					}

					T16S region = atoi((char*)&tempdata);
//					ESP_LOGI(TAG, "Rec time rg:%u", region);

					RTC_convert_to_term_ms(&t_millisecond, region);

					tbl_screen.tbl_break[tbl_screen.breakCount].end_t = (T32U) t_millisecond;
//					ESP_LOGI(TAG, "tbl_screen.tbl_break[%u].end_t:%u", tbl_screen.breakCount, tbl_screen.tbl_break[tbl_screen.breakCount].end_t);
					tbl_screen.breakCount++;
					tempcount = 0;
					state = 120;
					if(tbl_screen.breakCount >= MAX_BREAK)
					{
						state = 0;
					}
				}
				break;
			}
			default:
			{
				state = 0;
				break;
			}
		}
		i++;
	}
	tbl_screen.targetTotalWorkHr = tbl_screen.ScheduleEnd - tbl_screen.ScheduleStart;
	for(i = 0; i < tbl_screen.breakCount; i++)
	{
		if((tbl_screen.ScheduleStart < tbl_screen.tbl_break[i].start_t) && (tbl_screen.ScheduleEnd > tbl_screen.tbl_break[i].end_t))
		{
			tbl_screen.targetTotalWorkHr = tbl_screen.targetTotalWorkHr - (tbl_screen.tbl_break[i].end_t - tbl_screen.tbl_break[i].start_t);
		}
		else if((tbl_screen.ScheduleStart < tbl_screen.tbl_break[i].end_t) && (tbl_screen.ScheduleEnd > tbl_screen.tbl_break[i].end_t))
		{
			tbl_screen.targetTotalWorkHr = tbl_screen.targetTotalWorkHr - (tbl_screen.tbl_break[i].end_t - tbl_screen.ScheduleStart);
		}
		else if((tbl_screen.ScheduleStart < tbl_screen.tbl_break[i].start_t) && (tbl_screen.ScheduleEnd > tbl_screen.tbl_break[i].start_t))
		{
			tbl_screen.targetTotalWorkHr = tbl_screen.targetTotalWorkHr - (tbl_screen.ScheduleEnd - tbl_screen.tbl_break[i].start_t);
		}
//		else
//		{
//			tbl_screen.targetTotalWorkHr = 0;
//		}
	}
}

void parseProductionInfobyGroup(void)
{
	T16U i = 0;
	T8U state = 0;
	T8U tempdata[20] = {0};
	T8U tempcount = 0;
	T8U slot = 0;
	T8U firstTime = true;
	T8U takeIt = false;
	T8U firstTimeTermQty = true;

	T8U tblOpQtystr[9] = "tblOpQty\"";
	T8U OpNostr[5] = "OpNo\"";

	tbl_screen.breakCount = 0;
//	{
//	  "tblProduction": [
//	    {
//	      "ScheduleStart": "\/Date(1675296000000+0800)\/",
//	      "ScheduleEnd": "\/Date(1675335600000+0800)\/",
//	      "FirstTrxTime": "\/Date(1675295172000+0800)\/",
//	      "TotalClaim": 5460,
//	      "TotalClaimEarnMin": 0.000000,
//	      "LostTime": 0
//	    }
//	  ],
//	  "tblOpQty": [
//	    {
//	      "OpNo": 210,
//	      "AccQty": 790,
//	      "AccQtyEarnMin": 0.000000,
//	      "HourlyOutput": 390,
//	      "TerminalAccQty": 790
//	    },
//	    {
//	      "OpNo": 215,
//	      "AccQty": 790,
//	      "AccQtyEarnMin": 0.000000,
//	      "HourlyOutput": 590,
//	      "TerminalAccQty": 790
//	    }
//	  ],
//	  "tblBreakTime": [
//	    {
//	      "BreakTimeStart": "\/Date(1675314900000+0800)\/",
//	      "BreakTimeEnd": "\/Date(1675318500000+0800)\/"
//	    }
//	  ]
//	}

	while(i < len_return)
	{
		switch(state)
		{
			case 0:
			{
				if(checkstring(&i, (T8U*)&tblProductionstr, sizeof(tblProductionstr)) == _PASS)
				{
//					ESP_LOGI(TAG, "tblProductionstr PASS");
					state = 10;
				}
				if(checkstring(&i, (T8U*)&tblOpQtystr, sizeof(tblOpQtystr)) == _PASS)
				{
					state = 80;
				}
				if(checkstring(&i, (T8U*)&tblBreakTimestr, sizeof(tblBreakTimestr)) == _PASS)
				{
					state = 140;
				}
				break;
			}
			case 10:
			{
				if(checkstring(&i, (T8U*)&ScheduleStartstr, sizeof(ScheduleStartstr)) == _PASS)
				{
//					ESP_LOGI(TAG, "ScheduleStartstr PASS");
					state = 20;
				}
				if(checkstring(&i, (T8U*)&ScheduleEndstr, sizeof(ScheduleEndstr)) == _PASS)
				{
					state = 30;
				}
				if(checkstring(&i, (T8U*)&FirstTrxTimestr, sizeof(FirstTrxTimestr)) == _PASS)
				{
					state = 40;
				}
				if(checkstring(&i, (T8U*)&TotalClaimstr, sizeof(TotalClaimstr)) == _PASS)
				{
					state = 50;
				}
				if(checkstring(&i, (T8U*)&TotalClaimEarnMinstr, sizeof(TotalClaimEarnMinstr)) == _PASS)
				{
					state = 60;
				}
				if(checkstring(&i, (T8U*)&LostTimestr, sizeof(LostTimestr)) == _PASS)
				{
					state = 70;
				}
				break;
			}
			case 20:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == plusstr) || (myhttp_rx_buffer[i] == minusstr))
						{
							break;
						}
					}
//					ESP_LOGI(TAG, "Rec time:%s", tempdata);
					T64U millisecond = atoll((char*)&tempdata);//this atoll or atol can't be used to convert long

					long t_millisecond = (long) (millisecond / 1000);
//					ESP_LOGI(TAG, "Rec time:%lu", t_millisecond);

					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == closebracketstr)
						{
							break;
						}
					}

					T16S region = atoi((char*)&tempdata);
//					ESP_LOGI(TAG, "Rec time rg:%u", region);

					RTC_convert_to_term_ms(&t_millisecond, region);

					tbl_screen.ScheduleStart = (T32U) t_millisecond;
//					ESP_LOGI(TAG, "tbl_screen.ScheduleStart:%u", tbl_screen.ScheduleStart);

					tempcount = 0;
					state = 10;
				}
				break;
			}
			case 30:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == plusstr) || (myhttp_rx_buffer[i] == minusstr))
						{
							break;
						}
					}
//					ESP_LOGI(TAG, "Rec time:%s", tempdata);
					T64U millisecond = atoll((char*)&tempdata);//this atoll or atol can't be used to convert long

					long t_millisecond = (long) (millisecond / 1000);
//					ESP_LOGI(TAG, "Rec time:%lu", t_millisecond);

					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == closebracketstr)
						{
							break;
						}
					}

					T16S region = atoi((char*)&tempdata);
//					ESP_LOGI(TAG, "Rec time rg:%u", region);

					RTC_convert_to_term_ms(&t_millisecond, region);

					tbl_screen.ScheduleEnd = (T32U) t_millisecond;
//					ESP_LOGI(TAG, "tbl_screen.ScheduleEnd:%u", tbl_screen.ScheduleEnd);

					tempcount = 0;
					state = 10;
				}
				break;
			}
			case 40:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == plusstr) || (myhttp_rx_buffer[i] == minusstr))
						{
							break;
						}
					}
//					ESP_LOGI(TAG, "Rec time:%s", tempdata);
					T64U millisecond = atoll((char*)&tempdata);//this atoll or atol can't be used to convert long

					long t_millisecond = (long) (millisecond / 1000);
//					ESP_LOGI(TAG, "Rec time:%lu", t_millisecond);

					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == closebracketstr)
						{
							break;
						}
					}

					T16S region = atoi((char*)&tempdata);
//					ESP_LOGI(TAG, "Rec time rg:%u", region);

					RTC_convert_to_term_ms(&t_millisecond, region);

					tbl_screen.FirstTrxTime = (T32U) t_millisecond;
//					ESP_LOGI(TAG, "tbl_screen.FirstTrxTime:%u", tbl_screen.FirstTrxTime);

					tempcount = 0;
					state = 10;
				}
				break;
			}
			case 50:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
						{
							break;
						}
					}
					tbl_screen.TotalClaim = atol((char*)&tempdata);
//					ESP_LOGI(TAG, "tbl_screen.TotalClaim:%u", tbl_screen.TotalClaim);

					tempcount = 0;
					state = 10;
				}
				break;
			}
			case 60:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == next1str)
						{
							break;
						}
					}
					tbl_screen.TotalClaimEarnMin = atof((char*)&tempdata);
//					ESP_LOGI(TAG, "tbl_screen.TotalClaimEarnMin:%f", tbl_screen.TotalClaimEarnMin);

					tempcount = 0;
					state = 10;
				}
				break;
			}
			case 70:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(!(((myhttp_rx_buffer[i] >= '0') && (myhttp_rx_buffer[i] <= '9')) || (myhttp_rx_buffer[i] == dotstr)))
						{
							break;
						}
					}

					float t_LostTime = atof((char*)&tempdata);
					if(t_LostTime > tbl_screen.tbl_eoverall_info.LostTime)
					{
						tbl_screen.tbl_eoverall_info.LostTime = t_LostTime;
					}
//					ESP_LOGI(TAG, "tbl_screen.LostTime:%u", tbl_screen.LostTime);

					tempcount = 0;
					state = 0;
				}
				break;
			}
			case 80:
			{
				if(checkstring(&i, (T8U*)&OpNostr, sizeof(OpNostr)) == _PASS)
				{
					state = 90;
				}
				if(checkstring(&i, (T8U*)&AccQtystr, sizeof(AccQtystr)) == _PASS)
				{
					state = 100;
				}
				if(checkstring(&i, (T8U*)&AccQtyEarnMinstr, sizeof(AccQtyEarnMinstr)) == _PASS)
				{
					state = 110;
				}
				if(checkstring(&i, (T8U*)&HourlyOutputstr, sizeof(HourlyOutputstr)) == _PASS)
				{
					state = 120;
				}
				if(checkstring(&i, (T8U*)&TerminalAccQtystr, sizeof(TerminalAccQtystr)) == _PASS)
				{
					state = 130;
				}
				if(checkstring(&i, (T8U*)&tblBreakTimestr, sizeof(tblBreakTimestr)) == _PASS)
				{
					state = 140;
				}
				break;
			}
			case 90:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == next1str)
						{
							break;
						}
					}
					T16U tempOp = (T16U)atoi((char*)&tempdata);

					for(int i = 0; i < 20; i++)
					{
						if(tempOp == tbl_basic_info3.opNo[i])
						{
							slot = i;
						}
					}
					tempcount = 0;
					state = 80;
				}
				break;
			}
			case 100:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
						{
							break;
						}
					}
					T16U t_AccQty = atol((char*)&tempdata);

					if(firstTime)
					{
						firstTime = false;
						if(t_AccQty > (tbl_screen.AccClaimQty + tbl_screen.OpQty[slot]))
						{
							tbl_screen.AccClaimQty = 0;
							tbl_screen.OpQty[slot] = t_AccQty;
							takeIt = true;
						}
					}
					else if(takeIt)
					{
						tbl_screen.OpQty[slot] = t_AccQty;
					}
//					ESP_LOGI(TAG, "tbl_screen.AccQty:%u", tbl_screen.AccQty);

					tempcount = 0;
					state = 80;
				}
				break;
			}
			case 110:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == next1str)
						{
							break;
						}
					}
					float t_AccQtyEarnMin = atof((char*)&tempdata);
					if(t_AccQtyEarnMin > tbl_screen.AccQtyEarnMin)
					{
						tbl_screen.AccQtyEarnMin = t_AccQtyEarnMin;
					}
//					ESP_LOGI(TAG, "tbl_screen.AccQtyEarnMin:%f", tbl_screen.AccQtyEarnMin);

					tempcount = 0;
					state = 80;
				}
				break;
			}
			case 120:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
						{
							break;
						}
					}
					T16U t_HourlyOutput = atol((char*)&tempdata);
//					if(t_HourlyOutput > tbl_screen.HourlyOutput)
//					{
//						if(currenttime.minute > 4)
							tbl_screen.HourlyOutput = t_HourlyOutput;
//					}
//					ESP_LOGI(TAG, "tbl_screen.HourlyOutput:%u", tbl_screen.HourlyOutput);

					tempcount = 0;
					state = 80;
				}
				break;
			}
			case 130:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == next1str) || (myhttp_rx_buffer[i] == dotstr))
						{
							break;
						}
					}
					T16U t_TerminalAccQty = atol((char*)&tempdata);
					if((t_TerminalAccQty > tbl_screen.TerminalAccQty) && (firstTimeTermQty == true))
					{
						firstTimeTermQty = false;
						tbl_screen.TerminalAccQty = t_TerminalAccQty;
					}
//					if(takeIt)
//					{
//						tbl_screen.TerminalAccQty = 0;
//					}
//					ESP_LOGI(TAG, "tbl_screen.TerminalAccQty:%u", tbl_screen.TerminalAccQty);

					tempcount = 0;
					state = 80;
				}
				break;
			}
			case 140:
			{
				if(checkstring(&i, (T8U*)&BreakTimeStartstr, sizeof(BreakTimeStartstr)) == _PASS)
				{
					state = 150;
				}
				if(checkstring(&i, (T8U*)&BreakTimeEndstr, sizeof(BreakTimeEndstr)) == _PASS)
				{
					state = 160;
				}
				break;
			}
			case 150:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == plusstr) || (myhttp_rx_buffer[i] == minusstr))
						{
							break;
						}
					}
//					ESP_LOGI(TAG, "Rec time:%s", tempdata);
					T64U millisecond = atoll((char*)&tempdata);//this atoll or atol can't be used to convert long

					long t_millisecond = (long) (millisecond / 1000);
//					ESP_LOGI(TAG, "Rec time:%lu", t_millisecond);

					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == closebracketstr)
						{
							break;
						}
					}

					T16S region = atoi((char*)&tempdata);
//					ESP_LOGI(TAG, "Rec time rg:%u", region);

					RTC_convert_to_term_ms(&t_millisecond, region);

					tbl_screen.tbl_break[tbl_screen.breakCount].start_t = (T32U) t_millisecond;
//					ESP_LOGI(TAG, "tbl_screen.tbl_break[%u].start_t:%u", tbl_screen.breakCount, tbl_screen.tbl_break[tbl_screen.breakCount].start_t);

					tempcount = 0;
					state = 140;
				}
				break;
			}
			case 160:
			{
				if((myhttp_rx_buffer[i] >= '0') &&(myhttp_rx_buffer[i] <= '9'))
				{
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if((myhttp_rx_buffer[i] == plusstr) || (myhttp_rx_buffer[i] == minusstr))
						{
							break;
						}
					}
//					ESP_LOGI(TAG, "Rec time:%s", tempdata);
					T64U millisecond = atoll((char*)&tempdata);//this atoll or atol can't be used to convert long

					long t_millisecond = (long) (millisecond / 1000);
//					ESP_LOGI(TAG, "Rec time:%lu", t_millisecond);

					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == closebracketstr)
						{
							break;
						}
					}

					T16S region = atoi((char*)&tempdata);
//					ESP_LOGI(TAG, "Rec time rg:%u", region);

					RTC_convert_to_term_ms(&t_millisecond, region);

					tbl_screen.tbl_break[tbl_screen.breakCount].end_t = (T32U) t_millisecond;
//					ESP_LOGI(TAG, "tbl_screen.tbl_break[%u].end_t:%u", tbl_screen.breakCount, tbl_screen.tbl_break[tbl_screen.breakCount].end_t);
					tbl_screen.breakCount++;
					tempcount = 0;
					state = 140;
					if(tbl_screen.breakCount >= MAX_BREAK)
					{
						state = 0;
					}
				}
				break;
			}
			default:
			{
				state = 0;
				break;
			}
		}
		i++;
	}
	tbl_screen.targetTotalWorkHr = tbl_screen.ScheduleEnd - tbl_screen.ScheduleStart;
	for(i = 0; i < tbl_screen.breakCount; i++)
	{
		if((tbl_screen.ScheduleStart < tbl_screen.tbl_break[i].start_t) && (tbl_screen.ScheduleEnd > tbl_screen.tbl_break[i].end_t))
		{
			tbl_screen.targetTotalWorkHr = tbl_screen.targetTotalWorkHr - (tbl_screen.tbl_break[i].end_t - tbl_screen.tbl_break[i].start_t);
		}
		else if((tbl_screen.ScheduleStart < tbl_screen.tbl_break[i].end_t) && (tbl_screen.ScheduleEnd > tbl_screen.tbl_break[i].end_t))
		{
			tbl_screen.targetTotalWorkHr = tbl_screen.targetTotalWorkHr - (tbl_screen.tbl_break[i].end_t - tbl_screen.ScheduleStart);
		}
		else if((tbl_screen.ScheduleStart < tbl_screen.tbl_break[i].start_t) && (tbl_screen.ScheduleEnd > tbl_screen.tbl_break[i].start_t))
		{
			tbl_screen.targetTotalWorkHr = tbl_screen.targetTotalWorkHr - (tbl_screen.ScheduleEnd - tbl_screen.tbl_break[i].start_t);
		}
//		else
//		{
//			tbl_screen.targetTotalWorkHr = 0;
//		}
	}
}

void GetProdGroupNameByProdGroupID(void)
{
	T16U i = 0;
	T8U state = 0;
	T8U tempcount = 0;

	while(i < len_return)
	{
		switch(state)
		{
			case 0:
			{
				if(checkstring(&i, (T8U*)&ProdGroupNamestr, sizeof(ProdGroupNamestr)) == _PASS)
				{
					state = 10;
				}
				break;
			}
			case 10:
			{
				if(myhttp_rx_buffer[i] == startstopstr)
				{
					i++;
					if(myhttp_rx_buffer[i] == startstopstr)
					{
						tempcount = 0;
						state = 0;
						break;
					}

					for(tempcount = 0; tempcount < 10; tempcount++)
					{
						temp_prod_group.prod_group_name[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == startstopstr)
						{
							break;
						}
					}
					FLAG_STATE |= F_CHECKLINEPROD;
					tempcount = 0;
					state = 0;
				}
				break;
			}
			default:
			{
				state = 0;
				break;
			}
		}
		i++;
	}
}

void CheckInputOpClaimedGroup(void)
{
	T16U i = 0;
	T8U state = 0;
	T8U tempcount = 0;
	T8U tempdata[20] = {0};

//	TEMP_PROD_GROUP temp_prod_group;

	while(i < len_return)
	{
		switch(state)
		{
			case 0:
			{
				if(checkstring(&i, (T8U*)&Successstr, sizeof(Successstr)) == _PASS)
				{
					state = 10;
				}
				if(checkstring(&i, (T8U*)&Messagestr, sizeof(Messagestr)) == _PASS)
				{
					state = 20;
				}
				break;
			}
			case 10:
			{
//				if(myhttp_rx_buffer[i] == startstopstr)
//				{
//					i++;
//					if(myhttp_rx_buffer[i] == startstopstr)
//					{
//						tempcount = 0;
//						state = 0;
//						break;
//					}
//					memset(tempdata, '\0', sizeof(tempdata));
//					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
//					{
//						tempdata[tempcount] = myhttp_rx_buffer[i++];
//						if(myhttp_rx_buffer[i] == next1str)
//						{
//							break;
//						}
//					}
//					StrNCpy(temp_prod_group.status, tempdata, sizeof(temp_prod_group.status));
//					tempcount = 0;
//					state = 0;
//				}
//				break;
				if(checkstring(&i, (T8U*)&Truestr, sizeof(Truestr)) == _PASS)
				{
					temp_prod_group.status = _PASS;
					state = 0;
				}
				if(checkstring(&i, (T8U*)&Falsestr, sizeof(Falsestr)) == _PASS)
				{
					temp_prod_group.status = _FAIL;
					state = 0;
				}
				break;
			}
			case 20:
			{
				if(myhttp_rx_buffer[i] == startstopstr)
				{
					i++;
					if(myhttp_rx_buffer[i] == startstopstr)
					{
						tempcount = 0;
						state = 0;
						break;
					}
					memset(tempdata, '\0', sizeof(tempdata));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == startstopstr)
						{
							break;
						}
					}
//					temp_prod_group.message = atol((char*)&tempdata);
					sprintf((char*)temp_prod_group.message,"%s",tempdata);
					tempcount = 0;
					state = 0;
//					FLAG_STATE &= ~F_CHECKLINEPROD;
					FLAG_STATE |= F_CHECKLINEPROD;
				}
				break;
			}
			default:
			{
				state = 0;
				break;
			}
		}
		i++;
	}
}

T8U parseRequestMessage(void)
{
	T16U i = 0;
	T8U state = 0;
	T8U tempdata[256] = {0};
	T16U tempcount = 0;
	T8U result = _FAIL;
	TBL_MSG tbl_msg;
	while(i < len_return)
	{
		switch(state)
		{
			case 0:
			{
				if(checkstring(&i, (T8U*)&Successstr, sizeof(Successstr)) == _PASS)
				{
					state = 10;
				}
				if(checkstring(&i, (T8U*)&Messagestr, sizeof(Messagestr)) == _PASS)
				{
					state = 20;
				}
				break;
			}
			case 10:
			{
				if(myhttp_rx_buffer[i] == '1')//message have data
				{
					result = _PASS;
					state = 0;
					if(callHelpData) return result;
				}
				else if(myhttp_rx_buffer[i] == '2')//message empty
				{
					result = _PASS;
					state = 0;
					if(callHelpData) return result;
					sprintf((char*)&tbl_msg.message, "No Message!");
					vTaskDelay(1);
					WriteTable(&T_MSG, (T8U *) &tbl_msg, 0);
					memset(&tbl_msg, '\0', sizeof(tbl_msg));
					vTaskDelay(1);
					return result;
				}
				else if(myhttp_rx_buffer[i] == '3')//message error
				{
//					result = _PASS;
					state = 0;
					if(callHelpData) return result;
					result = _PASS;
					sprintf((char*)&tbl_msg.message, "Request Error!");
					vTaskDelay(1);
					WriteTable(&T_MSG, (T8U *) &tbl_msg, 0);
					memset(&tbl_msg, '\0', sizeof(tbl_msg));
					vTaskDelay(1);
					return result;
				}
				break;
			}
			case 20:
			{
				if(myhttp_rx_buffer[i] == startstopstr)
				{
					i++;
					memset(tempdata, '\0', sizeof(tempdata));
					memset((char*)&tbl_msg, '\0', sizeof(tbl_msg));
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						if((tempcount == 1)&&(tbl_msg.message[0] == ' '))tempcount = 0;
						tbl_msg.message[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == startstopstr)
						{
							break;
						}
						if(tbl_msg.message[tempcount] == ' ')
						{
							T8U addEnter = 0;
							if(myhttp_rx_buffer[i] == '$')
							{
								addEnter = 1;
							}
							else if((myhttp_rx_buffer[i] == 'M') &&
								    (myhttp_rx_buffer[i + 1] == 'O'))
							{
								addEnter = 1;
							}
							else if((myhttp_rx_buffer[i] == 'S') &&
									(myhttp_rx_buffer[i + 1] == 'O'))
							{
								addEnter = 1;
							}
							else if((myhttp_rx_buffer[i] == 'O') &&
									(myhttp_rx_buffer[i + 1] == 'p'))
							{
								addEnter = 1;
							}
							else if((myhttp_rx_buffer[i] == 'F') &&
								    (myhttp_rx_buffer[i + 1] == 'o') &&
								    (myhttp_rx_buffer[i + 2] == 'u') &&
								    (myhttp_rx_buffer[i + 3] == 'n') &&
								    (myhttp_rx_buffer[i + 4] == 'd'))
							{
								addEnter = 1;
							}

							if(addEnter == 1)
							{
								tbl_msg.message[tempcount] = '\n';
							}
						}
					}
					vTaskDelay(1);
					WriteTable(&T_MSG, (T8U *) &tbl_msg, 0);
					memset(&tbl_msg, '\0', sizeof(tbl_msg));
					vTaskDelay(1);
					tempcount = 0;
					state = 23;
				}
				break;
			}
			default:
			{
				state = 0;
				break;
			}
		}
		i++;
	}
	return result;
}

T8U parseSuccessMessage(void)
{
	T16U i = 0;
	T8U state = 0;
	T8U result = _FAIL;
	T8U tempdata[256] = {0};
	T16U tempcount = 0;

	while(i < len_return)
	{
//		ESP_LOGI(TAG, "i:%u", i);
		switch(state)
		{
			case 0:
			{
				if(checkstring(&i, (T8U*)&Successstr, sizeof(Successstr)) == _PASS)
				{
					state = 10;
//					ESP_LOGI(TAG, "found success");
				}
				if((pos == 16) && (checkstring(&i, (T8U*)&Messagestr, sizeof(Messagestr)) == _PASS))
				{
					state = 20;
				}
				break;
			}
			case 10:
			{
				if(checkstring(&i, (T8U*)&Truestr, sizeof(Truestr)) == _PASS)
				{
					result = _PASS;
					state = 0;
				}
				if(checkstring(&i, (T8U*)&Falsestr, sizeof(Falsestr)) == _PASS)
				{
					state = 0;
				}
				break;
			}
			case 20:
			{
				if(myhttp_rx_buffer[i] == startstopstr)
				{
					i++;
					memset(tempdata, '\0', sizeof(tempdata));
//					vTaskDelay(1);
					for(tempcount = 0; tempcount < sizeof(tempdata); tempcount++)
					{
						tempdata[tempcount] = myhttp_rx_buffer[i++];
						if(myhttp_rx_buffer[i] == startstopstr)
						{
							break;
						}
					}
//					vTaskDelay(1);
					if(strlen((char *)&tempdata) > 0)
					{
						sprintf((char *)&message, "%s", tempdata);
						TOP_Message |= TOPDATA_HAVEMSG;
					}
					tempcount = 0;
					state = 0;
				}
				break;
			}
			default:
			{
				state = 0;
				break;
			}
		}
		i++;
	}
	ESP_LOGI(TAG, "%u", result);
	return result;
}

T8U parsetrueStr(void)
{
	T16U i = 0;
	T8U state = 0;
	T8U result = _FAIL;
	while(i < len_return)
	{
		switch(state)
		{
			case 0:
			{
				if(checkstring(&i, (T8U*)&truestr, sizeof(truestr)) == _PASS)
				{
					result = _PASS;
					state = 0;
				}
//				if(checkstring(&i, (T8U*)&falsestr, sizeof(falsestr)) == _PASS)
//				{
//					state = 0;
//				}
				break;
			}
			default:
			{
				state = 0;
				break;
			}
		}
		i++;
	}
	ESP_LOGI(TAG, "%u", result);
	return result;
}

T8U checkstring(T16U * i, T8U * tempstring, T8U size)
{
	T8U result = _FAIL;
	T8U tempcount = 0;
	T16U pre_i = 0;
//	T16U tempi = 0;

	if(myhttp_rx_buffer[*i] == *(tempstring+tempcount))
	{
//		ESP_LOGI(TAG, "1, %u", *i);
		pre_i = *i;
//		tempi = *i;
		*i = *i + 1;
//		ESP_LOGI(TAG, "2, %u", *i);
		for(tempcount = 1; tempcount < size; tempcount++)
		{
			if(myhttp_rx_buffer[*i] == *(tempstring+tempcount))
			{
//				tempi = *i;
				*i = *i + 1;
			}
			else
			{
				tempcount = 0;
				*i = pre_i;
				break;
			}
		}

		if(tempcount == size)
		{
//			tempi = *i;
			*i = *i - 1;
			tempcount = 0;
			result = _PASS;
		}
	}
	return result;
}

T8U checkSendString(T16U * i, T8U * tempstring, T8U size)
{
	T8U result = _FAIL;
	T8U tempcount = 0;
	T16U pre_i = 0;
//	T16U tempi = 0;

	if(myhttp_tx_buffer[*i] == *(tempstring+tempcount))
	{
//		ESP_LOGI(TAG, "1, %u", *i);
		pre_i = *i;
//		tempi = *i;
		*i = *i + 1;
//		ESP_LOGI(TAG, "2, %u", *i);
		for(tempcount = 1; tempcount < size; tempcount++)
		{
			if(myhttp_tx_buffer[*i] == *(tempstring+tempcount))
			{
//				tempi = *i;
				*i = *i + 1;
			}
			else
			{
				tempcount = 0;
				*i = pre_i;
				break;
			}
		}

		if(tempcount == size)
		{
//			tempi = *i;
			*i = *i - 1;
			tempcount = 0;
			result = _PASS;
		}
	}
	return result;
}

