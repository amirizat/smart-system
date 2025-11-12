/*
 * led_mgr.h
 *
 *  Created on: 5 Nov 2020
 *      Author: Administrator
 */

#ifndef LEDBUZZ_H_
#define LEDBUZZ_H_

#include "target.h"

typedef T8U STATE;
typedef T32U DURATION;
typedef T32U COUNT;
typedef void CALLBACKFUNCTION;

#define OUTPUT_SM_ACTIVE

#define OUTPUT_STATE_IDLE	(T8U)(0x01)
#define OUTPUT_STATE_ON		(T8U)(0x02)
#define OUTPUT_STATE_OFF	(T8U)(0x04)
#define OUTPUT_STATE_BLINK	(T8U)(0x08)
#define OUTPUT_STATE_INIT	(T8U)(0x10)

#define BUZZER  GPIO_NUM_37
#define LED1    GPIO_NUM_38

#pragma pack(1)
typedef struct TASK
{
	STATE current_state;
	COUNT count;
	DURATION duration;
	DURATION reloadvalue;
	void (*out_ON)();
	void (*out_OFF)();
}TASK;
#pragma pack()

void PinOutput_init();
void PinOutput_invertState();
//******************************************************************************************
// Variable : BUZZER
//******************************************************************************************
extern TASK buzzer;
void Buzzer_ON(void);
void Buzzer_OFF(void);

//******************************************************************************************
// Variable :  LED1
//******************************************************************************************
extern TASK led1;
void Led1_ON(void);
void Led1_OFF(void);

////////////////////////////////////////////////////////////////////////////////////////////
// State Machine and Output manager
////////////////////////////////////////////////////////////////////////////////////////////
#ifdef OUTPUT_SM_ACTIVE

void Output_Mgr(void);
void Output_AddTask(TASK *task, STATE state, DURATION duration, DURATION reloadvalue, COUNT count);
void Output_SM(TASK *task);
void Output_SM_OFF(TASK *task);
void Output_SM_ON(TASK *task);
void Output_SM_Blink(TASK *task);

#endif // OUTPUT_SM_ACTIVE

#endif /* LEDBUZZ_H_ */
