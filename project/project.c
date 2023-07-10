/********************************************
 *
 *  Name: Daniel Suh
 *  Email:
 *  Section:
 *  Assignment: project
 *
 ********************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/eeprom.h>

#include "project.h"
#include "lcd.h" //link header files
#include "encoder.h"
#include "serial.h"
#include "timers.h"

// extern variables from encoder.h
extern volatile unsigned char new_state, old_state;
extern volatile unsigned char changed;  // Flag for state change
extern volatile int count;		// Count starts at duty cycle 128
extern volatile unsigned char a, b;

//init speed vars
int speedOnes=0;
int speedTenths=0;

//extern variables from timers.h
extern volatile int timedOut;

int timerCount = 0; // for speed sensors (in ms)

int state = 0;  // 0 not started
                // 1 started
                // 2 stoppedd
                // 3 timed out

int buzzerCount=0;  //how many ISR calls should the buzzer buzz
int buzzerState=0;  // 0 off (not yet buzzed this scan)
                    // 1 buzzing
                    // 2 has buzzed already (has already buzzed this scan)

//variables for serial use
int serialSpeed=0; // final var that hold serial speed on here
char serialData[5]="    \0"; //holds serial input data
int serialStarted = 0; //started flag
int serialNumDataRecieved =0; //how many data characters have been received and been stored in the buffer so far
int serialFlag=0;

int main(){
    //enable tri state gate
    PORTC &= ~(1<<PC4);

    //allow interrupts
	PCICR |= (1<<PCIE2);
	PCMSK2 |= (1<<PCINT18) | (1<<PCINT19);
	sei();

    // Initialize DDR and PORT registers
	PORTD |= ((1<<PD2) | (1<<PD3)); // port register for encoder buttons
    PORTC |= ((1<<PC1) | (1<<PC2));// port register for phototransistors
    DDRC |= (1<<PC5); //DDR for buzzer
    DDRB |= (1<<PB3); //DDR for servo
    DDRC |= (1<<PC3); // pullup for yellow LED
    DDRC |= (1<<PC4); // pullup for tri state gate enable
    DDRB |= (1<<PB4)|(1<<PB5);// pullup for red and green LED

    // Initialize timer
    timer0_init();
    timer1_init();
    timer2_init();

    //initialize encoder
    encoder_init();

    //initialize serial
    serial_init();

	//initilize lcd
	lcd_init();


    


	lcd_writecommand(1);
    lcd_moveto(0,0);
    lcd_stringout("Daniel Suh");
    lcd_moveto(1,0);
    lcd_stringout("EE109 PROJECT");
    _delay_ms(1000);
    lcd_writecommand(1);


    char buf[15];

    lcd_moveto(1,0);
    snprintf(buf, 15, "%d",count);
    lcd_stringout("Max=");
    lcd_stringout(buf);
    
    while (1) {                 // Loop forever
		if (changed) { // Did state change?
			changed = 0;        // Reset changed flag
        	//lcd_moveto(1,0);
            //snprintf(buf, 15, "Max=%d  ",count);
            //lcd_stringout(buf);
            unsigned char newCount;
            newCount=count;
            eeprom_update_byte((void *) 0, newCount);
            lcd_moveto(1,0);
            snprintf(buf, 15, "Max=%d  ",newCount);
            lcd_stringout(buf);
        } 
        if(serialFlag){
            serialFlag=0;
            char tmp[6];
            snprintf(tmp,5,"%d.%d",serialSpeed/10,serialSpeed%10);
            tmp[5]='\0';
            lcd_moveto(1,12);
            lcd_stringout(tmp);

            if(serialSpeed>0){
                if((speedOnes*10+speedTenths)>=serialSpeed){
                    PORTB |= (1<<PB5); //green led on
                    PORTB &= ~(1<<PB4); //red led off
                }
                else{
                    PORTB |= (1<<PB4); //red led on
                    PORTB &= ~(1<<PB5); //green led off
                }
            }    
        }
        if(state==1){
        //reset buzzer state
        buzzerState=0;
            lcd_moveto(0,0);
            lcd_stringout("SAMPLING        ");
            PORTC |= (1<<PC3);
        }
        if(state==2)
        {
            lcd_moveto(0,0);
            speedOnes=(50000/timerCount)/10;
            speedTenths=(50000/timerCount)%10;
            snprintf(buf, 15, "%4dms=%d.%d",timerCount,(speedOnes),(speedTenths));
            lcd_stringout(buf);
            if(speedOnes>=count && buzzerState==0){
                buzzerCount=156; //half second buzz if each interrupt is .0032 s
                buzzerState=1;
            }
            OCR2A = 37.5-.225*(speedOnes);
            PORTC &= ~(1<<PC3);

            snprintf(buf,5, "%d%d]",speedOnes,speedTenths);


            //if(serialStarted ==0){
                while ((UCSR0A & (1 << UDRE0)) == 0) { }
                UDR0 = '[';
                for(int i=0; buf[i]!=']';i++){
                    while ((UCSR0A & (1 << UDRE0)) == 0) { }
                    UDR0 = buf[i];
                }
                while ((UCSR0A & (1 << UDRE0)) == 0) { }
                UDR0 =']';
            //}

            if(serialSpeed>0){
                if((speedOnes*10+speedTenths)>=serialSpeed){
                    PORTB |= (1<<PB5); //green led on
                    PORTB &= ~(1<<PB4); //red led off
                }
                else{
                    PORTB |= (1<<PB4); //red led on
                    PORTB &= ~(1<<PB5); //green led off
                }
            }    

        }
        if(state==3){
            //reset buzzer state
            buzzerState=0;
            lcd_moveto(0,0);
            lcd_stringout("Sensor Timed Out");
            PORTC &= ~(1<<PC3);
        }
    }
}


ISR(TIMER1_COMPA_vect){

    if(state==0){//not started or after measurement
        if(!(PINC & (1<<PINC1)))//left phototransistor blocked
        {
            state = 1; //state in 'start'
        }
        else
        {
            //reset timer
            timerCount=0;
        }
    }
    if(state==1){
        
        if(!(PINC & (1<<PINC2)))//right phototransistor blocked
        {
            state = 2; // state in 'stopped'
        }
        else
        {
            // increments every 0.001s
            timerCount++;
        }
        if(timerCount>4000){ //timeout if timer hits 4001
            state=3;
        }
    }
    if(state==2){
        if(!(PINC & (1<<PINC1)))//left phototransistor blocked
        {
            state = 1; //state in 'start'
            timerCount=0;
        }
    }
    if(state==3){
        if(!(PINC & (1<<PINC1)))//left phototransistor blocked
        {
            state = 1; //state in 'start'
            timerCount=0;
        }
    }
}

ISR(TIMER0_COMPA_vect){
    if(buzzerCount>0 ){
        PORTC ^= (1<<PC5);
        buzzerCount--;
    }
    if(buzzerCount==0 && buzzerState==1){
        buzzerState=2;
    }
}

ISR(USART_RX_vect){
    while(!(UCSR0A & (1<<RXC0))){}
    char temp=UDR0;
    if(temp=='['){
        serialStarted=1;
        serialNumDataRecieved=0;
        for(int i=0; i<4; i++){
            serialData[i]=' ';
        }
        serialData[4]='\0';
    }
    else if(serialStarted==1 && temp>='0' && temp<='9'){
        serialData[serialNumDataRecieved]=temp;
        serialNumDataRecieved++;
    }
    else if(temp==']' && serialStarted==1 ){
        //is now valid
        //start parsing
        serialSpeed=0;
        serialData[serialNumDataRecieved]='\0';

        sscanf(serialData, "%d", &serialSpeed);

        serialFlag=1;

        serialStarted=0;
        
    }
    else{
        serialStarted=0;
        serialNumDataRecieved=0;
        for(int i=0; i<4; i++){
            serialData[i]=' ';
        }
        serialData[4]='\0';
    }
}