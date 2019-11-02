#ifndef __INCDECSM_H__
#define __INCDECSM_H__

#include <avr/io.h>
#include "lcd.h"
#define buttons (~PINA & 0x07)
const unsigned char Inc = (1 << PA0); // Which pin is the increment button
const unsigned char Dec = (1 << PA1); // Which pin is the decrement button
const unsigned char Reset = (1 << PA3) | (1 << PA0); // Reset counter
const unsigned char Display_me = (1<< PA2); //Display_me

volatile unsigned char count;
enum IncDec_States { Start, Wait, IncHold, DecHold, ResetHold };
enum check_wakebtn {check_wake, show_display};
void updateCount(int incValue);
void display_lcd();
const unsigned char InitialCount = 3;
unsigned char Flag_on = 0;
unsigned char Flag_on1 = 0;
task* task_2;
task* task_3;
int WakeDisplay_tick(int state){
	
	//Transitions 
	switch(state){
		case check_wake:
		
			//Check if wake display button is pressed.
			if(buttons == Display_me){
				state = show_display;
			}
			break;
		case show_display:
			state = check_wake;
			break;
		default:
			state = check_wake;
			break;
	}
	
	//State Actions
	switch(state){
		case check_wake:
			break;
		case show_display:
			display_lcd();
// 			Flag_on = 0;
//  		task_2 = &tasks[0];
// 			task_2->active = 0x01;
// 			task_3 = &tasks[1];
// 			task_3->active = 0x00;
			break;
		default:
			break;
	}
	return state;
}
int IncDec_tick(int state) {
	
    switch (state) { // Transitions
        case Start:
            count = InitialCount;
            state = Wait;
            break;
        case Wait:
            if (buttons == Inc) {
				updateCount(1);
                state = IncHold;
            } else if (buttons == Dec) {
                updateCount(-1);
                state = DecHold;
            } else if (buttons == Reset) {
                count = 0;
                state = ResetHold;
            } else {
                state = Wait;
            }
            break;
        case IncHold:
            if (buttons == Inc) {
                state = IncHold;
            } else if (buttons == Reset) { // Intentional bug. Leave in.
                PORTD = 0xFF;
                for(;;);
            } else {
                state = Wait;
            }
            break;
        case DecHold:
            if (buttons == Dec) {
                state = DecHold;
            } else {
                state = Wait;
            }
            break;
        case ResetHold:
            if (buttons == Reset) {
                state = ResetHold;
            } else {
                state = Wait;
            }
            break;
        default:
            state = Wait;
            break;
    }
    switch (state) { // Actions
        case Start:
			break;
        case Wait:
			break;
        case IncHold:
			break;
        case DecHold:
			break;
        case ResetHold:
            break;
    }
    return state;
}
void Move_interrupts(void){

 	PCMSK0 |= (1<<PCINT2);
	PCICR |= (1<<PCIE0);
}
void display_lcd(){
	//LCD_Cursor(1);
	
	LCD_DisplayString(1,"Value: ");
	LCD_WriteData(count + '0');
	LCD_Cursor(1);
	delay_ms(600);

	LCD_ClearScreen();
}
void updateCount(int incValue) {
    const char MAX_COUNT = 9;
    const char MIN_COUNT = 0;
    unsigned char newCount = count + incValue;
    if (MIN_COUNT <= newCount && newCount <= MAX_COUNT) {
        count = newCount;
    }
}


#endif // __INCDECSM_H__
