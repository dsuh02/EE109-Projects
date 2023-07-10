/********************************************
*
*  Name:
*  Email:
*  Lab section:
*  Assignment: Lab 3 - Arduino Input and Output
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>

#define DOT_LENGTH  250         /* Define the length of a "dot" time in msec */

void dot(void);
void dash(void);
void makeOutput(char);
char checkInput(char);

int main(void)
{
    DDRD = 1 << PD2;
    PORTB |= 0x38;
    
    // Loop forever
    while (1) {                 
    //  Use "if" statements and the checkInput()
    //  function to determine if a button
    //  is being pressed and then output
    //  the correct dot/dash sequence by
    //  calling the dot() and dash(), and 
    //  using appropriate delay functions

      if (checkInput(PB3) == 0) 
      {
        dot();
        dot(); 
        dash();
        _delay_ms(DOT_LENGTH);
        _delay_ms(DOT_LENGTH);
      }

      else if (checkInput(PB4) == 0) 
      {
        dot();
        dot();
        dot();
        
        _delay_ms(DOT_LENGTH);
        _delay_ms(DOT_LENGTH);
      }
      else if (checkInput(PB5) == 0)
      {
        dash();
        dot();
        dash();
        dot();

        _delay_ms(DOT_LENGTH);
        _delay_ms(DOT_LENGTH);
      }

    


    }

    return 0;   /* never reached */
}

/*
  dot() - Makes the output LED blink a "dot".

  Write code to generate a dot by using the makeOutput function
  to turn the output ON and OFF with appropriate delays.
  Be sure you don't forget to also delay an
  appropriate time after you turn the output off.
*/
void dot()
{
  makeOutput(1);
  _delay_ms(DOT_LENGTH);//makes output and then holds it before changing back in next line
  makeOutput(0);
  _delay_ms(DOT_LENGTH);//delays for 1 DOT_LENGTH before next function

}

/*
  dash() - Makes the output LED blink a "dash".

  Write code to generate a dash by using the makeOutput function
  to turn the output ON and OFF with appropriate delays.
  Be sure you don't forget to also delay an
  appropriate time after you turn the output off.
*/
void dash()
{
  makeOutput(1);//same as dot function but holds for 3 times DOT_LENGTH
  _delay_ms(DOT_LENGTH * 3);
  makeOutput(0);
  _delay_ms(DOT_LENGTH);

}

/*
  makeOutput() - Changes the output bit (Group D, bit 2) to either
  a zero or one, based on the input argument "value".
  
  If the argument is zero, turn the output OFF,
  otherwise turn the output ON. 
  
  Do not use any delays here.  Just use bit-wise operations
  to make the appropriate PORT bit turn on or off.
*/
void makeOutput(char value)
{

  if (value != 0)//make output lights the bulb when value!=0
  {
    PORTD |= 0x04; 
  }
  else//turns off
  {
    PORTD &= ~(0x04); 

  }

}

/*
  checkInput(bit) - Checks the state of the input bit in Group B specified by
  the "bit" argument (0-7), and returns either 0 or 1 depending on its state.
  
  Write code to use the appropriate group's PIN register and determine if  
  the specified bit (which is passed as the argument) of that group is
  pressed or not.  Think carefully about what bit value means "pressed"
  when connected to a pushbutton.
 */
char checkInput(char bit)
{
  if((PINB & (1 << bit))){ //returns 1 when button is not pressed
    return 1;
  }
  else //0 if button pressed
    return 0;
}

