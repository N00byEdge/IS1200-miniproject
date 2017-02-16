#include "Board.h"
#include "Renderer.h"
#include "Parameters.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define sze(a) sizeof((a))/sizeof((a)[0])

enum tileType ourBoard[COLUMNS * ROWS],
			theirBoard[COLUMNS * ROWS];

int main() {
	const static int shipLengths[] = {1, 1, 1, 1, 2, 2, 2, 3, 3, 4};

	for(int ship = 0; ship < sizeof(shipLengths)/sizeof(shipLengths[0]); ++ ship) {
		int cursorX = 0, cursorY = 0, rotation = 0, i;
		retryplace:
		while (1) {
			if(0) // If right cursor is pressed
				++ cursorX;
			if(0) // ..
				-- cursorX;
			if(0)// ..
				rotation = !rotation;
			if(0) // place button is pressed
}
