# EE109-Projects
Electrical Engineering Projects - Embedded Systems, Microcontrollers

!!!!!!!!!!!!!!!!!!!!!!! FINAL PROJECT INFORMATION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

PART 1 - Basic functionality
1.  Splash screen with name shown at the start.
2.  Sensors start and stop timing.  If a measurement is started but not
    completed after 4 sec, the measurement is terminated and a message is shown on
    the LCD.
3.  Elapsed time and speed displayed properly.  The elapsed time is shown to 1
    ms precision and the calculation of the speed based on the time delay
    between the sensors is correct and is shown to a precision of 0.1 cm/sec.
4.  The LED timing indicator operates properly, going on at the start of a
    timing event and off when it is complete.
5.  Rotary encoder can adjust the threshold between 1 and 99 cm/sec.
6.  Buzzer sounds warning if the speed measured on the local device is above the
    speed set with the rotary encoder.

PART 2 - Remote connectivity
1. The servo motor adjusts the indicator to correctly point at the speed that was
    measured on the local device.
2.  The maximum speed threshold is stored in EEPROM and retrieved when
    restarted.
3.  After a speed measurement is complete the device sends local speed to
    remote device over the serial link.
4.  Device can receive a speed from the remote device and displays it on LCD
    with 0.1 cm/sec precision.
5.  The red and green LEDs operate to show how the local and remote speeds
    compare.
