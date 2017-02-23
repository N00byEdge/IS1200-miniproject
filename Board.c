#include "Board.h"

void init(enum tileType *b) {
	for(int col = 0; col < COLUMNS; ++ col)
		for(int row = 0; row < ROWS; ++ row)
			b[col + row * COLUMNS] = 0;
}

void printBoard(enum tileType *b) {
	setArea(XRES - TILE_WIDTH /2 * COLUMNS,
			XRES + TILE_WIDTH /2 * COLUMNS,
			YRES - TILE_HEIGHT/2 * ROWS,
			YRES + TILE_HEIGHT/2 * ROWS);
	for(int y = 0; y < ROWS; ++ y) {
		for(int x = 0; x < COLUMNS; ++ x) {

		}
	}
}
