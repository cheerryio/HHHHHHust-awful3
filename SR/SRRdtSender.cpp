#include "stdafx.h"
#include "Global.h"
#include "SRRdtSender.h"


SRRdtSender::SRRdtSender() :
base(1),
nextSeqNum(1),
N(Configuration::SRWINDOW_SIZE), packetWindow(Configuration::SRWINDOW_SIZE),
ackWindow(Configuration::SRWINDOW_SIZE)
{}


SRRdtSender::~SRRdtSender()
{
}



bool SRRdtSender::getWaitingState() {
	if (nextSeqNum < base + N)
		return false;
	return true;
}




bool SRRdtSender::send(const Message &message) {
	if (this->getWaitingState())	/// ���ͷ����ڵȴ�ȷ��״̬
		return false;
	Packet pkt(nextSeqNum, -1, 0);
	memcpy(pkt.payload, message.data, sizeof(message.data));
	pkt.checksum = pUtils->calculateCheckSum(pkt);
	packetWindow[nextSeqNum]= pkt;
	ackWindow[nextSeqNum] = false;
	pUtils->printPacket("���ͷ����ͱ���", pkt);

	pns->startTimer(SENDER, Configuration::TIME_OUT, nextSeqNum);			//�������ͷ���ʱ��
	pns->sendToNetworkLayer(RECEIVER, pkt);
	nextSeqNum++;

	return true;
}

void SRRdtSender::receive(const Packet &ackPkt) {
	int checkSum = pUtils->calculateCheckSum(ackPkt);	///< ���У���
	/// ���У�����ȷ������ȷ�����=���ͷ��ѷ��Ͳ��ȴ�ȷ�ϵ����ݰ����
	if (checkSum == ackPkt.checksum && (ackPkt.acknum >= base && ackPkt.acknum < nextSeqNum)) {
		ackWindow[ackPkt.acknum] = true;
		pns->stopTimer(SENDER, ackPkt.acknum);
	}
	else {
		// do nothing
	}
	/// �����Ƿ��д�base��ʼ������acked packet
	while (base != nextSeqNum && ackWindow[base] == true)
		base++;

	packetWindow.printPackets(base);	// print packets in packetSlideWindow

}

void SRRdtSender::timeoutHandler(int seqNum) {
	pUtils->printPacket("���ͷ���ʱ��ʱ�䵽���ط��ϴη��͵ı���", packetWindow[seqNum]);
	pns->stopTimer(SENDER,seqNum);										//���ȹرն�ʱ��
	pns->startTimer(SENDER, Configuration::TIME_OUT,seqNum);			//�����������ͷ���ʱ��
	pns->sendToNetworkLayer(RECEIVER, packetWindow[seqNum]);			//���·������ݰ�
	
	return;
}
