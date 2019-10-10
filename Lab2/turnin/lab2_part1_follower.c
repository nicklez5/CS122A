/*	Partner(s) Name & E-mail: Jackson Lu, jlu080@ucr.edu2
 *	Lab Section: 22
 *	Assignment: Lab 2  Exercise 1 
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */

#include <avr/io.h>
#include "usart_ATmega1284.h"

//Follower Code

void instant_tick(){
	if(USART_HasReceived(0)){
		unsigned char temp;
		temp = USART_Receive(0);
		PORTA = temp;
		USART_Flush(0);
	}
}

int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	initUSART(0);

    
    while (1) 
    {
		instant_tick();
    }
}

