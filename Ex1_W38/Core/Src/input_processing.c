/*
 * input_processing.c
 *
 *  Created on: Sep 26, 2021
 *      Author: 64BIT
 */

#include "main.h"
#define N0_OF_BUTTONS 2
#define DURATION_FOR_AUTO_INCREASING_I 100  //1s
#define DURATION_FOR_AUTO_INCREASING_II 300  //3s
// the buffer that the final result is stored after debouncing
static GPIO_PinState buttonBuffer [N0_OF_BUTTONS];
// we define two buffers for debouncing
static GPIO_PinState debounceButtonBuffer1 [N0_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer2 [N0_OF_BUTTONS];

static uint16_t counterForButtonPress1s [N0_OF_BUTTONS];

static uint8_t flagForButtonPress1s [N0_OF_BUTTONS];
static uint8_t flagForButtonPress3s [N0_OF_BUTTONS];

void button_reading ( void ) {
	for (int i = 0; i < N0_OF_BUTTONS ; i++) {
		debounceButtonBuffer2[i] = debounceButtonBuffer1[i];

		if(i==0) debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(BUTTON_1_GPIO_Port , BUTTON_1_Pin);
		if(i==1) debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(BUTTON_2_GPIO_Port , BUTTON_2_Pin);

		if( debounceButtonBuffer1[i] == debounceButtonBuffer2[i])
			buttonBuffer[i] = debounceButtonBuffer1[i];

		if( buttonBuffer[i] == GPIO_PIN_RESET ) // BUTTON IS PRESSED
		{
			if( counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING_I )
			{
				counterForButtonPress1s[i]++;
			} else { //BUTTON IS PRESSED MORE THAN 1 SECOND
				flagForButtonPress1s[i] = 1;
				if( counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING_II )
				{
					counterForButtonPress1s[i]++;
				}
				else
				{
					flagForButtonPress3s[i] = 1;
				}
			}
		}
		else //BUTTON IS REPLEASED
		{
			counterForButtonPress1s[i] = 0;
			flagForButtonPress1s[i] = 0;
			flagForButtonPress3s[i] = 0;
		}
	}
 }


GPIO_PinState get_button_value ( uint8_t index ) {
	if( index >= N0_OF_BUTTONS ) return 0xff ;
	return buttonBuffer[index];
}

unsigned char get_flag_for_button_press_1s(unsigned char index) {
	if( index >= N0_OF_BUTTONS ) return 0xff;
	return flagForButtonPress1s[index];
}

unsigned char get_flag_for_button_press_3s(unsigned char index) {
	if( index >= N0_OF_BUTTONS ) return 0xff;
	return flagForButtonPress3s[index];
}
