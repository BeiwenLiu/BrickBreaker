#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mylib.h"

u16* videoBuffer = (u16*) 0x6000000;

double sqrt(double square) {
	double root = square/3, last, diff=1;
	if (square <= 0) return 0;
	do {
		last = root;
		root = (root + square / root) / 2;
		diff = root - last;
	} while (diff > 2.25e-308 || diff < -2.25e-308);
	return root;
}

void setPixel(int x, int y, u16 color)
{
    videoBuffer[x * 240 + y] = color;
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void drawCircle(int x0, int y0, int radius, unsigned short color)
{
    int x = radius;
    int y = 0;
    int xChange = 1 - (radius << 1);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y)
    {
        for (int i = y0 - x; i <= y0 + x; i++)
        {
            setPixel(x0 + y, i, color);
            setPixel(x0 - y, i, color);
        }
        for (int i = y0 - y; i <= y0 + y; i++)
        {
            setPixel(x0 + x, i, color);
            setPixel(x0 - x, i, color);
        }

        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}

void redrawCircle(int x0, int y0, int radius, const unsigned short *image)
{
    int x = radius;
    int y = 0;
    int xChange = 1 - (radius << 1);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y)
    {
        for (int i = y0 - x; i <= y0 + x; i++)
        {
            setPixel(x0 + y, i, image[(x0+y)*240 + i]);
            setPixel(x0 - y, i, image[(x0-y)*240 + i]);
        }
        for (int i = y0 - y; i <= y0 + y; i++)
        {
            setPixel(x0 + x, i, image[(x0+x)*240 + i]);
            setPixel(x0 - x, i, image[(x0-x)*240 + i]);
        }

        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}

void drawSquare(int x, int y, int side, unsigned short color) 
{
	for (int i = 0; i < side; i++) {
		for (int j = 0; j < side; j++) {
			setPixel(x + i, y +j, color);
		}
	}
}

void spawnBlocker(int x, int y, int width, int height, u16 color) {
	for (int i = 0; i < height; i++) {
		DMA[DMA_CHANNEL_3].src = &color;
		DMA[DMA_CHANNEL_3].dst = &videoBuffer[(i+x)*240 + y];
		DMA[DMA_CHANNEL_3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;
	}
}

void respawnBlockerLeft(int x, int y, int width, int height, const unsigned short *color) {
	for (int i = 0; i < height; i++) {
		DMA[DMA_CHANNEL_3].src = &color[(i+x)*240 + y + width - 1];
		DMA[DMA_CHANNEL_3].dst = &videoBuffer[(i+x)*240 + y];
		DMA[DMA_CHANNEL_3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;
	}
}

void respawnBlockerRight(int x, int y, int width, int height, const unsigned short *color) {
	for (int i = 0; i < height; i++) {
		DMA[DMA_CHANNEL_3].src = &color[(i+x)*240 + y];
		DMA[DMA_CHANNEL_3].dst = &videoBuffer[(i+x)*240 + y];
		DMA[DMA_CHANNEL_3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;
	}
}

int checkBottomCollision(BRICK brick, BALL ball) {
	if ((ball.x0 - ball.radius <= brick.x + brick.height && ball.x0 - ball.radius > brick.x + brick.height/2) && (ball.y0 + ball.radius > brick.y && ball.y0 - ball.radius < brick.y + brick.width)) { return 1; }
	else { return 0; }
}

int checkTopCollision(BRICK brick, BALL ball) {
	if ((ball.radius+ball.x0 > brick.x - 1 && ball.radius + ball.x0 < brick.x + brick.height/2) && (ball.y0 + ball.radius > brick.y && ball.y0 - ball.radius < brick.y + brick.width))
	{return 1;}
	else {return 0;}
}

int checkLeftCollision(BRICK brick, BALL ball) {
	if ((ball.radius+ball.y0 > brick.y && ball.radius + ball.y0 < brick.y + brick.width/2) && (ball.x0 + ball.radius > brick.x && ball.x0 - ball.radius < brick.x + brick.height))
	{return 1;}
	else {return 0;}
}

int checkRightCollision(BRICK brick, BALL ball) {
	if ((ball.y0 - ball.radius < brick.y + brick.width && ball.y0 - ball.radius > brick.y + brick.width/2) && (ball.x0 + ball.radius > brick.x && ball.x0 - ball.radius < brick.x + brick.height))
	{return 1;}
	else {return 0;}
}

void delay(int n)
{
	volatile int x = 0;
	for (int i = 0; i < 5000*n; i++) {
		x = x + 1;
	}
}

void drawImage3(int x, int y, int width, int height, const unsigned short *image) {

	// TODO Draw the image with the top left corner at (x,y).
	// Recall that the GBA screen is 240 pixels wide, and 160 tall.
	// The videoBuffer is a 1-dimensional array of these pixels.
	// For example, to set pixel (37,21) to white:
	//   videoBuffer[21 * 240 + 37] = 0x7FFF;
	
	for (int i = 0; i < height ; i++) {
		for (int j = 0; j < width; j++) {
			videoBuffer[(y+i)*240 + (x+j)] = image[i*width+j];		
		}	
	}


}

void redrawBrick(int x, int y, int width, int height, const unsigned short *image) {
	for (int i = 0; i < height; i++) {
		DMA[DMA_CHANNEL_3].src = &image[(i+x)*240 + y];
		DMA[DMA_CHANNEL_3].dst = &videoBuffer[(i+x)*240 + y];
		DMA[DMA_CHANNEL_3].cnt = width | DMA_ON;
	}
}

void drawScoreLine(int row) {
	for (int i = 0; i < 240; i++) {
		setPixel(row,i,WHITE);
	}
}


