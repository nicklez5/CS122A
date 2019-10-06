/*	Partner(s) Name & E-mail: Jackson Lu , jlu080@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab 1  Exercise #1 
 *	Exercise Description: [optional - include for your own benefit]
 *	3 LED's blinking
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tmpb = 0x00;
void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}
void TimerOff(){
	TCCR1B = 0x00;
}
void TimerISR(){
	TimerFlag = 1;
}
ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
//#ifdef _SIMULATE_
//#include "simAVRHeader.h"


//#endif

enum States{Init,led_0,led_1,led_2,led_3} state;
unsigned char _score = 0;
void tick(){
	
	//Transitions
	switch(state){
		case Init:
			state = led_0;
			break;
		
		case led_0:
			if(~PINA & 0x01){
				state = led_3;
				break;
			}
			state = led_1;
			break;
		
		case led_1:
			if(~PINA & 0x01){
				state = led_3;
				break;
			}
			state = led_2;
			break;
		
		case led_2:
			if(~PINA & 0x01){
				state = led_3;
				break;
			}
			state = led_0;
			break;
		case led_3:		
			if(~PINA & 0x01){
				state = led_3;
				break;
			}else{
				state = led_0;
				break;
			}
		default:
			state = Init;
			break;
	}
	
	//State actions
	switch(state){
		case led_0:
			tmpb = 0x01;
			_score += 1;
			if(_score == 2){
				tmpb = 0x09;
				_score = 0;
				break;
			}
			break;
		case led_1:
			tmpb = 0x02;
			_score += 1;
			if(_score == 2){
				tmpb = 0xA;
				_score = 0;
				break;
			}
			break;
		case led_2:
			tmpb = 0x04;
			_score += 1;
			if(_score == 2){
				tmpb = 0xC;
				_score = 0;
				break;
			}
			break;	
		case led_3:
			tmpb = 0x00;
			_score = 0;
			break;
		default:
			break;
	}
	PORTB = tmpb;

	
};

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(500);
	TimerOn();
	state = Init;
	while(1){
		tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	
}
