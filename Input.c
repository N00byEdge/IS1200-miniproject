#include "Input.h"

// TODO: Set up bus for buttons
// TODO: find pins for buttons

void buttonsInit(){

}

unsigned getButtons() {
	return 0x7 &(PORTB >> 5);
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
