/* mipslabfunc.c
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson
   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header.h"  /* Declatations for these labs */


#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)


//Code written by Edward Leander, modifications done by Andrejs Prihodjko, suggestions given by Fredrik Karlsson

char display_matrix [128][32];
char textbuffer[4][16];

extern dir;
extern score;
extern highScore;
extern *LED;

/* quicksleep:
   A simple function to create a small delay.
   Very inefficient use of computing resources,
   but very handy in some special cases. */
void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

//Edward Leander
// set a pixel to a value
void pixel_set(char x, char y, char set){
	display_matrix [x][y] = set;
}

//Andrejs Prihodko
// turns the number score in to displayable text
void display_score(int line, char *s) {
	int i, j, c, k;
	if(line < 0 || line >= 4)
		return;
	if(!s)
		return;
	
	for(i = 0; i < 16; i++) {
		if(*s) {
			textbuffer[line][i] = *s;
			s++;
		} else
			textbuffer[line][i] = ' ';	
	}
}

//Edward Leander
// turns a pixel on
void pixel_on(char x, char y){
	display_matrix[x][y] = 1;
}

//Edward Leander
// turns a pixel off
void pixel_off(char x, char y){
	display_matrix[x][y] = 0;
}

// sends the information to the screen
uint8_t display_send(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}

//Edward Leander
// turns all pixels off
void reset(void){
	int x, y;
	for(y = 0; y < 32; y++){
		for(x = 0; x < 128; x++){
			display_matrix[x][y] = 0;
		}
	}
	display_update();
}

//Edward Leander, similar to lab 3, minor alterations
// updates the screen
void display_update(void) {
	int y, x, k;
	int value;
	for(y = 0; y < 4; y++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		display_send(0x22);
		display_send(y);
		
		display_send(0x0);
		display_send(0x10);
		
		DISPLAY_CHANGE_TO_DATA_MODE;
		
		for(x = 0; x < 128; x++){
			value = display_matrix[x][y*8];
			for(k = 1; k < 8; k++) {
				value += (display_matrix[x][y*8+k]<<k);
			}
			display_send(value);
		}
	}
}

//Edward Leander
// Start screen
void display_start(void) {
	reset();
	int x, y; 					
	for(x = 0; x < 128; x++){
		for(y = 0; y < 32; y++){
			pixel_set(x, y, start_bitmap[y][x]);
		}
	}
	display_update();
}


//Edward Leander
// Display map
void display_map(void) {
	int x, y; 					
	for(x = 0; x < 128; x++){
		for(y = 0; y < 32; y++){
			pixel_set(x, y, map_bitmap[y][x]);
		}
	}
}


//Edward Leander
// Game over screen 
void display_end(void) {
	reset();
	int x, y;
	int i, j, c, k;
	// start screen
	for(x = 0; x < 128; x++){
		for(y = 0; y < 16; y++){
			pixel_set(x, y, gameover_bitmap[y][x]);
		}
	}
	
	//Andrejs Prihodko
	display_update();
	for(i = 2; i < 4; i++) {
		
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		display_send(0x22);
		display_send(i);
		
		display_send(0x0);
		display_send(0x10);
		
		DISPLAY_CHANGE_TO_DATA_MODE;

		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;
			for(k = 0; k < 8; k++)
				display_send(font[c*8 + k]);
		}
	}
}



//Edward Leander and Andrejs Prihodko
// displaying the pacman
void pacman_draw(char x, char y){
	switch(dir) {
		case 0: //RIGHT
			drawSprite(pacman_bitmapRIGHT, x, y);
			break;
		case 1: //LEFT
			drawSprite(pacman_bitmapLEFT, x, y);
			break;
		case 2: //DOWN
			drawSprite(pacman_bitmapDOWN, x, y);
			break;	
		case 3: //UP
			drawSprite(pacman_bitmapUP, x, y);
			break;
		default:
			drawSprite(pacman_bitmapRIGHT, x, y);
			break;
	}
   
}

//Andrejs Prihodko
void drawSprite(char array[8][8], char x, char y) {
	int i, j;
	 for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){
           if (array[j][i] == 1) {
				pixel_on(x + i, y + j);   
            }
        }
    } 
}

//Edward Leander
// displaying the ghost
void ghost_draw(char x, char y){
	int i, j;
    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){
           if (ghost_bitmap[j][i] == 1) {
				pixel_on(x + i, y + j);   
            }
        }
    } 
}

// display initialization
void display_init(void) {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);
	
	display_send(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);
	
	display_send(0x8D);
	display_send(0x14);
	
	display_send(0xD9);
	display_send(0xF1);
	
	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);
	
	display_send(0xA1);
	display_send(0xC8);
	
	display_send(0xDA);
	display_send(0x20);
	
	display_send(0xAF);
}

// turns numbers in to chars
#define ITOA_BUFSIZ ( 24 )
char * itoaconv( int num )
{
  register int i, sign;
  static char itoa_buffer[ ITOA_BUFSIZ ];
  static const char maxneg[] = "-2147483648";
  
  itoa_buffer[ ITOA_BUFSIZ - 1 ] = 0;   /* Insert the end-of-string marker. */
  sign = num;                           /* Save sign. */
  if( num < 0 && num - 1 > 0 )          /* Check for most negative integer */
  {
    for( i = 0; i < sizeof( maxneg ); i += 1 )
    itoa_buffer[ i + 1 ] = maxneg[ i ];
    i = 0;
  }
  else
  {
    if( num < 0 ) num = -num;           /* Make number positive. */
    i = ITOA_BUFSIZ - 2;                /* Location for first ASCII digit. */
    do {
      itoa_buffer[ i ] = num % 10 + '0';/* Insert next digit. */
      num = num / 10;                   /* Remove digit from number. */
      i -= 1;                           /* Move index to next empty position. */
    } while( num > 0 );
    if( sign < 0 )
    {
      itoa_buffer[ i ] = '-';
      i -= 1;
    }
  }
  /* Since the loop always sets the index i to the next empty position,
   * we must add 1 in order to return a pointer to the first occupied position. */
  return( &itoa_buffer[ i + 1 ] );
}
