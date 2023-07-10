#include <avr/io.h>

#include "adc.h"


void adc_init(void)
{
    // Initialize the ADC
    ADMUX |= (0x40);

    // channel a3 to 
    ADCSRA |= (0x07);
    ADMUX |= (1<<5);
    ADCSRA |= (1<<7);
}

unsigned char adc_sample(unsigned char channel)
{
    // Set ADC input mux bits to 'channel' value
    char MASK = 0x0F;
    ADMUX &= ~MASK;
    ADMUX |= (channel & MASK);
    ADCSRA |= (1<<6);

    while((ADCSRA &(1<<6))!=0){}

    unsigned char result =ADCH;
    return result;

    // Convert an analog input and return the 8-bit result

}
