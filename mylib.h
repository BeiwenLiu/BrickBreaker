#ifndef MY_LIB_H
#define MY_LIB_H

typedef unsigned short u16;
typedef unsigned int u32;

typedef volatile unsigned short vu16;


#define REG_DISPCNT *(u16*) 0x4000000
#define SCANLINECOUNTER *(vu16 *)0x4000006

#define BG2_ENABLE (1 << 10)
#define MODE3 3

#define RGB(r, g, b) ((r) | ((g) << 5) | ((b) << 10))
#define WHITE RGB(31,31,31)
#define BLACK RGB(0,0,0)
#define RED RGB(31,0,0)
#define WIDTH 240
#define HEIGHT 160

/* Buttons */

#define BUTTON_A      (1<<0)
#define BUTTON_B      (1<<1)
#define BUTTON_SELECT (1<<2)
#define BUTTON_START  (1<<3)
#define BUTTON_RIGHT  (1<<4)
#define BUTTON_LEFT   (1<<5)
#define BUTTON_UP     (1<<6)
#define BUTTON_DOWN   (1<<7)
#define BUTTON_R      (1<<8)
#define BUTTON_L      (1<<9)

#define BUTTONS (*(vu16*)0x04000130)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)


/* Ignore this for now LOL STRUCT*/
typedef struct
{
	const volatile void *src;
	volatile void *dst;
	volatile u32 cnt;
} DMAREC;

#define DMA ((volatile DMAREC *)0x040000B0)

#define DMA_CHANNEL_3 3

#define DMA_SOURCE_FIXED (2 << 23)

/* When to DMA flags */
#define DMA_NOW (0 << 28)

#define DMA_ON (1 << 31)

#define SCORELINEMARGIN 10
#define BRICKYOFFSET 2
#define BRICKMARGIN 4
#define BRICKXOFFSET 20
#define BRICKWIDTH 20
#define BRICKHEIGHT 4
#define BRICKSIZE 60
#define NUMLIVES 3
#define SLIDERWIDTH 20
#define SLIDERHEIGHT 2
#define SLIDERSPEED 2
#define INITIALSLIDERX 150
#define INITIALSLIDERY 110
#define BALLRADIUS 2
#define INITIALBALLX 147
#define INITIALBALLY 120
extern u16* videoBuffer;

void setPixel(int x, int y, u16 color);
void waitForVblank();
void drawCircle(int x0, int y0, int radius, unsigned short color);
void drawSquare(int x, int y, int side, unsigned short color);
void spawnBlocker(int x, int y, int width, int height, u16 color);

typedef struct
{
	int x;
	int y;
	int width;
	int height;
	int speed;
} SLIDER;

typedef struct
{
	int x0;
	int y0;
	int radius;
	int xdel;
	int ydel;
} BALL;

typedef struct
{
	int x;
	int y;
	int width;
	int height;
	int hit;
	u16 color;
} BRICK;

int checkBottomCollision(BRICK brick, BALL ball);
int checkTopCollision(BRICK brick, BALL ball);
int checkLeftCollision(BRICK brick, BALL ball);
int checkRightCollision(BRICK brick, BALL ball);
void drawImage3(int x, int y, int width, int height, const unsigned short *image);
void drawChar(int row, int col, char ch, u16 color);
void drawString(int row, int col, char*str, u16 color);
void delay(int n);
void redrawBrick(int x, int y, int width, int height, const unsigned short *image);
void redrawCircle(int x0, int y0, int radius, const unsigned short *image);
void redrawCircle1(int x0, int y0, int radius, const unsigned short *image);
void respawnBlockerRight(int x, int y, int width, int height, const unsigned short *color);
void respawnBlockerLeft(int x, int y, int width, int height, const unsigned short *color);
void drawScoreLine(int row);

#endif 
