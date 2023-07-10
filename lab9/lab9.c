/********************************************
 *
 *  Name: Daniel Suh
 *  Email:
 *  Section:
 *  Assignment: Lab 9 - PWM
 *
 ********************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

volatile unsigned char new_state, old_state;
volatile unsigned char changed = 0;  // Flag for state change
volatile int count = 375;		// Count starts at duty cycle 128
volatile unsigned char a, b;


void timer2_init(void)
{
    TCCR2A |= (0b11 << WGM00);  // Fast PWM mode, modulus = 256
    TCCR2A |= (0b10 << COM0A0); // Turn D11 on at 0x00 and off at OCR2A
    OCR2A = 128;                // Initial pulse duty cycle of 50%
    TCCR2B |= (0b111 << CS20);  // Prescaler = 1024 for 16ms period
}

void timer1_init(void){

    TCCR1A |= ((1 << WGM11) | (1 << WGM10)); // (Fast PWM Top=OCR1A, Thresh=OCR1B)
    TCCR1B |= ((1<<WGM13) | (1 << WGM12)); // (Fast PWM Top=OCR1A, Thresh=OCR1B)

    OCR1A = 5000;//ceiling
    OCR1B = 375; //start servo centered and light mid-brightness

    TCCR1A |= (1 << COM1B1);// Set Pin on CTR=0x00, Clear pin on match=OCR?
    TCCR1A &= ~(1 << COM1B0);
    TCCR1B |= ((1<<CS10) | (1<<CS11)); // Prescalar =64
    TCCR1B &= ~(1<<CS12); //prescalar


}   


int main(){
    //allow interrupts
	PCICR |= (1<<PCIE1);
	PCMSK1 |= (1<<PCINT12) | (1<<PCINT13);
	sei();

    // Initialize DDR and PORT registers
	DDRB |= ((1<<PB3) | (1<<PB2)); // DDRB for LED
	PORTC |= ((1<<PC4) | (1<<PC5)); // port register for encoder buttons

    // Initialize timer
    timer1_init();
    timer2_init();

    // Read the A and B inputs to determine the intial state.
    // In the state number, B is the MSB and A is the LSB.
    // Warning: Do NOT read A and B separately.  You should read BOTH inputs
    // at the same time, then determine the A and B values from that value.    

	if(!(PINC & (1<<PINC4)))//A pressed
        a=1;
	else 
		a=0;
    if(!(PINC & (1<<PINC5)))
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

    while (1) {                 // Loop forever
		if (changed) { // Did state change?
			changed = 0;        // Reset changed flag
            OCR1B = count;  //servo directly dependent on count
            OCR2A = (count-188)*.68181818; //led has to modify the value of count to be relevant
        }   // getting the lower bound to be 0 and the upper to be 255 instead of 188 and 562, respectively
    }



    return 0; //never reached
}


ISR(PCINT1_vect){
	// Read the input bits and determine A and B.
	if(!(PINC & (1<<PINC4)))//A pressed
		a=1;
	else 
		a=0;
	if(!(PINC & (1<<PINC5)))
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
			count+=2;
		}
		else if(b==1){
			old_state=new_state;
			new_state=3;
			count-=2;
		}

	}
	else if (old_state == 1) {

		// Handle A and B inputs for state 1
		if(a==0){
			old_state=new_state;
			new_state=0;
			count-=2;
		}
		else if(b==1){
			old_state=new_state;
			new_state=2;
			count+=2;
		}
	}
	else if (old_state == 2) {

		// Handle A and B inputs for state 2
		if(a==0){
			old_state=new_state;
			new_state=3;
			count+=2;
		}
		else if(b==0){
			old_state=new_state;
			new_state=1;
			count-=2;
		}
	}
	else {   // old_state = 3

		// Handle A and B inputs for state 3
		if(a==1){
			old_state=new_state;
			new_state=2;
			count-=2;
		}
		else if(b==0){
			old_state=new_state;
			new_state=0;
			count+=2;
		}
	}
	// If state changed, update the value of old_state,
    // and set a flag that the state has changed.
    if (new_state != old_state) {
        changed = 1;
        old_state = new_state;
    }
    if(count<188){
        count=188;
    }
    else if(count>562){
        count=562;
    }
    
}