#include "Input.h"

// TODO: Set up bus for buttons
// TODO: find pins for buttons

void buttonsInit(){
	TRISDSET = 0x7 | (0x3 << 9); //0, 1, 2, 9, 10
	TRISFSET = (1 << 1); // 1
}

unsigned getButtons() {
	return ((PORTD >> 0) & 0x1) |
		   ((PORTF >> 1) & 0x1) << 1 |
		   ((PORTD >> 1) & 0x1) << 2 |
		   ((PORTD >> 2) & 0x1) << 3 |
		   ((PORTD >> 9) & 0x1) << 4 |
		   ((PORTD >> 10) & 0x1) << 5;
}

unsigned getButtonAccept() {
	return getButtons() & 0x1;
}

unsigned getButtonRotate() {
	return (getButtons() >> 1) & 0x1;
}

unsigned getButtonUp() {
	return (getButtons() >> 2) & 0x1;
}

unsigned getButtonDown() {
	return (getButtons() >> 3) & 0x1;
}

unsigned getButtonLeft() {
	return (getButtons() >> 4) & 0x1;
}

unsigned getButtonRight() {
	return (getButtons() >> 5) & 0x1;
}
