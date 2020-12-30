#include "slideWindow.h"
#include <iostream>

slideWindow::slideWindow(size_t &size):size(size),vector<Packet>(size) {}

Packet& slideWindow::operator[](int seq) {
	return vector<Packet>::operator[](seq%size);
}

void slideWindow::printPackets(const int base) {
	for (int i = base; i < base + size; i++)
		pUtils->printPacket("Packet in slide window", this->operator[](i));

	return;
}

slideWindow::~slideWindow() {}

//slideWindow::~slideWindow() {}