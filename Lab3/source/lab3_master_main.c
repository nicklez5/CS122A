/*
 * Lab_3.c
 *
 * Created: 10/11/2019 7:23:37 PM
 * Author : jackson lu
 */
 
#include <avr/io.h>w
#include <avr/interrupt.h>
#include "lcd.h"
#include "keypad.h"
#include "timer.h"
#define BIT_MASK(bit) (1<<(bit))
#define set_bit(value,bit) ((value) |= BIT_MASK(bit))
#define clear_bit(value,bit) ((value) &= ~BIT_MASK(bit))
#define toggle_bit(value,bit,bit_value) value ^= (-bit_value ^ value) & (1UL << bit);
#define bit_clear(a,b) ((a) &= ~(1ULL<<(b)))
#define DD_SS 4
#define DD_MOSI 5
#define DD_MISO 6
#define DD_SCK 7
unsigned char keypad_value;
volatile unsigned char blink_pattern;
volatile unsigned char blink_speed;
volatile unsigned char servant_mc;
volatile unsigned char speed_setting;
volatile unsigned char ptn_value;
volatile unsigned char spd_value;
volatile unsigned char val;
unsigned char temp10;
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
void display_value(){
	LCD_WriteData(val);
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
 
unsigned char determine_pattern(){
	unsigned char temp_lyfe = 0x00;
    if(blink_pattern == 1){
		temp_lyfe = 0x01;



    }else if(blink_pattern == 2){
		temp_lyfe = 0x02;

		
    }else if(blink_pattern == 3){
		temp_lyfe = 0x03;

	 
    }else if(blink_pattern == 4){
		temp_lyfe = 0x04;

		
    }
	return temp_lyfe;

}
unsigned char get_bits(unsigned char a, char no_of_bits){
	return a & ((no_of_bits << 1) - 1);
}
unsigned char determine_speed(){
	unsigned char temp_lyfe2 = 0x00;
    if(blink_speed == 1){
		temp_lyfe2 = 0x01;
		//val = val | 0x01;
    }else if(blink_speed == 2){
		temp_lyfe2 = 0x02;
		//val = val | 0x02;
    }else if(blink_speed == 3){
		temp_lyfe2 = 0x03;
		//val = val | 0x03;
    }else if(blink_speed == 4){
		temp_lyfe2 = 0x04;
		//val = val | 0x04;
    }else if(blink_speed == 5){
		temp_lyfe2 = 0x05;
		//val = val | 0x05;
    }else if(blink_speed == 6){
		temp_lyfe2 = 0x06;
		//val = val | 0x06;

    }
	return temp_lyfe2;

	

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

void pt2(){
	unsigned char temp1 = determine_speed();
	unsigned char temp3 = determine_pattern();
	
	val = temp1 << 4;
	val |= temp3;
	//val |= temp1;

	



	
	
	
	
	/*
	lower_bits = get_bits(spd_value,8);
	higher_bits = get_bits(ptn_value,4);
	unsigned char total_value = (higher_bits << 8) | lower_bits;
	*/
	//SPI_MasterTransmit(val);
	//PORTA = lower_bits | higher_bits; 
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
    loop_counter = 1;
    keypad_value = '0';
     
     
    blink_speed = 1;
    blink_pattern = 1;
    servant_mc = 1;


    SPI_MasterInit();
    display_lcd();
    unsigned char _Data;
	unsigned char temp2;
	temp2 = 0x00;
	val = 0x00;
    //SPI_MasterTransmit(_Data);
	/*pt2();*/
    while (1) 
    {
		unsigned char temp;

		
        //PART 1
        //------
		/*
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
		*/
         
        
        temp = GetKeypadKey();
        if(temp != '\0'){
            LCD_ClearScreen();
            hardcode_func(temp);
            display_lcd();
			pt2();
			temp10 = val;
			SPI_MasterTransmit(temp10);

        }
		//SPI_MasterTransmit(temp10);
		//SPI_MasterTransmit(temp10);
		//SPI_MasterTransmit(temp10);
		//SPI_MasterTransmit(temp2);
		//SPI_MasterTransmit(temp2);
		//SPI_MasterTransmit(val);
         
    }
}
      