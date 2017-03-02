#include "Minicomms.h"

#include <stdlib.h>

#define SETRDY PORTDSET=1<<5
#define CLRRDY PORTDCLR=1<<5
#define ACK ((PORTD>>11) & 1)
#define WRITEDATA(k) \
	TRISDCLR = 1<<6; \
	PORTDCLR = 1<<6; \
	PORTDSET = (k&1) << 6;
#define READDATA(k) \
	TRISDSET = 1 << 6; \
	(*k) &= ~1; \
	(*k) |= (PORTD >> 6) & 1;



void commsinit(){
	TRISDSET = (1 << 11);
	TRISDCLR = (1 << 5);
	PORTDCLR = (1 << 5);
	CLRRDY;
	return;
}

void sendBit(int b) {
	WRITEDATA(b);
	SETRDY;
	while(!ACK);
	CLRRDY;
	while(ACK);
}

void recieveBit(int *b) {
	while(!ACK);
	SETRDY;
	READDATA(b);
	while(ACK);
	CLRRDY;
}

void sendShot(struct packet *p) {
	static int hitcounter = 0;
	static int c = 0;

	for(int i = 4; i --> 0;)
		sendBit((p->x) >> i);

	for(int i = 4; i --> 0;)
		sendBit((p->y) >> i);

	int didHit = 0;
	recieveBit(&didHit);

	p->didHit = didHit;
	hitcounter += didHit;

	if (hitcounter == 17)
		p->didWin = 1;
	else
		p->didWin = 0;
	return;
}

struct packet listenShot(enum tileType *board) {
	struct packet p;

	p.x = 0, p.y = 0;
	for(int i = 0; i < 4; ++ i) {
		p.x <<= 1;
		recieveBit(&(p.x));
	}

	for(int i = 0; i < 4; ++ i) {
		p.y <<= 1;
		recieveBit(&(p.y));
	}

	if(board[p.x + p.y*COLUMNS] & TILE_SHIP)
		p.didHit = 1;
	else
		p.didHit = 0;
	board[p.x + p.y*COLUMNS] |= (p.didHit ? TILE_HIT : TILE_MISS);

	p.didWin = 1;
	for(int i = 0; i < ROWS*COLUMNS; ++ i)
		if((board[i] & TILE_SHIP) && ((board[i]) & TILE_HIT) == 0)
			p.didWin = 0;

	sendBit(p.didHit);
	return p;
}

// data -> 1 bit of data I/O, linked to data on other device							(brown cable)	- rd 6
// ack  -> ready for data transfer? linked to rdy on other device					(Red cable)		-	rd 11
// rdy  -> are we ready for data transfer? linked to ack on other device	(Orange cable)- rd 5
