# GBN RDT IMPLEMENTATION
## About GBN
1. Set timer when sending `base == nextSeqNum` || base is acked. When base is acked, stop previous timer and set another timer.

2. GBN uses culmulative ack. Ack the sender receives mean that seqnum before acknum has all been received by receiver. Just set `base = acknum + 1`.

3. If timeout, resend packets from base to nextSeqNum. 