#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header.h"  /* Declatations for these labs */
#include <stdlib.h>
#include <stdio.h>

char pacman_x = 56;
char pacman_y = 10;
char ghost1_x = 32;
char ghost1_y = 10;
char ghost2_x = 47;
char ghost2_y = 10;

extern *LED;

extern char difficulty;

//Created in conjunction by Andrejs and Edward


// *x and *y are top left corner
void moveUp(char * x, char * y) {
	char nextY = (*y) - 1;
	char x1 = (*x);
	char x2 = (*x) + 7;
	
	//Teleport to bottom of screen if reaching top of screen
	if (*y <= 0)
		*y = 32;
	
	//Looking at upper corners of object. If no wall, allow current Y to decrement. Test for x1 (top left) & x2 (top right)
	if(map_bitmap[nextY][x1] == 0 && map_bitmap[nextY][x2] == 0) {
		(*y) = (*y) - 1;
	}
	

}

void moveDown(char * x, char * y) {
	char nextY = (*y) + 7 + 1;
	char x1 = (*x);
	char x2 = (*x) + 7;
	
	//Teleport to top of screen if reaching bottom of screen
	if ((*y) + 8 >= 32)
		*y = -4;
	
	//Looking at lower corners of object (hence - 8). If no wall, allow current Y to increment. Test for x1 (bottom left) & x2 (bottom right)
	if(map_bitmap[nextY][x1] == 0 && map_bitmap[nextY][x2] == 0) {
		(*y) = (*y) + 1;
	}
	

}

void moveRight(char * x, char * y) {
	char nextX = (*x) + 7 + 1;
	char y1 = (*y);
	char y2 = (*y) + 7;
	
	//Teleport to left of screen if reach furthest right
	if ((*x) >= 120){
		*x = 0;
	}
	
	//Looking at right corners of object (hence + 8). If no wall, allow current X to increment (right). Test for y1 (top right) & y2 (bottom right)
	if(map_bitmap[y1][nextX] == 0 && map_bitmap[y2][nextX] == 0) 
		(*x) = (*x) + 1;
		

}

void moveLeft(char * x, char * y) {
	char nextX = (*x) - 1;
	char y1 = (*y);
	char y2 = (*y) + 7;
	
	//Teleport to right of screen if reach furthest left
	if ((*x) <= 1)
		*x = 120;
	
	//Looking at left corners of object. If no wall, allow current X to decrement (left). Test for y1 (top left) & y2 (bottom left)
	if(map_bitmap[y1][nextX] == 0 && map_bitmap[y2][nextX] == 0) {
		(*x) = (*x) - 1;
	}
}


void player_move(){
	volatile int btnvalue = getbtns();
	volatile int btn1 = getbtn1();
	btnvalue += (btn1 << 3);

		if ((btnvalue & 4) >> 2) //BTN4
			moveUp(&pacman_x, &pacman_y);
		if ((btnvalue & 3) >> 1) //BTN3
			moveDown(&pacman_x, &pacman_y);
		if ((btnvalue & 1) >> 0) //BTN2
			moveLeft(&pacman_x, &pacman_y);
		if ((btnvalue & 8) >> 3) //BTN1
			moveRight(&pacman_x, &pacman_y);	
			

//BTN2 = 001
//BTN3 = 010
//BTN4 = 100

//SW1 = 0001
//SW2 = 0010
//SW3 = 0100
//SW4 = 1000				
}

//AI Section

//Easy AI - given address of ghost 1 or 2 position, then calculates random number
// between 0 and 3 to decide which direction to move
void easyDiffG1() {
	int randomDir = rand() % 4;
		switch(randomDir) {
		case 0:
			moveUp(&ghost1_x, &ghost1_y);
			break;
		case 1:
			moveDown(&ghost1_x, &ghost1_y);
			break;
		case 2:
			moveLeft(&ghost1_x, &ghost1_y);
			break;
		case 3:
			moveRight(&ghost1_x, &ghost1_y);
			break;
		default:
			break;
	}
}

//Medium AI - given address of ghost 1 or 2 position, then calculates minimum distance based upon hypotenuse
//Uses update strategy to determine which move results in the minimum distance. Then sets flag to determine which move
//is most appropriate
void mediumDiffG1() {
	char flag = 0;
	double temp = 0;
	double minDist = ((pacman_x - ghost1_x) * (pacman_x - ghost1_x)) + ((pacman_y - ghost1_y) * (pacman_y - ghost1_y));
	moveUp(&ghost1_x, &ghost1_y);
	temp = ((pacman_x - ghost1_x) * (pacman_x - ghost1_x)) + ((pacman_y - ghost1_y) * (pacman_y - ghost1_y));
	if(temp < minDist) {
		minDist = temp;
		flag = 0;
	}
	moveDown(&ghost1_x, &ghost1_y);
	moveDown(&ghost1_x, &ghost1_y);
	temp = ((pacman_x - ghost1_x) * (pacman_x - ghost1_x)) + ((pacman_y - ghost1_y) * (pacman_y - ghost1_y));
	if(temp < minDist) {
		minDist = temp;
		flag = 1;
	}
	moveUp(&ghost1_x, &ghost1_y);
	moveLeft(&ghost1_x, &ghost1_y);
	temp = ((pacman_x - ghost1_x) * (pacman_x - ghost1_x)) + ((pacman_y - ghost1_y) * (pacman_y - ghost1_y));
	if(temp < minDist) {
		minDist = temp;
		flag = 2;
	}
	moveRight(&ghost1_x, &ghost1_y);
	moveRight(&ghost1_x, &ghost1_y);
	temp = ((pacman_x - ghost1_x) * (pacman_x - ghost1_x)) + ((pacman_y - ghost1_y) * (pacman_y - ghost1_y));
	if(temp < minDist) {
		minDist = temp;
		flag = 3;
	}
	moveLeft(&ghost1_x, &ghost1_y);
	switch(flag) {
		case 0:
			moveUp(&ghost1_x, &ghost1_y);
			break;
		case 1:
			moveDown(&ghost1_x, &ghost1_y);
			break;
		case 2:
			moveLeft(&ghost1_x, &ghost1_y);
			break;
		case 3:
			moveRight(&ghost1_x, &ghost1_y);
			break;
		default:
			break;
	}
}

//Hard AI - performs medium difficulty twice effectively doubling speed of ghosts
void hardDiffG1() {
	mediumDiffG1();
	mediumDiffG1();
}

void easyDiffG2() {
	int randomDir = rand() % 4;
		switch(randomDir) {
		case 0:
			moveUp(&ghost2_x, &ghost2_y);
			break;
		case 1:
			moveDown(&ghost2_x, &ghost2_y);
			break;
		case 2:
			moveLeft(&ghost2_x, &ghost2_y);
			break;
		case 3:
			moveRight(&ghost2_x, &ghost2_y);
			break;
		default:
			break;
	}
}

//Medium AI - given address of ghost 1 or 2 position, then calculates minimum distance based upon hypotenuse
//Uses update strategy to determine which move results in the minimum distance. Then sets flag to determine which move
//is most appropriate
void mediumDiffG2() {
	char flag = 0;
	double temp = 0;
	double minDist = ((pacman_x - ghost2_x) * (pacman_x - ghost2_x)) + ((pacman_y - ghost2_y) * (pacman_y - ghost2_y));
	moveUp(&ghost2_x, &ghost2_y);
	temp = ((pacman_x - ghost2_x) * (pacman_x - ghost2_x)) + ((pacman_y - ghost2_y) * (pacman_y - ghost2_y));
	if(temp < minDist) {
		minDist = temp;
		flag = 0;
	}
	moveDown(&ghost2_x, &ghost2_y);
	moveDown(&ghost2_x, &ghost2_y);
	temp = ((pacman_x - ghost2_x) * (pacman_x - ghost2_x)) + ((pacman_y - ghost2_y) * (pacman_y - ghost2_y));
	if(temp < minDist) {
		minDist = temp;
		flag = 1;
	}
	moveUp(&ghost2_x, &ghost2_y);
	moveLeft(&ghost2_x, &ghost2_y);
	temp = ((pacman_x - ghost2_x) * (pacman_x - ghost2_x)) + ((pacman_y - ghost2_y) * (pacman_y - ghost2_y));
	if(temp < minDist) {
		minDist = temp;
		flag = 2;
	}
	moveRight(&ghost2_x, &ghost2_y);
	moveRight(&ghost2_x, &ghost2_y);
	temp = ((pacman_x - ghost2_x) * (pacman_x - ghost2_x)) + ((pacman_y - ghost2_y) * (pacman_y - ghost2_y));
	if(temp < minDist) {
		minDist = temp;
		flag = 3;
	}
	moveLeft(&ghost2_x, &ghost2_y);
	switch(flag) {
		case 0:
			moveUp(&ghost2_x, &ghost2_y);
			break;
		case 1:
			moveDown(&ghost2_x, &ghost2_y);
			break;
		case 2:
			moveLeft(&ghost2_x, &ghost2_y);
			break;
		case 3:
			moveRight(&ghost2_x, &ghost2_y);
			break;
		default:
			break;
	}
}

//Hard AI - performs medium difficulty twice effectively doubling speed of ghosts
void hardDiffG2() {
	mediumDiffG2();
	mediumDiffG2();
}



	
