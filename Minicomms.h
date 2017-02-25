#pragma once

struct packet {
	int x, y;
	bool didHit;
	bool didWin;
}

bool sendShot(struct packet *);

struct packet listenShot();
