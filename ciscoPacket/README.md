# Network Build
## What is VLAN
virtual lan
## Tips
### About EXP 3
> default mask for rip routing protocol while ip is above 192 is 255.255.255.
> So,use `summary` such thing to change the default mask

> it's definitely positive to connect ends of different ips and different vlans to be connected
> together with just one router. Just configure the router's sub interface with like 
> fa0/0.1 fa0/0.2. Specific commands is
```
int fa0/0.1
encapsulation dot1q 2
ip .......
```
> commands above enter sub interface fa0/0.1, deploy the subinterface to 
> vlan2 and set an ip for the subinterface

> When router port is configured to deny any ip. Whatever in or out, PC1 wont
> be able to ping PC2, so dose PC2. This is a two-way process.

> Debug methods!! Use the simulation feature in Packet Tracer.