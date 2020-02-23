/* mipslabwork.c

This file written 2015 by F Lundevall
Updated 2017-04-21 by F Lundevall

This file should be changed by YOU! So you must
add comment(s) here with your name(s) and date(s):

This file modified 2017-04-31 by Ture Teknolog 

For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declarations for these labs */
//#include "functions.c"  /* General use functions for program */

int prime = 1234567;
int mytime = 0x5957;
int timeoutcount = 0;

char difficulty = 0;
char lives = 3;
char movementDir = 'R';

void enableInterrupt(void);

char textstring[] = "text, more text, and even more text!";

volatile int *LED; //Declare *LED as a global pointer variable

void displayLives();
void displayDifficulty();

/* Interrupt Service Routine */
void user_isr( void )	{
	
	if(IFS(0) & 0x100) {
		IFSCLR(0) = 0x100;
		timeoutcount++;
	}
	
	if(timeoutcount == 10) {
		time2string( textstring, mytime );
		display_string( 3, textstring );
		display_update();
		tick( &mytime );
		timeoutcount = 0;
	}
}

/* Lab-specific initialization goes here */
void labinit( void )
{
	LED = (volatile int *) 0xbf886110; //Set LED address equal to the PORTE address
	*LED &= 0x0ff; //Mask the last byte 
	
	TRISD = 0xFE0;
	PR2 = ((80000000 / 256) / 10);
	TMR2 = 0;

	IEC(0) = 0x100;
	
	PORTE = 0;
	IEC(0) = 0x80;
	IPC(1) = 0xF000000;
	T2CON = 0x70;
	IECSET(0) = 0x00000100;
	IPC(2) = 0x1F;
	T2CONSET = 0x8000;

	enable_interrupt();
	
	return;
}

/* This function is called repetitively from the main program */
void labwork( void ) {
 prime = nextprime( prime );
 display_string( 0, itoaconv( prime ) );
 display_update();
 displayLives();
 displayDifficulty();
  
//SW1 = 0001
//SW2 = 0010
//SW3 = 0100
//SW4 = 1000
  
  int swValue;
	if (difficulty == 0) {
	//SW4, Furthest left switch
			swValue = getsw();
			if (swValue == 8) difficulty = 1;

	//SW3, Middle switch
			if (swValue == 4) difficulty = 2; 


	//SW2, Furthest right Switch
			if (swValue == 2) difficulty = 3; 
	}
}

//Andrejs Prihodjko
void displayLives() {
	
	//Check number of lives and turn on LEDs accordingly
	switch(lives) {
		case 3:
			*LED = (0x1 << 2) | (0x1 << 1) | (0x1 << 0);	//Three lights on
			break;
		case 2:
			*LED = (0x1 << 1) | (0x1 << 0); //Two lights on
			break;
		case 1:
			*LED = (0x1 << 0);	//One light on
			break;
		case 0:
			*LED = 0x0;	//No lights on
			break;
		default:
			break;
	}
	
}

void displayDifficulty() {
	
	//Check difficulty setting and turn on LED's accordingly (three on far left)
	switch(difficulty) {
		case 3:
			*LED += (0x1 << 5);
			break;
		case 2:
			*LED += (0x1 << 6);
			break;
		case 1:
			*LED += (0x1 << 7);
			break;
		default:
			break;
	}
}

//BTN2 = 001
//BTN3 = 010
//BTN4 = 100

//SW1 = 0001
//SW2 = 0010
//SW3 = 0100
//SW4 = 1000