/*
 * Lab_6.c
 *
 * Created: 10/30/2019 7:29:31 PM
 * Author : 16265
 */ 

#include <avr/io.h>
#include "scheduler.h"
#include "lcd.h"
#include "incDecSM.h"

int main(void)
{
	DDRA = 0x00; PORTA = 0x0F;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
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

