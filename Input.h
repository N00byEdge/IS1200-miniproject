#pragma once

#include <pic32mx.h>

unsigned getButtons();
unsigned getButtonAccept();
unsigned getButtonRotate();
unsigned getButtonUp();
unsigned getButtonDown();
unsigned getButtonLeft();
unsigned getButtonRight();
void enable_interrupt();
void buttonsINIT();
