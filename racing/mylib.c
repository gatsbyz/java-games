//Yuna Lee

#include "mylib.h"
#define OFFSET(r, c, width) ((r)*width + (c))

u16 *videoBuffer = (u16*) 0x6000000;

void setPixel(int r, int c, u16 color){
	videoBuffer[OFFSET(r, c, SCREEN_WIDTH)] = color;}

void drawRect(int r, int c, int width, int height, u16 color){
	for (int i = 0; i < height; i++) {
		REG_DMA[3].src = &color;
		REG_DMA[3].dst = videoBuffer + OFFSET(r+i, c, SCREEN_WIDTH);
		REG_DMA[3].cnt = DMA_ON | DMA_SRC_FIXED | width;}
}

void removeRect(int r, int c, int width, int height){
	u16 bg_color = BLACK;
	for (int i = 0; i < height; i++) {
		REG_DMA[3].src = &bg_color;
		REG_DMA[3].dst = videoBuffer + OFFSET(r+i, c, SCREEN_WIDTH);
		REG_DMA[3].cnt = DMA_ON | DMA_SRC_FIXED | width;
	}
}

void drawImage3(int r, int c, int width, int height, const u16* image){
	for (int i = 0; i < height; i++) {
		REG_DMA[3].src = &image[OFFSET(i, 0, width)];
		REG_DMA[3].dst = videoBuffer + OFFSET(r+i, c, SCREEN_WIDTH);
		REG_DMA[3].cnt = DMA_ON | width;}
}

// returns the color
u16 getPixel(int r, int c){
	return videoBuffer[OFFSET(r, c, SCREEN_WIDTH)];}

void waitForVBlank(){
    while (REG_VCOUNT >= 160); 
    while (REG_VCOUNT < 160); 
}

// random int
int random(int start, int end)
{
	return (rand()% (end-start)) + start;
}
