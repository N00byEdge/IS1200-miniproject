#include "Board.h"

void init(struct board* b) {
	for(int col = 0; col < COLUMNS; ++ col)
		for(int row = 0; row < ROWS; ++ row)
			b->shipAt[col][row] = 0, b->hasBombed[col][row] = 0;
}
