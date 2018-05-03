/*
 * Project2.c
 *
 * Created: 1/26/2018 2:27:44 PM
 * Author : Kody
 */ 

#include <avr/io.h>
#include "avr.h"
#include "lcd.h"
//#include <util/delay.h>
#include <stdio.h>



int is_pressed(int row, int column){
	
	// 0 sends output, 1 reads in input
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



struct date_time{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	int night;
	int military;
} dt;


void reset(){
	dt.year = 2100;
	dt.month = 2;
	dt.day = 28;
	dt.hour = 11;
	dt.minute = 59;
	dt.second = 57;
	dt.night = 1;
	dt.military = 0;
}

void display(char *top_row, char *bottom_row){
	
	sprintf(top_row, "%02d/%02d/%04d", dt.month, dt.day, dt.year);
	// military time
	if (dt.military == 1)
	{
		sprintf(bottom_row, "%02d:%02d:%02d %s", dt.hour, dt.minute, dt.second, "HR");
	}
	// night time
	if (dt.night % 2 == 1 && !dt.military)
	{
		sprintf(bottom_row, "%02d:%02d:%02d %s", dt.hour, dt.minute, dt.second, "PM");
	}
	// day time
	else if (dt.night % 2 == 0 && !dt.military)
	{
		sprintf(bottom_row, "%02d:%02d:%02d %s", dt.hour, dt.minute, dt.second, "AM");
	}
	pos_lcd(0,0);
	puts_lcd2(top_row);
	pos_lcd(1,0);
	puts_lcd2(bottom_row);
}


int main(void)
{
	// Disable JTAG for keypad/DDRC to work
	MCUCSR = (1<<JTD);
	MCUCSR = (1<<JTD);
	
	ini_lcd();
	
	char top_row[11];
	char bottom_row[12];
	
	// default date and time
	reset();
		
	display(top_row, bottom_row);
	
	int edit = 0;	
		
	while (1)
	{
		int number = get_key();
		
		if (4 == number){
			edit = 1;
		}
	
		
		// edit mode
		while (edit){
			number = get_key();

			pos_lcd(1,12);
			puts_lcd2("Edit");
			
			pos_lcd(0, 11);
			switch(number){
				// exit edit mode
				case 8:
					edit = 0;
					break;
					
				// increment month
				case 1:
					puts_lcd2("Month");
					dt.month++;
					if (dt.month == 13){
						dt.month = 1;
					}
					if (dt.day > 28){
						dt.day = 28;
					}
					break;
		
				// decrement month
				case 5:
					puts_lcd2("Month");
					dt.month--;
					if (dt.month == 0){
						dt.month = 12;
					}
					if (dt.day > 28){
						dt.day = 28;
					}
					break;
				
				// increment day
				case 2:
					puts_lcd2("Day  ");
					dt.day++;
					if (dt.month == 2 && (((dt.year % 4) != 0) || ((dt.year%100==0) && dt.year% 400 != 0))){
						if (dt.day >= 29){
							dt.day = 1;
						}
					}
					if (dt.month == 2 && (dt.year % 4) == 0){
						if (dt.day >= 30){
							dt.day =  1;
						}
					}
					if (dt.month == 4 || dt.month == 6 || dt.month == 9 || dt.month == 11){
						if (dt.day >= 31){
							dt.day = 1;							
						}
					}
					if (dt.month == 1 || dt.month == 3 || dt.month == 5 || dt.month == 7 ||
					dt.month == 8 || dt.month == 10 || dt.month == 12){
						if (dt.day >= 32){
							dt.day = 1;
						}
					}
					break;
					
				// decrement day
				case 6:
					puts_lcd2("Day  ");
					dt.day--;
					if (dt.month == 2 && (((dt.year % 4) != 0) || (dt.year%100==0)) && dt.year% 400 != 0){
						if (0 == dt.day){
							dt.day = 28;
						}
					}
					if (dt.month == 2 && (dt.year % 4) == 0){
						if (0 ==  dt.day){
							dt.day =  29;
						}
					}
					if (dt.month == 4 || dt.month == 6 || dt.month == 9 || dt.month == 11){
						if (0 == dt.day){
							dt.day = 30;							
						}
					}
					if (dt.month == 1 || dt.month == 3 || dt.month == 5 || dt.month == 7 ||
					dt.month == 8 || dt.month == 10 || dt.month == 12){
						if (0 == dt.day){
							dt.day = 31;
						}
					}
					break;
					
				// increment year
				case 3:
					puts_lcd2("Year ");
					dt.year++;
					break;
				// decrement year
				case 7:
					puts_lcd2("Year ");
					dt.year--;
					break;
				
				// increment hour
				case 9:
					puts_lcd2("Hour ");
					dt.hour++;
					if (dt.military == 0){
						if (dt.hour == 13){ dt.hour = 1; }
						
					}
					else if (dt.military == 1) {
						if (dt.hour == 24){ dt.hour = 0; }
						if (dt.hour >= 12){ dt.night = 1; }
						else if (dt.hour < 12) {dt.night = 0;}
					}
					break;
				// decrement hour 
				case 13:
					puts_lcd2("Hour ");
					dt.hour--;
					if (dt.military == 0){
						if (dt.hour == 0){ dt.hour = 12; }
					}
					else{
						if (dt.hour == -1){ dt.hour = 23; }
						if (dt.hour >= 12){ dt.night = 1; }
						else if (dt.hour < 12) {dt.night = 0;}
					}
					break;
					
				// increment minutes
				case 10:
					puts_lcd2("Min  ");
					dt.minute++;
					if (dt.minute == 60){ dt.minute = 00; }
					break;
				// decrement minutes
				case 14:
					puts_lcd2("Min  ");
					dt.minute--;
					if (dt.minute == -1){ dt.minute = 59; }
					break;
					
				// increment seconds
				case 11:
					puts_lcd2("Sec  ");
					dt.second++;
					if (dt.second == 60){ dt.second = 00; }
					break;
				// decrement seconds
				case 15:
					puts_lcd2("Sec  ");
					dt.second--;
					if (dt.second == -1){ dt.second = 59; }
					break;
				
				// change am/pm
				case 12:
					puts_lcd2("AMPM ");
					dt.night++;
					dt.night %= 2;
					if (dt.military == 1){
						if (dt.hour > 12){
							dt.night = 1;
						}
						else if (dt.hour < 12){
							dt.night = 0;
						}
					}
					break;
					
				// military time
				case 16:
					if (dt.military == 0){
						dt.military = 1;
						if (dt.night == 1  && dt.hour != 12){
							dt.hour = (dt.hour + 12) % 24;
						} 
						else if (dt.night == 0){
							dt.hour = dt.hour % 12;
						}
					}
					else if (dt.military == 1){
						dt.military = 0;
						if (dt.night == 0){
							if (dt.hour == 12){
								dt.hour += 12;
							}
						}
						else if (dt.night == 1 && dt.hour != 12){
							dt.hour -= 12;
						}
					}
					break;
			}
			
			//print changes
			display(top_row, bottom_row);

			wait_avr(200);
		}
		// clear edit mode
		if (edit == 0){
			pos_lcd(0,11);
			puts_lcd2("     ");
			pos_lcd(1,12);
			puts_lcd2("    ");
		}
		
		// increment second
		dt.second++;
		
		// increment minute
		if (60 == dt.second){
			dt.second %= 60;
			dt.minute++;
		}
		
		// increment hour
		if (60 == dt.minute){
			dt.minute %= 60;
			dt.hour++;
		}
		
		// increment day if 12 AM, else change AM to PM
		if (dt.military == 0){
			if (12 == dt.hour && 00 == dt.minute && 00 == dt.second){
				if (dt.night == 1){
					dt.day++;
				}
				dt.night++;
				dt.night %= 2;
			}
			if (13 == dt.hour){
				dt.hour = 1;
			}
		}
		else {
			if (24 == dt.hour){
				dt.day++;
				dt.hour = 0;
			}
			if (dt.hour < 12){
				dt.night = 0;
			}
			else{
				dt.night = 1;
			}

		}

		// increment month
		if (dt.month == 2 && (((dt.year % 4) != 0) || (dt.year%100==0)) && dt.year% 400 != 0){
			if (29 == dt.day){
				dt.day = 1;
				dt.month++;
			}
		}
		if (dt.month == 2 && (dt.year % 4) == 0){
			if (30 ==  dt.day){
				dt.day =  1;
				dt.month++;
			}
		}
		if (dt.month == 4 || dt.month == 6 || dt.month == 9 || dt.month == 11){
			if (31 == dt.day){
				dt.day = 1;
				dt.month++;
				
			}
		}
		if (dt.month == 1 || dt.month == 3 || dt.month == 5 || dt.month == 7 || 
			dt.month == 8 || dt.month == 10 || dt.month == 12){
			if (32 == dt.day){
				dt.day = 1;
				dt.month++;
			}
		}
		
		// increment year
		if  (dt.month == 13){
			dt.month = 1;
			dt.year++;
		}
	
		
		// wait a second	
		wait_avr(1000);

		// print update
		display(top_row, bottom_row);

	}
	
	return 0;
}

