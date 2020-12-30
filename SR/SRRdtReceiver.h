#ifndef STOP_WAIT_RDT_RECEIVER_H
#define STOP_WAIT_RDT_RECEIVER_H
#include "RdtReceiver.h"
#include "slideWindow.h"
class SRRdtReceiver :public RdtReceiver
{
private:
	int base;
	Packet ackPacket;
	slideWindow<Message> messageWindow;
	slideWindow<uint8_t> ackWindow;

public:
	SRRdtReceiver();
	virtual ~SRRdtReceiver();

public:
	
	void receive(const Packet &packet);	//接收报文，将被NetworkService调用
};

#endif

