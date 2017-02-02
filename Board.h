#include "Parameters.h"

struct board {
	int shipAt[COLUMNS][ROWS], hasBombed[COLUMNS][ROWS];
	
};

void init(struct board*);
