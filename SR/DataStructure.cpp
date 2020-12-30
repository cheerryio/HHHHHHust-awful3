
#include "DataStructure.h"

Packet::Packet(const int seqnum, const int acknum, const int checksum):
	seqnum(seqnum),
	acknum(acknum),
	checksum(checksum) {}


