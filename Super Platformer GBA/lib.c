#include "lib.h"
#include "startScreen.h"
#include "endScreen.h"
#include "gameScreen.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void loadPalette(int newPalette) {
	//load start screen palette
	if (newPalette == 0) {
		for (int i = 1; i < STARTSCREEN_PALETTE_SIZE; i++) {
			PALETTE[i] = startScreen_palette[i];
		}
	}
	//load game palette
	if (newPalette == 1) {
		for (int i = 0; i < GAMESCREEN_PALETTE_SIZE; i++) {		
			PALETTE[i] = gameScreen_palette[i];
		}
		PALETTE[250] = RGB(17, 31, 31);
		PALETTE[251] = RGB(0, 0, 0);
		PALETTE[252] = RGB(31, 31, 31);	
		PALETTE[253] = RGB(31, 31, 0);
		PALETTE[254] = RGB(31, 0, 0);
	}	
	//load game over screen palette
	if (newPalette == 2) {
		for (int i = 1; i < ENDSCREEN_PALETTE_SIZE; i++) {	
			PALETTE[i] = endScreen_palette[i];
		}
	}
}

void drawImage4(int x, int y, int width, int height, const u16* image) {
	//volatile u16 background = *startScreen_palette;
	/*
	DMA[3].src = image;
	DMA[3].dst = videoBuffer + (x + y * width);
	DMA[3].cnt = (height * width / 2) | DMA_ON | DMA_SOURCE_INCREMENT;
	*/
	for (int a = 0; a < height; a++) {
		DMA[3].src = (image + (width * a) / 2);
		DMA[3].dst = &videoBuffer[(x + 240*(y + a)) / 2];
		DMA[3].cnt = (width / 2) | DMA_ON | DMA_SOURCE_INCREMENT;
	}
}

void drawRect4(int x, int y, int width, int height, const u8 index) {
	volatile u16 color = index | (index << 8);
	for (int a = 0; a < height; a++) {
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[(x + 240*(y + a))/2];
		DMA[3].cnt = (width / 2) | DMA_ON | DMA_SOURCE_FIXED;
	}
}


void flipPage()
{
    if(REG_DISPCNT & BUFFER1FLAG)
    {
        // Bit was on so we were displaying BUFFER1
        REG_DISPCNT = REG_DISPCNT & (~BUFFER1FLAG);
        videoBuffer = VIDEOBUFFER1;
    }
    else
    {
        REG_DISPCNT = REG_DISPCNT | BUFFER1FLAG;
        videoBuffer = VIDEOBUFFER0;
    }
}

void waitForVBlank()
{
    while(SCANLINECOUNTER > 160);
    while(SCANLINECOUNTER < 160);
}