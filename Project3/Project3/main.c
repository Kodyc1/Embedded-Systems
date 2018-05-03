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


// TEMPO 
#define Whole 0.8
#define Half Whole/2.0f
#define Quarter Whole/4.0f
#define Eighth Whole/8.0f
#define Sixteenth Whole/16.0f

#define C 261.626f
#define CS 277.183f
#define D 293.665f
#define DS 311.127f
#define E 329.628f
#define F 349.228f
#define FS 369.994f
#define G 391.995f
#define GS 415.305
#define A 440.0f
#define AS 466.164f
#define B 493.883f
#define CP 523.251f
#define CSP 554.365f
#define DP 587.33f
#define DSP 622.2554f
#define EP 659.255f
#define FP 698.456f
#define FSP 739.989f
#define GP 783.991f 
#define AP 880.0f
#define BP 987.767f
#define R 1000000


// note struct e.g. quarter A = {A, Quarter}
struct note{
	float freq, duration;
};

static struct note song[95] = {
	{F, Eighth}, {A, Eighth}, {B, Quarter}, {F, Eighth}, {A, Eighth}, {B, Quarter},
	{F, Eighth}, {A, Eighth}, {B, Eighth}, {EP, Eighth}, {DP, Quarter}, {B, Eighth}, {CP, Eighth},
	{B, Eighth}, {G, Eighth}, {E, Half}, {R, Whole}, {D, Eighth}, {E, Eighth}, {G, Eighth}, {E, Quarter}, {E, Half},
	{F, Eighth}, {A, Eighth}, {B, Quarter}, {F, Eighth}, {A, Eighth}, {B, Quarter},
	{F, Eighth}, {A, Eighth}, {B, Eighth}, {EP, Eighth}, {DP, Quarter}, {B, Eighth}, {CP, Eighth},
	{EP, Eighth}, {B, Eighth}, {G, Half}, {R, Whole}, {B, Eighth}, {G, Eighth}, {D, Eighth}, {E, Quarter}, {E, Half},
	{D, Eighth}, {E, Eighth}, {F, Quarter}, {G, Eighth}, {A, Eighth}, {B, Quarter}, {CP, Eighth}, {B, Eighth}, {E, Quarter}, {E, Half},
	{D, Eighth}, {E, Eighth}, {F, Quarter}, {G, Eighth}, {A, Eighth}, {B, Quarter}, {CP, Eighth}, {DP, Eighth}, {EP, Quarter}, {EP, Half},
	{D, Eighth}, {E, Eighth}, {F, Quarter}, {G, Eighth}, {A, Eighth}, {B, Quarter}, {CP, Eighth}, {B, Eighth}, {E, Quarter}, {E, Half},
	{D, Eighth}, {C, Eighth}, {E, Eighth}, {D, Eighth},{G, Eighth},{F, Eighth}, {A, Eighth},{G, Eighth},{B,  Eighth}, {A, Eighth},{CP, Eighth},{B, Eighth},{DP, Eighth}, {CP,  Eighth},{EP, Sixteenth},{F, Sixteenth},{R, Whole},
	{DP, Sixteenth}, {EP, Whole}
};

static struct note POEKMON[80] = {
	{DP, Eighth}, {A, Eighth}, {DP, Eighth}, {AP, Quarter}, {GP, Quarter}, {FSP, Eighth}, {EP, Eighth}, {CSP, Quarter}, {CSP, Eighth}, {R, Whole}, {R, Whole}, {R, Whole}, {R, Whole},
	{CSP, Eighth}, {A, Eighth}, {CSP, Eighth}, {FSP, Quarter}, {EP, Quarter}, {CSP, Eighth}, {DP, Eighth}, {FSP, Quarter}, {FSP, Eighth}, {R, Whole}, {R, Whole}, {R, Whole}, {R, Whole},
	{DP, Eighth}, {A, Eighth}, {DP, Eighth}, {AP, Quarter}, {GP, Quarter}, {FSP, Eighth}, {EP, Eighth}, {CSP, Quarter}, {CSP, Eighth}, {R, Whole}, {R, Whole}, {R, Whole}, {R, Whole},
	{CSP, Eighth}, {A, Eighth}, {CSP, Eighth}, {FSP, Quarter}, {EP, Quarter}, {CSP, Eighth}, {DP, Half}, {R, Whole}, {R, Whole}, {R, Whole}, {R, Whole},
	{FSP, Half}, {AP, Half}, {GP, Eighth}, {AP, Eighth}, {GP, Eighth}, {FSP, Eighth}, {EP, Half}, {CSP, Half}, {EP, Half}, {FSP, Eighth}, {GP, Eighth}, {FSP, Eighth}, {EP, Eighth}, {DP, Half}, 
	{FSP, Half}, {AP, Half}, {GP, Eighth}, {FSP, Eighth}, {GP, Eighth}, {AP, Eighth}, {BP, Half}, {AP, Quarter}, {GP, Eighth}, {FSP, Eighth}, {GP, Half}, {FSP, Eighth}, {GP, Eighth}, {FSP, Eighth}, {EP, Eighth}, {DP, Half} 
	
	
};

void play_note(float freq, float duration){
	
	float period = 1/freq;
	
	float Thigh = (period/2) * 1000000;
	
	int K = duration/(period);
	
	for (int i = 0; i < K; i++){
		// fuse Ext. Crystal/Resonator High Freq: start-up time 16kck + 64ms EXTHIFXTXLES 16CK_64ms
		PORTB |= 0b00010000;
		wait_avr(Thigh);
		PORTB &= 0b11101111;
		wait_avr(Thigh);
	}
}

int main(void)
{
	MCUCSR = (1<<JTD);
	MCUCSR = (1<<JTD);
	
	ini_lcd();
	
	DDRB  = 0b00010111;
	PORTB &= 0b11101111;
	
    while (1) 
    {
		
		int number = get_key();

		switch(number){
			case 1:
				pos_lcd(0,0);
				puts_lcd2("Legend of Zelda");
				pos_lcd(1,0);
				puts_lcd2("Lost Woods");
				for (int i = 0; i < 95; i++){
					play_note(song[i].freq, song[i].duration);
				}
				clr_lcd();
				break;
			case 2:
				pos_lcd(0,0);
				puts_lcd2("POEKMON");
				pos_lcd(1,0);
				puts_lcd2("POEKMON CENTER");
				for (int i = 0; i < 80; i++){
					play_note(POEKMON[i].freq, POEKMON[i].duration);
				}
				clr_lcd();
				break;
			case 3:
				play_note(19.11, Quarter);
				break;
			case 4:
				play_note(95.55643, Quarter);
				break;
			
			
		}
				
    }
}

