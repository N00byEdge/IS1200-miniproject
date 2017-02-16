#pragma once

#include "Parameters.h"

enum tileType {
	TILE_EMPTY = 1,
	TILE_HIT = 1 << 1,
	TILE_MISS = 1 << 2,
	TILE_SHIP = 1 << 3,
};

void init(enum tileType *b);
