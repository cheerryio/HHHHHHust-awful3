# SR RDT IMPLEMENTATION
## Details
1. Set timer for each packet in slideWindow. When timeout, resend the timeout packet. Not all packets in slideWindow.

2. `base` move when continuous acked packets ahead.

3. There is also slideWindow in receiver which acts as buffer. Deliver packets continuous reveived from base to application layer.

