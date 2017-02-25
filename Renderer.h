#pragma once

#include "Parameters.h"
#include "Board.h"

void renderTile(const unsigned char *renderThis, unsigned char *ontoThis, int startX, int startY, int endX, int endY, int atX, int atY);
void renderSimple(const unsigned char *renderThis, unsigned char *ontoThis, int startX, int startY, int endX, int endY, int atX, int atY);
unsigned fillColor(unsigned char *data, const unsigned char *color, int xSize, int ySize);
void frame(unsigned char* data);
