/* mipslab.h
   Header file for all labs.
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

// display
void display_init(void);
void display_score( char *s, char line);
void display_end(void);
void display_start(void);
void display_map(void);
void display_update(void);
uint8_t display_send(uint8_t data);
void pixel_on(char x, char y);
void quicksleep(int cyc);
void pixel_off(char x, char y);
void reset(void);
char * itoaconv( int num );
void pixel_set(char x, char y, char set);


//Bitmaps
char display_matrix[128][32];
extern const uint8_t const font[128*8];
extern char textbuffer[4][16];
char gameover_bitmap[16][128];
char obstacle_bitmap[8][8];
char superman_bitmap[8][8];
char start_bitmap[32][128];


//Main
void init();

//Input
int getbtns(void);
int getsw(void);
void enable_interrupt(void);

//work
void setDifficulty();
void displayDifficulty();
void displayLives();
void work();