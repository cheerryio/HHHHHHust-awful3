#include "stdafx.h"
#include "Global.h"
#include "naiveTCPSender.h"
#include <stdio.h>

#include <iostream>

GBNRdtSender::GBNRdtSender() :
nextSeqNum(1),
window((size_t&)Configuration::SLIDEWINDOW_SIZE),
countWindow((size_t&)Configuration::SLIDEWINDOW_SIZE),
base(1),
expectAcknum(1),
N(Configuration::SLIDEWINDOW_SIZE) {}

GBNRdtSender::~GBNRdtSender() {}

bool GBNRdtSender::getWaitingState() {
	if (nextSeqNum < base + N)
		return false;
	return true;
}

/**
* available to send packet number of window size
* set a timer for each packet
*/
bool GBNRdtSender::send(const Message &message) {
	if (this->getWaitingState())	///< 处于slideWindow满状态，无法发送报文
		return false;

	Packet pkt(nextSeqNum, -1, 0);
	memcpy(pkt.payload, message.data, sizeof(message.data));
	pkt.checksum = pUtils->calculateCheckSum(pkt);
	window[nextSeqNum] = pkt;

	pUtils->printPacket("发送方发送报文", pkt);
	pns->startTimer(SENDER, Configuration::TIME_OUT, nextSeqNum);	// set timer for nextSeqNum packet
	pns->sendToNetworkLayer(RECEIVER, pkt);
	nextSeqNum++;
	return true;
}

/**
* fast resend complementation
* 
*/
void GBNRdtSender::receive(const Packet &ackPkt) {
	int checkSum = pUtils->calculateCheckSum(ackPkt);
	expectAcknum = ackPkt.acknum;
	if (checkSum == ackPkt.checksum) {
		if (expectAcknum >= base) {
			// stop timer and clear cumulative acks of previous send and acked packets
			if (expectAcknum > base) {
				for (int i = base; i < expectAcknum; i++) {
					countWindow[i] = 0;
					pns->stopTimer(SENDER, i);
				}
				countWindow[expectAcknum] = 0;
			}
			else {
				// when base == expectAcknum
				if (window[base].seqnum != expectAcknum) {
					// do nothing
					// dont send this packet again as this has been send
					// and no new packet comes from application layer
				}
				else {
					countWindow[expectAcknum] = countWindow[expectAcknum] + 1;
					if (countWindow[base] == 4) {
						cout << "----------------fast resend-----------------" << endl;
						cout << "!!!!!!!!!!!!!!!!!!" << endl;
						countWindow[base] = 0;
						// as this is fast resend, stop the previous untimeout timer
						// and set new timer
						pns->stopTimer(SENDER, expectAcknum);
						pns->startTimer(SENDER, Configuration::TIME_OUT, expectAcknum);
						pns->sendToNetworkLayer(RECEIVER, window[expectAcknum]);
					}
				}
			}
			base = expectAcknum;
			window.printPackets(base);
		}
		else {
			// do nothing return ack(expectAcknum) < base, ignore as naiveTCP's cumulative confirmation
		}
	}
	else {
		// do nothin when ack is broken
	}

	return;
}

void GBNRdtSender::timeoutHandler(int seqNum) {
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);
	pns->sendToNetworkLayer(RECEIVER, window[seqNum]);
	return;
}
