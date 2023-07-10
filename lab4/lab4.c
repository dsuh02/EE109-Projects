/********************************************
*
*  Name:
*  Email:
*  Section:
*  Assignment: Lab 4 - Up/Down counter on LCD display
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

enum states { UP, DOWN };

int main(void) {

    // Setup DDR and PORT bits for the 2 input buttons as necessary
    PORTC |= 0x14;
    // Initialize the LCD
    lcd_init();

    // Use a state machine approach to organize your code
    //   - Declare and initialize a variable to track what state you
    //     are in by assigning the values UP or DOWN to that variable.
    int state = UP;
    unsigned char cnt=5;
    int i=0;
    while (1) {               // Loop forever

        // Use an outer if statement to select what state you are in
        // Then inside each 'if' or 'else' block, perform the desired
        // output operations and then sample the inputs to update
        // the state for the next iteration

        lcd_writecommand(1);    // clear screen
        lcd_writedata(cnt + 0x30);  // display cnt

        if(!(PINC & (1<<PINC2))){//up pressed
            state=UP;
        }
        if(!(PINC & (1<<PINC4))){
            state=DOWN;
        }
        if(i==5){
            i=0;
            if(state==UP){
                cnt++;
                if(cnt==10){
                    cnt=0;
                }
            }
            else{
                if(cnt==0){
                    cnt=9;
                }
                else{
                    cnt--;
                }
            }
        }
        i++;
        // Delay before we go to the next iteration of the loop
        _delay_ms(100);
    }

    return 0;   /* never reached */
}

