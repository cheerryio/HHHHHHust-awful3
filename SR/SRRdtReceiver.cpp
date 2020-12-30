#include "stdafx.h"
#include "Global.h"
#include "SRRdtReceiver.h"


SRRdtReceiver::SRRdtReceiver():base(1), messageWindow(Configuration::SRWINDOW_SIZE),ackWindow(Configuration::SRWINDOW_SIZE,false){
	ackPacket.acknum = -1; //��ʼ״̬�£��ϴη��͵�ȷ�ϰ���ȷ�����Ϊ-1��ʹ�õ���һ�����ܵ����ݰ�����ʱ��ȷ�ϱ��ĵ�ȷ�Ϻ�Ϊ-1
	ackPacket.checksum = 0;
	ackPacket.seqnum = -1;	//���Ը��ֶ�
	for(int i = 0; i < Configuration::PAYLOAD_SIZE;i++){
		ackPacket.payload[i] = '.';
	}
	ackPacket.checksum = pUtils->calculateCheckSum(ackPacket);
}


SRRdtReceiver::~SRRdtReceiver() {}

void SRRdtReceiver::receive(const Packet &packet) {
	//���У����Ƿ���ȷ
	int checkSum = pUtils->calculateCheckSum(packet);

	//���У�����ȷ��ͬʱ�յ����ĵ���ŵ��ڽ��շ��ڴ��յ��ı������һ��
	if (checkSum == packet.checksum) {
		pUtils->printPacket("���շ���ȷ�յ����ͷ��ı���", packet);

			//ȡ��Message�����ϵݽ���Ӧ�ò�
			Message msg;
			memcpy(msg.data, packet.payload, sizeof(packet.payload));

			ackPacket.acknum = packet.seqnum; //ȷ����ŵ����յ��ı������
			ackPacket.checksum = pUtils->calculateCheckSum(ackPacket);
			pUtils->printPacket("���շ�����ȷ�ϱ���", ackPacket);
			pns->sendToNetworkLayer(SENDER, ackPacket);

		if (packet.seqnum >= base) {
			messageWindow[packet.seqnum] = msg;
			ackWindow[packet.seqnum] = true;
		}

		while (ackWindow[base] == true) {
			pns->delivertoAppLayer(RECEIVER, messageWindow[base]);
			ackWindow[base] = false;
			base++;
		}
	}
	else {
		if (checkSum != packet.checksum) {
			pUtils->printPacket("���շ�û����ȷ�յ����ͷ��ı���,����У�����", packet);
		}
		else {
			pUtils->printPacket("���շ�û����ȷ�յ����ͷ��ı���,������Ų���", packet);
		}
		pUtils->printPacket("���շ������·����ϴε�ȷ�ϱ���", ackPacket);
	}
}