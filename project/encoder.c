#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/eeprom.h>

#include "encoder.h"
#include "project.h"



void encoder_init(){
	changed = 0;  // Flag for state change

	unsigned char x;
	x = eeprom_read_byte((void *) 0);
	count=x;
	if(count<1 || count>=100){
		count=50;
	}
	//speed lim starts at 50

    // Read the A and B inputs to determine the intial state.
    // In the state number, B is the MSB and A is the LSB.
    // Warning: Do NOT read A and B separately.  You should read BOTH inputs
    // at the same time, then determine the A and B values from that value.    

	if(!(PIND & (1<<PIND2)))//A pressed
        a=1;
	else 
		a=0;
    if(!(PIND & (1<<PIND3)))
        b=1;
	else 
		b=0;
    
    if (!b && !a)
		old_state = 0;
    else if (!b && a)
		old_state = 1;
    else if (b && !a)
		old_state = 2;
    else
		old_state = 3;

    new_state = old_state;

    // while (1) {                 // Loop forever
	// 	break;
	// 	if (changed) { // Did state change?
	// 		changed = 0;        // Reset changed flag
    //         OCR1B = count;  //servo directly dependent on count
    //         OCR2A = (count-188)*.68181818; //led has to modify the value of count to be relevant
    //     }   // getting the lower bound to be 0 and the upper to be 255 instead of 188 and 562, respectively
    // }
}


ISR(PCINT2_vect){
	// Read the input bits and determine A and B.
	if(!(PIND & (1<<PIND2)))//A pressed
		a=1;
	else 
		a=0;
	if(!(PIND & (1<<PIND3)))
		b=1;
	else 
		b=0;

	// For each state, examine the two input bits to see if state
	// has changed, and if so set "new_state" to the new state,
	// and adjust the count value.
	if (old_state == 0) {

		// Handle A and B inputs for state 0
		if(a==1){
			old_state=new_state;
			new_state=1;
			count++;
		}
		else if(b==1){
			old_state=new_state;
			new_state=3;
			count--;
		}

	}
	else if (old_state == 1) {

		// Handle A and B inputs for state 1
		if(a==0){
			old_state=new_state;
			new_state=0;
			count--;
		}
		else if(b==1){
			old_state=new_state;
			new_state=2;
			count++;
		}
	}
	else if (old_state == 2) {

		// Handle A and B inputs for state 2
		if(a==0){
			old_state=new_state;
			new_state=3;
			count++;
		}
		else if(b==0){
			old_state=new_state;
			new_state=1;
			count--;
		}
	}
	else {   // old_state = 3

		// Handle A and B inputs for state 3
		if(a==1){
			old_state=new_state;
			new_state=2;
			count--;
		}
		else if(b==0){
			old_state=new_state;
			new_state=0;
			count++;
		}
	}
	// If state  , update the value of old_state,
    // and set a flag that the state has changed.
    if (new_state != old_state) {
        changed = 1;
        old_state = new_state;
    }
    if(count<1){
        count=1;
    }
    else if(count>99){
        count=99;
    }
    
}