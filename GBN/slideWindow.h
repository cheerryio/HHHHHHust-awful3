#pragma once

#include <vector>
#include "DataStructure.h"
#include "Global.h"

/// slideWindow for GBNRdtSender
class slideWindow:std::vector<Packet>
{
private:
	/// slideWindow size
	const size_t size;

public:
	/**
	* init function for slideWindow class
	* 
	* @param size set the size of slideWindow
	*/
	slideWindow(size_t &size);

	/**
	* overload vector operator []
	* 
	* @param seq get the seq'th element in vector
	* 
	* @returns Package element
	*/
	Packet& operator[](int seq);

	/**
	* print all packets in current window
	* start with index base
	*
	* @param base start print index
	*
	*/
	void printPackets(const int base);

	~slideWindow();
};

