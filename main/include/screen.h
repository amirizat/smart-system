/*
 * src.h
 *
 *  Created on: 4 Nov 2020
 *      Author: Administrator
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include "target.h"
#include "display.h"

/*!< LCD PAD */
#ifdef GPRO_Z5
#define Z5_LCD_CLK   GPIO_NUM_36
#define Z5_LCD_MOSI  GPIO_NUM_35
#define Z5_LCD_DC    GPIO_NUM_20
#define Z5_LCD_RST   GPIO_NUM_21
#define Z5_LCD_CS    GPIO_NUM_34
#define Z5_LCD_BK    GPIO_NUM_39
#endif

typedef T8U KEYPRESS;

#define MENUTYPE_NONUM_NOSEL		0
#define MENUTYPE_NONUM_SEL			1
#define MENUTYPE_NUM_NOSEL			2
#define MENUTYPE_NUM_SEL			3
//#define MENUTYPE_OPTIONBOX		4
//#define MENUTYPE_CHECKBOX			5
//#define MENUTYPE_PENDING			6

#define MAX_MSGBOX  2
#define MSGBOX_IDLE					0
#define MSGBOX_TYPE_TIMEOUT			0
#define MSGBOX_TYPE_USERFEEDBACK	1
#define MSGBOX_TYPE_DONE			2
#define MSGBOX_TYPE_ERROR			3
#define MSGBOX_TYPE_MSG				4
#define MSGBOX_TYPE_SUCCESS			5
#define MSGBOX_TYPE_ERROR2			6
#define MSGBOX_NEWPOPUP				0x81
#define MSGBOX_NEWUSERFEEDBACK		0x82
#define MSGBOX_EVENTDURATION 		30

#define BUZZER_FASTONE		0x0001
#define BUZZER_SLOWONE		0x0002
#define BUZZER_SLOWTWO		0x0004
#define BUZZER_FASTTWO		0x0008
#define BUZZER_FASTTHREE	0x0010
#define BUZZER_FASTFIVE		0x0020

#define LED1_SLOWONE		0x0040

#define LED2_FASTTWO		0x0080
#define LED2_FASTFIVE		0x0100
#define LED2_SLOWONE		0x0200
#define LED2_FASTTHREE		0x0400
#define LED1LED2_FASTFIVE	0x0800

#define DETECTCARD_EMPLOYEE		0x01
#define DETECTCARD_BUNDLE		0x02
#define DETECTCARD_MASTER		0x04
#define DETECTCARD_QC			0x08
#define DETECTCARD_DEFECT		0x10
#define DETECTCARD_MACHINE		0x20
#define DETECTCARD_GROUP		0x40
#define DETECTCARD_LOCATION		0x80
#define DETECTCARD_ALL			(DETECTCARD_EMPLOYEE) | (DETECTCARD_BUNDLE) |(DETECTCARD_MASTER) | (DETECTCARD_QC) | (DETECTCARD_DEFECT | (DETECTCARD_MACHINE) | DETECTCARD_GROUP) | (DETECTCARD_LOCATION)

#define UPDATESUCCESS			0
#define DATAREADY				1
#define MOOPREQUEST				2
#define NODATA					3
#define WRONGNUMBER				4
#define UPDATEMAYFAIL			5
#define NOTSAMECARD				6
#define EMPISOUT				7
#define PARTIALNOTCLEAR			8
#define MONOTMATCH				9
#define OPSELECTERROR			10
#define UNCLAIMOP				11
#define SAMECARD				12
#define MOREQUESTING			13
#define LOGINFAIL				14
#define CLAIMOP					15
#define WRONGPRODGRP			16
#define CARDERROR				17
#define LOGINCONTROL			18
#define MEMORYFULL				19
#define NOTAUTHORIZE			20
#define LOSTTIMEON				21
#define PARTIALTAG				22
#define LOGOUTSUCCESS			23
#define DISCRETECLAIMDONE		24
#define WRONGMOTHERTAG			25
#define WRONGCHILDTAG			26
#define QCMWRONGDEFCODE			27
#define QCMWRONGDEFQTY			28
#define QCMWRONGDEFOP			29
#define QTY0					30
#define WRONGOPERATION			31
#define NOTSCANLOCTAG			32
#define NOTSELECTOP				33
#define PREOPUNCLAIMED			34
#define WRONGVERSION			35
#define	SOSCALLON				36
#define	SOSCALLOFF				37
#define	TECHCALLON				38
#define	TECHCALLOFF				39
#define REPAIRDONE				40
#define OPEXISTED				41
#define CANNOTSPLIT				42
#define NOTORITAG				43
#define REQTOSPLIT				44
#define REPAIRDONEQC			45
#define DATACHANGED				46
#define LOSTTIMEOFF				47
#define WRONGTAG				48
#define ONEPIECEDONE			49
#define CLAIMNOTDONE 			50
#define LOGINSUCCESS			51
#define CHECKINGSCHEDULE		52
#define WRONGOPNUMBER			53
#define WRITETAGFAIL			54
#define WIFINOTCONNECT 			55
#define IPNOTSET				56
#define UPDATEFIRMWAREFAILED 	57
#define SAMEVERSION			 	58
#define LOWVERSION			 	59
#define SOCKETFAILED		 	60
#define LOOKFIRMFAILED			61
#define NOOTHERFIRM				62
#define REQFAILED				63
#define RETURNBLANK				64
#define PREFIXSUCCESS			65
#define PREFIXFAILED			66
#define GETBUNDLEFAILED			67
#define REGISTERSUCCESS			68
#define REGISTERFAILED			69
#define TAGEXPIRED				70
#define PREFIXNOTMATCHED		71
#define	OPNOTAVAILABLE			72
#define	CLAIMED					73
#define SELECTFIXOPFORSPLIT		74
#define CHECKOPUNCLAIMED		75
#define SPLITON					76
#define GCODEERROR				77
#define LISTENMODE				78
#define ASSETREGISTERED			79
#define CHILDCONVERT			80
//#define CHANGEDGROUPFAILED		77
#define	MAX_MSG					80//always last

typedef struct MSGBOX
{
	T8U type;
	T8U *disp;
	T8U *header;
	T16U indicator;
//	T8U (*triggerfunction)();
	unsigned int enabled : 1;
//	unsigned int id : 7;
}MSGBOX;
T8U MsgBox(void);
extern MSGBOX msgbox[2];

#define MAXTEXTBOXLEN 20
typedef struct TEXTBOX
{
	T8U caption[MAXTEXTBOXLEN + 1];
	T8U text[MAXTEXTBOXLEN +1 ];
	T8U inputmethod;
	T8U enabled;
}TEXTBOX;

typedef struct INFOBOX
{
//	T8U *infoheader;
	T8U *caption;
}INFOBOX;

#define MAXINPUTTEXTBOX	3
typedef struct WINAPP
{
	T8U header[21];
	T8U scr_state;
	TEXTBOX textbox[MAXINPUTTEXTBOX];
	T8U currenttextbox;
	T8U cursor;
	INFOBOX infobox;
}WINAPP;

typedef struct TEXTBOX2
{
	T8U caption[5];
	T8U text[5];
	T8U inputmethod;
	T8U enabled;
}TEXTBOX2;

#define MAXINPUTTEXTBOX_2	20
typedef struct WINAPP2
{
	T8U header[21];
	T8U subheader[15];
	T8U scr_state;
	TEXTBOX textbox[MAXINPUTTEXTBOX_2];
	T8U currenttextbox;
	T8U cursor;
//	INFOBOX infobox;
}WINAPP2;

typedef T8U SCR_STATUS;
#define SCR_INPUT_INPROGRESS	(0)
#define SCR_INPUT_DONE			(1)
#define SCR_INPUT_CANCEL		(2)

#define SCR_STATE_PREPARE		(0)
#define SCR_STATE_UPDATEINPUT	(2)
#define SCR_STATE_DRAWSCR		(1)
#define SCR_INPUTSTATEDETECT	(3)
#define SCR_INPUTSTATERELEASE	(4)
#define SCR_STATE_MSG			(5)

#define SCR_INPUT_BY_KEYPAD	(0)
#define SCR_INPUT_BY_NUMBER	(2)

#define ONOFF		1
#define YESNO		2
#define OKORNOTOK	4

#define LCD_MAXWIDTH 20

extern T8U state_detectcard;

extern T8U MsgBox_Timer;
extern T8U *IButton_Header;
extern T8U *IButton_Message;
extern T8U state_detectcardUHF;
extern const T8U HEADER_DONE[];
extern const T8U DESCRIPTION_UPDATESUCCESS[];
extern const T8U DESCRIPTION_CLAIMED[];

KEYPRESS SCR_MenuItem(T8U *SCR_HEADERS, T8U *SCR_MESSAGES, T8U ItemCount, T8U PreSelect, T8U *PostSelect, T8U MenuType);
SCR_STATUS SCR_InputString(WINAPP *winapp);
SCR_STATUS SCR_InputString_2(WINAPP2 *winapp2);
KEYPRESS SCR_ONOFF(T8U *SCR_HEADERS, T8U *SCR_MESSAGES, T8U YESNO_ORONOFF, T8U *ON_OR_OFF);
//void clrscr(void);
T8U SCR_DetectCard(T8U DetectCardType, T8U *Message, T8U *UID);//T8U *Card, T8U *Message, T8U *UID);
T8U SCR_DetectCardUHF(T8U DetectCardType, T8U *Message, T8U showMessage);
T8U MsgBox(void);
void Msg_Done(T8U Done_Type);
void Msg_Error(T8U Error_Type);
void Msg_Error2(T8U Error_Type);
void Msg_Error3(T8U *Error_msg);
//void Msg_Error2_addOn(T8U *text);
void Msg_Msg(T8U Msg_Type);
void Msg_Success(T8U Success_Type);
void display_message(T8U *Message);
void display_message_BG(T8U *Message, lv_color_t textcolor, lv_color_t bgcolor);
void display_message_2(T8U *tempstring, int x, int y);
void display_header(T8U *Header);
//KEYPRESS SCR_MenuMultiSelect(T8U *SCR_HEADERS, T8U *SCR_MESSAGES,
//					  T8U ItemCount, T8U *PreSelect);
void SCR_IBUTTONEnabled(T8U IBUTTONONOFF);
//void SCR_IButton(void);
T8U GetMSGPart(T8U *MESSAGE, T16U *target_table, T8U * NewLineIndicator, T8U * tempitemtoline);
T8U findstartitem(T8U totalline, T8U temppagestart, T8U tempcurrent, T8U * tempitemtolibe, T8U type);
T8U SCR_MenuItemV(T8U *SCR_HEADERS, T8U *SCR_MESSAGES, T8U itemcount, T8U PreSelect, T8U *PostSelect);
void showScreenSuccess(T8U *SCR_MESSAGES);
void showScreenWarning(T8U type, T8U *SCR_MESSAGES);
void display_message_wHeader(T8U *Header, T8U *Messages);
void showBigLoading(void);
void msg_done_immediate(T8U *SCR_MESSAGES);
//void msg_special_immediate(T8U *SCR_HEADER, T8U *SCR_MESSAGES, lv_color_t bgColor);
char scr_keyboard(T32S btnCount, T8U btnPress);
void MSG_RETRY(const char * header,T8U count);
#endif /* SCREEN_H_ */
