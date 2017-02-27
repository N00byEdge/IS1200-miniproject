#include "Board.h"
#include "Parameters.h"
#include "Input.h"
#include "Display.h"
#include "Lib.h"
#include "Renderer.h"
#include "Minicomms.h"

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
	const static int shipLengths[] = {5, 4, 3, 3, 2};

	SPI2CONSET = 0x20;		// Yes
	SPI2CONSET = 0x8000;	// Yep

	displayinit();
	buttonsInit();
	printBoard(theirBoard, 0);

	// Ship placement loop
	for(int ship = 0; ship < sizeof(shipLengths)/sizeof(shipLengths[0]); ++ ship) {
		int cursorX = 0, cursorY = 0, rotation = 0;
		for(int i = 0; i < shipLengths[ship]; ++ i)
			ourBoard[cursorX + !rotation*i + (cursorY + rotation*i)*COLUMNS] |= TILE_IS_PLACING;
		retryplace:
		while (1) {
			for(int i = 0; i < shipLengths[ship]; ++ i)
				ourBoard[cursorX + !rotation*i + (cursorY + rotation*i)*COLUMNS] |= TILE_IS_PLACING;
			printBoard(ourBoard, 160);
			nvm:
			while(!getButtons());
			if(getButtonRight()) {
				if(cursorX + !rotation*shipLengths[ship] + rotation >= COLUMNS) goto nvm;
				for(int i = 0; i < shipLengths[ship]; ++ i)
					ourBoard[cursorX + !rotation*i + (cursorY + rotation*i)*COLUMNS] &= ~TILE_IS_PLACING;
				++ cursorX;
				while(getButtonRight());
			}

			if(getButtonLeft()) {
				if(!cursorX) goto nvm;
				for(int i = 0; i < shipLengths[ship]; ++ i)
					ourBoard[cursorX + !rotation*i + (cursorY + rotation*i)*COLUMNS] &= ~TILE_IS_PLACING;
				-- cursorX;
				while(getButtonLeft());
			}

			if(getButtonDown()) {
				if(cursorY + rotation*shipLengths[ship] + !rotation >= ROWS) goto nvm;
				for(int i = 0; i < shipLengths[ship]; ++ i)
					ourBoard[cursorX + !rotation*i + (cursorY + rotation*i)*COLUMNS] &= ~TILE_IS_PLACING;
				++ cursorY;
				while(getButtonDown());
			}

			if(getButtonUp()) {
				if(!cursorY) goto nvm;
				for(int i = 0; i < shipLengths[ship]; ++ i)
					ourBoard[cursorX + !rotation*i + (cursorY + rotation*i)*COLUMNS] &= ~TILE_IS_PLACING;
				-- cursorY;
				while(getButtonUp());
			}

			if(getButtonRotate()) {
				if(shipLengths[ship] == 1) goto nvm;
				for(int i = 0; i < shipLengths[ship]; ++ i)
					ourBoard[cursorX + !rotation*i + (cursorY + rotation*i)*COLUMNS] &= ~TILE_IS_PLACING;
				rotation = !rotation;
				if(cursorX + !rotation*shipLengths[ship] >= 10) cursorX -= shipLengths[ship] - 1;
				if(cursorY +  rotation*shipLengths[ship] >= 10) cursorY -= shipLengths[ship] - 1;
				while(getButtonRotate());
			}

			if(getButtonAccept()) {
				while(getButtonAccept());
				break;
			}
		}
		for(int i = 0; i < shipLengths[ship]; ++ i)
			if(ourBoard[i*!rotation + cursorX + COLUMNS*(i*rotation + cursorY)] & TILE_SHIP)
				goto nvm;
		for(int i = 0; i < shipLengths[ship]; ++ i)
			ourBoard[i*!rotation + cursorX + COLUMNS*(i*rotation + cursorY)] |= TILE_SHIP,
			ourBoard[i*!rotation + cursorX + COLUMNS*(i*rotation + cursorY)] &= ~TILE_IS_PLACING;
	}

	int cursorX = 0, cursorY = 0, goesFirst = 1;
	struct packet p;
	if(!goesFirst) goto waitotherplayer;
	backtogame:
	printBoard(theirBoard, 0);
	theirBoard[cursorX + cursorY*COLUMNS] |= TILE_IS_AIMING;

	while(1) {
		while(!getButtons());
		if(getButtonUp()){
			if(!cursorY) continue;
			theirBoard[cursorX + cursorY*COLUMNS] &= ~TILE_IS_AIMING;
			-- cursorY;
			theirBoard[cursorX + cursorY*COLUMNS] |= TILE_IS_AIMING;
			printBoard(theirBoard, 0);
			while(getButtonUp());
		}

		if(getButtonDown()){
			if(cursorY == 9) continue;
			theirBoard[cursorX + cursorY*COLUMNS] &= ~TILE_IS_AIMING;
			++ cursorY;
			theirBoard[cursorX + cursorY*COLUMNS] |= TILE_IS_AIMING;
			printBoard(theirBoard, 0);
			while(getButtonDown());
		}

		if(getButtonLeft()){
			if(!cursorX) continue;
			theirBoard[cursorX + cursorY*COLUMNS] &= ~TILE_IS_AIMING;
			-- cursorX;
			theirBoard[cursorX + cursorY*COLUMNS] |= TILE_IS_AIMING;
			printBoard(theirBoard, 0);
			while(getButtonLeft());
		}

		if(getButtonRight()){
			if(cursorX == 9) continue;
			theirBoard[cursorX + cursorY*COLUMNS] &= ~TILE_IS_AIMING;
			-- cursorX;
			theirBoard[cursorX + cursorY*COLUMNS] |= TILE_IS_AIMING;
			printBoard(theirBoard, 0);
			while(getButtonRight());
		}

		if(getButtonAccept()){
			if(!theirBoard[cursorX + cursorY*COLUMNS] & (TILE_HIT|TILE_MISS)) continue;
			theirBoard[cursorX + cursorY*COLUMNS] &= ~TILE_IS_AIMING;
			while(getButtonAccept());
			break;
		}
	}

	p.x = cursorX;
	p.y = cursorY;
	sendShot(&p);
	if(p.didHit)
		theirBoard[cursorX + cursorY*COLUMNS] |= TILE_SHIP|TILE_HIT;
	else
		theirBoard[cursorX + cursorY*COLUMNS] |= TILE_MISS;
	printBoard(theirBoard, 0);

	waitotherplayer:
	p = listenShot(ourBoard);
	if(p.didHit)
		ourBoard[p.x + p.y*COLUMNS] |= TILE_HIT; 
	else
		ourBoard[p.x + p.y*COLUMNS] |= TILE_MISS;
}
