#include <stdint.h>
#include <pic32mx.h>
#include "header.h" 

int getsw(void);
int getbtns(void);

int getsw(void) {
	volatile int switches;
	switches = (volatile int)(PORTD >> 8) & 0x00F;
	return switches;
}

int getbtns(void) {
	while(1) {
		int buttons;
		buttons = (PORTD >> 5) & 0x07;
		return buttons;
	}
}
