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
unsigned char switch_status;
unsigned char leader_follower_status;
unsigned char current_value;
uint8_t current_status;
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
	current_status = 6;
	unsigned char leader_on = 0x00;
	initUSART(0);
    while (1) 
    {
		while(!TimerFlag){
			switch_status = ~PINB & 0x02;
		};
		TimerFlag = 0;
		current_status--;
		if(current_status <= 0){
			if(!switch_status){
				initUSART(1);
				PORTC = 0x01;
				TimerSet(500);
				leader_on = 0x01;
			}
		}
		if(leader_on == 0x01){
			if(switch_status){
				initUSART(0);
				PORTC = 0x00;
				TimerSet(1);
				current_status = 3000;
				leader_on = 0x00;
			}
		}
		if(leader_on == 0x01){
			tick();
		}else{
			instant_tick();
		}
		
	
		/* PART B
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
		*/
		
	}

}

