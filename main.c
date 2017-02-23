#include "Board.h"
#include "Parameters.h"
#include "Input.h"
#include "Display.h"
#include "Lib.h"

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

	buttonsINIT();

	displayinit();

	while(1) {
		paint(White);
		//fastsleep(100);
		paint(Black);
		//fastsleep(100);
		paint(Blue);
		//fastsleep(100);
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
