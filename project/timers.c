#include "timers.h"                // Declarations of the timer functions
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

void timer2_init(void)
{
    TCCR2A |= (0b11 << WGM20);  // Fast PWM mode, modulus = 256
    TCCR2A |= (0b10 << COM2A0); // Turn D11 on at 0x00 and off at OCR2A
    OCR2A = 37.5;                // Initial pulse duty cycle of 50%
    TCCR2B |= (0b111 << CS20);  // Prescaler = 1024 for 16ms period
    //16.4 ms period
    //.75 ms pw for full left
    //2.25 ms pw for full right

    //100 cm/s OCR2A = 15
    // 0 cm/s OCR2A = 37.5

}

void timer1_init(void){

    timedOut=0;
    // .001 sec update time
    // 4 seconds timeout time is 4/.001 = 4000 cycles 

    // prescalar = 64
    // 16,000,000 MHz * .001 seconds / 64 = 250 clocks for ISR

    // Set to CTC mode
    TCCR1B |= (1 << WGM12);
    // Enable Timer Interrupt
    TIMSK1 |= (1 << OCIE1A);
    // Set prescalar = 64
    // and start counter
    TCCR1B |= (1 << CS11) | (1<<CS10);
    // Load the MAX count
    // Assuming prescalar=64
    // counting to 250 =
    // 0.001s w/ 16 MHz clock
    OCR1A = 250;
}   

void timer0_init(void){
    // Set to CTC mode
    TCCR0A |= (1 << WGM01);  // CTC mode
    // Enable Timer Interrupt
	TIMSK0 |= (1 << OCIE0A);
    // Load the MAX count
	// Assuming prescalar=256
	// 0.0032s w/ 16 MHz clock
    // 320 
	OCR0A = 200;

    // Set prescalar = 256
    // and start counter
    TCCR0B |= (1 << CS02);
}