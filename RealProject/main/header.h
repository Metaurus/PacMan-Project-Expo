/* mipslab.h
   Header file for all labs.
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

//Heavily modified and added on to by Edward Leander and Andrejs Prihodjko 2020-02

// display
void display_init(void);
void display_score(int line, char *s);
void display_end(void);
void display_start(void);
void display_map(void);
void display_update(void);
uint8_t display_send(uint8_t data);
void pixel_on(char x, char y);
void quicksleep(int cyc);
void pixel_off(char x, char y);
void pacman_draw(char x, char y);
void ghost_draw(char x, char y);
void reset(void);
char * itoaconv( int num );
void pixel_set(char x, char y, char set);
void drawSprite(char array[8][8], char x, char y);


//Bitmaps
char display_matrix[128][32];
extern const uint8_t const font[128*8];
extern char textbuffer[4][16];
char gameover_bitmap[16][128];
char map_bitmap[32][128];
char pacman_bitmapRIGHT[8][8];
char pacman_bitmapLEFT[8][8];
char pacman_bitmapUP[8][8];
char pacman_bitmapDOWN[8][8];
char start_bitmap[32][128];
char ghost_bitmap[8][8];

//Movement
void moveUp(char * x, char * y);
void moveDown(char * x, char * y);
void moveLeft(char * x, char * y);
void moveRight(char * x, char * y);

void player_move();
char checkCollision();
//AI
void easyDiffG1();
void mediumDiffG1();
void hardDiffG1();
void easyDiffG2();
void mediumDiffG2();
void hardDiffG2();
double sqrt(double n);
double absVal(double n);

//Main
void init();

//Input
int getbtns(void);
int getbtn1(void);
int getsw(void);
void enable_interrupt(void);

//work
void setDifficulty();
void displayDifficulty();
void displayLives();
void work();
void resetGameSwitch();
void resetGame();
void convertScore();