#include <stdint.h>
#include <pic32mx.h>
#include "header.h" 

//Written by Andrejs Prihodjko and Edward Leander

int getsw(void);
int getbtns(void);
//Returns values of switches 1->4
int getsw(void) {
	volatile int switches;
	switches = (volatile int)(PORTD >> 8) & 0x00F;
	return switches;
}
//Returns values of buttons 2->4
int getbtns(void) {
	while(1) {
		int buttons;
		buttons = (PORTD >> 5) & 0x07;
		return buttons;
	}
}
//Returns value of button 1 because some genius decided to make it a separate port...
int getbtn1(void) {
	while(1) {
		int buttons;
		buttons = (PORTF >> 1) & 0x01;
		return buttons;
	}
}
