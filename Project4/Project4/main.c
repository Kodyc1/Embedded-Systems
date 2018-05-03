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
	
	sprintf(top_row, "Ins:--- Avg:--- ");
	
	sprintf(bottom_row, "Max:--- Min:--- ");
	
	pos_lcd(0,0);
	puts_lcd2(top_row);
	pos_lcd(1,0);
	puts_lcd2(bottom_row);
	
}

unsigned short adc_convert(){
	unsigned short x; 
	
	SET_BIT(ADMUX, 6);
		
	// Analog to Digital Status Register
	SET_BIT(ADCSRA, 7);
	SET_BIT(ADCSRA, 6);

	
	while (GET_BIT(ADCSRA,6)) {}
		
	x = ADC;
		
	CLR_BIT(ADCSRA, 7);		
	
	return x;
}


int main(void)
{
	// turn off debugging
	MCUCSR = (1<<JTD);
	MCUCSR = (1<<JTD);
	
	// init lcd
	ini_lcd();
	
	DDRA = 0;
	
	// initial display
	char top_row[17];
	char bottom_row[17];
	reset(top_row, bottom_row);

	
	// For Average voltage
	float sum = 0;
	int counter = 0;
	float avg = 0;

	// Min Max init values
	float max = -999;
	float min = 999;
						
	// Buffers for the 4 values
	char instantaneous[4];
	char maximum[4];
	char minimum[4];
	char average[4];
	
	// conversion flag
	int start = 0;
	
	// voltage
	float voltage;

	
    while (1) 
    {
		
		int number = get_key();

		switch (number){
			// Initialize Button
			case 4:
				start = 1;
				break;
			
			// Reset Button
			case 16:
				reset(top_row, bottom_row);
				start = 0;
				avg = 0;
				sum = 0;
				counter = 0;
				min = 999;
				max = -999;
				break;
		}
		
		
		if (start){
			
			voltage = adc_convert();
			voltage = (float)(voltage/1024.0)*5.0;
			
			sum += voltage;
			
			if (counter != 0)
				avg = sum/counter;

			if (voltage > max)
				max = voltage;
			
			if (voltage < min)
				min = voltage;
			
			counter++;
				
			dtostrf(voltage, 3, 1, instantaneous);
			dtostrf(min, 3, 1, minimum);
			dtostrf(max, 3, 1, maximum);
			dtostrf(avg, 3, 1, average);
					
			// Print voltage coordinates
			// (0,4) (0,12)
			// (1,4) (1,12)
			pos_lcd(0,4);
			puts_lcd2(instantaneous);
			pos_lcd(0,12);
			puts_lcd2(average);
			
			pos_lcd(1,4);
			puts_lcd2(maximum);
			pos_lcd(1,12);
			puts_lcd2(minimum);	
				
			wait_avr(500);
		}		
    }
	

	
}

