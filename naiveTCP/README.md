# naiveTCP

## Details
1. Receiver send the expected ack. For example, receives seqnum 5 and sends acknum 6

2. Sender sets timer for each packet send. When timeout, sends the timeout packet.

3. When receives dulplicate 4 acks, fast resend this packet.