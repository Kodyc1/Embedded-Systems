/*
 * Project5_Music.c
 *
 * Created: 3/12/2018 11:50:17 AM
 * Author : Kody
 */ 

#include <avr/io.h>
#include "avr.h"
#include "lcd.h"
#include <stdio.h>

// TEMPO
#define W 1
#define H W/2.0f
#define Q W/4.0f
#define E W/8.0f
#define T W/12.0f
#define S W/16.0f

#define C 261.626f
#define CS 277.183f
#define D 293.665f
#define DS 311.127f
#define EM 329.628f
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
#define GSP 830.609f
#define AP 880.0f
#define ASP 932.328f
#define BP 987.767f
#define CPP 1046.50f
#define DPP 1174.66f

#define R 1000000

struct note{
	float freq, duration;
};


void play_note(float freq, float duration){
	
	float period = 1/freq;
	
	float Thigh = (period/2) * 1000000;
	
	int K = duration/(period);
	
	for (int i = 0; i < K; i++){
		// fuse Ext. Crystal/Resonator High Freq: start-up time 16kck + 64ms EXTHIFXTXLES 16CK_64ms
		PORTB |= 0b00000001;
		wait_avr(Thigh);
		PORTB &= 0b11111110;
		wait_avr(Thigh);
	}
}

static struct note victory[161] = {
	{CP, T}, {CP, T}, {CP, T}, {CP, Q}, {GS, Q}, {AS, Q}, {CP, T}, {R, W}, {AS, T}, {CP, H}, {CP, Q}, 
	{GSP, S}, {CPP, S}, {GSP, S}, {FP, S}, {GSP, E}, {GSP, S}, {CPP, S}, {GSP, S}, {FP, S}, {GSP, S}, {ASP, S}, {CPP, S}, {ASP, S}, {GSP, S}, {FP, S},
	{AP, S}, {CPP, S}, {AP, S}, {FP, S}, {AP, E}, {AP, S}, {CPP, S}, {AP, S}, {FP, S}, {AP, S}, {ASP, S}, {CPP, S}, {ASP, S}, {AP, S}, {FP, S},
	{GSP, S}, {CPP, S}, {GSP, S}, {FP, S}, {GSP, E}, {GSP, S}, {CPP, S}, {GSP, S}, {FP, S}, {GSP, S}, {ASP, S}, {CPP, S}, {ASP, S}, {GSP, S}, {FP, S},
	{GSP, S}, {CPP, S}, {GSP, S}, {EP, S}, {GSP, E}, {GSP, S}, {CPP, S}, {GSP, S}, {EP, S}, {GSP, S}, {ASP, S}, {CPP, S}, {ASP, S}, {GSP, S}, {EP, S},
	{GSP, S}, {CPP, S}, {GSP, S}, {FP, S}, {GSP, E}, {GSP, S}, {CPP, S}, {GSP, S}, {FP, S}, {GSP, S}, {ASP, S}, {CPP, S}, {ASP, S}, {GSP, S}, {FP, S},
	{AP, S}, {CPP, S}, {AP, S}, {FP, S}, {AP, E}, {AP, S}, {CPP, S}, {AP, S}, {FP, S}, {AP, S}, {ASP, S}, {CPP, S}, {ASP, S}, {AP, S}, {FP, S},
	{ASP, S}, {CSP, S}, {ASP, S}, {FSP, S}, {ASP, E}, {ASP, S}, {CSP, S}, {ASP, S}, {FSP, S}, {ASP, S}, {CPP, S}, {CSP, S}, {CPP, S}, {ASP, S}, {FSP, S},
	{ASP, S}, {CSP, S}, {ASP, S}, {FSP, S}, {ASP, E}, {ASP, S}, {CSP, S}, {ASP, S}, {FSP, S}, {ASP, S}, {CPP, S}, {CSP, S}, {CPP, S}, {ASP, S}, {FSP, S},
	{ASP, S}, {DP, S}, {ASP, S}, {FSP, S}, {ASP, E}, {ASP, S}, {DP, S}, {ASP, S}, {FSP, S}, {ASP, S}, {CPP, S}, {DP, S}, {CPP, S}, {ASP, S}, {FSP, S},
	{ASP, S}, {DP, S}, {ASP, S}, {GP, S}, {ASP, E}, {ASP, S}, {DP, S}, {ASP, S}, {GP, S}, {ASP, S}, {CPP, S}, {DP, S}, {CPP, S}, {ASP, S}, {GP, S},

};

static struct note victorytwo[150] = {
	{GSP, S}, {CPP, S}, {GSP, S}, {FP, S}, {GSP, E}, {GSP, S}, {CPP, S}, {GSP, S}, {FP, S}, {GSP, S}, {ASP, S}, {CPP, S}, {ASP, S}, {GSP, S}, {FP, S},
	{AP, S}, {CPP, S}, {AP, S}, {FP, S}, {AP, E}, {AP, S}, {CPP, S}, {AP, S}, {FP, S}, {AP, S}, {ASP, S}, {CPP, S}, {ASP, S}, {AP, S}, {FP, S},
	{GSP, S}, {CPP, S}, {GSP, S}, {FP, S}, {GSP, E}, {GSP, S}, {CPP, S}, {GSP, S}, {FP, S}, {GSP, S}, {ASP, S}, {CPP, S}, {ASP, S}, {GSP, S}, {FP, S},
	{GSP, S}, {CPP, S}, {GSP, S}, {EP, S}, {GSP, E}, {GSP, S}, {CPP, S}, {GSP, S}, {EP, S}, {GSP, S}, {ASP, S}, {CPP, S}, {ASP, S}, {GSP, S}, {EP, S},
	{GSP, S}, {CPP, S}, {GSP, S}, {FP, S}, {GSP, E}, {GSP, S}, {CPP, S}, {GSP, S}, {FP, S}, {GSP, S}, {ASP, S}, {CPP, S}, {ASP, S}, {GSP, S}, {FP, S},
	{AP, S}, {CPP, S}, {AP, S}, {FP, S}, {AP, E}, {AP, S}, {CPP, S}, {AP, S}, {FP, S}, {AP, S}, {ASP, S}, {CPP, S}, {ASP, S}, {AP, S}, {FP, S},
	{ASP, S}, {CSP, S}, {ASP, S}, {FSP, S}, {ASP, E}, {ASP, S}, {CSP, S}, {ASP, S}, {FSP, S}, {ASP, S}, {CPP, S}, {CSP, S}, {CPP, S}, {ASP, S}, {FSP, S},
	{ASP, S}, {CSP, S}, {ASP, S}, {FSP, S}, {ASP, E}, {ASP, S}, {CSP, S}, {ASP, S}, {FSP, S}, {ASP, S}, {CPP, S}, {CSP, S}, {CPP, S}, {ASP, S}, {FSP, S},
	{ASP, S}, {DP, S}, {ASP, S}, {FSP, S}, {ASP, E}, {ASP, S}, {DP, S}, {ASP, S}, {FSP, S}, {ASP, S}, {CPP, S}, {DP, S}, {CPP, S}, {ASP, S}, {FSP, S},
	{ASP, S}, {DP, S}, {ASP, S}, {FSP, S}, {ASP, E}, {ASP, S}, {DP, S}, {ASP, S}, {FSP, S}, {ASP, S}, {CPP, S}, {DP, S}, {CPP, S}, {ASP, S}, {FSP, S}	
};


void
old_wait_avr(unsigned short msec)
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

int main(void)
{
	old_wait_avr(1000); 
	
	MCUCSR = (1<<JTD);
	MCUCSR = (1<<JTD);
		
	// B0 output speaker
	// B1 input button
	// 0b 0000 0001
	DDRB = 0x01;
	// B1 zero'd out
	PORTB &= 0b11111101;
	
	// 0b 0000 0000 input
	DDRA = 0x01;
	PORTA &= 0b11111101;
	
    while (1) 
    {
		
		if (GET_BIT(PINA, 1)){
			PORTA = 1;
			old_wait_avr(150);
			for (int i = 0; i < 161; i++){
				play_note(victory[i].freq, victory[i].duration);
			}
			for (int i = 11; i < 161; i++){
				play_note(victory[i].freq, victory[i].duration);
			}
			PORTA = 0;
		}
		else{
			PORTB = 0;
			PORTA = 0;
			
		}
    }
}

