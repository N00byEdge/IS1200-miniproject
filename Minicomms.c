#include "Minicomms.h"

#include <stdlib.h>

void sendShot(struct packet *p) {
	static int t = 0;

	// Send data

	// Recieve didHit and didWin

	p->didHit = p->x%2;
	p->didWin = t++ > 30;

	return;
}

struct packet listenShot(enum tileType *board) {
	static unsigned int t = 38423;
	t ^= t*3;
	struct packet p;

	// Recieve data
	p.x = t%10;
	p.y = (t%100)/10;

	// Game logic
	p.didHit = board[p.x + p.y*COLUMNS] & TILE_SHIP;
	board[p.x + p.y*COLUMNS] |= p.didHit ? TILE_HIT : TILE_MISS;

	p.didWin = 1;
	for(int i = 0; i < ROWS*COLUMNS; ++ i)
		if((board[i] & TILE_SHIP) && ((board[i]) & TILE_HIT) == 0)
			p.didWin = 0;

	// Send back didWin and didHit data

	return p;
}
