/*	Partner(s) Name & E-mail: Jackson Lu, jlu080@ucr.edu
 *	Lab Section: 22
 *	Assignment: Lab 3  Exercise 1
 *	Exercise Description: Master
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */


#include <avr/io.h>w
#include <avr/interrupt.h>
#include "lcd.h"
#include "keypad.h"
#include "timer.h"
#define clear_bit(a,z) (a &= ~_BV(z))
#define set_bit(a,z) (a |= _BV(z))
#define DD_SS 4
#define DD_MOSI 5
#define DD_MISO 6
#define DD_SCK 7
unsigned char keypad_value;
volatile unsigned char blink_pattern;
volatile unsigned char blink_speed;
volatile unsigned char servant_mc;
volatile unsigned char speed_setting;
void SPI_MasterInit(void){
	
	//DDRB = (1<<DD_MOSI) | (1<<DD_SCK) | (1<<DD_SS) |~(1<<DD_MISO);
	
	DDRB |= (1<<DD_MOSI) | (1<<DD_SCK) | (1<<DD_SS);
	//DDRB &= (1<<DD_MISO);
		
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);

}
void SPI_MasterTransmit(char cData){
	//clear_bit(DDRB,4);
	SPDR = cData;
	while(!(SPSR & (1<<SPIF)))
		;
	//set_bit(DDRB,4);
}

void display_blink_pattern(){
	if(blink_pattern == 1){
		LCD_DisplayString(1,"Ptrn:");
		LCD_WriteData('1');
	}else if(blink_pattern == 2){
		LCD_DisplayString(1,"Ptrn:");
		LCD_WriteData('2');
	}else if(blink_pattern == 3){
		LCD_DisplayString(1,"Ptrn:");
		LCD_WriteData('3');
	}else if(blink_pattern == 4){
		LCD_DisplayString(1,"Ptrn:");
		LCD_WriteData('4');
	}
}
void display_blink_speed(){
	if(blink_speed == 1){
		LCD_DisplayString(8," Spd:");
		LCD_WriteData('1');
	}else if(blink_speed == 2){
		LCD_DisplayString(8," Spd:");
		LCD_WriteData('2');
	}else if(blink_speed == 3){
		LCD_DisplayString(8," Spd:");
		LCD_WriteData('3');
	}else if(blink_speed == 4){
		LCD_DisplayString(8," Spd:");
		LCD_WriteData('4');
	}else if(blink_speed == 5){
		LCD_DisplayString(8," Spd:");
		LCD_WriteData('5');
	}else if(blink_speed == 6){
		LCD_DisplayString(8," Spd:");
		LCD_WriteData('6');
	}
}
void display_servant_mc(){
	if(servant_mc == 1){
		LCD_DisplayString(17,"uC:");
		LCD_WriteData('1');
	}else if(servant_mc == 2){
		LCD_DisplayString(17,"uC:");
		LCD_WriteData('2');
	}else if(servant_mc == 3){
		LCD_DisplayString(17,"uC:");
		LCD_WriteData('3');
	}
}
void display_lcd(){
	display_blink_pattern();
	display_blink_speed();
	display_servant_mc();

	
}

void hardcode_func(unsigned char c){
	if(c == '1'){
		blink_speed = 1;
		speed_setting = 1;
	}else if(c == '2'){
		blink_speed = 2;
		speed_setting = 2;
	}else if(c == '3'){
		blink_speed = 3;
		speed_setting = 3;
	}else if(c == '4'){
		blink_speed = 4;
		speed_setting = 4;
	}else if(c == '5'){
		blink_speed = 5;
		speed_setting = 5;
	}else if(c == '6'){
		blink_speed = 6;
		speed_setting = 6;
	}else if(c == '7'){
		servant_mc = 1;
		speed_setting = 7;
	}else if(c == '8'){
		servant_mc = 2;
		speed_setting = 8;
	}else if(c == '9'){
		servant_mc = 3;
		speed_setting = 9;
	}else if(c == 'A'){
		blink_pattern = 1;
		speed_setting = 10;
	}else if(c == 'B'){
		blink_pattern = 2;
		speed_setting = 11;
	}else if(c == 'C'){
		blink_pattern = 3;
		speed_setting = 12;
	}else if(c == 'D'){
		blink_pattern = 4;
		speed_setting = 13;
	}else{
		
	}
	//SPI_MasterTransmit(speed_setting);
}
void test_keypad(){
	
	unsigned char tempz;
	tempz = GetKeypadKey();
	if(tempz != '\0'){
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_WriteData(tempz);
	}
	//LCD_WriteData(tempz);
}
//Master code
int main(void)
{
	uint8_t loop_counter;
	//DDRB = 0x0D; PORTB = 0x02;
	DDRA = 0xF0; PORTA = 0x0F;   // Keypad
	DDRC = 0xFF; PORTC = 0x00;   //LCD Control Lines
	DDRD = 0xFF; PORTD = 0x00;   //LCD Data Lines
    /* Replace with your application code */
	LCD_init();
	//LCD_DisplayString(1,"Hello World");
	loop_counter = 0;
	keypad_value = '0';
	
	
	blink_speed = 1;
	blink_pattern = 1;
	servant_mc = 1;
	
	TimerSet(1000);
	TimerOn();
	speed_setting = 0;
	SPI_MasterInit();
	display_lcd();
	unsigned char _Data;
	//SPI_MasterTransmit(_Data);
    while (1) 
    {
		
		while(!TimerFlag){
		};
		TimerFlag = 0;
		if(loop_counter == 1){
			_Data = 0x01;
		}else if(loop_counter == 2){
			_Data = 0x02;
		}else if(loop_counter == 3){
			_Data = 0x03;
			loop_counter = 0;
		}
		loop_counter++;
		SPI_MasterTransmit(_Data);
		
		
		/*
		unsigned char temp;
		temp = GetKeypadKey();
		if(temp != '\0'){
			LCD_ClearScreen();
			hardcode_func(temp);
			display_lcd();
		}
		*/
		
    }
}
		//SPI_MasterTransmit(_Data);
		/*
		if(loop_counter == 1){
			SPI_MasterTransmit(1);
		}else if(loop_counter == 2){
			SPI_MasterTransmit(2);
		}else if(loop_counter == 3){
			SPI_MasterTransmit(3);
			loop_counter = 0;
		}
		*/
		/*
		unsigned char temp;
		temp = GetKeypadKey();
		if(temp != '\0'){
			LCD_ClearScreen();
			hardcode_func(temp);
			display_lcd();
		}
		*/
