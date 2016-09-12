#include <stdlib.h>
#include <stdio.h>
#include "mylib.h"
#include "arkanoid.h"
#include "background.h"
#include "gameover.h"
#include "text.h"
#include "ball.h"

int main(void)
{	
	drawImage3(0,0,ARKANOID_WIDTH,ARKANOID_HEIGHT,arkanoid);
	int drewBeginning = 0;
	int drewEnd = 0;
	int changed = 0;
	int changedLives = 0;
	int lives = NUMLIVES;
	char lifeCount[80];
	char oldLifeCount[80];
	char oldStr[80]; //Keep track of old score
	char str[80]; //To print out score
	int score = 0; //Keep track of score
	int startover = 0; //For reinitialization
	int state = 0; //To select which image to display
	int started = 0; //To determine when ball should release
//------------------------------------------------------------
//Initial Slider conditions
	SLIDER slider;
	slider.x = INITIALSLIDERX;
	slider.y = INITIALSLIDERY;
	slider.width = SLIDERWIDTH;
	slider.height = SLIDERHEIGHT;
	slider.speed = SLIDERSPEED;
//------------------------------------------------------------
//Old Slider
	SLIDER oldSlider = slider;
//------------------------------------------------------------
// Initial Ball Conditions
	BALL ball;
	ball.x0 = INITIALBALLX;
	ball.y0 = INITIALBALLY;
	ball.radius = BALLRADIUS;
	ball.xdel = -1;
	ball.ydel = 0;
	REG_DISPCNT = MODE3 | BG2_ENABLE;
//------------------------------------------------------------
// Old Ball Conditions
	BALL oldBall = ball;
//------------------------------------------------------------
// Brick array
	BRICK brickArr[BRICKSIZE];
	//BRICK *ballPointer;
	int prevCol = 0;
	int prevRow = BRICKXOFFSET;
	for (int i = 0; i < BRICKSIZE; i++) {
		brickArr[i].width = BRICKWIDTH;
		brickArr[i].height = BRICKHEIGHT;
		brickArr[i].hit = 0;
		brickArr[i].color = WHITE;
		if (i%10 == 0) {
			brickArr[i].x = prevRow;
			brickArr[i].y = BRICKYOFFSET + prevCol;
			prevCol = brickArr[i].y + brickArr[i].width + BRICKMARGIN;
		} else if (i%10 == 9) {
			brickArr[i].y = prevCol;
			brickArr[i].x = prevRow;
			prevCol = 0;
			prevRow = prevRow + brickArr[i].height + BRICKMARGIN;
		} else {
			brickArr[i].x = prevRow;
			brickArr[i].y = prevCol;
			prevCol = prevCol + brickArr[i].width + BRICKMARGIN;
		}
	}
//-----------------------------------------------------------------
	while (1)
{	

	if(KEY_DOWN_NOW(BUTTON_SELECT)) {
		drawImage3(0,0,ARKANOID_WIDTH,ARKANOID_HEIGHT,arkanoid);
		state = 0;
		drewEnd = 0;
		drewBeginning = 0;
	}

	if(state == 2) {
		if(!drewEnd) {
			drawImage3(0,0,GAMEOVER_WIDTH,GAMEOVER_HEIGHT,gameover);
			drewEnd = 1;
		}
		if(score == 60) {
			drawString(10,95,"YOU WIN!",WHITE);
		}
	}

	if (state == 0) {
		if (!drewBeginning) {
			drawString(130,65,"Made by Beiwen Liu", WHITE);
			drawString(100,50,"Press Start To Continue", WHITE);
			drewBeginning = 1;
		}
		if(KEY_DOWN_NOW(BUTTON_START)) {
			drawImage3(0,0,BACKGROUND_WIDTH,BACKGROUND_HEIGHT,background);
			//------------------------------------------------------------
			//INITIALIZE CONDITIONS
			slider.x = INITIALSLIDERX;
			slider.y = INITIALSLIDERY;
			slider.width = SLIDERWIDTH;
			slider.height = SLIDERHEIGHT;
			slider.speed = SLIDERSPEED;
			oldSlider = slider;
			ball.x0 = INITIALBALLX;
			ball.y0 = INITIALBALLY;
			ball.radius = BALLRADIUS;
			ball.xdel = -2;
			ball.ydel = 0;
			oldBall = ball;
			//---------------------------------------------------------------
			startover = 0;
			started = 0;
			prevCol = 0;
			prevRow = BRICKXOFFSET;
			lives = NUMLIVES;
			score = 0; //Keep track of score
			sprintf(str,"Score: %i",score);
			drawString(1,10,str,WHITE);
			sprintf(lifeCount,"Lives: %i",lives);
			drawString(1,180,lifeCount, WHITE);
			
			for (int i = 0; i < BRICKSIZE; i++) {
				brickArr[i].width = BRICKWIDTH;
				brickArr[i].height = BRICKHEIGHT;
				brickArr[i].hit = 0;
				brickArr[i].color = WHITE;
				if (i%10 == 0) {
					brickArr[i].x = prevRow;
					brickArr[i].y = BRICKYOFFSET + prevCol;
					prevCol = brickArr[i].y + brickArr[i].width + BRICKMARGIN;
				} else if (i%10 == 9) {
					brickArr[i].y = prevCol;
					brickArr[i].x = prevRow;
					prevCol = 0;
					prevRow = prevRow + brickArr[i].height + BRICKMARGIN;
				} else {
					brickArr[i].x = prevRow;
					brickArr[i].y = prevCol;
					prevCol = prevCol + brickArr[i].width + BRICKMARGIN;
				}		
			}
			for (int i = 0; i < BRICKSIZE; i++) {
				spawnBlocker(brickArr[i].x, brickArr[i].y, brickArr[i].width, brickArr[i].height, brickArr[i].color);
			}
			drawScoreLine(SCORELINEMARGIN);
			state = 1;
		}
	}


	if (state == 1) {
		if (startover) {
			//------------------------------------------------------------
			//REINITIALIZE CONDITIONS
			slider.x = INITIALSLIDERX;
			slider.y = INITIALSLIDERY;
			slider.width = SLIDERWIDTH;
			slider.height = SLIDERHEIGHT;
			slider.speed = SLIDERSPEED;
			ball.x0 = INITIALBALLX;
			ball.y0 = INITIALBALLY;
			ball.radius = BALLRADIUS;
			ball.xdel = -2;
			ball.ydel = 0;
			startover = 0;
			started = 0;
			//---------------------------------------------------------------
		}
		if(ball.x0 + ball.radius > 159) {
			if(lives == 0) {
				state = 2;
			} else {
				changedLives = 1;
				sprintf(oldLifeCount,lifeCount);
				lives--;
				sprintf(lifeCount, "Lives: %i", lives);
				startover = 1;
			}
		}

		if(score == BRICKSIZE) {
			state = 2;
		}

		if(KEY_DOWN_NOW(BUTTON_LEFT)) {
			if(!started) {
				started = 1;
				ball.ydel = -1;
			}
			slider.y = slider.y - slider.speed;
			if (slider.y < 0) {
				slider.y = 0;
			}
		}
		
		if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
			if(!started) {
				started = 1;
				ball.ydel = 1;
			}
			slider.y = slider.y + slider.speed;
			if (slider.y + slider.width > 239) {
				slider.y = 239 - slider.width;
			}
		}

		if(KEY_DOWN_NOW(BUTTON_R)) {
			if (slider.y + slider.width + 1 <= 239) {
				slider.width = slider.width + 1;
			}	
		}

		if(KEY_DOWN_NOW(BUTTON_L)) {
			if (slider.width >= 15) {
				slider.width = slider.width - 1;
			}
		}
	
		if(started) {
			ball.x0 += ball.xdel;
			ball.y0 += ball.ydel;
			if(ball.x0 - ball.radius <= 11) {
				ball.xdel = -ball.xdel; //Checks for collision with top line
			}
			if(ball.radius + ball.y0 > 239) { //Checks for collision with right wall
				ball.y0 = 239 - ball.radius;
				ball.ydel = -ball.ydel;
			}
		
			if(ball.y0 - ball.radius < 0) { //Checks for collision with left wall
				ball.y0 = ball.radius;
				ball.ydel = -ball.ydel;
			}
			if((ball.radius+ball.x0) > (slider.x)) {
				if(ball.y0 + ball.radius > slider.y && ball.y0 - ball.radius < slider.y + slider.width) {
					ball.xdel = -ball.xdel;
					if (ball.y0 + ball.radius > slider.y && ball.y0 <= slider.y + slider.width/5) {
						ball.ydel = -2;				
					} else if (ball.y0 > slider.y + (slider.width/5) && ball.y0 <= slider.y + 2*slider.width/5) {
						ball.ydel = -1;				
					} else if (ball.y0 > slider.y + 2*(slider.width/5) && ball.y0 < slider.y + 3*slider.width/5) {
						
					} else if (ball.y0 >= slider.y + 3*(slider.width/5) && ball.y0 < slider.y + 4*slider.width/5) {
						ball.ydel = 1;
					} else if (ball.y0 >= slider.y + 4*slider.width/5 && ball.y0 - ball.radius < slider.y + slider.width) 						{
						ball.ydel = 2;				
					} 
				}
			} 

			for (int i = 0; i < BRICKSIZE; i++) {
				if(!brickArr[i].hit) {
					if (checkBottomCollision(brickArr[i],ball)) {
						brickArr[i].hit = 1;
						brickArr[i].color = BLACK;
						ball.xdel = -ball.xdel;
						sprintf(oldStr, str);
						score++;
						sprintf(str,"Score: %i",score);
						changed = 1;
					} else if (checkTopCollision(brickArr[i],ball)) {
						brickArr[i].hit = 1;
						brickArr[i].color = BLACK;
						ball.xdel = -ball.xdel;
						sprintf(oldStr, str);
						score++;
						sprintf(str,"Score: %i",score);
						changed = 1;
					} else if (checkLeftCollision(brickArr[i],ball)) {
						brickArr[i].hit = 1;
						brickArr[i].color = BLACK;
						ball.ydel = -ball.ydel;
						sprintf(oldStr, str);
						score++;
						sprintf(str,"Score: %i",score);
						changed = 1;
					} else if (checkRightCollision(brickArr[i],ball)) {
						brickArr[i].hit = 1;
						brickArr[i].color = BLACK;
						ball.ydel = -ball.ydel;
						sprintf(oldStr, str);
						score++;
						sprintf(str,"Score: %i",score);
						changed = 1;
					}
				}
			}
		}


		
		
		waitForVblank();
		for (int i = 0; i < BRICKSIZE; i++) {
			if(brickArr[i].hit) {
				redrawBrick(brickArr[i].x, brickArr[i].y, brickArr[i].width, brickArr[i].height, background);
			}
		}
		redrawCircle(oldBall.x0,oldBall.y0,oldBall.radius,background);
		drawCircle(ball.x0,ball.y0,ball.radius,WHITE);
		oldBall = ball;
		
		if(slider.y > oldSlider.y) {
			respawnBlockerRight(oldSlider.x,oldSlider.y,oldSlider.width,oldSlider.height,background);
		} else {
			respawnBlockerLeft(oldSlider.x,oldSlider.y,oldSlider.width,oldSlider.height,background);
		}
		spawnBlocker(slider.x,slider.y,slider.width,slider.height,WHITE);
		oldSlider = slider;
		}
		
		if (changed) {
			drawOldString(1,10,oldStr,background);
			drawString(1,10,str,WHITE);
			changed = 0;
		}
		
		if (changedLives) {
			drawOldString(1,180,oldLifeCount, background);
			drawString(1,180,lifeCount,WHITE);
			changedLives = 0;
		}
	}
	return 0;
}
