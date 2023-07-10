 volatile unsigned char new_state, old_state;
 volatile unsigned char changed;  // Flag for state change
 volatile int count;	// Count starts at duty cycle 128
 volatile unsigned char a, b;

void encoder_init();