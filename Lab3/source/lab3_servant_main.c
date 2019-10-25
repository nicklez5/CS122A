#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <unistd.h>
#include "scheduler.h"

#include <avr/interrupt.h>
//#include "timer.h"
#include <time.h>
#define DD_MOSI 5
#define DD_MISO 6
#define DD_SS 4
#define DD_SCK 7
//Servant code
char Secret_code;
volatile unsigned char Blink_pattern;
volatile unsigned char Blink_pattern2;
unsigned char spd_bits;
unsigned char ptn_bits;

unsigned char Current_Pattern = 0x00;
unsigned long timer_speed;
//task taskz[2];
const unsigned long periodControl = 250;
const unsigned long periodPattern = 100;
unsigned long counter;


unsigned char ptnA_output = 0x00;
unsigned char ptnB_output = 0x00;
unsigned char ptnC_output = 0x00;
unsigned char ptnD_output = 0x00;

unsigned long ptnA_counter = 0;
unsigned long ptnB_counter = 0;
unsigned long ptnC_counter = 0;
unsigned long ptnD_counter = 0;


void do_speed(){
	if(spd_bits == 0x10){
		timer_speed = 2000;
		counter = 20;
	}else if(spd_bits == 0x20){
		timer_speed = 1000;
		counter = 10;
	}else if(spd_bits == 0x30){
		timer_speed = 500;
		counter = 5;
	}else if(spd_bits == 0x40){
		timer_speed = 250;
		counter = 3;
	}else if(spd_bits == 0x50){
		timer_speed = 100;
		counter = 1;
	}else if(spd_bits == 0x60){
		timer_speed = 50;
		counter = 0;
	}
}
void SPI_ServantInit(void){
	DDRB &= ~((1<<DD_MISO) | (1<<DD_SCK) | (1<<DD_SS));
	DDRB |= (1<<DD_MISO);
	//DDRB = ~((1<<DD_MOSI) | (1 << DD_SCK) | (1 << DD_SS)) | (1<<DD_MISO);
	
	//DDRB = 0x02; PORTB = 0x0D;
	SPCR = (1<<SPE);
	
}
char SPI_SlaveRecieve(void){
	
	while(!(SPSR & (1<<SPIF)))
	;
	return SPDR;
}
enum States{wait,receive};
enum States2{start_1,ptn_A,ptn_A2,ptn_B,ptn_B2,go_left,go_right,ptn_D,ptn_D2};
//State Machine that writes to a shared variable output
void waitFor(unsigned int secs){
	unsigned int retTime = time(0) + secs;
	while(time(0) < retTime);
}
enum Pattern_1{ptn1_wait,ptn1_A, ptn1_B};
enum Pattern_2{ptn2_wait,ptn2_A, ptn2_B};
enum Pattern_3{ptn3_wait,ptn3_goleft,ptn3_goright};
enum Pattern_4{ptn4_wait,ptn4_A, ptn4_B};
enum display_States{ display_display};
int CHECK_RECEIVE(int state){
	
	//Transitions
	switch(state){
		case wait:
			state = receive;
			break;
		case receive:
			state = receive;
			break;
		default:
			state = wait;
			break;
	}
	switch(state){
		case wait:
			
			break;
		case receive:
			Secret_code = SPI_SlaveRecieve();
			spd_bits = Secret_code & 0xF0;
			ptn_bits = Secret_code & 0x0F;
			do_speed();
			break;
		default:
			break;
	}
	return state;
}
void delay(unsigned long milliseconds){
	while(milliseconds--){
		_delay_us(1000);
	}
}
int PATTERN_A(int state){
	switch(state){
		case ptn1_wait:
			if(ptn_bits == 0x01){
				state = ptn1_A;
			}
			break;
		case ptn1_A:
			if(ptn_bits == 0x01){
				state = ptn1_B;
			}else{
				state = ptn1_wait;
			}
			break;
		case ptn1_B:
			if(ptn_bits == 0x01){
				state = ptn1_A;
			}else{
				state = ptn1_wait;
			}
			break;
		default:
			state = ptn1_wait;
			break;
	}
	switch(state){
		case ptn1_wait:
			ptnA_counter = 0;
			ptnA_output = 0x00;
			break;
		case ptn1_A:
			if(ptnA_counter >= counter){
				ptnA_output = 0x0F;
				ptnA_counter = 0;
			}
			//ptnA_output = 0x0F;
			break;
		case ptn1_B:
			if(ptnA_counter >= counter){
				ptnA_output = 0xF0;
				ptnA_counter = 0;
			}
			//ptnA_output = 0xF0;
			break;
		default:
			break;
	}
	ptnA_counter++;
	return state;
}

int PATTERN_B(int state){
	switch(state){
		case ptn2_wait:
			if(ptn_bits == 0x02){
				state = ptn2_A;
			}
			break;
		case ptn2_A:
			if(ptn_bits == 0x02){
				state = ptn2_B;
			}else{
				state = ptn2_wait;
			}
			break;
		case ptn2_B:
			if(ptn_bits == 0x02){
				state = ptn2_A;
			}else{
				state = ptn2_wait;
			}
			break;
		default:
			state = ptn2_wait;
			break;
	}
	switch(state){
		case ptn2_wait:
			ptnB_counter = 0;
			ptnB_output = 0x00;
			break;
		case ptn2_A:
			if(ptnB_counter >= counter){
				ptnB_output = 0xAA;
				ptnB_counter = 0;
			}
			break;
		case ptn2_B:
			if(ptnB_counter >= counter){
				ptnB_output = 0x55;
				ptnB_counter = 0;
			}
			break;
		default:
			 break;
	}
	ptnB_output++;
	return state;
}
int PATTERN_C(int state){
	switch(state){
		case ptn3_wait:
			if(ptn_bits == 0x03){
				state = ptn3_goleft;
			}
			break;
		case ptn3_goleft:
			if(ptn_bits != 0x03){
				state = ptn3_wait;
			}else if((ptn_bits == 0x03) && (Current_Pattern == 0x80)){
				state = ptn3_goright;
			}else if((ptn_bits == 0x03) && (Current_Pattern != 0x80)){
				state = ptn3_goleft;
			}
			break;
		case ptn3_goright:
			if(ptn_bits != 0x03){
				state = ptn3_wait;
			}else if((ptn_bits == 0x03) && (Current_Pattern != 0x01)){
				state = ptn3_goright;
			}else if((ptn_bits == 0x03) && (Current_Pattern == 0x01)){
				state = ptn3_goleft;
			}
			break;
		default:
			state = ptn3_wait;
			break;
	}
	switch(state){
		case ptn3_wait:
			//ptnC_counter = 0;
			ptnC_output = 0x00;
			break;
		case ptn3_goleft:
			if(Current_Pattern == 0x00){
					if(ptnC_counter >= counter){
						Current_Pattern = 0x01;
						ptnC_output = 0x01;
						ptnC_counter = 0;					
					}
			}else{
					if(ptnC_counter >= counter){
						Current_Pattern = Current_Pattern << 1;
						ptnC_output = Current_Pattern;
						ptnC_counter = 0;
					}
			}
			break;
		case ptn3_goright:
			if(Current_Pattern == 0x00){
					if(ptnC_counter >= counter){
						Current_Pattern = 0x01;
						ptnC_output = 0x01;
						ptnC_counter = 0;
					}
						
						
			}else{
				if(ptnC_counter >= counter){
					Current_Pattern = Current_Pattern >> 1;
					ptnC_output = Current_Pattern;
					ptnC_counter = 0;
				}
				
			}
			break;
		default:
			break;
	}
	ptnC_counter++;
	return state;
}
int PATTERN_D(int state){
	switch(state){
		case ptn4_wait:
			if(ptn_bits == 0x04){
				state = ptn4_A;
			}
			break;
		case ptn4_A:
			if(ptn_bits != 0x04){
				state = ptn4_wait;
			}else{
				state = ptn4_B;
			}
			break;
		case ptn4_B:
			if(ptn_bits != 0x04){
				state = ptn4_wait;
			}else{
				state = ptn4_A;
			}
			break;
		default:
			state = ptn4_wait;
			break;
	}
	switch(state){
		case ptn4_wait:
			//ptnD_counter = 0;
			ptnD_output = 0x00;
			break;
		case ptn4_A:
			if(ptnD_counter >= counter){
				ptnD_output = 0x81;
				ptnD_counter = 0;
			}
			break;
		case ptn4_B:
			if(ptnD_counter >= counter){
				ptnD_output = 0x18;
				ptnD_counter = 0;
			}
			break;
		default:
			break;
	}
	ptnD_counter++;
	return state;
}

int displaySMTick(int state){
	unsigned char output = 0x00;
	unsigned long current_count = 0;

	switch(state){
		case display_display:
			state = display_display;
			break;
		default:
			state = display_display;
			break;
	}
	switch(state){
		case display_display:
			if(ptn_bits == 0x01){
				output = ptnA_output;
			}else if(ptn_bits == 0x02){
				output = ptnB_output;
			}else if(ptn_bits == 0x03){
				output = ptnC_output;
			}else if(ptn_bits == 0x04){
				output = ptnD_output;
			}
			break;
	}

	PORTA = output;


	return state; 
}


void test_lights(){
	if(Blink_pattern == 4){
		if(Blink_pattern2 == 0){
			PORTA = 0x81;
			Blink_pattern2++;
			}else{
			PORTA = 0x18;
			Blink_pattern2 = 0;
		}
		}else if(Blink_pattern == 3){
		if(Blink_pattern2 == 0){
			PORTA = 0x80;
			Blink_pattern2++;
			}else if(Blink_pattern2 == 1){
			PORTA = 0x40;
			Blink_pattern2++;
			}else if(Blink_pattern2 == 2){
			PORTA = 0x20;
			Blink_pattern2++;
			}else if(Blink_pattern2 == 3){
			PORTA = 0x10;
			Blink_pattern2++;
			}else if(Blink_pattern2 == 4){
			PORTA = 0x08;
			Blink_pattern2++;
			}else if(Blink_pattern2 == 5){
			PORTA = 0x04;
			Blink_pattern2++;
			}else if(Blink_pattern2 == 6){
			PORTA = 0x02;
			Blink_pattern2++;
			}else if(Blink_pattern2 == 7){
			PORTA = 0x01;
			Blink_pattern2 = 0;
			}else{
			Blink_pattern2 = 0;
		}

		}else if(Blink_pattern == 2){
		if(Blink_pattern2 == 0){
			PORTA = 0xAA;
			Blink_pattern2 = 1;
			}else{
			PORTA = 0x55;
			Blink_pattern2 = 0;
		}
		
		}else if(Blink_pattern == 1){
		if(Blink_pattern2 == 0){
			PORTA = 0xF0;
			//PORTA = 0xF0;
			Blink_pattern2 = 1;
			}else{
			PORTA = 0x0F;
			//PORTA = 0x0F;
			Blink_pattern2 = 0;
		}
	}
}
void create_lights(){
	//Secret_code = SPI_SlaveRecieve();
	if(ptn_bits == 0x04){
		Blink_pattern = 4;
		//Blink Pattern 4
		}else if(ptn_bits == 0x03){
		Blink_pattern = 3;
		//Blink Pattern 3
		}else if(ptn_bits == 0x02){
		Blink_pattern = 2;
		//Blink Pattern 2
		}else if(ptn_bits == 0x01){
		//Blink Pattern 1
		Blink_pattern = 1;
		
	}
	
}


ISR(SPI_STC_vect){
	unsigned char receivedData = SPDR;
	Secret_code = receivedData;

}

void extract_bits(){
	
	//Higher bits is speed
	unsigned char temp99 = Secret_code & 0xF0;
	
	//Lower bits is pattern
	unsigned char friend = Secret_code & 0x0F;
	
	
	spd_bits = temp99;
	

	ptn_bits = (Secret_code & 0x0F);
}
int main(void)
{
	unsigned char i = 0;

	
	DDRA = 0xFF; PORTA = 0x00;
	//Secret_code = 0x00;
	//output = 0x00;
	Secret_code = 0x00;
	SPI_ServantInit();
	//task tasks[6];
	counter = 0;
	//task1 - recieve
	//task2,3,4,5 - pattern 1,2,3,4
	//task 6 - display
	//task *tasks[] = { &task1,&task2,&task3,&task4, &task5,&task6};
	//const unsigned short numTask = 6;
	
	tasks[i].state = wait;
	tasks[i].period = 100;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &CHECK_RECEIVE;
	++i;
	tasks[i].state = ptn1_wait;
	tasks[i].period = 100;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &PATTERN_A;
	++i;
	tasks[i].state = ptn2_wait;
	tasks[i].period = 100;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &PATTERN_B;
	++i;
	tasks[i].state = ptn3_wait;
	tasks[i].period = 100;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &PATTERN_C;
	++i;
	tasks[i].state = ptn4_wait;
	tasks[i].period = 100;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &PATTERN_D;
	++i;
	tasks[i].state = display_display;
	tasks[i].period = 100;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &displaySMTick;
	
	//Secret_code = 0x00;
	
	//Unnecessary
	Blink_pattern = 0;
	Blink_pattern2 = 0;
	//
	
	
	//unsigned long TL_elapsedTime = 0;
	//unsigned long TimerPeriod = 1;
	//counter = 0;
	timer_speed = 50;
	
	
	TimerSet(100);
	TimerOn();
	
	//state = wait;
	//state_2 = start_1;
	
	/* Replace with your application code */
	while (1)
	{
		
		//Secret_code = SPI_SlaveRecieve();
		//extract_bits();
		TimerISR();

		
		/*delay(tasksPeriodGCD);*/
		//delay(timer_speed);
		while(!TimerFlag);
		TimerFlag = 0;
		//PORTA = 0xFF;

	}
	return 0;
}