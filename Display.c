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
				 	PORTESET = d & 0x0ff

#define writeCmd(c, d) \
	clrrs(); \
	clrcs(); \
	setbus(c); \
	clrwr(); \
	setwr(); \
	setbus(d); \
	clrwr(); \
	setwr(); \
	setcs();

#define writeData(c, d) \
	setrs(); \
	clrcs(); \
	setbus(c); \
	clrwr(); \
	setwr(); \
	setbus(d); \
	clrwr(); \
	setwr(); \
	setcs();

#define writeData16(d) \
	writeData(d >> 8, d);

#define setData(a, d) \
	writeCmd(0x00, a); \
	writeData16(d);

#define setArea(x0, x1, y0, y1) \
	setData(WINDOW_XADDR_START, x0); \
	setData(WINDOW_XADDR_END,   x1); \
	setData(WINDOW_YADDR_START, y0); \
	setData(WINDOW_YADDR_END,   y1); \
	setData(GRAM_XADDR, x0); \
	setData(GRAM_YADDR, y0); \
	writeCmd(0x00, 0x22);

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
	for(int y = 0; y < 320; ++ y) {
		for(int x = 0; x < 240; ++ x) {
			setArea(x, x + 1, y, y + 1);
			writeData16(c);
		}
	}
}

void paintArea(unsigned c, unsigned x0, unsigned x1, unsigned y0, unsigned y1) {
	setArea(x0, x1, y0, y1);
	for(int x = x0; x < x1; ++ x)
		for(int y = y0; y < y1; ++ y)
			writeData16(c);
}

#define col(arr, pos) (((unsigned int)(arr[(pos)]>>3) << 11) | ((unsigned int)(arr[(pos) + 1] >> 2) << 5) | (unsigned int)(arr[(pos) + 2] >> 3))

void paintimg(const unsigned char *data, unsigned xSize, unsigned ySize, unsigned atX, unsigned atY) {
	setArea(atX, atX + xSize, atY, atY + ySize);
    for(int y = 0; y < ySize; ++ y) {
        for(int x = 0; x < xSize; ++ x) {
			writeData16(col(data, (x-y)*4 + y*4*xSize));
        }
    }
}

void paintimgWithScale(const unsigned char *data, unsigned xSize, unsigned ySize, unsigned atX, unsigned atY, unsigned scale) {
	for(int y = 0; y < ySize; ++ y) {
		for(int i = 0; i < scale; ++ i) {
			for(int x = 0; x < xSize; ++ x) {
				for(int j = 0; j < scale; ++ j) {
					setArea(atX + x*scale + j, atX + x*scale + 1 + j, atY + y*scale + i, atY + y*scale + i + 1);
					writeData16(col(data, x*4 + y*4*xSize));
				}
			}
		}
	}
}
