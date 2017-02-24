#pragma once

#include "Parameters.h"
#include "Board.h"

void renderTile(const unsigned char *renderThis, unsigned char *ontoThis, int startX, int startY, int endX, int endY, int atX, int atY);
void renderSimple(const unsigned char *renderThis, unsigned char *ontoThis, int startX, int startY, int endX, int endY, int atX, int atY);
