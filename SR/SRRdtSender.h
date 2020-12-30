#ifndef STOP_WAIT_RDT_SENDER_H
#define STOP_WAIT_RDT_SENDER_H
#include "RdtSender.h"
#include "slideWindow.h"

class SRRdtSender :public RdtSender
{
public:
	int base;
	int N;
	int nextSeqNum;
	slideWindow<Packet> packetWindow;
	slideWindow<uint8_t> ackWindow;

public:
	/**
	* return true when nextSeqNum<base+N, which means it still accept Packet
	* else return false
	*/
	bool getWaitingState();

	/**
	* called by application layer
	* send one Message from application layer
	*
	* @param message a Message from application layer
	*
	* @returns if send success, return true. return false
	*/
	bool send(const Message &message);

	/**
	* in SR protocal, set the received ackPkt.acknum packet to accepted
	* check if base and following packets are accepted
	* 
	* @param ackPkt ack packet from receiver
	*/
	void receive(const Packet &ackPkt);

	/**
	* packet of seqNum timeout
	* resend this packet
	* 
	* @param seqNum the sequence number of the timeout packet
	*/
	void timeoutHandler(int seqNum);

public:
	SRRdtSender();
	virtual ~SRRdtSender();
};

#endif

