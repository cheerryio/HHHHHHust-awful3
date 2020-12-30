#include "slideWindow.h"
#include <iostream>

slideWindow::slideWindow(size_t &size):size(size),vector<Packet>(size) {}

Packet& slideWindow::operator[](int seq) {
	std::cout << "seq:"<<seq << std::endl;
	std::cout << "seqnum:"<<(vector<Packet>::operator[](seq% size)).seqnum << std::endl;
	return vector<Packet>::operator[](seq%size);
}

slideWindow::~slideWindow() {}

//slideWindow::~slideWindow() {}