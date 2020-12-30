#ifndef STOP_WAIT_RDT_SENDER_H
#define STOP_WAIT_RDT_SENDER_H
#include "RdtSender.h"
#include "slideWindow.h"

class GBNRdtSender :public RdtSender
{
private:
	int base;						/// ����base
	int N;							/// ���ڴ�С
	int nextSeqNum;					/// ��һ���������Ͱ����
	slideWindow window;		/// �������ڣ����������ش�ʱ�����˰������ݣ�

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
	void receive(const Packet &ackPkt);	//����ȷ��Ack������NetworkServiceSimulator����	
	void timeoutHandler(int seqNum);	//Timeout handler������NetworkServiceSimulator����

public:
	GBNRdtSender();
	virtual ~GBNRdtSender();
};

#endif

