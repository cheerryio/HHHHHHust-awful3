#include "stdafx.h"
#include "Global.h"
#include "GBNRdtSender.h"
#include <stdio.h>

#include <iostream>

GBNRdtSender::GBNRdtSender() :nextSeqNum(1),
							  window((size_t &)Configuration::SLIDEWINDOW_SIZE),
							  base(1),
							  N(Configuration::SLIDEWINDOW_SIZE)

{
}

GBNRdtSender::~GBNRdtSender() {}

bool GBNRdtSender::getWaitingState() {
	if (nextSeqNum < base + N)
		return false;
	return true;
}

bool GBNRdtSender::send(const Message &message) {
	if (this->getWaitingState())	///< 处于slideWindow满状态，无法发送报文
		return false;

	Packet pkt(nextSeqNum, -1, 0);
	memcpy(pkt.payload, message.data, sizeof(message.data));
	pkt.checksum = pUtils->calculateCheckSum(pkt);
	window[nextSeqNum] = pkt;
	pUtils->printPacket("发送方发送报文", pkt);
	if (base == nextSeqNum)
		pns->startTimer(SENDER, Configuration::TIME_OUT, base);			//启动发送方定时器
	pns->sendToNetworkLayer(RECEIVER, pkt);
	nextSeqNum++;
	return true;
}

void GBNRdtSender::receive(const Packet &ackPkt) {
	int checkSum = pUtils->calculateCheckSum(ackPkt);
	int acknum = ackPkt.acknum;
	if (checkSum == ackPkt.checksum) {
		base = acknum + 1;
		window.printPackets(base);

		nextSeqNum = nextSeqNum < base ? base : nextSeqNum;

		pns->stopTimer(SENDER, acknum);
		if (base == nextSeqNum)
		{
			pns->stopTimer(SENDER, acknum);
		}
		else {
			pns->startTimer(SENDER, Configuration::TIME_OUT, base);
		}

	}
	else {
		
	}

	return;
}

void GBNRdtSender::timeoutHandler(int seqNum) {
	if(base!=nextSeqNum)
		pns->startTimer(SENDER, Configuration::TIME_OUT, base);
	for (int i = base; i < nextSeqNum; i++) {
		pns->sendToNetworkLayer(RECEIVER, window[i]);
	}
	return;
}
