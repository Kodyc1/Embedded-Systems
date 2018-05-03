///*
 //* Project2.c
 //*
 //* Created: 1/26/2018 2:27:44 PM
 //* Author : Kody
 //*/ 
//
//#include <avr/io.h>
//#include "avr.h"
//#include "lcd.h"
////#include <util/delay.h>
//
//
//int is_pressed(int row, int column){
	//
	//// 0 reads input, 1 sends output
	//// all row inputs from keypad
	//// all columns output into rows
	//// 0000 1111
	//DDRC = 0x0F;
	//
	//// initially all rows weak 1
	//// initially all columns output 1
	//PORTC = 0x0F;
	//
	//// column constantly outputting 0 to signify press
	//CLR_BIT(PORTC, column);
	//
	//// row is reading a weak 1 input
	//SET_BIT(PORTC, row+4);
	//
	//// check if the bit was changed from weak 1 to strong 0
	//if (!GET_BIT(PINC, row+4)){
		//return 1;
	//}
//
	//return 0;
//}
//
//
//int get_key(void){
	//for (int r = 0; r < 4; r++){
		//for (int c = 0; c < 4; c++){
			//if (is_pressed(r,c)){
				//return c*4 + r +1;
			//}
		//}
	//}
	//return 0;
//}
//
//
//int main(void)
//{
	//// Disable JTAG for keypad/DDRC to work
	//MCUCSR = (1<<JTD);
	//MCUCSR = (1<<JTD);
	//
	//ini_lcd();
	//
	//while (1)
	//{
		//// Push button - LED 
		//DDRB  = 0b00010111;
		//PORTB &= 0b11101111;
//
		//if (GET_BIT(PINB, 3)){
			//PORTB &= 0b11101111;
		//}
		//else{	
			//// fuse Ext. Crystal/Resonator High Freq: start-up time 16kck + 64ms EXTHIFXTXLES 16CK_64ms
			//PORTB |= 0b00010000;
			//wait_avr(500);
			//PORTB &= 0b11101111;
			//wait_avr(500);
		//}
		//
	//
		//// Keypad - LED
		//
		//int number = get_key();
//
		//if (number > 0)
		//{
			//for (int i = 0; i < number;  i++){
				//PORTB |= 0b00010000;
				//wait_avr(500);
				//PORTB &= 0b11101111;
				//wait_avr(500);
			//}
			//wait_avr(1000);
		//}
		//else{
			//PORTB &= 0b11101111;
		//}
		//
		//
		//// LCD
//
		//switch(number){
			//case 1:
				//pos_lcd(0,0);
				//put_lcd('a');
				//break;
			//default:
				//break;
		//}
		 //
		//
	//}
	//
	//return 0;
//}
//
