#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header.h"  /* Declatations for these labs */

char pacman_x = 56;
char pacman_y = 10;
char ghost1_x = 80;
char ghost1_y = 10;
char ghost2_x = 30;
char ghost2_y = 10;

extern *LED;
char dir = 0; //0 = RIGHT, 1 = LEFT, 2 = DOWN, 3 = UP

extern char difficulty;


//Edward Leander
// *x and *y are top left corner
void moveUp(char * x, char * y) {
	char nextY = (*y) - 1;
	char x1 = (*x)+1;
	char flag = 0;
	int i;
	
	//Teleport to bottom of screen if reaching top of screen
	if (*y <= 0)
		*y = 31;
	
	//Looking at upper corners of object. If no wall, allow current Y to decrement. Test for x1 (top left) & x2 (top right)
	for(i = 0; i < 7; i++) {
		if(map_bitmap[nextY][x1 + i] == 1) {
			flag = 1;
		}
	}
	if(!flag) (*y) = (*y) - 1;
	

}

//Edward Leander
void moveDown(char * x, char * y) {
	char nextY = (*y) + 7 + 1;
	char x1 = (*x)+1;
	char flag = 0;
	int i;
	//Teleport to top of screen if reaching bottom of screen
	if ((*y) + 8 >= 32)
		*y = 1;
	
	//Looking at lower corners of object (hence - 8). If no wall, allow current Y to increment. Test for x1 (bottom left) & x2 (bottom right)
	for(i = 0; i < 7; i++) {
		if(map_bitmap[nextY][x1 + i] == 1) {
				flag = 1;
		}
	}
	if(!flag) (*y) = (*y) + 1;
	
	

}
//Edward Leander
void moveRight(char * x, char * y) {
	char nextX = (*x) + 7 + 1;
	char y1 = (*y)+1;
	char flag = 0;
	int i;
	//Teleport to left of screen if reach furthest right
	if ((*x) >= 120){
		*x = 1;
	}
	
	//Looking at right corners of object (hence + 8). If no wall, allow current X to increment (right). Test for y1 (top right) & y2 (bottom right)
	for(i = 0; i < 7; i++) {
		if(map_bitmap[y1+i][nextX] == 1) {
			flag = 1;
		}
	}
	if(!flag) (*x) = (*x) + 1;

}

//Edward Leander
void moveLeft(char * x, char * y) {
	char nextX = (*x) - 1;
	char y1 = (*y)+1;
	char flag = 0;
	int i;
	//Teleport to right of screen if reach furthest left
	if ((*x) <= 1)
		*x = 120;
	
	//Looking at left corners of object. If no wall, allow current X to decrement (left). Test for y1 (top left) & y2 (bottom left)
	for(i = 0; i < 7; i++) {
		if(map_bitmap[y1 + i][nextX] == 1) {
			flag = 1;
		}
	}

	if(!flag) (*x) = (*x) - 1;
}

//Andrejs Prihodko
void player_move(){
	volatile int btnvalue = getbtns();
	volatile int btn1 = getbtn1();
	btnvalue += (btn1 << 3);
		//BTN4
		if ((btnvalue & 4) >> 2)  {
			dir = 3;
			moveUp(&pacman_x, &pacman_y);
		}
		//BTN3
		if ((btnvalue & 3) >> 1) {
			dir = 2;
			moveDown(&pacman_x, &pacman_y);
		} 
		//BTN2
		if ((btnvalue & 1) >> 0) {
			dir = 1;
			moveLeft(&pacman_x, &pacman_y);
		} 
		//BTN1
		if ((btnvalue & 8) >> 3) {
			dir = 0;
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

//Andrejs Prihodko
char checkCollision() {
	int i, j;
	//If distance between center point of each ghost and pacman is below certain threshold, they have collided
	//therefore return 1, otherwise return 0
	if(absVal((ghost1_x + 3) - (pacman_x + 3)) < 3 && absVal((ghost1_y + 3) - (pacman_y + 3)) < 3) return 1;
	else if(absVal((ghost2_x + 3) - (pacman_x + 3)) < 3 && absVal((ghost2_y + 3) - (pacman_y + 3)) < 3) return 1;
	else return 0;
}

//AI Section (G1 = Ghost1, G2 = Ghost2)

//Andrejs Prihodjko
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

//Edward Leander
//Medium AI - given address of ghost 1 or 2 position, then calculates minimum distance based upon hypotenuse
//Uses update strategy to determine which move results in the minimum distance. Then sets flag to determine which move
//is most appropriate
void mediumDiffG1() {
	char flag = 0;
	double temp = 0;
	double minDist = sqrt(absVal(((pacman_x - ghost1_x) * (pacman_x - ghost1_x)) + ((pacman_y - ghost1_y) * (pacman_y - ghost1_y))));
	moveUp(&ghost1_x, &ghost1_y);
	temp = sqrt(absVal(((pacman_x - ghost1_x) * (pacman_x - ghost1_x)) + ((pacman_y - ghost1_y) * (pacman_y - ghost1_y))));
	if(temp <= minDist) {
		minDist = temp;
		flag = 0;
	}
	moveDown(&ghost1_x, &ghost1_y);
	moveDown(&ghost1_x, &ghost1_y);
	temp = sqrt(absVal(((pacman_x - ghost1_x) * (pacman_x - ghost1_x)) + ((pacman_y - ghost1_y) * (pacman_y - ghost1_y))));
	if(temp <= minDist) {
		minDist = temp;
		flag = 1;
	}
	moveUp(&ghost1_x, &ghost1_y);
	moveLeft(&ghost1_x, &ghost1_y);
	temp = sqrt(absVal(((pacman_x - ghost1_x) * (pacman_x - ghost1_x)) + ((pacman_y - ghost1_y) * (pacman_y - ghost1_y))));
	if(temp <= minDist) {
		minDist = temp;
		flag = 2;
	}
	moveRight(&ghost1_x, &ghost1_y);
	moveRight(&ghost1_x, &ghost1_y);
	temp = sqrt(absVal(((pacman_x - ghost1_x) * (pacman_x - ghost1_x)) + ((pacman_y - ghost1_y) * (pacman_y - ghost1_y))));
	if(temp <= minDist) {
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

//Edward Leander
//Hard AI - performs medium difficulty twice effectively doubling speed of ghosts
void hardDiffG1() {
	mediumDiffG1();
	mediumDiffG1();
}

//Andrejs Prihodjko
//Same AI but for Ghost 2 (had to be separate functions because of problems with x and y coordinate pointers)
//This is exactly why object oriented programming was invented...
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

//Andrejs Prihodjko
void mediumDiffG2() {
	char flag = 0;
	double temp = 0;
	double minDist = sqrt(absVal(((pacman_x - ghost2_x) * (pacman_x - ghost2_x)) + ((pacman_y - ghost2_y) * (pacman_y - ghost2_y))));
	moveUp(&ghost2_x, &ghost2_y);
	temp = sqrt(((pacman_x - ghost2_x) * (pacman_x - ghost2_x)) + ((pacman_y - ghost2_y) * (pacman_y - ghost2_y)));
	if(temp <= minDist) {
		minDist = temp;
		flag = 0;
	}
	moveDown(&ghost2_x, &ghost2_y);
	moveDown(&ghost2_x, &ghost2_y);
	temp = sqrt(absVal(((pacman_x - ghost2_x) * (pacman_x - ghost2_x)) + ((pacman_y - ghost2_y) * (pacman_y - ghost2_y))));
	if(temp <= minDist) {
		minDist = temp;
		flag = 1;
	}
	moveUp(&ghost2_x, &ghost2_y);
	moveLeft(&ghost2_x, &ghost2_y);
	temp = sqrt(absVal(((pacman_x - ghost2_x) * (pacman_x - ghost2_x)) + ((pacman_y - ghost2_y) * (pacman_y - ghost2_y))));
	if(temp <= minDist) {
		minDist = temp;
		flag = 2;
	}	
	moveRight(&ghost2_x, &ghost2_y);
	moveRight(&ghost2_x, &ghost2_y);
	temp = sqrt(absVal(((pacman_x - ghost2_x) * (pacman_x - ghost2_x)) + ((pacman_y - ghost2_y) * (pacman_y - ghost2_y))));
	if(temp <= minDist) {
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

//Andrejs Prihodjko
void hardDiffG2() {
	mediumDiffG2();
	mediumDiffG2();
}

//https://stackoverflow.com/questions/18861111/pseudo-code-for-sqrt-function
//Returns square root value of a parameter (taken from StackOverflow)
double sqrt(double n) {
	double x = 1; x < 10; ++x;
	return (x+n/x)/2;
}

//Andrejs Prihodjko
//Returns absolute value of a parameter (had problems importing C Math Library, made our own function)
double absVal(double n) {
	if(n < 0) {
		n *= -1;
	}
	return n;
}



	
