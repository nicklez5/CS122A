/*	Partner(s) Name & E-mail: Jackson Lu, jlu080@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab 6 Exercise 2
 *	Exercise Description: Master
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "scheduler.h"
#include "lcd.h"
#include "incDecSM.h"

ISR(PCINT2_vect){
		Flag_on = 1;

}
int main(void)
{
	DDRA = 0x00; PORTA = 0x0F;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	Move_interrupts();
	sei();
	
	LCD_init();
	
	count = 0;
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

