/*
 * lab_2.c
 *
 * Created: 10/8/2019 8:19:14 AM
 * Author : jackson lu
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

