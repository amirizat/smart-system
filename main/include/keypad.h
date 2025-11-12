#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "target.h"

#define KEYPADA		GPIO_NUM_25
#define KEYPADB		GPIO_NUM_33
#define KEYPADC		GPIO_NUM_27
#define KEYPADD		GPIO_NUM_26


extern T8U keypad_shift;
void PinInput_init();
T8U Keypad_Scan(void);

#endif /* KEYPAD_H_ */
