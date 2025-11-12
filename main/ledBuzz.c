/*
 * led.c
 *
 *  Created on: 5 Nov 2020
 *      Author: Administrator
 */
#include "ledBuzz.h"

#include "driver/gpio.h"

T8U LED_STATE = false;

void PinOutput_init()
{
	gpio_set_direction(BUZZER, GPIO_MODE_OUTPUT);

    gpio_set_direction(LED1,	GPIO_MODE_OUTPUT);

    Buzzer_OFF();
    Led1_OFF();
}

void PinOutput_invertState()
{
	if(LED_STATE)
	{
		Led1_ON();

		LED_STATE = false;
	}
	else
	{
		Led1_OFF();

		LED_STATE = true;
	}
}

//******************************************************************************************
// Variable : declare buzzer property
//******************************************************************************************
//#define BUZZER_TYPE_PEIZO
//T8U Buzzer_Switch = OFF;

TASK buzzer =
{
	OUTPUT_STATE_OFF,
	0,
	0, 0,
	(void *)Buzzer_ON,
	(void *)Buzzer_OFF
};

void Buzzer_ON(void)
{
//	if(gpio_get_level(BUZZER) != 1)
//	{
		gpio_set_level(BUZZER, 1);
//	}
}

void Buzzer_OFF(void)
{
//	if(gpio_get_level(BUZZER) != 0)
//	{
		gpio_set_level(BUZZER, 0);
//	}
}

//******************************************************************************************
// Variable :  Led1
//******************************************************************************************
TASK led1 =
{
	OUTPUT_STATE_OFF,
	0,
	0, 0,
	(void *)Led1_ON,
	(void *)Led1_OFF
};

void Led1_ON(void)
{
//	if(gpio_get_level(LED1) != 0)
//	{
		gpio_set_level(LED1, 0);
//	}
}

void Led1_OFF(void)
{
//	if(gpio_get_level(LED1) != 1)
//	{
		gpio_set_level(LED1, 1);
//	}
}

#ifdef OUTPUT_SM_ACTIVE
/******************************************************************************************
                                  Output Manager Start
******************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////
// Output_Mgr handler
///////////////////////////////////////////////////////////////////////////////////////////
void Output_Mgr(void)
{
	Output_SM(&buzzer);
	Output_SM(&led1);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Output add task
///////////////////////////////////////////////////////////////////////////////////////////
void Output_AddTask(TASK *task, STATE state, DURATION duration, DURATION reloadvalue, COUNT count)
{
	task->current_state = state;
	task->duration = duration;
	task->reloadvalue = reloadvalue;
	task->count = count;
}
/******************************************************************************************
                                  Output Manager End
******************************************************************************************/


/******************************************************************************************
                                  Output State Machine Start
******************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////
// Light State Machine handler
///////////////////////////////////////////////////////////////////////////////////////////
void Output_SM(TASK *task)
{
//	vTaskDelay(1);
	if(task->current_state==OUTPUT_STATE_IDLE)
	{
	}
	else if (task->current_state & OUTPUT_STATE_BLINK)
	{
		Output_SM_Blink(task);
	}
	else if(task->current_state & OUTPUT_STATE_ON)
	{
		Output_SM_ON(task);
	}
	else if(task->current_state & OUTPUT_STATE_OFF)
	{
		Output_SM_OFF(task);
	}
/*	else if(light->current_state & LED_STATE_INIT)
	{
		light->init_light();
		Output_SM_OFF(light);
	}
	*/
	else
	{
		task->current_state = OUTPUT_STATE_IDLE;
		Output_SM_OFF(task);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
// Output State Machine OFF State
///////////////////////////////////////////////////////////////////////////////////////////
void Output_SM_OFF(TASK *task)
{
	if ( task->duration>0)
	{
		task->out_OFF();
		task->duration--;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
// Output State Machine On State
///////////////////////////////////////////////////////////////////////////////////////////
void Output_SM_ON(TASK *task)
{
	task->out_ON();
	if (task->duration>0)
	{
		task->out_ON();
		task->duration--;

		if(task->duration == 0)
		{
			task->out_OFF();
			task->current_state = OUTPUT_STATE_IDLE;
		}
	}


}

///////////////////////////////////////////////////////////////////////////////////////////
// Output State Machine Blink State
///////////////////////////////////////////////////////////////////////////////////////////
void Output_SM_Blink(TASK *task)
{
	if (task->duration ==0)
	{
		if(task->count > 0)
		{
			task->count--;
			if(task->count ==0)
			{
				task->out_OFF();
				task->current_state = OUTPUT_STATE_IDLE;
				return;
			}
		}
		task->duration = task->reloadvalue;
		if(task->current_state & OUTPUT_STATE_ON)
		{
			task->current_state &= ~(OUTPUT_STATE_ON);
		}
		else
		{
			task->current_state |= OUTPUT_STATE_ON;
		}
	}

	if (task->current_state & OUTPUT_STATE_ON)
	{
		task->out_ON();
	}
	else
	{
		task->out_OFF();
	}
	task->duration--;
}

#endif // OUTPUT_SM_ACTIVE
