#include "Board.h"
#include "Parameters.h"
#include "Input.h"
#include "Display.h"
#include "Lib.h"
#include "Renderer.h"

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

	for(int ship = 0; ship < sizeof(shipLengths)/sizeof(shipLengths[0]); ++ ship) {
		int cursorX = 0, cursorY = 0, rotation = 0;
		printBoard(ourBoard);
		retryplace:
		while (1) {
			for(int i = 0; i < shipLengths[ship]; ++ i)
				ourBoard[cursorX + !rotation*i + (cursorY + rotation*i)*COLUMNS] |= TILE_IS_PLACING;
			while(!getButtons());
			if(getButtonRight()) {
				if(cursorX + !rotation*shipLengths[ship] >= COLUMNS) continue;
				for(int i = 0; i < shipLengths[ship]; ++ i)
					ourBoard[cursorX + !rotation*i + (cursorY + rotation*i)*COLUMNS] &= ~TILE_IS_PLACING;
				++ cursorX;
				while(getButtonRight());
			}

			if(getButtonLeft()) {
				if(!cursorX) continue;
				for(int i = 0; i < shipLengths[ship]; ++ i)
					ourBoard[cursorX + !rotation*i + (cursorY + rotation*i)*COLUMNS] &= ~TILE_IS_PLACING;
				-- cursorX;
				while(getButtonLeft());
			}

			if(getButtonDown()) {
				if(cursorY + rotation*shipLengths[ship] >= ROWS) continue;
				for(int i = 0; i < shipLengths[ship]; ++ i)
					ourBoard[cursorX + !rotation*i + (cursorY + rotation*i)*COLUMNS] &= ~TILE_IS_PLACING;
				++ cursorY;
				while(getButtonDown());
			}

			if(getButtonUp()) {
				if(!cursorY) continue;
				for(int i = 0; i < shipLengths[ship]; ++ i)
					ourBoard[cursorX + !rotation*i + (cursorY + rotation*i)*COLUMNS] &= ~TILE_IS_PLACING;
				-- cursorY;
				while(getButtonUp());
			}

			if(getButtonRotate()) {
				rotation = !rotation;
				for(int i = 0; i < shipLengths[ship]; ++ i)
					ourBoard[cursorX + !rotation*i + (cursorY + rotation*i)*COLUMNS] &= ~TILE_IS_PLACING;
				if(cursorX + !rotation*shipLengths[ship] >= 10) cursorX -= shipLengths[ship] - 1;
				if(cursorY +  rotation*shipLengths[ship] >= 10) cursorY -= shipLengths[ship] - 1;
				while(getButtonRotate());
			}

			if(getButtonAccept()) {
				while(getButtonAccept());
				break;
			}
			printBoard(ourBoard);
			fastsleep(1000);
		}
		for(int i = 0; i < shipLengths[ship]; ++ i)
			if(ourBoard[i*!rotation + cursorX + COLUMNS*(i*rotation + cursorY)])
				goto retryplace;
		for(int i = 0; i < shipLengths[ship]; ++ i)
			ourBoard[i*!rotation + cursorX + COLUMNS*(i*rotation + cursorY)] = 1;
	}
}
