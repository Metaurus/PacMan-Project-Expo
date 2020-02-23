#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h" 

int getsw(void);
int getbtns(void);

int getsw(void) {
	int switches;
	switches = (PORTD >> 8) & 0x0F;
	return switches;
}

int getbtns(void) {
	while(1) {
		int buttons;
		buttons = (PORTD >> 5) & 0x07;
		return buttons;
	}
}