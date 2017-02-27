#pragma once

#include "Parameters.h"
#include "Display.h"
#include "Renderer.h"

enum tileType {
	TILE_EMPTY = 1,
	TILE_HIT = 1 << 1,
	TILE_MISS = 1 << 2,
	TILE_SHIP = 1 << 3,
	TILE_IS_PLACING = 1 << 4,
	TILE_IS_AIMING = 1 << 5
};

void init(enum tileType *b);

void printBoard(enum tileType *b, int yOffset, int isActive);
