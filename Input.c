#include "Input.h"

// TODO: Set up bus for buttons
// TODO: find pins for buttons
// TODO: setup interrupts

<<<<<<< Updated upstream
void buttonsInit(){
	enable_interrupt();
=======
void buttonsINIT(){
>>>>>>> Stashed changes
	T2CON = 0;             // Reset timer
	T2CONSET = 7 << 4;     // 1:256 prescaling
	PR2 = 3125;          	 // 800,000/256 = 3,125 will be 1/100 second period
	TMR2 = 0;               // Reset counter
	T2CONSET = 0x08000;     // Start csounter
	IPCCLR(2) = 0x1C;       // Clear pri for interrupt
	IPCSET(2) = 0x08;       // Set pri for interrupt
	IPCCLR(2) = 0x3;        // Clear spri for interrupt
	IPCSET(2) = 0x2;        // Set spri for interrupt
	IECSET(0) = 1 << 8;     // Enable Interrupt
}

void user_isr () {


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
