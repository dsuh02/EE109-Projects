/********************************************
*
*  Name:
*  Email:
*  Section:
*  Assignment: Lab 5 - Combination Lock
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "lcd.h"

enum states { S0, S1, S2, S3, S4, S5 };

int main(void) {

    /* Call lcd_init to set up the LCD */
    lcd_init();
    lcd_writecommand(1);
    /* Call lcd_stringout to print out your name */
    lcd_stringout("Daniel Suh");
    /* Use snprintf to create a string with your birthdate */
    char buf[17];
    unsigned char month, day, year;
    month = 8;
    day = 26;
    year = 02;

    snprintf(buf, 17, "0%d/%d/0%d", month, day, year);

    /* Use lcd_moveto to start at an appropriate column 
       in the bottom row to appear centered */
    lcd_moveto(1,0);
    /* Use lcd_stringout to print the birthdate string */
    lcd_stringout(buf);
    /* Delay 1 second */
    _delay_ms(1000);
    lcd_writecommand(1);

    PORTC |= 0x14;

    int state =S0;
    int i=0;

    lcd_stringout("Entered: ");
    lcd_moveto(1,0);
    lcd_stringout("Status: Locked");
    lcd_moveto(0,9);

    while(1)
    { 
        if(i==30){
            state=S0;
            i=0;
            lcd_moveto(0,9);
            lcd_stringout("     ");
            lcd_moveto(1,8);
            lcd_stringout("Locked  ");
            lcd_moveto(0,9);

        }
        if(!(PINC & (1<<PINC2))){//A pressed
            i=0;//user input so reset timer
            _delay_ms(20);
            if(state==S0){
                //do nothing
            }
            else if(state==S1){
                lcd_moveto(0,10);
                lcd_stringout("A");
                state=S2;
            }
            else if(state==S2){
                lcd_moveto(0,9);
                lcd_stringout("  ");
                state=S0;
                lcd_moveto(0,9);
            }
            else if(state==S3){
                lcd_moveto(0,11);
                lcd_stringout(" ");
                state=S2;
                lcd_moveto(0,11);

            }
            else if(state==S4){
                lcd_moveto(0,13);
                lcd_stringout("A");
                state=S5;
                lcd_moveto(1,8);
                lcd_stringout("Unlocked");
            }
            else if(state==S5){
                lcd_moveto(0,9);
                lcd_stringout("     ");
                lcd_moveto(1,8);
                lcd_stringout("Locked  ");
                state=S0;
                lcd_moveto(0,9);
            }
            while(!(PINC & (1<<PINC2))){
                _delay_ms(50);
            }
            _delay_ms(30);
        }
        if(!(PINC & (1<<PINC4))){//B pressed
            i=0;
            if(state==S0){
                lcd_moveto(0,9);
                lcd_stringout("B");
                state=S1;
            }
            else if(state==S1){
                //do nothing
            }
            else if(state==S2){
                lcd_moveto(0,11);
                lcd_stringout("B");
                state=S3;
            }
            else if(state==S3){
                lcd_moveto(0,12);
                lcd_stringout("B");
                state=S4;
            }
            else if(state==S4){
                lcd_moveto(0,10);
                lcd_stringout("    ");
                state=S1;
                lcd_moveto(0,10);
            }
            else if(state==S5){
                lcd_moveto(0,9);
                lcd_stringout("     ");
                lcd_moveto(1,8);
                lcd_stringout("Locked  ");
                state=S0;
                lcd_moveto(0,9);
            }
            while(!(PINC & (1<<PINC4))){
                _delay_ms(50);
            }

            _delay_ms(30);
        }
        _delay_ms(100);
        i++;
    }

    return 0;   /* never reached */
}
