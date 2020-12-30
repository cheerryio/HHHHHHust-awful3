#pragma once

#include <type_traits>
#include <vector>
#include "DataStructure.h"
#include "Global.h"

using namespace std;

template <typename T>
class slideWindow:std::vector<T>
{
private:
	const size_t size;

public:
	/**
	* init vector size
	*
	* @param size fixed size of SRSlideWindow
	*/
	slideWindow(size_t size);

	slideWindow(size_t size, T initElem);

	/**
	* get relative element
	* 
	* @param seq the seq's number
	* 
	* @returns element
	*/
	T& operator[](int seq);

	/**
	* print all packets in current window
	* start with index base
	* 
	* @param base start print index
	* 
	*/
	void printPackets(const int base);

};

template <typename T>
slideWindow<T>::slideWindow<T>(size_t size) : size(size), vector<T>(size) {}

template <typename T>
slideWindow<T>::slideWindow<T>(size_t size,T initElem) : size(size), vector<T>(size,initElem) {}

template <typename T>
T& slideWindow<T>::operator[](int seq) {
	return vector<T>::operator[]((seq - 1) % size);
}

template <typename T>
void slideWindow<T>::printPackets(const int base) {
	static_assert(std::is_base_of<Packet, T>::value, "T must inherit from list");
	for (int i = base; i < base + size; i++)
		pUtils->printPacket("Packet in slide window", this->operator[](i));

	return;
}