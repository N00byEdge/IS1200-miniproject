#include "Parameters.h"

struct board {
	int shipAt[COLUMNS][ROWS], hasBombed[COLUMNS][ROWS];
	int cursorPosX, cursorPosY;
};

void init(struct board*);
