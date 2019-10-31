/*	Partner(s) Name & E-mail: Jackson Lu, jlu080@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab 5  Exercise 1
 *	Exercise Description: Master
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */
#include <avr/io.h>
# define F_CPU 10000000UL
#include <util/delay.h>
#include "timer.h"

#define RCLCK 0
#define SRCLCK 1
#define SER 2
#define SRCLR 3
uint8_t bits[8] = {0,0,0,0,0,0,0,0};
enum LA_States {LA_init,LA_increment,LA_decrement,LA_reset} LA_state;
unsigned char xyz;

void init_me(){
	DDRB |= ((1<<SRCLCK) | (1<<RCLCK) | (1<<SER));
}
#define high_data() (PORTB |= (1<<SER))
#define low_data() (PORTB &= (~(1<<SER)))
void send_pulse(){
	PORTB |= (1<<SRCLCK);
	PORTB &= (~(1<<SRCLCK));
}
void latch_on(){
	PORTB |= (1<<RCLCK);


	PORTB |= (~(1<<RCLCK));

}
void transmit_data(unsigned char data){
	for(uint8_t i = 0; i < 8;i++){
		if(data & 0x80){
			high_data();
		}else{
			low_data();
		}
		send_pulse();
		data = data << 1;
	}
	latch_on();
	PORTB = 0x00;
}
void Wait(){
	for(uint8_t i = 0;i<30;i++){
		_delay_loop_2(0);
	}
}
void extract_bits(unsigned char data_2){
	uint8_t x = 0;
	for(;x < 8;x++){
		if((data_2 & (1<<x)) != 0){
			bits[x] = 1;
		} else{
			bits[x] = 0;
		}
	}
}

void tick(){
	unsigned char increment_button = ~PINA & 0x01;
	unsigned char decrement_button = ~PINA & 0x02;

	
	//Transitions
	switch(LA_state){
		case LA_init:
			if((increment_button) && (!(decrement_button))){
				LA_state = LA_increment;
			}else if((decrement_button) && (!(increment_button))){
				LA_state = LA_decrement;
			}else if((increment_button) && (!(decrement_button))){
				LA_state = LA_reset;
			}
			break;
		case LA_increment:
			LA_state = LA_init;
			break;
		case LA_decrement:
			LA_state = LA_init;
			break;
		case LA_reset:
			LA_state = LA_init;
			break;
		default:
			LA_state = LA_init;
			break;
	}
	
	//State Actions 
	switch(LA_state){
		case LA_init:
			break;
		case LA_increment:
			if(xyz < 0xFF){
				xyz = xyz + 1;
				transmit_data(xyz);

			}
			break;
		case LA_decrement:
			if(xyz > 0x00){
				xyz = xyz - 1;
				transmit_data(xyz);

			}
			break;
		case LA_reset:
			xyz = 0x00;
			transmit_data(xyz);

			break;
		default:
			break;
	}
}


int main(void)
{	
	DDRA = 0x00; PORTA = 0xFF;
	init_me();
	
	
	
	//Test
	xyz = 0x04;
	
	TimerSet(100);
	TimerOn();
	transmit_data(xyz);


	LA_state = LA_init;
	
	while(1){
		tick();
		
		while(!TimerFlag){}
		TimerFlag = 0;
	}
	
}

