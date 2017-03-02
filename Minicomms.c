#include "Minicomms.h"

#include <stdlib.h>

void sendShot(struct packet *p) {
	static int t = 0;

	// Send data

	// Recieve didHit and didWin

	p->didHit = p->x%2;
	t += p->didHit;
	p->didWin = t == 17;

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
	if(board[p.x + p.y*COLUMNS] & TILE_SHIP)
		p.didHit = 1;
	else
		p.didHit = 0;
	board[p.x + p.y*COLUMNS] |= p.didHit ? TILE_HIT : TILE_MISS;

	p.didWin = 1;
	for(int i = 0; i < ROWS*COLUMNS; ++ i)
		if((board[i] & TILE_SHIP) && ((board[i]) & TILE_HIT) == 0)
			p.didWin = 0;

	// Send back didWin and didHit data

	return p;
}
