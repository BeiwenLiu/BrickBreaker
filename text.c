#include "mylib.h"
#include "text.h"

void drawChar(int row, int col, char ch, u16 color)
{
	for(int r=0; r<8; r++) {
		for(int c=0; c<6; c++) {
			if(fontdata_6x8[((r*6) + c) + ch*48]) {
				setPixel(row+r, col+c, color);
			}
		}
	}
}

void drawString(int row, int col, char*str, u16 color)
{
	while(*str) {
		drawChar(row,col,*str++,color);
		col += 6;
	}
}

void drawOldChar(int row, int col, char ch, const unsigned short *image) {
	for(int r=0; r<8; r++) {
		for(int c=0; c<6; c++) {
			if(fontdata_6x8[((r*6 +c) + ch*48)]) {
				setPixel(row+r, col+c, image[(row+r)*240 + col+c]);
			}
		}
	}
}

void drawOldString(int row, int col, char*str, const unsigned short *image) {
	while(*str) {
		drawOldChar(row,col,*str++,image);
		col += 6;
	}
}