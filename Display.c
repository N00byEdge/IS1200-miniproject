#include "Display.h"

#define WINDOW_XADDR_START	0x0050
#define WINDOW_XADDR_END	0x0051
#define WINDOW_YADDR_START	0x0052
#define WINDOW_YADDR_END	0x0053
#define GRAM_XADDR		    0x0020
#define GRAM_YADDR		    0x0021
#define GRAMWR 			    0x0022

#define setrs() PORTFSET = 1 << 6
#define clrrs() PORTFCLR = 1 << 6

#define setwr() PORTFSET = 1 << 4
#define clrwr() PORTFCLR = 1 << 4

#define setrd() PORTFSET = 1 << 2
#define clrrd() PORTFCLR = 1 << 2

#define setcs() PORTFSET = 1 << 5
#define clrcs() PORTFCLR = 1 << 5

#define setrst() PORTBSET = 1 << 1
#define clrrst() PORTBCLR = 1 << 1

#define setbus(d)	PORTECLR = 0x0ff; \
				 	PORTESET = d

void writeCmd(unsigned char c, unsigned char d) {
	clrrs();
	clrcs();
	setbus(c);
	clrwr();
	setwr();
	setbus(d);
	clrwr();
	setwr();
	setcs();
}

void writeData(unsigned char c, unsigned char d) {
	setrs();
	clrcs();
	setbus(c);
	clrwr();
	setwr();
	setbus(d);
	clrwr();
	setwr();
	setcs();
}

void writeData16(unsigned data) {
	writeData(data >> 8, data);
}

void setData(unsigned char addr, unsigned data) {
	writeCmd(0x00, addr);
	writeData16(data);
}

void setArea(unsigned x0, unsigned x1, unsigned y0, unsigned y1) {
	setData(WINDOW_XADDR_START, x0);
	setData(WINDOW_XADDR_END,   x1);
	setData(WINDOW_YADDR_START, y0);
	setData(WINDOW_YADDR_END,   y1);
	setData(GRAM_XADDR, x0);
	setData(GRAM_YADDR, y0);
	writeCmd(0x00, 0x22);
}

void displayinit() {
	TRISBCLR = (1 << 1);
	TRISFCLR = (1 << 2) | (1 << 4) | (1 << 5) | (1 << 6);
	TRISECLR = 0x0ff;

	setrd();
	setrst();
	fastsleep(5);
	clrrst();
	fastsleep(10);
	setrst();
	setcs();
	setwr();
	fastsleep(70);

	setData(0x01, 0x0100);
	setData(0x02, 0x0700);
	setData(0x03, 0x1030);
	setData(0x04, 0x0000);
	setData(0x08, 0x0207);
	setData(0x09, 0x0000);
	setData(0x0A, 0x0000);
	setData(0x0C, 0x0000);
	setData(0x0D, 0x0000);
	setData(0x0F, 0x0000);
	setData(0x10, 0x0000);
	setData(0x11, 0x0007);
	setData(0x12, 0x0000);
	setData(0x13, 0x0000);
	setData(0x10, 0x1290);
	setData(0x11, 0x0227);
	setData(0x12, 0x001d);
	setData(0x13, 0x1500);
	setData(0x29, 0x0018);
	setData(0x2B, 0x000D);
	setData(0x30, 0x0004);
	setData(0x31, 0x0307);
	setData(0x32, 0x0002);
	setData(0x35, 0x0206);
	setData(0x36, 0x0408);
	setData(0x37, 0x0507);
	setData(0x38, 0x0204);
	setData(0x39, 0x0707);
	setData(0x3C, 0x0405);
	setData(0x3D, 0x0F02);
	setData(0x50, 0x0000);
	setData(0x51, 0x00EF);
	setData(0x52, 0x0000);
	setData(0x53, 0x013F);
	setData(0x60, 0xA700);
	setData(0x61, 0x0001);
	setData(0x6A, 0x0000);
	setData(0x80, 0x0000);
	setData(0x81, 0x0000);
	setData(0x82, 0x0000);
	setData(0x83, 0x0000);
	setData(0x84, 0x0000);
	setData(0x85, 0x0000);
	setData(0x90, 0x0010);
	setData(0x92, 0x0600);
	setData(0x93, 0x0003);
	setData(0x95, 0x0110);
	setData(0x97, 0x0000);
	setData(0x98, 0x0000);
	setData(0x07, 0x0133);
}

void paint(unsigned c) {
	setArea(0, 240, 0, 320);
	for(int y = 0; y < 320; ++ y)
		for(int x = 0; x < 240; ++ x)
			writeData16(c);
}

void paintimg(const char *data, unsigned xSize, unsigned ySize, unsigned atX, unsigned atY) {
    for(int x = 0; x < xSize; ++ x) {
        for(int y = 0; y < ySize; ++ y) {
        	if(data[3 + x*4 + y*4*xSize]) {
        		setArea(atX + x, atX + x + 1, atY + y, atY + y + 1);
            	writeData16(colorsTo16Bit(data + x*4 + y*4*xSize));
        	}
        }
    }
}

void frame(unsigned char* data) {
	for(int x = 0; x < 16; ++ x)
		for(int y = 0; y < 16; ++ y)
			for(int c = 0; c < 3; ++ c)
				if(x == 0 || y == 0 || x == 15 || y == 15)
					data[c + x*4 + y*4*16] = c == 3 ? 0xff : 0x00;
}

unsigned colorsTo16Bit(const unsigned char *d) {
	//return 0x0000;
	return ((d[0]>>3) << 11) | ((d[1]>>2) << 5) | (d[2]>>3);
}

unsigned fillColor(unsigned char *data, const unsigned char *color, int xSize, int ySize) {
	for(int x = 0; x < xSize; ++ x)
		for(int y = 0; y < ySize; ++ y)
			for(int c = 0; c < 4; ++ c)
				data[c + x*4 + y*4* xSize] = color[c];
}
