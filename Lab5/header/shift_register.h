#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

# define F_CPU 10000000UL
#include <util/delay.h>

#define RCLCK 0
#define SRCLCK 1
#define SER 2
#define SRCLR 3
#define RCLCK2 4
#define SRCLR2 5
#define high_data() (PORTB |= (1<<SER))
#define low_data() (PORTB &= (~(1<<SER)))
volatile unsigned char global_reg_open;
void init_me(){
	DDRB |= ((1<<SRCLCK) | (1<<RCLCK) | (1<<SER));
}
void transmit_data_fake(unsigned char data, uint8_t reg_num){
	if(reg_num == 1){
		PORTB |= (1<<SRCLR);
		PORTB &= ~(1<<RCLCK);
	}else{
		PORTB |= (1<<SRCLR2);
		PORTB &= ~(1<<RCLCK2);
	}
	for(uint8_t i = 0;i < 8;i++){
		PORTB &= (~(1<<SRCLCK));
		if(data & 0x80){
			//If there is a 1 on 1000 0000 put a one ON SER
			high_data();
		}else{
			//Put a 0 on SER
			low_data();
		}
		PORTB |= (1<<SRCLCK);
		data = data << 1;
	}
	if(reg_num == 1){
		PORTB |= (1<<RCLCK);
		PORTB &= ~(1<<SRCLR);
	}else{
		PORTB |= (1<<RCLCK2);
		PORTB &= ~(1<<SRCLR2);
	}
}
void init_me2(){
	
	DDRB |= (1<<RCLCK2);
}
void send_pulse(){
	PORTB |= (1<<SRCLCK);
	PORTB &= (~(1<<SRCLCK));
}
void latch_on(){
	PORTB |= (1<<RCLCK);
	PORTB &= (~(1<<RCLCK));
}
void latch_on2(){
	PORTB |= (1<<RCLCK2);
	PORTB &= (~(1<<RCLCK2));
}

void transmit_data(unsigned char data,uint8_t reg_num){


	for(uint8_t i = 0; i < 8;i++){
		if(data & 0x80){
			//If there is a 1 on 1000 0000 put a one ON SER
			high_data(); 
		}else{
			//Put a 0 on SER
			low_data();
		}
		send_pulse();
		data = data << 1;
	}
	if(reg_num == 1){
		latch_on();

	}else if(reg_num == 2){
		latch_on2();
	}
	PORTB = 0x00;
	
	
}
void Wait(){
	for(uint8_t i = 0;i<30;i++){
		_delay_loop_2(0);
	}
}

#endif