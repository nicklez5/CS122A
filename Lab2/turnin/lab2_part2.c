/*	Partner(s) Name & E-mail: Jackson Lu, jlu080@ucr.edu2
 *	Lab Section: 22
 *	Assignment: Lab 2  Exercise 2 
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */
#include <avr/io.h>
#include "usart_ATmega1284.h"
#include "timer.h"

enum States{Init,led_on,led_off} state;
unsigned char switch_status;
unsigned char leader_follower_status;
unsigned char current_value;
unsigned char current_status;
unsigned char quick_time;
/*
int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	initUSART(0);

	
	while (1)
	{
		instant_tick();
	}
}

*/
void new_tick(){
	if(switch_status){
		if(USART_IsSendReady(1)){
			USART_Send(0x00,1);
			PORTC = 0x01;
			leader_follower_status = 1;
		}
	}else{
		if(USART_HasReceived(0)){
			unsigned char temp;
			temp = USART_Receive(0);
			if(temp == 0x00){
				PORTA = 0x00;
			}else{
				leader_follower_status = 0;
			}
			USART_Flush(0);
		}
		PORTC = 0x00;
	}
}
void instant_tick(){
	if(USART_HasReceived(0)){
		unsigned char temp;
		temp = USART_Receive(0);
		PORTA = temp;
		USART_Flush(0);
	}
}
void tick(){
	//Transitions
	unsigned char temp_value;
	switch(state){
		case Init:
			state = led_on;
			break;
		case led_on:
			if(USART_HasTransmitted(1)){
				state = led_off;
			}else{
				state = led_on;
			}
			break;
		case led_off:
			if(USART_HasTransmitted(1)){
				state = led_on;
			}else{
				state = led_off;
			}
			break;

		default:
			state = Init;
			break;
	}
	//State Actions
	switch(state){
		case Init:
			break;
		case led_on:
			temp_value = 0xFF;
			if(USART_IsSendReady(1)){
				USART_Send(0xFF,1);
				if(USART_HasTransmitted(1)){
					USART_Flush(1);
				}
			}
			break;
		case led_off:
			temp_value = 0x00;
			if(USART_IsSendReady(1)){
				USART_Send(0x00,1);
				if(USART_HasTransmitted(1)){
					USART_Flush(1);
				}
			}
			break;
		default:
			break;
	}
	PORTA = temp_value;
	
	
};
//LEADER - USART 1 


int main(void)
{
	//Output
	DDRA = 0xFF; PORTA = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
	//Input
	DDRB = 0x00; PORTB = 0xFF;
	
    /* Replace with your application code */
	//initUSART(1);
	//initUSART(0);

	leader_follower_status = 0x00;
	TimerSet(500);
	TimerOn();

	current_value = 0x00;
	state = Init;
	switch_status = 0x00;
	current_status = 0x09;
	
    while (1) 
    {
		while(!TimerFlag){
			switch_status = ~PINB & 0x01;
		};
		TimerFlag = 0;
		if(switch_status){
			initUSART(1);
			PORTC = 0x01;
			tick();
			TimerSet(500);
		}else{
			initUSART(0);
			PORTC = 0x00;
			instant_tick();
			TimerSet(1);
		}
		
	}

}

