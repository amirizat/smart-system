#include "screen.h"
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "keypad.h"
#include "ledBuzz.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "app.h"

static const char* TAG = "screen";
//Displaying Message
const T8U HEADER_DONE[5] 	= {"Done"};
const T8U HEADER_ERROR[6] 	= {"Error"};
const T8U HEADER_MSG[8] 	= {"Message"};

const T8U DESCRIPTION_UPDATESUCCESS[16] 		= {"Update Success!"};
const T8U DESCRIPTION_DATAREADY[24]				= {"Generate Data\nSuccess!"};
const T8U DESCRIPTION_MOOPREQUEST[30]			= {"MO OP Request!"};//\n      Is Sent!"};
const T8U DESCRIPTION_NODATA[9] 				= {"No Data!"};
const T8U DESCRIPTION_WRONGNUMBER[26] 			= {"Wrong Number!\nTry Again."};
const T8U DESCRIPTION_UPDATEMAYFAIL[22] 		= {"Update Card/Tag Fail!"};
const T8U DESCRIPTION_NOTSAMECARD[25] 			= {"Not SAME Tag!\nTry Again."};
const T8U DESCRIPTION_EMPISOUT[24]				= {"Employee In Out Status!"};
const T8U DESCRIPTION_PARTIALNOTCLEAR[50]		= {" Bundle tag Has\n    Unfinished\nPartial Quantity!"};
const T8U DESCRIPTION_MONOTMATCH[16]			= {"MO Not Matched!"};
const T8U DESCRIPTION_OPSELECTERROR[17]			= {"OP Select Error!"};
const T8U DESCRIPTION_UNCLAIMOP[42]				= {"This OP Number\nHas Not Been\nClaimed Yet!"};
const T8U DESCRIPTION_CLAIMOP[33]				= {"This OP Number\nIs Not Available!"};
const T8U DESCRIPTION_SAMECARD[32]				= {"Can Not Present\n     SAME Tag!"};
const T8U DESCRIPTION_MOREQUESTING[42]			= {"Requesting The MO,\nPlease Try Again Later."};
const T8U DESCRIPTION_LOGINFAIL[12]				= {"Login Fail!"};
const T8U DESCRIPTION_WRONGGROUP[27]			= {"Wrong Production\n   Group!"};
const T8U DESCRIPTION_CARDERROR[16]				= {"Card/Tag Error!"};
const T8U DESCRIPTION_LOGINCONTROL[33]			= {"Login Fail!\nLogin Control Is ON."};
const T8U DESCRIPTION_MEMORYFULL[13]			= {"Memory Full!"};
const T8U DESCRIPTION_NOTAUTHORIZE[21]			= {"Not Authorized Card!"};
const T8U DESCRIPTION_LOSTTIMEON[16]			= {"Lost Time Is ON!"};
const T8U DESCRIPTION_PARTIALTAG[13]			= {"Partial Tag!"};
const T8U DESCRIPTION_LOGOUTSUCCESS[16]			= {"Logout Success!"};
const T8U DESCRIPTION_DISCRETECLAIMDONE[15] 	= {"Discrete Done!"};
const T8U DESCRIPTION_WRONGMOTHERTAG[40] 		= {"Please Not Present\nChild Tag To Split!"};
const T8U DESCRIPTION_WRONGCHILDTAG[48] 		= {"Please Not Present\nMother Tag To\nReceive Split!"};
const T8U DESCRIPTION_QCMWRONGDEFCODE[19] 		= {"Wrong Defect Code!"};
const T8U DESCRIPTION_QCMWRONGDEFQTY[18] 		= {"Wrong Defect Qty!"};
const T8U DESCRIPTION_QCMWRONGDEFOP[17] 		= {"Wrong Defect OP!"};
const T8U DESCRIPTION_QTY0[14] 					= {"Quantity = 0!"};
const T8U DESCRIPTION_WRONGOPERATION[17] 		= {"Wrong Operation!"};
const T8U DESCRIPTION_NOTSCANLOCTAG[31] 		= {"Please scan location\ntag first!"};
const T8U DESCRIPTION_NOTSELECTOP[24] 			= {"Please select OP\nfirst!"};
const T8U DESCRIPTION_PREVIOUSOPUNCLAIMED[23]	= {"Previous OP\n Unclaimed"};
const T8U DESCRIPTION_WRONGVERSION[31]			= {"Tag Version\nare not supported!"};//\nfor this features!"};
const T8U DESCRIPTION_SOSCALLON[16]				= {"Call Help is ON!"};
const T8U DESCRIPTION_SOSCALLOFF[17]			= {"Call Help is OFF"};
const T8U DESCRIPTION_TECHCALLON[17]			= {"Call Tech is ON!"};
const T8U DESCRIPTION_TECHCALLOFF[17]			= {"Call Tech is OFF"};
const T8U DESCRIPTION_REPAIRDONE[16]			= {"Repair is Done."};
const T8U DESCRIPTION_OPEXISTED[14]				= {"OP Existed!!!"};
const T8U DESCRIPTION_CANNOTSPLIT[44]			= {"Balance quantity is 1\nCannot split anymore!"};
const T8U DESCRIPTION_NOTORIGINALTAG[46]		= {"Please present\nunused tag.\nNot original tag!"};
const T8U DESCRIPTION_REQTOSPLIT[30]			= {"Have defect\nRequire to split!"};
const T8U DESCRIPTION_REPAIRDONEQC[22]			= {"RepairDone.\nBack To QC"};
const T8U DESCRIPTION_DATACHANGED[50]			= {"Tag Data Changed!\nTerminal will not\nupdate Tag."};
const T8U DESCRIPTION_LOSTTIMEOFF[17]			= {"Lost Time is OFF"};
const T8U DESCRIPTION_WRONGTAG[11]				= {"Wrong Tag!"};
const T8U DESCRIPTION_ONEPIECEDONE[19]			= {"One Piece Finished"};
const T8U DESCRIPTION_CLAIMNOTDONE[13]			= {"Not Claimed!"};
const T8U DESCRIPTION_LOGINSUCCESS[15] 			= {"Login Success!"};
const T8U DESCRIPTION_CHECKINGSCHEDULE[15]		= {"Schedule Check"};
const T8U DESCRIPTION_WRONGOPNUMBER[17] 		= {"Wrong OP Number!"};
const T8U DESCRIPTION_WRITETAGFAIL[17] 			= {"Write tag failed!"};
const T8U DESCRIPTION_WIFINOTCONNECT[20] 		= {"WIFI not\nconnected!"};
const T8U DESCRIPTION_IPNOTSET[22] 				= {"Please set\nserver IP!"};
const T8U DESCRIPTION_UPDATEFIRMWAREFAILED[24]	= {"Update firmware\nFailed!"};
const T8U DESCRIPTION_SAMEVERSION[20]			= {"Found same\nVersion!"};
const T8U DESCRIPTION_LOWVERSION[21]			= {"Found lower\nVersion!"};
const T8U DESCRIPTION_SOCKETFAILED[14]			= {"Socket Failed!"};
const T8U DESCRIPTION_LOOKFIRMFAILED[25]		= {"Looking\nFirmware\nFailed!"};
const T8U DESCRIPTION_NOOTHERFIRM[29]			= {"No Other\nFirmware\nAvailable!"};
const T8U DESCRIPTION_REQFAILED[16]				= {"Request Failed!"};
const T8U DESCRIPTION_RETURNBLANK[21]			= {"Server Return Blank!"};
const T8U DESCRIPTION_PREFIXSUCCESS[21]			= {"Get Prefix Success..."};
const T8U DESCRIPTION_PREFIXFAILED[18]			= {"Get Prefix FAILED!"};
const T8U DESCRIPTION_GETBUNDLEFAILED[28]		= {"Get Bundle Info\n\t\t\t\tFAILED!"};
const T8U DESCRIPTION_REGISTERSUCCESS[19]		= {"Register Success..."};
const T8U DESCRIPTION_REGISTERFAILED[17]		= {"Register FAILED!"};
const T8U DESCRIPTION_TAGEXPIRED[55]			= {"\t\t\tInvalid Split,\nTag Cycle reached\n Max cycle Count!"};//FAILED\nTag Expired!!!"};
const T8U DESCRIPTION_PREFIXNOTMATCHED[22]		= {"Prefix Not\nMatched!!!"};
const T8U DESCRIPTION_OPNOTAVAILABLE[20]		= {"  OP Not\nAvailable!"};
const T8U DESCRIPTION_CLAIMED[9]				= {"Claimed!"};
const T8U DESCRIPTION_SELECTFIXOPFORSPLIT[41]	= {"Please select Fix OP\n  Before use Split!"};
const T8U DESCRIPTION_CHECKOPUNCLAIMED[22]		= {" Check OP\nUnclaimed!!!"};
const T8U DESCRIPTION_SPLITON[13]				= {"Split is ON!"};
const T8U DESCRIPTION_GCODEERROR[19]			= {"Group Code Invalid!"};
const T8U DESCRIPTION_LISTENMODE[14]			= {"Listening Mode"};
const T8U DESCRIPTION_ASSETREGISTER[23]			= {"Asset Login is Required"};


const  T8U *PTR_MESSAGE[MAX_MSG] =
{
	DESCRIPTION_UPDATESUCCESS,
	DESCRIPTION_DATAREADY,
	DESCRIPTION_MOOPREQUEST,
	DESCRIPTION_NODATA,
	DESCRIPTION_WRONGNUMBER,
	DESCRIPTION_UPDATEMAYFAIL,
	DESCRIPTION_NOTSAMECARD,
	DESCRIPTION_EMPISOUT,
	DESCRIPTION_PARTIALNOTCLEAR,
	DESCRIPTION_MONOTMATCH,
	DESCRIPTION_OPSELECTERROR,
	DESCRIPTION_UNCLAIMOP,
	DESCRIPTION_SAMECARD,
	DESCRIPTION_MOREQUESTING,
	DESCRIPTION_LOGINFAIL,
	DESCRIPTION_CLAIMOP,
	DESCRIPTION_WRONGGROUP,
	DESCRIPTION_CARDERROR,
	DESCRIPTION_LOGINCONTROL,
	DESCRIPTION_MEMORYFULL,
	DESCRIPTION_NOTAUTHORIZE,
	DESCRIPTION_LOSTTIMEON,
	DESCRIPTION_PARTIALTAG,
	DESCRIPTION_LOGOUTSUCCESS,
	DESCRIPTION_DISCRETECLAIMDONE,
	DESCRIPTION_WRONGMOTHERTAG,
	DESCRIPTION_WRONGCHILDTAG ,
	DESCRIPTION_QCMWRONGDEFCODE,
	DESCRIPTION_QCMWRONGDEFQTY,
	DESCRIPTION_QCMWRONGDEFOP,
	DESCRIPTION_QTY0,
	DESCRIPTION_WRONGOPERATION,
	DESCRIPTION_NOTSCANLOCTAG,
	DESCRIPTION_NOTSELECTOP,
	DESCRIPTION_PREVIOUSOPUNCLAIMED,
	DESCRIPTION_WRONGVERSION,
	DESCRIPTION_SOSCALLON,
	DESCRIPTION_SOSCALLOFF,
	DESCRIPTION_TECHCALLON,
	DESCRIPTION_TECHCALLOFF,
	DESCRIPTION_REPAIRDONE,
	DESCRIPTION_OPEXISTED,
	DESCRIPTION_CANNOTSPLIT,
	DESCRIPTION_NOTORIGINALTAG,
	DESCRIPTION_REQTOSPLIT,
	DESCRIPTION_REPAIRDONEQC,
	DESCRIPTION_DATACHANGED,
	DESCRIPTION_LOSTTIMEOFF,
	DESCRIPTION_WRONGTAG,
	DESCRIPTION_ONEPIECEDONE,
	DESCRIPTION_CLAIMNOTDONE,
	DESCRIPTION_LOGINSUCCESS,
	DESCRIPTION_CHECKINGSCHEDULE,
	DESCRIPTION_WRONGOPNUMBER,
	DESCRIPTION_WRITETAGFAIL,
	DESCRIPTION_WIFINOTCONNECT,
	DESCRIPTION_IPNOTSET,
	DESCRIPTION_UPDATEFIRMWAREFAILED,
	DESCRIPTION_SAMEVERSION,
	DESCRIPTION_LOWVERSION,
	DESCRIPTION_SOCKETFAILED,
	DESCRIPTION_LOOKFIRMFAILED,
	DESCRIPTION_NOOTHERFIRM,
	DESCRIPTION_REQFAILED,
	DESCRIPTION_RETURNBLANK,
	DESCRIPTION_PREFIXSUCCESS,
	DESCRIPTION_PREFIXFAILED,
	DESCRIPTION_GETBUNDLEFAILED,
	DESCRIPTION_REGISTERSUCCESS,
	DESCRIPTION_REGISTERFAILED,
	DESCRIPTION_TAGEXPIRED,
	DESCRIPTION_PREFIXNOTMATCHED,
	DESCRIPTION_OPNOTAVAILABLE,
	DESCRIPTION_CLAIMED,
	DESCRIPTION_SELECTFIXOPFORSPLIT,
	DESCRIPTION_CHECKOPUNCLAIMED,
	DESCRIPTION_SPLITON,
	DESCRIPTION_GCODEERROR,
	DESCRIPTION_LISTENMODE,
	DESCRIPTION_ASSETREGISTER,
};

MSGBOX msgbox[2]={0};
T8U MsgBox_Timer=0;
T8U IBUTTON_ENABLED = OFF;
T8U *IButton_Header;
T8U *IButton_Message;
T8U itemcount_perpage = 6;

KEYPRESS SCR_MenuItem(T8U *SCR_HEADERS, T8U *SCR_MESSAGES, T8U ItemCount,
					  T8U PreSelect, T8U *PostSelect, T8U MenuType)
{
	#define SCR_INIT	0
	#define SCR_SHOW	2
	#define SCR_DETECTKEY	3
	#define SCR_RELEASEKEY	4
	#define BUTTON_TIMEOUT	1

	T8U returnkey = 0;
	T8U PageItemCount = 0;
	T8U tempstring[40] = {0};

	T8U y;
	static T8U menucount,buttontimeout,buttonloop = 0;
	static T8U CurrentItem;
	static T8U pagestartitem;
	static T8U scr_state = SCR_INIT;
	static T8U previouskey = 0;

	switch(scr_state)
	{
		case SCR_INIT:
		{
			if(ItemCount == 0)
			{
				ItemCount = 1;
			}
			menucount = ItemCount;
			CurrentItem = PreSelect;
			if(Keypad_Scan() == 0)
			{
				scr_state = SCR_SHOW;
			}
			break;
		}
		case SCR_SHOW:
		{
			clear_content_display();
			title_display((char*)SCR_HEADERS,2,23);
			pagestartitem = (CurrentItem / itemcount_perpage) * itemcount_perpage;


			for(PageItemCount = 0; PageItemCount < itemcount_perpage; PageItemCount++)
			{

				if((pagestartitem + PageItemCount) > (menucount-1))
				{
					break;
				}

				y = PageItemCount*30+60;
				memset(tempstring, '\0', sizeof(tempstring));
				GetLine(SCR_MESSAGES,tempstring,pagestartitem+PageItemCount);

				if(CurrentItem == (pagestartitem + PageItemCount))
				{
					if (MenuType == MENUTYPE_NONUM_NOSEL)
					{
						text_display((char*)&tempstring, 20, y);
					}
					else if (MenuType == MENUTYPE_NONUM_SEL)
					{
						text_select_display((char*)&tempstring, 20, y, LV_COLOR_WHITE, LV_COLOR_TEAL);
					}
					else if (MenuType == MENUTYPE_NUM_NOSEL)
					{
						num_display((int)(pagestartitem + PageItemCount + 1), 2, y);
						text_display((char*)&tempstring, 40, y);
					}
					else if (MenuType == MENUTYPE_NUM_SEL)
					{
						num_select_display((int)(pagestartitem + PageItemCount + 1), 2, y, LV_COLOR_WHITE, LV_COLOR_TEAL);
						text_select_display((char*)&tempstring, 40, y, LV_COLOR_WHITE, LV_COLOR_TEAL);
					}
				}
				else
				{
					if ((MenuType == MENUTYPE_NONUM_NOSEL)||(MenuType == MENUTYPE_NONUM_SEL))
					{
						text_display((char*)&tempstring, 20, y);
					}
					else if ((MenuType == MENUTYPE_NUM_NOSEL)||(MenuType == MENUTYPE_NUM_SEL))
					{
						num_display((int)(pagestartitem + PageItemCount + 1), 2, y);
						text_display((char*)&tempstring, 40, y);
					}
				}
			}
			border_display();
			scr_state = SCR_DETECTKEY;
			break;
		}
		case SCR_DETECTKEY:
		{
			previouskey = Keypad_Scan();
			if (previouskey > 0)
			{
				Output_AddTask(&buzzer, OUTPUT_STATE_ON, 2,0, 1);
				scr_state = SCR_RELEASEKEY;

				if (previouskey == 'A')
				{
						*PostSelect = CurrentItem;
						menucount = 0;
						CurrentItem = 0;
						pagestartitem = 0;
				}

				if (previouskey == 'C')
		        {
					if(CurrentItem > 0)
					{
						if (((MenuType == MENUTYPE_NONUM_NOSEL)||(MenuType == MENUTYPE_NUM_NOSEL)) && (CurrentItem>=itemcount_perpage))
				        {
			        		CurrentItem -= itemcount_perpage;
				        }
						else
						{
							CurrentItem--;
						}
					}
					else
					{
						CurrentItem = menucount - 1;
						if ((MenuType == MENUTYPE_NONUM_NOSEL)||(MenuType == MENUTYPE_NUM_NOSEL))
						{
							T8U a = CurrentItem / itemcount_perpage;
							CurrentItem = itemcount_perpage * a;
						}
					}
					buttontimeout = BUTTON_TIMEOUT;
				}
				else if (previouskey == 'D')
				{
					if ((MenuType == MENUTYPE_NONUM_NOSEL)||(MenuType == MENUTYPE_NUM_NOSEL))
					{
						if((CurrentItem + itemcount_perpage) < menucount)
						{
							CurrentItem += itemcount_perpage;
						}
						else
						{
							CurrentItem = 0;
						}
					}
					else
					{
						if(CurrentItem < menucount - 1)
						{
							CurrentItem++;
						}
						else
						{
							CurrentItem = 0;
						}
					}
					buttontimeout = BUTTON_TIMEOUT;
					break;
				}
				else if (previouskey == 'B')
				{
					menucount = 0;
					CurrentItem = 0;
					pagestartitem = 0;
				}
			}
			else
			{
				buttonloop = 15;
			}
			break;
		}

		case SCR_RELEASEKEY:
		{
			if((previouskey == 'A') || (previouskey == 'B'))
			{
				if(Keypad_Scan() == 0)
				{
					scr_state = SCR_INIT;
					returnkey = previouskey;
					previouskey = 0;
				}
			}
			else if((previouskey == 'C') || (previouskey == 'D'))
			{
				if(buttonloop > 2)
				{
					buttonloop--;
				}
				else
				{
					if(--buttontimeout == 0)
					{
						scr_state = SCR_SHOW;
						returnkey = 0;
					}
				}

				if(Keypad_Scan() == 0)
				{
					scr_state = SCR_SHOW;
					returnkey = 0;
					previouskey = 0;
				}
			}
			else
			{
				if(Keypad_Scan() == 0)
				{
					scr_state = SCR_DETECTKEY;
					returnkey = 0;
					previouskey = 0;
				}
			}
			break;
		}
	}
	return returnkey;
}

SCR_STATUS SCR_InputString(WINAPP *winapp)
{
	T8U tempcount = 0, itemshowcount = 0, returnvalue = 0;
	T8U tempstring[20];
	static T8U location=0,key=0;
	static T8U tempcursor=0x01;

	switch(winapp->scr_state)
	{
		case SCR_STATE_PREPARE:
		{
			if(Keypad_Scan() != 0)return 0;
			location = winapp->textbox[winapp->currenttextbox].text[winapp->cursor];
			winapp->scr_state = SCR_STATE_DRAWSCR;
			break;
		}
		case SCR_STATE_DRAWSCR:
		{
			clear_content_display();
			title_display((char*)&winapp->header,2,23);
			for(tempcount = 0; tempcount < MAXINPUTTEXTBOX; tempcount++)
			{
				if(winapp->textbox[tempcount].enabled != 0)
				{
					num_display(tempcount+1,2,tempcount*2*30+60);
					text_display((char*)winapp->textbox[tempcount].caption, 40, tempcount*2*30+60);
					itemshowcount++;
				}
				else
				{
					break;
				}
			}
			if (itemshowcount == 0)
			{
				winapp->currenttextbox = 0;
				winapp->scr_state = SCR_STATE_PREPARE;
				location = 0;
				winapp->cursor = 0;
				return 'B';
			}
			for(tempcount = 0; tempcount < MAXINPUTTEXTBOX; tempcount++)
			{
				if(winapp->textbox[tempcount].enabled != 0)
				{
					StrNCpy(tempstring, winapp->textbox[tempcount].text, sizeof(winapp->textbox[tempcount].text));
					text_display((char*)tempstring, 40, (tempcount*2+1)*30+60);
				}
				else
				{
					break;
				}
			}

			tempcount = winapp->currenttextbox;
			StrNCpy(tempstring, winapp->textbox[tempcount].text, sizeof(winapp->textbox[tempcount].text));
			if((tempcursor % 0x08) == 0)
			{
				tempstring[winapp->cursor] = '_';
			}
			else
			{
				tempstring[winapp->cursor] = location;
			}
			text_display((char*)tempstring, 40, (tempcount*2+1)*30+60);
			border_custom_display(35, 320 - 35 - 10, tempcount*2*30+60+1, 55);
			border_display();
			winapp->scr_state = SCR_INPUTSTATEDETECT;
			break;
		}
		case SCR_INPUTSTATEDETECT:
		{
			tempcursor++;
			key = Keypad_Scan();
			if(key != 0)
			{
				Output_AddTask(&buzzer, OUTPUT_STATE_ON, 2,0, 1);
				winapp->scr_state = SCR_INPUTSTATERELEASE;
			}
			else if((tempcursor % 0x08) == 0)
			{
				winapp->scr_state = SCR_STATE_DRAWSCR;
			}
			else if(((tempcursor-1) % 0x08) == 0)
			{
				winapp->scr_state = SCR_STATE_DRAWSCR;
			}
			break;
		}
		case SCR_INPUTSTATERELEASE:
		{
			if(Keypad_Scan() == 0)
			{
				if(key == 'C')	
				{
					tempcursor = 0x09;
					for(int i = 0; i < 10; i++)
					{
						if(location == 0)
						{
							location = '1';
							break;
						}
						else if(location == (0x30 + i))
						{
							location = 0x30 + i + 1;
							if(location > '9')
							{
								location = '0';
							}
							break;
						}
					}
					winapp->textbox[winapp->currenttextbox].text[winapp->cursor] = location;
					winapp->scr_state = SCR_STATE_DRAWSCR;
				}
				else if(key == 'D')	
				{
					tempcursor = 0x09;
					if(winapp->textbox[winapp->currenttextbox].text[winapp->cursor] == 0)
					{
						winapp->cursor = 0;
					}
					else
					{
						winapp->cursor++;
					}
					location = winapp->textbox[winapp->currenttextbox].text[winapp->cursor];
					winapp->scr_state = SCR_STATE_DRAWSCR;
				}
				else if(key == 'A')
				{
					key = 0;
					winapp->currenttextbox = 0;
					winapp->scr_state = SCR_STATE_PREPARE;
					location = 0;
					winapp->cursor = 0;
					return 'A';
				}
				else if(key == 'B')
				{
					key = 0;
					winapp->currenttextbox = 0;
					winapp->scr_state = SCR_STATE_PREPARE;
					location = 0;
					winapp->cursor = 0;
					return 'B';
				}
				key = 0;
			}
			break;
		}
		case SCR_STATE_MSG:
		{
			if (Keypad_Scan() != 0)
			{
				winapp->scr_state = SCR_STATE_DRAWSCR;
			}
			break;
		}
		default:
		{
			winapp->scr_state = SCR_STATE_PREPARE;
			break;
		}
	}
	return returnvalue;
}

SCR_STATUS SCR_InputString_2(WINAPP2 *winapp2)
{
	T8U tempcount = 0, itemshowcount = 0, returnvalue = 0;
	T8U tempstring[20];

	static T8U location=0,key=0;
	static T8U tempcursor=0x01;
	static T8U keyIn = SPECIAL_ON;
	static T8U totalitem = 0;

	switch(winapp2->scr_state)
	{
		case SCR_STATE_PREPARE:
		{
			if(Keypad_Scan() != 0)return 0;
			location = winapp2->textbox[winapp2->currenttextbox].text[winapp2->cursor];
			totalitem = 0;
			for(tempcount = 0; tempcount < MAXINPUTTEXTBOX_2; tempcount++)
			{
				if(winapp2->textbox[tempcount].enabled != 0)
				{
					totalitem++;
				}
			}

			winapp2->scr_state = SCR_STATE_DRAWSCR;
			break;
		}
		case SCR_STATE_DRAWSCR:
		{
			clear_content_display();
			title_display((char*)&winapp2->header,2,23);

			if(keyIn == SPECIAL_ON)
			{
				text_select_display((char*)&winapp2->subheader, 20, 60, LV_COLOR_WHITE, LV_COLOR_TEAL);
			}
			else
			{
				text_display((char*)&winapp2->subheader, 20, 60);
			}

			itemshowcount = 0;
			for(tempcount = 0; tempcount < MAXINPUTTEXTBOX_2; tempcount++)
			{
				if((winapp2->currenttextbox / (itemcount_perpage - 1)) != (tempcount / (itemcount_perpage - 1)))
				{
					continue;
				}
				if(winapp2->textbox[tempcount].enabled != 0)
				{
					if((tempcount == winapp2->currenttextbox) && (keyIn == false))
					{
						num_select_display(tempcount+1,2,itemshowcount*30+90,LV_COLOR_WHITE, LV_COLOR_TEAL);
						text_select_display((char*)winapp2->textbox[tempcount].caption, 40, itemshowcount*30+90, LV_COLOR_WHITE, LV_COLOR_TEAL);
						StrNCpy(tempstring, winapp2->textbox[tempcount].text, sizeof(winapp2->textbox[tempcount].text));
						text_display((char*)tempstring, 140, itemshowcount*30+90);
					}
					else if((tempcount == winapp2->currenttextbox) && (keyIn == true))
					{
						tempcount = winapp2->currenttextbox % 5;
						StrNCpy(tempstring, winapp2->textbox[winapp2->currenttextbox].text, sizeof(winapp2->textbox[winapp2->currenttextbox].text));
						if((tempcursor % 0x08) == 0)
						{
							tempstring[winapp2->cursor] = '_';
						}
						else
						{
							tempstring[winapp2->cursor] = location;
						}

						num_display(winapp2->currenttextbox+1,2,tempcount*30+90);
						text_display((char*)winapp2->textbox[winapp2->currenttextbox].caption, 40, tempcount*30+90);
						text_display((char*)tempstring, 140, tempcount*30+90);
						border_custom_display(135, 320 - 135 - 10, tempcount*30+90, 26);
					}
					else
					{
						num_display(tempcount+1,2,itemshowcount*30+90);
						text_display((char*)winapp2->textbox[tempcount].caption, 40, itemshowcount*30+90);
						StrNCpy(tempstring, winapp2->textbox[tempcount].text, sizeof(winapp2->textbox[tempcount].text));
						text_display((char*)tempstring, 140, itemshowcount*30+90);
					}
					itemshowcount++;
				}
				else if(itemshowcount == (itemcount_perpage - 1))
				{
					break;
				}
				else
				{
					break;
				}
			}
			if (itemshowcount == 0)
			{
				winapp2->currenttextbox = 0;
				winapp2->scr_state = SCR_STATE_PREPARE;
				location = 0;
				winapp2->cursor = 0;
				return 'B';
			}

			border_display();

			winapp2->scr_state = SCR_INPUTSTATEDETECT;
			key = Keypad_Scan();
			if(key != 0)
			{
				Output_AddTask(&buzzer, OUTPUT_STATE_ON, 2,0, 1);
				winapp2->scr_state = SCR_INPUTSTATERELEASE;
			}
			break;
		}
		case SCR_INPUTSTATEDETECT:
		{
			tempcursor++;
			key = Keypad_Scan();
			if(key != 0)
			{
				Output_AddTask(&buzzer, OUTPUT_STATE_ON, 2,0, 1);
				winapp2->scr_state = SCR_INPUTSTATERELEASE;
			}
			if(keyIn == true)
			{
				if((tempcursor % 0x08) == 0)
				{
					winapp2->scr_state = SCR_STATE_DRAWSCR;
				}
				else if(((tempcursor-1) % 0x08) == 0)
				{;
					winapp2->scr_state = SCR_STATE_DRAWSCR;
				}
			}
			break;
		}
		case SCR_INPUTSTATERELEASE:
		{
			if(Keypad_Scan() == 0)
			{
				if(key == 'C')	
				{
					if(keyIn == SPECIAL_ON)
					{}
					else if(keyIn == false)
					{
						if(winapp2->currenttextbox > 0)
						{
							winapp2->currenttextbox--;
						}
						else
						{
							keyIn = SPECIAL_ON;
						}
					}
					else
					{
						tempcursor = 0x09;
						for(int i = 0; i < 10; i++)
						{
							if(location == 0)
							{
								location = '1';
								break;
							}
							else if(location == (0x30 + i))
							{
								location = 0x30 + i + 1;
								if(location > '9')
								{
									location = '0';
								}
								break;
							}
						}
						winapp2->textbox[winapp2->currenttextbox].text[winapp2->cursor] = location;
					}
					winapp2->scr_state = SCR_STATE_DRAWSCR;
				}
				else if(key == 'D')	
				{
					if(keyIn == SPECIAL_ON)
					{
						keyIn = false;
					}
					else if(keyIn == false)
					{
						if(winapp2->currenttextbox < totalitem - 1)
						{
							winapp2->currenttextbox++;
						}
					}
					else
					{
						tempcursor = 0x09;
						if(winapp2->textbox[winapp2->currenttextbox].text[winapp2->cursor] == 0)
						{
							winapp2->cursor = 0;
						}
						else
						{
							winapp2->cursor++;
						}
						location = winapp2->textbox[winapp2->currenttextbox].text[winapp2->cursor];
					}
					winapp2->scr_state = SCR_STATE_DRAWSCR;
				}
				else if(key == 'A')
				{
					if(keyIn == SPECIAL_ON)
					{
						key = 0;
						winapp2->currenttextbox = 0;
						winapp2->scr_state = SCR_STATE_PREPARE;
						location = 0;
						winapp2->cursor = 0;
						return 'A';
					}
					if(keyIn == false)
					{
						keyIn = true;
					}
					else
					{
						keyIn = false;
						location = 0;
						winapp2->cursor = 0;
					}
					winapp2->scr_state = SCR_STATE_DRAWSCR;
				}
				else if(key == 'B')
				{
					if((keyIn == SPECIAL_ON) || (keyIn == false))
					{
						keyIn = SPECIAL_ON;
						key = 0;
						winapp2->currenttextbox = 0;
						winapp2->scr_state = SCR_STATE_PREPARE;
						location = 0;
						winapp2->cursor = 0;
						return 'B';
					}
					else
					{
						keyIn = false;
						memset(winapp2->textbox[winapp2->currenttextbox].text, 0, sizeof(winapp2->textbox[winapp2->currenttextbox].text));
						winapp2->scr_state = SCR_STATE_DRAWSCR;
						location = 0;
						winapp2->cursor = 0;
					}
				}
				key = 0;
			}
			break;
		}
		case SCR_STATE_MSG:
		{
			if (Keypad_Scan() != 0)
			{
				winapp2->scr_state = SCR_STATE_DRAWSCR;
			}
			break;
		}
		default:
		{
			winapp2->scr_state = SCR_STATE_PREPARE;
			break;
		}
	}
	return returnvalue;
}

T8U MsgBox(void)
{
	T8U isbusy = 0;
	static T8U itemcount=0, keytorelease=0;

	if(msgbox[0].enabled || msgbox[1].enabled )
	{
		isbusy = true;
		if(MsgBox_Timer == 0)
		{
			for(itemcount = 0; itemcount < MAX_MSGBOX; itemcount++)
			{
				if(msgbox[itemcount].enabled)
				{
					MsgBox_Timer = 25;
					clear_content_display();

					if(itemcount != 0)
					{
						if(msgbox[1].type == MSGBOX_TYPE_DONE)
						{
							msgbox[1].type = MSGBOX_IDLE;
							content_set_bg(LV_COLOR_LIME);
							text_display_with_bg_alignX((char*)msgbox[itemcount].disp, 120, LV_COLOR_BLACK, LV_COLOR_LIME, LV_ALIGN_IN_TOP_MID);
							title_display((char*)msgbox[itemcount].header, 2, 23);
						}
						else if(msgbox[1].type == MSGBOX_TYPE_ERROR)
						{
							msgbox[1].type = MSGBOX_IDLE;
							content_set_bg(LV_COLOR_RED);
							text_display_with_bg_alignX((char*)msgbox[itemcount].disp, 120, LV_COLOR_BLACK, LV_COLOR_RED, LV_ALIGN_IN_TOP_MID);
							title_display((char*)msgbox[itemcount].header, 2, 23);
						}
						else if(msgbox[1].type == MSGBOX_TYPE_ERROR2)
						{
							char tempstr[50] = {};
							msgbox[1].type = MSGBOX_IDLE;
							content_set_bg(LV_COLOR_ORANGE);
							if(CHK_DEFAULTOP_CTRL())
							{
								sprintf(tempstr, "Pre-OP [%u] not scanned!", tbl_fixop_select.previous_op);
							}
							else
							{
								sprintf(tempstr, "Pre-OP [%u] not scanned!", tbl_moop_select.previous_op);
							}
							text_display_with_bg_alignX(tempstr, 120, LV_COLOR_BLACK, LV_COLOR_ORANGE, LV_ALIGN_IN_TOP_MID);

							if(tbl_check_op.previousClaimed > 0)
							{
								sprintf(tempstr, "Last Claimed OP: %u", tbl_check_op.previousClaimed);
								text_display_with_bg_alignX(tempstr, 160, LV_COLOR_BLACK, LV_COLOR_ORANGE, LV_ALIGN_IN_TOP_MID);
							}
							else
							{

							}
							title_display((char*)msgbox[itemcount].header, 2, 23);
						}
						else if(msgbox[1].type == MSGBOX_TYPE_MSG)
						{
							msgbox[1].type = MSGBOX_IDLE;
							content_set_bg(LV_COLOR_CYAN);
							text_48_bold1_bg_alignX((char*)msgbox[itemcount].disp, 120, LV_COLOR_BLACK, LV_COLOR_CYAN, LV_ALIGN_IN_TOP_MID);
							title_display((char*)msgbox[itemcount].header, 2, 23);
						}
						else if(msgbox[1].type == MSGBOX_TYPE_SUCCESS)
						{
							msgbox[1].type = MSGBOX_IDLE;
							showScreenSuccess((T8U*)msgbox[itemcount].disp);
						}
						else
						{
							content_set_bg(LV_COLOR_WHITE);
							text_display_with_bg_alignX((char*)msgbox[itemcount].disp, 120, LV_COLOR_BLACK, LV_COLOR_WHITE, LV_ALIGN_IN_TOP_MID);
						}
						border_display();
					}
					else
					{
//						content_set_bg(LV_COLOR_WHITE);
//						title_display((char*)msgbox[itemcount].header,23);
//						text_display_with_bg_alignX((char*)msgbox[itemcount].disp, 90, LV_COLOR_BLACK, LV_COLOR_WHITE, LV_ALIGN_IN_TOP_LEFT);
//						text_display((char*)msgbox[itemcount].disp, 20, 90);
//						display_message_2(msgbox[itemcount].disp, 20, 90);
//						ESP_LOGI(TAG, "%s", msgbox[itemcount].disp);
					}

					if(msgbox[itemcount].type == MSGBOX_TYPE_USERFEEDBACK)
					{
//						tempstring[0] = 0x0F;
//						tempstring[1] = '\0';
						//sprintf(tempstring,"%c",0x0f);
//						Write_To_Screen(tempstring, 116, 126, 0, 10, WND_STYLE_ALIGN_VCENTER | WND_STYLE_ALIGN_RIGHT | WND_STYLE_REVERSE_VIDEO);
//						text_display(tempstring,40,210);
					}

					if(msgbox[itemcount].indicator & BUZZER_SLOWONE)
					{
						Output_AddTask(&buzzer, OUTPUT_STATE_ON, 20, 20, 2);
					}

					if(msgbox[itemcount].indicator & BUZZER_FASTTWO)
					{
						Output_AddTask(&buzzer, OUTPUT_STATE_BLINK, 5, 5, 4);
					}

					if(msgbox[itemcount].indicator & BUZZER_FASTTHREE)
					{
						Output_AddTask(&buzzer, OUTPUT_STATE_BLINK, 4, 4, 6);
					}

					if(msgbox[itemcount].indicator & BUZZER_FASTFIVE)
					{
						Output_AddTask(&buzzer, OUTPUT_STATE_BLINK, 2, 2, 10);
					}

					if(msgbox[itemcount].indicator & LED1_SLOWONE)
					{
						Output_AddTask(&led1, OUTPUT_STATE_ON, 20, 20, 2);
					}

					if(msgbox[itemcount].indicator & LED1LED2_FASTFIVE)
					{
						Output_AddTask(&led1, OUTPUT_STATE_BLINK, 0, 2, 10);
					}

					break;
				}
		   	}
		}
		else
		{
			if(msgbox[itemcount].type == MSGBOX_TYPE_USERFEEDBACK)
			{

				if(keytorelease == 0)
				{
					if(Keypad_Scan() == 'B')
					{
						keytorelease = 1;
						Output_AddTask(&buzzer, OUTPUT_STATE_BLINK, 2, 2, 2);
					}
				}
				else
				{
					if(Keypad_Scan() == 0)
					{
						keytorelease = 0;
						msgbox[itemcount].enabled = 0;
						msgbox[itemcount].type = MSGBOX_TYPE_TIMEOUT;
						MsgBox_Timer = 0;
						isbusy = 0;

					}
				}
			}
			else
			{
				MsgBox_Timer--;
				if(MsgBox_Timer==0)
				{

					msgbox[itemcount].enabled=0;
					Output_AddTask(&buzzer, OUTPUT_STATE_OFF, 2, 0, 1);
					isbusy=0;
				}
			}
		}
	}

	return isbusy;
}

const T8U DISPLAYON[] = {"ON"};
const T8U DISPLAYOFF[] = {"OFF"};
const T8U DISPLAYYES[] = {"YES"};
const T8U DISPLAYNO[] = {"NO"};
const T8U DISPLAYOK[] = {"OK"};
const T8U DISPLAYNOTOK[] = {"X OK"};
KEYPRESS SCR_ONOFF(T8U *SCR_HEADERS, T8U *SCR_MESSAGES, T8U YESNO_ORONOFF, T8U *ON_OR_OFF)
{
	T8U border=0,returnkey=0;
	T8U *ptr1;
	T8U *ptr2;
	static T8U sm_scr_onoff=0,loopcount=0,key=0;
	static T8U currentONOFF =0;
	if(sm_scr_onoff == 0)
	{
		clear_content_display();
		title_display((char*)SCR_HEADERS,2,23);
		currentONOFF = *ON_OR_OFF;
		text_display((char *)SCR_MESSAGES, 40, 90);
		sm_scr_onoff = 1;
		vTaskDelay(10);
	}
	else if(sm_scr_onoff == 1)
	{
		if(YESNO_ORONOFF == ONOFF)
		{
			ptr1 = (T8U *)DISPLAYON;
			ptr2 = (T8U *)DISPLAYOFF;
		}
		else if(YESNO_ORONOFF == OKORNOTOK)
		{
			ptr1 = (T8U *)DISPLAYOK;
			ptr2 = (T8U *)DISPLAYNOTOK;
		}
		else
		{
			ptr1 = (T8U *)DISPLAYYES;
			ptr2 = (T8U *)DISPLAYNO;
		}

		if(currentONOFF == OFF)
		{
			border = BUTTON_BORDER2;
		}
		else if(currentONOFF == ON)
		{
			border = BUTTON_BORDER1;
		}

		two_button_display((char*)ptr1, (char*)ptr2, 40, 160, 190, 160, border);
		sm_scr_onoff = 2;
		border_display();
	}
	else
	{
		if (loopcount==0)
		{
			key = Keypad_Scan();
			if(key != 0)
			{
				Output_AddTask(&buzzer, OUTPUT_STATE_BLINK, 2,2, 2);
				if((key >= 'A') && (key <= 'D'))
				{
					loopcount=10;
				}
			}
		}
		else
		{
			if ((--loopcount)==0)
			{
				returnkey = key;
				if((key == 'C') || (key == 'D'))
				{
					currentONOFF = (currentONOFF  == OFF) ? (ON) : (OFF);
					*ON_OR_OFF = currentONOFF;
					sm_scr_onoff=1;
				}
				else
				{
					*ON_OR_OFF = currentONOFF;
					currentONOFF = OFF;
					sm_scr_onoff=0;
				}
			}
		}
	}
	return returnkey;

}

void display_message(T8U *Message)
{
	T8U pagecount = 0;
	T8U tempstring[30] = {""};
	clear_content_display();
	for(pagecount = 0; pagecount < GetLineCount(Message); pagecount++)
	{
		GetLine(Message,tempstring,pagecount);
		text_display((char*)&tempstring,40,pagecount*30+60);
	}
	border_display();
}

void display_message_BG(T8U *Message, lv_color_t textcolor, lv_color_t bgcolor)
{
	T8U pagecount = 0;
	T8U tempstring[30] = {""};
	clear_content_display();
	content_set_bg(bgcolor);
	for(pagecount = 0; pagecount < GetLineCount(Message); pagecount++)
	{
		GetLine(Message,tempstring,pagecount);
		text_display_with_bg((char*)&tempstring, 40, pagecount*30+60, textcolor, bgcolor);
	}
	border_display();
}

void Msg_Done(T8U Done_Type)
{

	msgbox[1].enabled = true;
	msgbox[1].type = MSGBOX_TYPE_DONE;
	msgbox[1].header = (T8U *)HEADER_DONE;
	msgbox[1].indicator =  LED2_FASTTWO | BUZZER_FASTTWO;
	msgbox[1].disp = (T8U *)PTR_MESSAGE[Done_Type];
	MsgBox_Timer=0;
}

void Msg_Error(T8U Error_Type)
{
	msgbox[1].enabled = true;
	msgbox[1].type = MSGBOX_TYPE_ERROR;
	msgbox[1].header = (T8U *)HEADER_ERROR;
	msgbox[1].indicator = LED1_SLOWONE | BUZZER_SLOWONE;
	msgbox[1].disp = (T8U *)PTR_MESSAGE[Error_Type];
	MsgBox_Timer=0;
}

void Msg_Error2(T8U Error_Type)
{
	msgbox[1].enabled = true;
	msgbox[1].type = MSGBOX_TYPE_ERROR2;
	msgbox[1].header = (T8U *)HEADER_ERROR;
	msgbox[1].indicator = LED1_SLOWONE | BUZZER_SLOWONE;
	msgbox[1].disp = (T8U *)PTR_MESSAGE[Error_Type];
	MsgBox_Timer=0;
}

void Msg_Error3(T8U *Error_msg)
{
	msgbox[1].enabled = true;
	msgbox[1].type = MSGBOX_TYPE_ERROR;
	msgbox[1].header = (T8U *)HEADER_ERROR;
	msgbox[1].indicator = LED1_SLOWONE | BUZZER_SLOWONE;
	msgbox[1].disp = Error_msg;
	MsgBox_Timer=0;
}

void Msg_Msg(T8U Msg_Type)
{
	msgbox[1].enabled = true;
	msgbox[1].type = MSGBOX_TYPE_MSG;
	msgbox[1].header = (T8U *)HEADER_MSG;
	msgbox[1].indicator = BUZZER_FASTTHREE;
	msgbox[1].disp = (T8U *)PTR_MESSAGE[Msg_Type];
	MsgBox_Timer=0;
}

void Msg_Success(T8U Success_Type)
{
	msgbox[1].enabled = true;
	msgbox[1].type = MSGBOX_TYPE_SUCCESS;
	msgbox[1].header = (T8U *)HEADER_DONE;
	msgbox[1].indicator =  LED2_FASTTWO | BUZZER_FASTTWO;
	msgbox[1].disp = (T8U *)PTR_MESSAGE[Success_Type];
	MsgBox_Timer=0;
}

void display_header(T8U *Header)
{
	title_display((char*)Header,2,23);
}

void display_message_2(T8U *tempstring, int x, int y)//cannot use message[}
{
	T8U linetoshow = 0;
	T16U datapointer[40] = {0};
	T8U newlineindicator[40] = {""};
	T8U itemtoline[20] = {""};
	T8U menucount = 0;
	T8U tempstring3[40] = {""};

	if(tempstring == message)return;
	memset(message, 0, sizeof((char*)&message));
	menucount = GetMSGPart(tempstring, datapointer, newlineindicator, itemtoline);
	for(linetoshow = 0; linetoshow < menucount; linetoshow++)
	{
		if(linetoshow > (menucount-1))
		{
			break;
		}

		memset(tempstring3, 0, sizeof(tempstring3));
		StrNCpy(tempstring3, tempstring + datapointer[linetoshow], datapointer[linetoshow + 1] - datapointer[linetoshow]);

		strcat((char*)&message, (char*)&tempstring3);
		if(linetoshow != (menucount-1))
		{
			strcat((char*)&message, "\n");
		}
	}
	text_display((char*)&message, x, y);
}

void SCR_IBUTTONEnabled(T8U IBUTTONONOFF)
{
	IBUTTON_ENABLED = IBUTTONONOFF;
}

T8U GetMSGPart(T8U *MESSAGE, T16U *target_table, T8U * NewLineIndicator, T8U * tempitemtoline)
{
	T8U tempstring[40] = {""};
	T8U i = 0, temp_prev = 0, j = 0;
	T16U wordcount = 0;
	T16U processlen = 0;
	T8U linecount = 0;
	wordcount = 0;
	processlen = 0;

	*target_table = 0;
	target_table++;
	*NewLineIndicator = ON;
	NewLineIndicator++;
	linecount++;
	while(*MESSAGE != 0)
	{
		tempstring[wordcount] = *MESSAGE;

		if(*MESSAGE =='\n')
		{
			if(*(MESSAGE + 1) != '\0')
			{
				processlen++;
				MESSAGE++;
				*target_table = processlen;
				*NewLineIndicator = ON;
				NewLineIndicator++;
				wordcount = 0;
				target_table++;
				*tempitemtoline = linecount - temp_prev;
				tempitemtoline++;
				temp_prev = linecount;
				linecount++;
				j = 0;
			}
			else
			{
				processlen--;
			}
		}
		else
		{
			if((tempstring[wordcount] == ' ') || (tempstring[wordcount] == ':') || (tempstring[wordcount] == ';') ||
			   (tempstring[wordcount] == ',') || (tempstring[wordcount] == '.') || (tempstring[wordcount] == '1') ||
			   (tempstring[wordcount] == '!') || (tempstring[wordcount] == 'i') || (tempstring[wordcount] == 'I'))
			{
				j++;
			}
			if(wordcount >= (LCD_MAXWIDTH+(j/2)))
			{
				for(i = 0; i < (LCD_MAXWIDTH+(j/2))-1; i++)
				{
					if((tempstring[wordcount - i] == ' ') || (tempstring[wordcount - i] == ','))
					{
						processlen = processlen+1-i;
						MESSAGE = MESSAGE+1-i;
						*target_table = processlen;
						*NewLineIndicator = OFF;
						NewLineIndicator++;
						wordcount = 0;
						target_table++;
						linecount++;
						break;
					}
				}
				if(i == (LCD_MAXWIDTH+(j/2))-1)
				{
					*target_table = processlen;
					wordcount = 0;
					target_table++;
					*NewLineIndicator = OFF;
					NewLineIndicator++;
					linecount++;
				}
				j = 0;
			}
		}
		wordcount++;
		processlen++;
		MESSAGE++;
	}
	processlen++;
	*target_table = processlen;
	*NewLineIndicator = OFF;
	NewLineIndicator++;
	*tempitemtoline = linecount - temp_prev;
	tempitemtoline++;
	return linecount;
}

T8U findstartitem(T8U totalline, T8U temppagestart, T8U tempcurrent, T8U * tempitemtolibe, T8U type)
{
	T8U currentline, i, j;

	currentline = 0;
	for(i = 0; i < tempcurrent; i++)
	{
		 currentline += *tempitemtolibe;
		 tempitemtolibe++;
	}

	if(type == 1)
	{
		currentline += *tempitemtolibe;

		currentline = currentline/itemcount_perpage;//5;
		currentline = currentline*itemcount_perpage;//5;

		if(currentline > temppagestart+itemcount_perpage)//+5)
		{
			currentline = temppagestart + itemcount_perpage;//+ 5;
		}

		if(currentline > totalline-1)
		{
			currentline = temppagestart;
		}
	}
	else if(type == 0)
	{
		i = currentline%itemcount_perpage;//5;
		j = currentline/itemcount_perpage;//5;
		j = j*itemcount_perpage;//5;
		if((i != 0)&&(j == temppagestart))
		{
			currentline = temppagestart;
			return currentline;
		}

		tempitemtolibe--;
		currentline -= *tempitemtolibe;

		currentline = currentline/itemcount_perpage;//5;
		currentline = currentline*itemcount_perpage;//5;

		if(currentline<temppagestart-itemcount_perpage)//-5)
		{
			currentline = temppagestart - itemcount_perpage;//- 5;
		}
	}
	return currentline;
}

T8U SCR_MenuItemV(T8U *SCR_HEADERS, T8U *SCR_MESSAGES, T8U itemcount, T8U PreSelect, T8U *PostSelect)
{
	#define SCR_INIT	0
	#define SCR_SHOW	2
	#define SCR_DETECTKEY	3
	#define SCR_RELEASEKEY	4
	#define BUTTON_TIMEOUT	1

	T8U returnkey = 0;
	T8U PageItemCount = 0;
	T8U tempstring[25] = {""};
	T8U linetoshow = 0, j = 0, y = 0;
	T8U tempcount = 0, temp = 0;
	static T8U menucount = 0, buttontimeout = 0, buttonloop = 0;
	static T8U CurrentItem = 0;
	static T8U pagestartitem = 0;
	static T8U scr_state = SCR_INIT;
	static T8U scr_selected = false;
	static T16U datapointer[40] = {0};
	static T8U newlineindicator[40] = {""};
	static T8U itemtoline[20] = {""};
	static T8U previouskey = 0;

	switch(scr_state)
	{
		case SCR_INIT:
		{
		   	CurrentItem = PreSelect;
			pagestartitem = 0;
			memset(datapointer, '\0', sizeof(datapointer));
			memset(newlineindicator, '\0', sizeof(newlineindicator));
			memset(itemtoline, '\0', sizeof(itemtoline));
			menucount = GetMSGPart(SCR_MESSAGES, datapointer, newlineindicator, itemtoline);
			if(Keypad_Scan() == 0)
			{
				scr_state = SCR_SHOW;
			}
			break;
		}
		case SCR_SHOW:
		{
			clear_content_display();
			title_display((char*)SCR_HEADERS,2,23);
			tempcount = 0;
			for(j = 0; j < CurrentItem; j++)
			{
				tempcount += itemtoline[j];
			}
			for(PageItemCount = 0; PageItemCount < itemcount_perpage; PageItemCount++)
			{
				linetoshow =  pagestartitem + PageItemCount;
				if(linetoshow > (menucount-1))
				{
					break;
				}

				y = PageItemCount*30+60;
				memset(tempstring, '\0', sizeof(tempstring));

				if((newlineindicator[linetoshow] == ON))
				{
					temp = 0;
					for(j = 0; j < 20; j++)
					{
						if(linetoshow == temp)
						{
							temp = j+1;
							break;
						}
						temp += itemtoline[j];
					}
					num_display(temp, 2, y);
				}

				if((newlineindicator[linetoshow+1] == ON)||(linetoshow == menucount-1))
				{
					StrNCpy(tempstring, SCR_MESSAGES + datapointer[linetoshow], datapointer[linetoshow + 1] - datapointer[linetoshow]-1);
				}
				else
				{
					StrNCpy(tempstring, SCR_MESSAGES + datapointer[linetoshow], datapointer[linetoshow + 1] - datapointer[linetoshow]);
				}

				if((linetoshow >= tempcount)&&(linetoshow < tempcount+itemtoline[CurrentItem]))
				{
					text_select_display((char*)&tempstring, 40, y, LV_COLOR_WHITE, LV_COLOR_TEAL);
				}
				else
				{
					text_display((char*)&tempstring, 40, y);
				}
			}
			border_display();
			scr_state = SCR_DETECTKEY;
			scr_selected = false;
			break;
		}
		case SCR_DETECTKEY:
		{
			returnkey = Keypad_Scan();
			if(returnkey != 0)
			{
				Output_AddTask(&buzzer, OUTPUT_STATE_ON, 2,0, 1);
				previouskey = returnkey;
				scr_state = SCR_RELEASEKEY;
				scr_selected = false;
				if(returnkey == 'A')
				{
						*PostSelect = CurrentItem;
						menucount = 0;
						CurrentItem = 0;
						pagestartitem = 0;
						scr_selected = true;
				}
				else if(returnkey == 'C')
		        {
					tempcount = 0;
					for(j = 0; j < CurrentItem; j++)
					{
						tempcount += itemtoline[j];
					}
					temp = pagestartitem;
					pagestartitem = findstartitem(menucount, temp, CurrentItem, itemtoline, 0);
					for(PageItemCount = 0; PageItemCount < itemcount_perpage; PageItemCount++)
					{
						linetoshow =  pagestartitem + PageItemCount;
						if(linetoshow > tempcount)
						{
							j = true;
							break;
						}
						else
						{
							j = false;
						}
					}
					if((CurrentItem > 0)&&((j == false)||(temp == pagestartitem)))
					{
						CurrentItem--;
					}
					buttontimeout = BUTTON_TIMEOUT;
				}
				else if(returnkey == 'D')
				{
					tempcount = 0;
					for(j = 0; j < CurrentItem+1; j++)
					{
						tempcount += itemtoline[j];
					}
					temp = pagestartitem;
					pagestartitem = findstartitem(menucount, temp, CurrentItem, itemtoline, 1);
					if((CurrentItem < itemcount-1)&&((pagestartitem >= tempcount)||(temp == pagestartitem)))
					{
						CurrentItem++;
					}
					buttontimeout = BUTTON_TIMEOUT;
				}
				else if(returnkey == 'B')
				{
					menucount = 0;
					CurrentItem = 0;
					pagestartitem = 0;
				}
			}
			else
			{
				buttonloop = 15;
			}
			returnkey = 0;
			break;
		}
		case SCR_RELEASEKEY:
		{
			if((previouskey == 'A') || (previouskey == 'B'))
			{
				if(Keypad_Scan() == 0)
				{
					ESP_LOGI(TAG, "Button %c", previouskey);
					returnkey = previouskey;
					previouskey = 0;
					scr_state = SCR_INIT;
				}
			}
			else
			{
				if((previouskey == 'C') || (previouskey == 'D'))
				{
					if(buttonloop > 2)
					{
						buttonloop--;
					}
					else
					{
						if(--buttontimeout == 0)
						{
							scr_state = (scr_selected==true) ?  SCR_INIT : SCR_SHOW;
							returnkey = 0;
						}
					}
				}
				if (Keypad_Scan() == 0)
				{
					previouskey = 0;
					scr_state = (scr_selected==true) ?  SCR_INIT : SCR_SHOW;
					returnkey = (scr_selected==true) ?  'A' : 0;
				}
			}
			break;
		}
	}
	return returnkey;
}

void showScreenSuccess(T8U *SCR_MESSAGES)
{
	clear_content_display();
	content_set_bg(LV_COLOR_LIME);
	text_display_with_bg_alignX((char*)SCR_MESSAGES, 60, LV_COLOR_BLACK, LV_COLOR_LIME, LV_ALIGN_IN_TOP_MID);
	img_show_success();
	border_display();
}

void display_message_wHeader(T8U *Header, T8U *Messages)
{
	T8U pagecount = 0;
	T8U tempstring[30] = {""};
	clear_content_display();
	title_display((char*)Header,2,23);
	for(pagecount = 0; pagecount < GetLineCount(Messages); pagecount++)
	{
		GetLine(Messages,tempstring,pagecount);
		text_display((char*)&tempstring,40,pagecount*30+60);
	}
	border_display();
}

T8U count = 0;
void showBigLoading(void)
{
	char tempstring[12] = "";
	count++;
	if(count > 10)count = 0;
	for(int i = 0; i < count; i++)
	{
		strcat(tempstring, ".");
	}
	clear_content_display();
	text_48_bold1_bg_alignX("LOADING", 80, LV_COLOR_BLUE_2, LV_COLOR_WHITE, LV_ALIGN_IN_TOP_MID);
	text_48_bold1_bg_alignX(tempstring, 120, LV_COLOR_BLUE_2, LV_COLOR_WHITE, LV_ALIGN_IN_TOP_MID);
	border_display();
	lv_task_handler();
}

void msg_done_immediate(T8U *SCR_MESSAGES)
{
	clear_content_display();
	content_set_bg(LV_COLOR_LIME);
	text_display_with_bg_alignX((char*)SCR_MESSAGES, 120, LV_COLOR_BLACK, LV_COLOR_LIME, LV_ALIGN_IN_TOP_MID);
	title_display("Done",2,23);
	border_display();
	lv_task_handler();
}

int cursor=0;
int cursor2=0;
char ssidd[33]={};
char ssidd2[33]={};
char tempstring[65]={};
char tempstring2[65]={};
T8U flag_cursor=0;
T32S tempbtnCount=0;
char scr_keyboard(T32S btnCount, T8U btnPress)
{
	char kybd=0;
	char temp_kybd=0;
	tempbtnCount = btnCount;
	static T8U btn_state=0;

	if(cursor<0)
	{
		cursor=0;
	}
	else if(cursor2<0)
	{
		cursor2=0;
	}

	if((btnCount>=10)&&(btnCount<20))
	{
		tempbtnCount++;
	}
	else if((btnCount>=20)&&(btnCount<30))
	{
		tempbtnCount+=2;
	}
	else if((btnCount>=30)&&(btnCount<40))
	{
		tempbtnCount+=3;
	}
	else if(btnCount>=40)
	{
		tempbtnCount+=4;
	}

	if(btnCount==44)
	{
		kybd='~';
	}
	else if(btnCount==43)
	{
		kybd=' ';
	}
	else if(btnCount==42)
	{
		kybd='^';
	}
	else if(btnCount==40)
	{
		kybd='<';
	}
	else
	{
		kybd = keyboard(tempbtnCount);
	}

	if(btnPress == 'A')
	{
		btn_state++;
		if(btn_state==2)
		{
			btn_state =0;
			Output_AddTask(&buzzer, OUTPUT_STATE_ON, 2, 2, 1);
			if(kybd=='<')
			{
				flag_txtbox =0;
				flag_kybd = 0;
				temp_kybd = '<';
				cursor=0;
				cursor2=0;
				memset(ssidd, '\0', sizeof(ssidd));
				memset(ssidd2, '\0', sizeof(ssidd));
				memset(tempstring, '\0', sizeof(tempstring));
				memset(tempstring2, '\0', sizeof(tempstring));
			}
			else if(kybd=='^')
			{
				if(flag_kybd == 0)
				{
					flag_kybd =1;
					clear_display();
					display_tbox_kybrd(tempstring,tempstring2, cursor,btnCount);
				}
				else if(flag_kybd == 1)
				{
					clear_display();
					display_tbox_kybrd(tempstring,tempstring2, cursor2,btnCount);
					flag_kybd =0;
				}
			}
			else if(kybd=='>')
			{
				if(flag_txtbox == 0)
				{
					clear_display();
					display_tbox_kybrd(tempstring,tempstring2, cursor,btnCount);
					flag_txtbox =1;
				}
				else if(flag_txtbox == 1)
				{
					clear_display();
					display_tbox_kybrd(tempstring,tempstring2, cursor2,btnCount);
					flag_txtbox =0;
				}
			}
			else if(kybd=='~')
			{
				if((strcmp(tempstring, "") ==0)&&(strcmp(tempstring2, "") ==0))
				{
					flag_txtbox =0;
					flag_kybd = 0;
					cursor=0;
					cursor2=0;
					temp_kybd = '#';
				}
				else
				{
					temp_wifi.hasSet = SPECIAL_ON;
					StrNCpy(temp_wifi.ssid, (T8U*)tempstring, sizeof(temp_wifi.ssid));
					StrNCpy(temp_wifi.pssw, (T8U*)tempstring2, sizeof(temp_wifi.pssw));
					flag_txtbox =0;
					flag_kybd = 0;
					temp_kybd = '~';
					cursor=0;
					cursor2=0;
					memset(ssidd, '\0', sizeof(ssidd));
					memset(ssidd2, '\0', sizeof(ssidd));
					memset(tempstring, '\0', sizeof(tempstring));
					memset(tempstring2, '\0', sizeof(tempstring));
				}
			}
			else
			{
				if(flag_txtbox == 0)
				{
					clear_display();
					cursor++;
					sprintf(ssidd, "%c", kybd);
					strcat(tempstring, ssidd);
					display_tbox_kybrd(tempstring,tempstring2, cursor,btnCount);
				}
				else if(flag_txtbox == 1)
				{
					clear_display();
					cursor2++;
					sprintf(ssidd2, "%c", kybd);
					strcat(tempstring2, ssidd2);
					display_tbox_kybrd(tempstring,tempstring2, cursor2,btnCount);
				}
			}
		}
	}
	else if(btnPress == 'B')
	{
		btn_state++;
		if(btn_state==2)
		{
			btn_state =0;
			Output_AddTask(&buzzer, OUTPUT_STATE_ON, 2, 2, 1);

			if(flag_txtbox == 0)
			{
				clear_display();
				cursor--;
				tempstring[cursor]='\0';
				display_tbox_kybrd(tempstring,tempstring2, cursor,btnCount);
			}
			else if(flag_txtbox == 1)
			{
				clear_display();
				cursor2--;
				tempstring2[cursor2]='\0';
				display_tbox_kybrd(tempstring,tempstring2, cursor2,btnCount);
			}
		}
	}
	else
	{
		if(cursor==0)
		{
			if(flag_txtbox == 0)
			{
				clear_display();
				display_tbox_kybrd(tempstring,tempstring2, cursor,btnCount);
			}
			else if(flag_txtbox == 1)
			{
				clear_display();
				display_tbox_kybrd(tempstring,tempstring2, cursor2,btnCount);
			}
		}
		clear_display();
		display_tbox_kybrd(tempstring,tempstring2, cursor,btnCount);
	}

	return temp_kybd;
}

void MSG_RETRY(const char * header,T8U count)
{
	clear_content_display();
	content_set_bg(LV_COLOR_RED);
	text_28_bold3_midFix2(header,150,80, LV_COLOR_BLACK, LV_COLOR_RED);
	sprintf((char*)&message, "O -> Retry");
	text_22_bold1_2((char*)&message,70,150, LV_COLOR_BLACK, LV_COLOR_RED);
	sprintf((char*)&message, "X -> Exit\t\t\t\t\t x%d",count);
	text_22_bold1_2((char*)&message,70,180, LV_COLOR_BLACK, LV_COLOR_RED);
	border_display();
}
