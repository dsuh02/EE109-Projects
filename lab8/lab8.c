/********************************************
 *
 *  Name: Daniel Suh
 *  Email:
 *  Section:
 *  Assignment: Lab 8 - Digital Stopwatch
 *
 ********************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcd.h"
#include "adc.h"

enum states { stop, start, lap};
volatile int state = stop;
volatile int deci=0, ones=0, tens=0;



void init_timer1(unsigned short m)
{
	// Set to CTC mode
	TCCR1B |= (1 << WGM12);

	// Enable Timer Interrupt
	TIMSK1 |= (1 << OCIE1A);
	// Load the MAX count
	// Assuming prescalar=64
	// counting to 25000 =
	// 0.1s w/ 16 MHz clock
	OCR1A = m;

	// Set prescalar = 256
	// and start counter
	TCCR1B |= (1 << CS11) | (1<<CS10);
}

int main(void)
{
	
    // Initialize the LCD
    lcd_init();

    // Initialize the ADC
    adc_init();


	
    // Write splash screen
    lcd_writecommand(1);
    lcd_moveto(0,0);
    lcd_stringout("Daniel Suh");
    lcd_moveto(1,0);
    lcd_stringout("EE109 LAB 8");
    _delay_ms(1000);
    lcd_writecommand(1);
	// Initialize the interrupts
	init_timer1(25000);
	// Enable interrupts
	sei();


	
	
	char str[4];
	sprintf(str, "%d%d.%d",tens,ones,deci);
	lcd_stringout(str);

	

    while (1) {                 // Loop forever
		// Check for button pressed
		unsigned char adc_result=adc_sample(0);
		// lcd_moveto(1,0);
		// char buf[4];
		// sprintf(buf,"%d", adc_result);
		// lcd_stringout(buf);
		if(adc_result>=46 && adc_result<=55){
			if(state==start){
				state=stop;
			}
			else if(state==stop){
				state=start;
			}
			else if(state==lap){
				state=start;
			}
			_delay_ms(200);
		}	
		else if(adc_result>95 && adc_result<106){
			if(state==start){
				state=lap;
			}
			else if(state==lap){
				state=start;
			}
			else if(state==stop){
				ones=deci=tens=0;
				state=stop;
				sprintf(str, "%d%d.%d",tens,ones,deci);
				lcd_moveto(0,0);
				lcd_stringout(str);
			}
			_delay_ms(200);
		}
		if(state==start){
			sprintf(str, "%d%d.%d",tens,ones,deci);
			lcd_moveto(0,0);
			lcd_stringout(str);
		}
		_delay_ms(100);
	}
    return 0;   /* never reached */
}

ISR(TIMER1_COMPA_vect){
	if(state==start || state==lap){
		if(deci==9){
			deci=0;
			if(ones==9){
				ones=0;
				if(tens==5){
					tens=0;
				}
				else{
					tens++;
				}
				
			}
			else{
				ones++;
			}
			
		}
		else{
			deci++;
		}
	}

	
}