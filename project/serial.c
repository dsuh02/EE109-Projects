#include "serial.h"                // Declarations of the serial functions
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#define FOSC 16000000 // Clock frequency
#define BAUD 9600 // Baud rate used
#define MYUBRR (FOSC/16/BAUD-1) // Value for UBRR0

void serial_init(void){
    UBRR0 = MYUBRR;
    
    UCSR0B |= (1 << TXEN0 | 1 << RXEN0); // Enable RX and TX
    UCSR0B |= (1<<RXCIE0); //enable interupts for reciever

    UCSR0C = (3 << UCSZ00); // Async., no parity,
                            // 1 stop bit, 8 data bits
}