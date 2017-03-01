#pragma once

#include "Board.h"

struct packet {
	int x, y;
	int didHit;
	int didWin;
};

void commsinit();

void sendShot(struct packet *p);

struct packet listenShot(enum tileType *board);

void recieveBit(int *b);

void sendBit(int b);
