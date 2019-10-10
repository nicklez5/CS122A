/*
 * Lab2.c
 *
 * Created: 10/6/2019 3:03:49 PM
 * Author : jackson lu
 */ 

#include <avr/io.h>
#include "usart_ATmega1284.h"
#include "timer.h"

enum States{Init,led_on,led_off} state;
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
	
    /* Replace with your application code */
	initUSART(1);
	TimerSet(1000);
	TimerOn();

	PORTA = 0x01;
	state = Init;
    while (1) 
    {
		tick();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

