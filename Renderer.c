#include "Renderer.h"

void renderTile(const unsigned *renderThis, unsigned *ontoThis, int startX, int startY, int endX, int endY, int atX, int atY) {
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
