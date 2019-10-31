/*	Partner(s) Name & E-mail: Jackson Lu, jlu080@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab 5  Exercise 3
 *	Exercise Description: Master
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */

#include <avr/io.h>
#include "shift_register.h"
#include "timer.h"


enum LA_States {LA_init,LA_increment,LA_decrement,LA_reset} LA_state;
enum Pattern1_States{ptn1_init,shift_in,shift_out} ptn1_state;
enum Pattern2_States{ptn2_init,move_left,move_right} ptn2_state;
enum Pattern3_States{ptn3_init,big_task} ptn3_state;
enum Button_States{btn_init,_increment,_decrement,_increment2,_decrement2,reset,reset2} btn_state;
unsigned char left_side[13] = {0x80,0x40,0xC0,0x20,0xA0,0x60,0xE0,0x10,0x90,0x30,0xB0,0x70,0xF0};
unsigned char right_side[13] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0C,0x0D,0x0E,0x0F};
unsigned char xyz;
unsigned char go;
unsigned char go_2;
unsigned char highest;
unsigned char lowest;
unsigned char freshly_enter;
unsigned char ptn3_index; 
unsigned char shift_reg_num;
void pattern1_tick(uint8_t _reg){
	unsigned char temp_value = 0x00;
	//Transitions
	switch(ptn1_state){
		case ptn1_init:
			ptn1_state = shift_in;
			freshly_enter = 1;
			break;
		case shift_in:
			if(xyz == 0xFF){
				ptn1_state = shift_out;
				freshly_enter = 1;
			}else{
				ptn1_state = shift_in;
			}
			break;
		case shift_out:
			if(xyz == 0x00){
				ptn1_state = shift_in;
				freshly_enter = 1;
			}else{
				ptn1_state = shift_out;
			}
			break;
		default:
			ptn1_state = ptn1_init;
			break;
	}
	
	//State Actions:
	switch(ptn1_state){
		case ptn1_init:
			break;
		case shift_in:
			if(freshly_enter){
				highest = 0x80;
				lowest = 0x01;
				temp_value = highest | lowest;
				xyz = temp_value;
				freshly_enter = 0;
			}else{
				highest = highest >> 1;
				lowest = lowest << 1;
				temp_value = highest | lowest;
				xyz |= temp_value;			
			}
			transmit_data_fake(xyz,_reg);
			break;
		case shift_out:
			if(freshly_enter){
				highest = 0x08;
				lowest = 0x10;
				temp_value = highest | lowest;
				xyz = ~(temp_value);
				freshly_enter = 0;
			}else{
				highest = highest >> 1;
				lowest = lowest << 1;
				temp_value = highest | lowest;
				xyz ^= (temp_value);
			}
			transmit_data_fake(xyz,_reg);
			break;
		default:
			break;
	}
}
void pattern2_tick(uint8_t _reg){
	
	//Transitions
	switch(ptn2_state){
		case ptn2_init:
			ptn2_state = move_left;
			freshly_enter = 1;
			break;
		case move_left:
			if(xyz == 0x80){
				ptn2_state = move_right;
			}else{
				ptn2_state = move_left;
			}
			break;
		case move_right:
			if(xyz == 0x01){
				ptn2_state = move_left;
				freshly_enter = 1;
			}else{
				ptn2_state = move_right;
			}
			break;
		default:
			ptn2_state = ptn2_init;
			break;
	}
	
	//State actions 
	switch(ptn2_state){
		case ptn2_init:
			break;
		case move_left:
			if(freshly_enter){
				xyz = 0x01;
				freshly_enter = 0;
			}else{
				xyz = xyz << 1;
			}
			transmit_data_fake(xyz,_reg);
			break;
		case move_right:
			xyz = xyz >> 1;
			transmit_data_fake(xyz,_reg);
			break;
		default:
			break;
	}
}
void pattern3_tick(uint8_t _reg){
	unsigned char im_left = 0x00;
	unsigned char im_right = 0x00;
	//Transitions
	switch(ptn3_state){
		case ptn3_init:
			ptn3_state = big_task;
			break;
		case big_task:
			if(ptn3_index == 13){
				ptn3_state = ptn3_init;
			}else{
				ptn3_state = big_task;
			}
			break;
		default:
			ptn3_state = ptn3_init;
			break;
	}
	
	//State Actions
	switch(ptn3_state){
		case ptn3_init:
			ptn3_index = 0;
			xyz = 0x00;
			transmit_data_fake(xyz,_reg);
			break;
		case big_task:
			 im_left = left_side[ptn3_index];
			 im_right = right_side[ptn3_index];
			 xyz = (im_left | im_right);
			 transmit_data_fake(xyz,_reg);
			 ptn3_index++;
			 break;
		default:
			break;
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
				transmit_data_fake(xyz,1);

			}
			break;
		case LA_decrement:
			if(xyz > 0x00){
				xyz = xyz - 1;
				transmit_data_fake(xyz,1);

			}
			break;
		case LA_reset:
			xyz = 0x00;
			transmit_data_fake(xyz,1);

			break;
		default:
			break;
	}
}
void btn_tick(){
	unsigned char decrement_btn2 = ~PINA & 0x08;
	unsigned char increment_btn2 = ~PINA & 0x04;
	unsigned char decrement_btn = ~PINA & 0x02;
	unsigned char increment_btn = ~PINA & 0x01;

	switch(btn_state){
		case btn_init:
			if(increment_btn && !decrement_btn && !increment_btn2 && !decrement_btn2){
				btn_state = _increment;
			}else if(decrement_btn && !increment_btn && !increment_btn2 && !decrement_btn2){
				btn_state = _decrement;
			}else if(increment_btn && decrement_btn && !increment_btn2 && !decrement_btn2){
				btn_state = reset;
			}else if(!increment_btn && !decrement_btn && increment_btn2 && decrement_btn2){
				btn_state = reset2;
			}else if(!increment_btn && !decrement_btn && increment_btn2 && !decrement_btn2){
				btn_state = _increment2;
			}else if(!increment_btn && !decrement_btn && !increment_btn2 && decrement_btn2){
				btn_state = _decrement2;
			}
			break;
		case _increment:
			btn_state = btn_init;
			break;
		case _decrement:
			btn_state = btn_init;
			break;
		case _increment2:
			btn_state = btn_init;
			break;
		case _decrement2:
			btn_state = btn_init;
			break;
		case reset:
			btn_state = btn_init;
			break;
		case reset2:
			btn_state = btn_init;
			break;
		default:
			btn_state = btn_init;
			break;
	}
	
	switch(btn_state){
		case btn_init:
			break;
		case _increment:
			if(go < 3){
				go++;
				xyz = 0x00;
				transmit_data_fake(xyz,1);
				freshly_enter = 1;
			}
			break;
		case _decrement: 
			if(go > 1){
				go--;
				xyz = 0x00;
				transmit_data_fake(xyz,1);
				freshly_enter = 1;
			}
			break;
		case _increment2:
			if(go_2 < 3){
				go_2++;
				xyz = 0x00;
				transmit_data_fake(xyz,2);
				freshly_enter = 1;
			}
			break;
		case _decrement2:
			if(go_2 > 1){
				go_2--;
				xyz = 0x00;
				transmit_data_fake(xyz,2);
				freshly_enter = 1;
			}
			break;
		case reset:
			xyz = 0x00;
			if(go == 0){
				go = 1;
			}else{
				go = 0;
			}

			transmit_data_fake(xyz,1);
			freshly_enter = 1;
			break;
		case reset2:
			xyz = 0x00;
			if(go_2 == 0){
				go_2 = 1;
			}else{
				go_2 = 0;
			}

			transmit_data_fake(xyz,2);
			freshly_enter = 1;
			break;
		default:
			break;
	}
}

int main(void)
{	
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	//init_me();
	TimerSet(100);
	TimerOn();
	
	xyz = 0x00;
	highest = 0x00;
	lowest = 0x00;
	global_reg_open = 0;
	go = 0;
	go_2 = 0;
	//transmit_data_fake(xyz);


	//LA_state = LA_init;
	
	ptn1_state = ptn1_init;
 	ptn2_state = ptn2_init;
 	ptn3_state = ptn3_init;
	btn_state = btn_init;
	while(1){
		//tick();
		if(go == 1){
			pattern1_tick(1);
		}else if(go == 2){
			pattern2_tick(1);
		}else if(go == 3){
			pattern3_tick(1);
		}
		btn_tick();
		if(go_2 == 1){
			pattern1_tick(2);
		}else if(go_2 == 2){
			pattern2_tick(2);
		}else if(go_2 == 3){
			pattern3_tick(2);
		}
		//btn_tick();
		while(!TimerFlag){}
		TimerFlag = 0;
	}
	
}

