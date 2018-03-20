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
#include <string.h>



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
	
	sprintf(top_row, "    2:^ Press A");
	
	sprintf(bottom_row, "4:< 8:v 6:>");
	
	// press A to start
	
	pos_lcd(0,0);
	puts_lcd2(top_row);
	pos_lcd(1,0);
	puts_lcd2(bottom_row);
	
}

void old_wait_avr(unsigned short msec)
{
	TCCR0 = 3;
	while (msec--) {
		TCNT0 = (unsigned char)(256 - (XTAL_FRQ / 64) * 0.001);
		SET_BIT(TIFR, TOV0);
		WDR();
		while (!GET_BIT(TIFR, TOV0));
	}
	TCCR0 = 0;
}


int tick(char arrow, int miss) {
	int hit = 0;
	pos_lcd(1, 0);
	puts_lcd2("          ");
	// 386
	for (int i = 0; i < 200; i++){
		if (arrow == ' ' && (miss == 2 ||  miss == 0) && i < 75) {
			pos_lcd(1, 0);
			puts_lcd2("  MISS!  ");	
		}
		else if (miss == 1 && arrow==' ' && i < 75) {
			pos_lcd(1, 0);
			puts_lcd2("  HIT!   ");
		}
		else if (i > 25 && arrow==' ' && miss == 1) {
			pos_lcd(1, 0);
			puts_lcd2("       ");	
		}
		else if(i > 75 && arrow==' ') {
			pos_lcd(1, 0);
			puts_lcd2("       ");
		}
		
		if (hit == 0) {
			int number = get_key();
			switch (number){
				// Initialize Button
				case 2:
					pos_lcd(1,0);
					put_lcd('^');
					if (arrow == '^') {
						hit = 1;
						pos_lcd(1, 2);
						puts_lcd2("HIT!");
					}
					else {
						hit = 2;
						pos_lcd(1,2);
						puts_lcd2("MISS!");
					}
					break;
				case 5:
					pos_lcd(1,0);
					put_lcd('<');
					if(arrow=='<') {
						hit = 1;
						pos_lcd(1, 2);
						puts_lcd2("HIT!");
					}
					else {
						hit = 2;
						pos_lcd(1,2);
						puts_lcd2("MISS!");
					}
					break;
				case 7:
					pos_lcd(1,0);
					put_lcd('>');
					if(arrow=='>') {
						hit = 1;
						pos_lcd(1, 2);
						puts_lcd2("HIT!");
					}
					else {
						hit = 2;
						pos_lcd(1,2);
						puts_lcd2("MISS!");
					}
					break;
				case 10:
					pos_lcd(1,0);
					put_lcd('v');
					if (arrow=='v') {
						hit = 1;
						pos_lcd(1, 2);
						puts_lcd2("HIT!");
					}
					else {
						hit = 2;
						pos_lcd(1,2);
						puts_lcd2("MISS!");
					}
					break;
			}
			//if (i > 380) {
				//if (hit == 1) {
					//pos_lcd(1, 0);
					//puts_lcd2("        ");
				//}
				//else {
					//pos_lcd(1, 0);
					//puts_lcd2("  MISS!");
				//}
			//}
		}
		


		old_wait_avr(1);		
  
	}

	return hit;
}

int play_game() {

	char arrows[5] = "<>^v";
	
	char bufff[17] = "                ";
	
	int space = 0;
	
	int score = 0;
	
	int ranum = rand() % 4;
	
	int hit = 0;
	
		
	for (int j = 0; j < 92; j++) {
		pos_lcd(0, 0);
		//sprintf(bufff, "%c", arrows[ranum]);
		puts_lcd2(bufff);
		if (j < 15) {
			hit = tick(bufff[0], 3);
		}
		else {
			hit = tick(bufff[0], hit);
		}
		if(hit == 1) {
			score++;
		}
		
		ranum = rand() % 4;
		for (int i = 0; i < 16; i++) {
			if (i == 15) {
				if (space == 0) {
					bufff[i] = arrows[ranum];
					space = 1;
				}
				else {
					bufff[i] = ' ';
					space = 0;
				}
			}
			else {
				bufff[i] = bufff[i+1];
			}
		}
		
	}
	for (int x = 0; x < 16; x++){
		pos_lcd(0, 0);
		//sprintf(bufff, "%c", arrows[ranum]);
		puts_lcd2(bufff);
		hit = tick(bufff[0], hit);
		if(hit == 1) {
			score++;
		}
		for (int y = 0; y < 16; y++) {
			if (y == 15) {
				bufff[y] = ' ';
				space = 0;
			}
			else {
				bufff[y] = bufff[y+1];
			}
		}
	}
	
	return score;
	
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
				// stop output
				CLR_BIT(PORTA, 0);
				pos_lcd(1,0);
				puts_lcd2("            ");
				int score = play_game();
				char score_ar[17];
				sprintf(score_ar, "SCORE: %d", score);
				pos_lcd(1, 0);
				puts_lcd2(score_ar);
				pos_lcd(0, 0);
				char accuracy[17];
				dtostrf(score*100/40, 3, 0, accuracy);
				//sprintf(accuracy, "ACCURACY: %d\%", score*100/46);
				puts_lcd2("Accuracy:    %");
				pos_lcd(0, 10);
				puts_lcd2(accuracy);
				break;
		}
		
		
	}
	

	
}

