/*	Partner(s) Name & E-mail: Jackson Lu, jlu080@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab 4  Exercise 1
 *	Exercise Description: Master
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
enum LEDState {INIT,L0,L1,L2,L3,L4,L5,L6,L7} led_state;
enum LEDS_State1 {INIT0,X0} led_state1;
enum LEDS_State2 {INIT1,X1} led_state2;
enum LEDS_State3 {INIT2,X2} led_state3;

volatile unsigned char current_pattern = 0x00;

void LEDS_Init(){
	led_state = INIT0;
}
void LEDS_Init2(){
	led_state2 = INIT1;
}
void LEDS_Init3(){
	led_state3 = INIT2;
}
void part1_tick(){
	//transitions
	switch(led_state1){
		case INIT0:
			led_state1 = X0;
			break;
		case X0:
			led_state1 = INIT0;
			break;
		default:
			led_state1 = INIT0;
			break;
	}
	//state actions
	switch(led_state1){
		case INIT:
			current_pattern = current_pattern | 0x01;
			PORTD = current_pattern;
			break;
		case X0:
			current_pattern = current_pattern & 0xFE;
			PORTD = current_pattern;
			break;
		default:
			break;
	}
}
void part2_tick(){
	//Transitions
	switch(led_state2){
		case INIT1:
			led_state2 = X1;
			break;
		case X1:
			led_state2 = INIT1;
			break;
		default:
			led_state2 = INIT1;
			break;
	}
	//State actions
	switch(led_state2){
		case INIT1:
			current_pattern = current_pattern | 0x04;
			PORTD = current_pattern;
			break;
		case X1:
			current_pattern = current_pattern & 0xFB;
			PORTD = current_pattern;
			break;
		default:
			break;
	}
}
void part3_tick(){
	//Transitions
	switch(led_state3){
		case INIT2:
			led_state3 = X2;
			break;
		case X2:
			led_state3 = INIT2;
			break;
		default:
			led_state3 = INIT2;
			break;
	}
	//State actions
	switch(led_state3){
		case INIT2:
			current_pattern = current_pattern | 0x10;
			PORTD = current_pattern;
			break;
		case X2:
			current_pattern = current_pattern & 0xF5;
			PORTD = current_pattern;
			break;
		default:
			break;
	}
}
void three_leds_tick(){
	//Transitions
	switch(led_state){
		case INIT:
			led_state = L0;
			break;
		case L0:
			led_state = INIT;
			break;
		default:
			led_state = INIT;
			break;
	}
	//Actions
	switch(led_state){
		case INIT:
			PORTD = 0x00;
			break;
		case L0:
			PORTD = 0x15;
			break;
		default:
			PORTD = 0x00;
			break;
	}
}
void LEDS_Tick(){
	//Actions
	switch(led_state){
		case INIT:
		PORTD = 0;
		break;
		case L0:
		PORTD = 1;
		break;
		case L1:
		PORTD = 2;
		break;
		case L2:
		PORTD = 4;
		break;
		case L3:
		PORTD = 8;
		break;
		case L4:
		PORTD = 16;
		break;
		case L5:
		PORTD = 32;
		break;
		case L6:
		PORTD = 64;
		break;
		case L7:
		PORTD = 128;
		break;
		default:
		PORTD = 0;
		break;
	}
	//Transitions
	switch(led_state){
		case INIT:
		led_state = L0;
		break;
		case L0:
		led_state = L1;
		break;
		case L1:
		led_state = L2;
		break;
		case L2:
		led_state = L3;
		break;
		case L3:
		led_state = L4;
		break;
		case L4:
		led_state = L5;
		break;
		case L5:
		led_state = L6;
		break;
		case L6:
		led_state = L7;
		break;
		case L7:
		led_state = L0;
		break;
		default:
		led_state = INIT;
		break;
	}
}
void part1_Task(){
	LEDS_Init();
	for(;;){
		part1_tick();
		//PORTD = current_pattern;
		vTaskDelay(500);
	}
}
void part2_Task(){
	LEDS_Init2();
	for(;;){
		part2_tick();
		vTaskDelay(1000);
	}
}
void part3_Task(){
	LEDS_Init3();
	for(;;){
		part3_tick();
		vTaskDelay(2500);
	}
}
void LedSecTask2(){
	LEDS_Init();
	for(;;){
		three_leds_tick();
		vTaskDelay(1000);
	}
}
void LedSecTask() {
	LEDS_Init();
	for(;;) {
		LEDS_Tick();
		vTaskDelay(100);
	}
}
/* Part 1
void StartSecPulse(unsigned portBASE_TYPE Priority) {
	xTaskCreate(LedSecTask2,
	(signed portCHAR *)"LedSecTask2",
	configMINIMAL_STACK_SIZE,
	NULL,
	Priority,
	NULL );
}
*/

void StartSecPulse(unsigned portBASE_TYPE Priority) {
	xTaskCreate(LedSecTask2,
	(signed portCHAR *)"LedSecTask2",
	configMINIMAL_STACK_SIZE,
	NULL,
	Priority,
	NULL );
	
}
void StartSecPulse2(unsigned portBASE_TYPE Priority){
	xTaskCreate(part2_Task,
	(signed portCHAR *)"part2_Task",
	configMINIMAL_STACK_SIZE,
	NULL,
	Priority,
	NULL );
}
void StartSecPulse3(unsigned portBASE_TYPE Priority){
	xTaskCreate(part3_Task,
	(signed portCHAR *)"part3_Task",
	configMINIMAL_STACK_SIZE,
	NULL,
	Priority,
	NULL );
}
int main(void) {
	DDRA = 0x00; PORTA=0xFF;
	DDRD = 0xFF;
	current_pattern = 0x00;
	//Start Tasks
	StartSecPulse(1);
	//StartSecPulse2(1);
	//StartSecPulse3(1);
	//RunSchedular
	vTaskStartScheduler();
	return 0;
}
