#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header.h"  /* Declatations for these labs */

char pacman_x = 56;
char pacman_y = 10;
char ghost1_x = 32;
char ghost1_y = 10;
char ghost2_x = 47;
char ghost2_y = 10;

// *x and *y are top left corner
void moveUp(char * x, char * y) {
	char nextY = (*y) - 1;
	char x1 = (*x);
	char x2 = (*x) + 7;
	//Looking at upper corners of object. If no wall, allow current Y to decrement. Test for x1 (top left) & x2 (top right)
	//if(map_bitmap[nextY][x1] == 0 && map_bitmap[nextY][x2] == 0) {
		(*y) = (*y) - 1;
	//}
}

void moveDown(char * x, char * y) {
	char nextY = (*y) + 7 + 1;
	char x1 = (*x);
	char x2 = (*x) + 7;
	//Looking at lower corners of object (hence - 8). If no wall, allow current Y to increment. Test for x1 (bottom left) & x2 (bottom right)
	if(map_bitmap[nextY][x1] == 0 && map_bitmap[nextY][x2] == 0) {
		(*y) = (*y) + 1;
	}
}

void moveRight(char * x, char * y) {
	char nextX = (*x) + 7 + 1;
	char y1 = (*y);
	char y2 = (*y) - 7;
	//Looking at right corners of object (hence + 8). If no wall, allow current X to increment (right). Test for y1 (top right) & y2 (bottom right)
	if(map_bitmap[y1][nextX] == 0 && map_bitmap[y2][nextX] == 0) {
		(*x) = (*x) + 1;
	}
}

void moveLeft(char * x, char * y) {
	char nextX = (*x) - 1;
	char y1 = (*y);
	char y2 = (*y) - 7;
	//Looking at left corners of object. If no wall, allow current X to decrement (left). Test for y1 (top left) & y2 (bottom left)
	if(map_bitmap[y1][nextX] == 0 && map_bitmap[y2][nextX] == 0) {
		(*x) = (*x) - 1;
	}
}



//Fix it - BTN1 must be implemented (move right)
void player_move(){
	volatile int btnvalue = getbtns();
	volatile int btn1 = getbtn1();
	
	switch(btnvalue) {
		case 4: //BTN4
			moveUp(&pacman_x, &pacman_y);
			break;
		case 2: //BTN3
			moveDown(&pacman_x, &pacman_y);
			break;
		case 1: //BTN2
			moveLeft(&pacman_x, &pacman_y);
			break;
		default: //If no BTNs or multiple BTNs, then no movement
			break;			
		}
		if(btn1 == 1) {
			moveRight(&pacman_x, &pacman_y);
		}

//BTN2 = 001
//BTN3 = 010
//BTN4 = 100

//SW1 = 0001
//SW2 = 0010
//SW3 = 0100
//SW4 = 1000		
}
