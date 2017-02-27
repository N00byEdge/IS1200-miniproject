#include "Renderer.h"

void renderTile(const unsigned char *renderThis, unsigned char *ontoThis, int startX, int startY, int endX, int endY, int atX, int atY) {
	for(int x = startX; x < endX; ++ x) {
		for(int y = startY; y < endY; ++ y) {
			if(renderThis[3 + x*4 + y*4*16]) {
				for(int c = 0; c <3; ++ c) // Alla hjärtans dag
					ontoThis[c + x*4 + y*4*16 + atX*4 + atY*4*16] = (float) (255-renderThis[3 + x*4 + y*4*16])/255 * ontoThis[c + x*4 + y*4*16 + atX*4 + atY*4*16]
											 + (float) (renderThis[3 + x*4 + y*4*16])/255 * renderThis[c + x*4 + y*4*16];
				ontoThis[3 + x*4 + y*4*16 + atX*4 + atY*4*16] += (float) (255 - ontoThis[3 + x*4 + y*4*16 + atX*4 + atY*4*16])/255 * renderThis[3 + x*4 + y*4*16];
			}
		}
	}
}

void renderSimple(const unsigned char *renderThis, unsigned char *ontoThis, int startX, int startY, int endX, int endY, int atX, int atY) {
	for(int x = startX; x < endX; ++ x) {
		for(int y = startY; y < endY; ++ y) {
			if(renderThis[3 + x*4 + y*4*16]) {
				for(int c = 0; c < 3; ++ c)
					ontoThis[c + x*4 + y*4*16] = renderThis[c + x*4 + y*4*16];
			}
		}
	}
}

void frame(unsigned char* data, int xSize, int ySize) {
	for(int x = 0; x < xSize; ++ x)
		for(int y = 0; y < ySize; ++ y)
			for(int c = 0; c < 3; ++ c)
				if(x == 0 || y == 0 || x == 15 || y == 15)
					data[c + x*4 + y*4*16] = c == 3 ? 0xff : 0x00;
}

unsigned fillColor(unsigned char *data, const unsigned char *color, int xSize, int ySize) {
	for(int x = 0; x < xSize; ++ x)
		for(int y = 0; y < ySize; ++ y)
			for(int c = 0; c < 4; ++ c)
				data[c + x*4 + y*4* xSize] = color[c];
}
