/*
 * Project 1.c
 *
 * Created: 1/19/2018 2:20:28 PM
 * Author : Kody
 */ 

#include <avr/io.h>
#include "avr.h"
#include <util/delay.h>


int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		
		// 1 is LED output
		// 0 is push button input
		// DDRB = 0000 0001
		DDRB = 0x01;


		// PINB = 0000 0010 
		// if PB'1' is high, remain off
		if (GET_BIT(PINB, 1)){
			PORTB = 0;
		}	
		
		// else if PB1 is low, turn on and blink
		else{
			
			// fuse Ext. Crystal/Resonator High Freq: start-up time 16kck + 64ms EXTHIFXTXLES 16CK_64ms
			PORTB = 1;
			wait_avr(500);
			
			// fuse DEFAULT
			//_delay_ms(500);
			
			// fuse Ext. Crystal/Resonator High Freq: start-up time 16kck + 64ms EXTHIFXTXLES 16CK_64ms
			PORTB = 0;
			wait_avr(500);
			
			// fuse DEFAULT
			//_delay_ms(500);
		}
    }
	
	return 0;
}

