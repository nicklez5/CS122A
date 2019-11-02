/*	Partner(s) Name & E-mail: Jackson Lu, jlu080@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab 6 Exercise 3
 *	Exercise Description: Master
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "scheduler.h"
#include "lcd.h"
#include "incDecSM.h"
task* taskz;
//Part2 - Enable
// ISR(PCINT2_vect){
// 		Flag_on = 1;
// 		
// 
// 
// }
// void WDT_Init(void){
// 	cli();
// 	MCUSR &= ~(1<<WDRF);
// 	WDTCSR |= (1<<WDCE) | (1<<WDE);
// 	WDTCSR = 0x00;
// 	sei();
// 	
// }
// void WDT_Prescaler_Change(void){
// 	cli();
// 	WDTCSR |= (1<<WDCE) | (1<<WDE);
// 	WDTCSR = (1<<WDE) | (1<<WDP2) | (1<<WDP0);
// }
ISR(WDT_vect){
	WDTCSR |= (1<<WDIE);

}
void setup_WDT(void){
	cli();
	
	MCUSR = 0;
	WDTCSR = (1<<WDCE)|(1<<WDE);
	WDTCSR = 0;
	WDTCSR = (1<<WDIE)|(1<<WDP2)|(1<<WDP0);

	sei();
}
int main(void)
{
	DDRA = 0x00; PORTA = 0x0F;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	count = 0;
	//Part 2- Enable
// 	Move_interrupts();
 	setup_WDT();
	LCD_init();
	

	display_lcd();
    /* Replace with your application code */
	unsigned char i = 0;
	
	tasks[i].state = Start;
	tasks[i].period = 100;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &IncDec_tick;
	tasks[i].active = 0x01;
	++i;
	tasks[i].state = check_wake;
	tasks[i].period = 50;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &WakeDisplay_tick;
	tasks[i].active = 0x01;
	
	TimerSet(500);
	TimerOn();
	
    while (1) 
    {
		TimerISR();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

