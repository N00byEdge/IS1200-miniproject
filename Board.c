#include "Board.h"

void init(enum tileType *b) {
	for(int col = 0; col < COLUMNS; ++ col)
		for(int row = 0; row < ROWS; ++ row)
			b[col + row * COLUMNS] = 0;
}
