/********************************************
 *
 *  Name:
 *  Email:
 *  Section:
 *  Assignment: Lab 6 - Analog-to-digital conversion
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcd.h"
#include "adc.h"

void rand_init(void);

int main(void)
{

    // Initialize the LCD
    lcd_init();

    // Initialize the ADC
    adc_init();

    // Initialize the random number function
    rand_init();

    // Write splash screen
    lcd_writecommand(1);
    lcd_moveto(0,0);
    lcd_stringout("Daniel Suh");
    lcd_moveto(1,0);
    lcd_stringout("EE109 LAB 6");
    _delay_ms(1000);
    lcd_writecommand(1);

    PORTC |= 0x14;

    // Find a random initial position of the 'X'
    int xPos=rand()%16;
    lcd_moveto(0,xPos);

    // Display the 'X' on the screen
    lcd_moveto(0,xPos);
    lcd_stringout("X");

    int previousX=0;
    int cPos=0;
    int timer=0;


    while (1) {                 // Loop forever
	// Check for button pressed
    unsigned char adc_result=adc_sample(0);
    // char buf[4];
    // snprintf(buf,4,"&d", adc_result);
    // snprintf(buf,4);
    // lcd_moveto(0,0);
    // lcd_stringout(buf);

    // If pressed, move 'X' to new position
    if(adc_result>150 && adc_result<160){
        lcd_moveto(0,xPos);
        lcd_stringout(" ");

        if(xPos!=0){
            xPos--;
        }

        lcd_moveto(0,xPos);
        lcd_stringout("X");
        _delay_ms(100);
    }
    else if(adc_result<20){
        lcd_moveto(0,xPos);
        lcd_stringout(" ");

        if(xPos!=15){
            xPos++;
        }

        lcd_moveto(0,xPos);
        lcd_stringout("X");
        _delay_ms(100);
    }

    // Do a conversion of potentiometer input
    unsigned char potentiometer = adc_sample(3);
    //sample 3 for potentiometer

	// Move '^' to new position
    if(potentiometer==255){
        previousX=cPos;
        cPos=15;
    }
    else if(potentiometer<255){
        previousX=cPos;
        cPos=potentiometer/17;
    }

	// Delay
    lcd_moveto(1,previousX);
    lcd_stringout(" ");
    lcd_moveto(1,cPos);
    lcd_stringout("^");
    _delay_ms(10);

	// Check if '^' is aligned with 'X'
    if(xPos==cPos){
        timer++;
    }
    else if(xPos!=cPos){
        timer=0;
    }

    if(timer==90){
        lcd_writecommand(1);
        lcd_moveto(0,0);
        lcd_stringout("   GAME OVER   ");
        while(1){}
    }
    }

    return 0;   /* never reached */
}

void rand_init()
{
    int seed = 0;
    unsigned char i, j, x;

    // Build a 15-bit number from the LSBs of an ADC
    // conversion of the channels 1-5, 3 times each
    for (i = 0; i < 3; i++) {
        for (j = 1; j < 6; j++) {
            x = adc_sample(j);
            x &= 1;	// Get the LSB of the result
            seed = (seed << 1) + x; // Build up the 15-bit result
        }
    }
    srand(seed);	// Seed the rand function
}
