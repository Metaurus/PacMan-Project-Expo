/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */
#include <stdlib.h>
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header.h"  /* Declatations for these labs */

void *stdin, *stdout, *stderr; //Had stdout error (with stdlib or something) https://github.com/is1200-example-projects/mcb32tools/issues/6

extern char pacman_x;
extern char pacman_y;
extern char ghost1_x;
extern char ghost1_y;
extern char ghost2_x;
extern char ghost2_y;

int workTimer = 0;
int score = 0;
int highScore = 0;
char score_text[16] = "Score: "; //Must be length of 16 to write to entire line (16 * 8 = 128)
char highScore_text[16] = "High Score: ";

volatile int *LED;
char difficulty = 0;
char lives = 3;

void wait(int ms);

void user_isr(){}


//Written in conjunction by Edward Leander and Andrejs Prihodko
//does everything
void work() {
	//Conditions to start up the game 
	while(difficulty == 0) {
		wait(100);
		setDifficulty();
		displayDifficulty();
	}
	displayLives();
	resetGameSwitch(); //Checks to see if SW4 is on to reset game
	//While game is running
	if (difficulty != 0 && lives != 0) {
		displayLives(); //Update lights to display lives
		display_map();
		wait(3);
		player_move();
		pacman_draw(pacman_x, pacman_y); //Draw all sprites on screen
		ghost_draw(ghost1_x, ghost1_y);
		ghost_draw(ghost2_x, ghost2_y);
		if(workTimer % 5 == 0) { //Only every 5th iteration of 'work' will increment score and do ghost AI (done to slow down game)
			score++;
			if(score > 9999) score = 9999;
			switch(difficulty) {
			case 1://Easy AI
				easyDiffG1();
				easyDiffG2();
				break;
			case 2://Medium AI
				mediumDiffG1();
				mediumDiffG2();
				break;
			case 3://Hard AI
				hardDiffG1();
				hardDiffG2();
				break;
			default:
				break;
			}
		}
		
		if(checkCollision()) { //Checks if there is a collision between PacMan and Ghosts
			lives--;
			pacman_x = 56;
			pacman_y = 10;
			ghost1_x = rand() % 40;
			ghost1_y = rand() % 28;
			ghost2_x = (rand() % 40) + 80;
			ghost2_y = rand() % 28;
		}
		
		display_update();
	}	
	
	//Game end
	if (lives == 0) {
		if(score > highScore) highScore = score; //If score > highScore, becomes new high score
		displayLives(); //Makes sure all LED lights are off on game over
		convertScore(); //Adds score values into displayable char array (makes sure everything is on one line)
		display_score(2, score_text); //Displays score on line 2 (2*8 = 16 bits down)
		display_score(3, highScore_text); //Displays highscore on line 3 (3*8 = 24 bits down)
		display_end(); //Shows end screen and prints scores
		wait(1000); //Waits certain amount of time before returning to main menu screen
		resetGame(); //Resets the game
	}
	workTimer++; //Separate counter of 'work' function, increments on each iteration of 'work'
}

// Initialization
void init(){
	//Created in Lab 3 by Andrejs Prihodjko and Edward Leander
	LED = (volatile int *) 0xbf886110; //Set LED address equal to the PORTE address
	*LED &= 0x0ff; //Mask the last byte 
	
	PORTE = 0;
	
	//Button 2-4 and switches initialization
	TRISDSET = 0xFE0;
	TRISFSET = 0x2; //BTN1 implementation
	
	
	//Timer initialization
	T2CONSET = 0x8070;
	PR2 = 0x4e2;

	enable_interrupt();
	
	return;
	
}

// delay function
//Andrejs Prihodjko
void wait(int ms){
	TMR2 = 0;
    int i = 0;
    while(i < ms)
    {
    	while((IFS(0) & 0x0100) == 0);
    	IFS(0) &= ~0x0100;
    	i++;
    	
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

//Andrejs Prihodjko
void displayDifficulty() {
	
	//Check difficulty setting and turn on LED's accordingly (three on far left)
	switch(difficulty) {
		case 3:
			PORTE += (0x1 << 5);
			break;
		case 2:
			PORTE += (0x1 << 6);
			break;
		case 1:
			PORTE += (0x1 << 7);
			break;
		default:
			break;
	}
	
}

//Edward Leander
void setDifficulty() {
		volatile int swValue = 0;
		while(difficulty == 0) {
			swValue = (volatile int) getsw();
			if (swValue == 8) {
				difficulty = 1;
			}

			//SW3, Middle switch
			if (swValue == 4) {
				difficulty = 2; 
			}

			//SW2, Furthest right Switch
			if (swValue == 2) {
				difficulty = 3; 
			}
		}
		
		
}

//BTN2 = 001
//BTN3 = 010
//BTN4 = 100

//SW1 = 0001
//SW2 = 0010
//SW3 = 0100
//SW4 = 1000	

//Edward Leander
void resetGameSwitch() {
	volatile int swValue = 0;
	swValue = getsw();
	if (swValue == 1){
		difficulty = 0;
		lives = 3;
		display_start();
		display_update();
		
		score = 0;
		highScore = 0;
		workTimer = 0;
		pacman_x = 56;
		pacman_y = 10;
		ghost1_x = 80;
		ghost1_y = 10;
		ghost2_x = 30;
		ghost2_y = 10;
	
	}
}
//Edward Leander
void resetGame() {
	//Resets a lot of frequently used variables and goes to start screen
		difficulty = 0;
		lives = 3;
		score = 0;
		display_start();
		display_update();
		
		pacman_x = 56;
		pacman_y = 10;
		ghost1_x = 80;
		ghost1_y = 10;
		ghost2_x = 30;
		ghost2_y = 10;
}
//Andrejs Prihodjko
void convertScore() {
	//Takes score and highscore, separates into digits, and adds them into score character array to fit all on one line
	int i, j, modulus = 10000;
	for(i = 7; i <= 10; i++) { //Indices of score text array which will display score value
		score_text[i] = ((score % modulus) / (modulus/10)) + 48; //Add 48 to convert into proper ASCII value
		modulus /= 10;
	}
	modulus = 10000;
	for(i = 12; i <= 15; i++) {
		highScore_text[i] = ((highScore % modulus) / (modulus/10)) + 48;
		modulus /= 10;
	}
}


	
