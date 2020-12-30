#ifndef STOP_WAIT_RDT_SENDER_H
#define STOP_WAIT_RDT_SENDER_H
#include "RdtSender.h"
#include "slideWindow.h"

class GBNRdtSender :public RdtSender
{
private:
	int base;						/// 窗口base
	int N;							/// 窗口大小
	int nextSeqNum;					/// 下一个即将发送包序号
	slideWindow window;		/// 滑动窗口，作用用来重传时缓存了包（数据）

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
	void receive(const Packet &ackPkt);	//接受确认Ack，将被NetworkServiceSimulator调用	
	void timeoutHandler(int seqNum);	//Timeout handler，将被NetworkServiceSimulator调用

public:
	GBNRdtSender();
	virtual ~GBNRdtSender();
};

#endif

