#include "Board.h"
#include "Parameters.h"
#include "Input.h"
#include "Display.h"
#include "Lib.h"
#include "Renderer.h"
#include "ImageData.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define sze(a) sizeof((a))/sizeof((a)[0])

enum tileType ourBoard[COLUMNS * ROWS],
			theirBoard[COLUMNS * ROWS];

#define White          0xFFFF
#define Black          0x0000
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

int main() {
	const static int shipLengths[] = {1, 1, 1, 1, 2, 2, 2, 3, 3, 4};

	buttonsInit();

	SPI2CONSET = 0x20;		// Yes
	SPI2CONSET = 0x8000;	// Yep

	displayinit();
	unsigned char paintArea[4*16*16];
	unsigned char background[4] = {0xff, 0xa7, 0x1a, 0xff};
	fillColor(paintArea, background, 16, 16);
	renderTile(shipPicture, paintArea, 0, 0, 16, 16, 0, 0);

	while(1) {
		paint(colorsTo16Bit(background));
		PORTE = 5;
		for(int i = 0; i < 200; i += 16)
			for(int j = 0; j < 200; j += 16);
		paintimg(paintArea, 16, 16, 0, 0);
		fastsleep(2000);
	}

	for(int ship = 0; ship < sizeof(shipLengths)/sizeof(shipLengths[0]); ++ ship) {
		int cursorX = 0, cursorY = 0, rotation = 0, i;
		retryplace:
		while (1) {
			if(getButtonRight()) // If right cursor is pressed
				++ cursorX;
			if(getButtonLeft()) // ..
				-- cursorX;
			if(getButtonRotate()) // ..
				rotation = !rotation;
			if(getButtonAccept()) // place button is pressed
				break;
			// rerenderPlacement(&ourBoard, cursorX, cursorY, rotation);
		}
		for(i = 0; i < shipLengths[ship]; ++ i)
			if(ourBoard[i*!rotation + cursorX + COLUMNS*(i*rotation + cursorY)])
				goto retryplace;
		for(i = 0; i < shipLengths[ship]; ++ i)
			ourBoard[i*!rotation + cursorX + COLUMNS*(i*rotation + cursorY)] = 1;
	}
}
