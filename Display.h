#pragma once

#include "Lib.h"

#include <pic32mx.h>
#include <stdint.h>

void displayinit();
void paint(unsigned);
void setArea(unsigned x0, unsigned x1, unsigned y0, unsigned y1);
void paintArea(unsigned c, unsigned x0, unsigned x1, unsigned y0, unsigned y1);
void paintimg(const unsigned char *data, unsigned xSize, unsigned ySize, unsigned atX, unsigned atY);
void paintimgWithScale(const unsigned char *data, unsigned xSize, unsigned ySize, unsigned atX, unsigned atY, unsigned scale);
