/*
 * Project3.c
 *
 * Created: 2/15/2018 4:13:52 PM
 * Author : Kody
 */ 

#include <avr/io.h>
#include "avr.h"
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>



int is_pressed(int row, int column){
	
	// 0 reads input, 1 sends output
	// all row inputs from keypad
	// all columns output into rows
	// 0000 1111
	DDRC = 0x0F;
	
	// initially all rows high impedance
	// initially all columns output 1
	PORTC = 0x0F;
	
	// column constantly outputting 0 to signify press
	CLR_BIT(PORTC, column);
	
	// row is reading a weak 1 input
	SET_BIT(PORTC, row+4);
	
	// check if the bit was changed from weak 1 to strong 0
	if (!GET_BIT(PINC, row+4)){
		return 1;
	}

	return 0;
}


int get_key(void){
	for (int r = 0; r < 4; r++){
		for (int c = 0; c < 4; c++){
			if (is_pressed(r,c)){
				return c*4 + r +1;
			}
		}
	}
	return 0;
}




void reset(char *top_row, char *bottom_row){
	
	sprintf(top_row, "KONICHIWA");
	
	sprintf(bottom_row, "CHIPOTLE");
	
	// press A to start
	
	pos_lcd(0,0);
	puts_lcd2(top_row);
	pos_lcd(1,0);
	puts_lcd2(bottom_row);
	
}




int main(void)
{
	// turn off debugging
	MCUCSR = (1<<JTD);
	MCUCSR = (1<<JTD);
	
	// init lcd
	ini_lcd();
	
	
	// initial display
	char top_row[17];
	char bottom_row[17];
	reset(top_row, bottom_row);
	
	// A0 output
	// 0b 0000 0001 
	DDRA |= 0x01;

	
    while (1) 
    {
		
		int number = get_key();

		switch (number){
			// Initialize Button
			case 4:
				// send an output
				SET_BIT(PORTA, 0);
				wait_avr(1000);
				// stop output
				CLR_BIT(PORTA, 0);
				break;
		}
		
		
	}
	

	
}

