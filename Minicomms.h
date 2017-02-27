#pragma once

#include "Board.h"

struct packet {
	int x, y;
	int didHit;
	int didWin;
};

void sendShot(struct packet *p);

struct packet listenShot(enum tileType *board);
