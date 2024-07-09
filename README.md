# B3 UMDF Feed SBE

This project is a library with the objective of receiving and processing data sent by B3 through a UDP Multicast channel and delivering it as quickly as possible. For this I used the AF_PACKET socket where it is possible to receive packages directly from the [queueing discipline (QDisc)](https://www.linuxjournal.com/content/queueing-linux-network-stack) , another advantage is that it is possible to use a shared memory between the user space and the kernel through a ring buffer where we can receive the packets without the need to make a single systemcall.

### include/io/multicast_udp_receiver.h
This file contains the linux udp multicast socket only it is necessary to enable the CAP_NET_RAW capability to run without sudo.

### include/b3/protocol/b3_message.hpp

implementation of messages sent by B3 through the UDP multicast channel.

### include/b3/channel.hpp

represents a B3 channel with 3 sockets instrument definition, snapshot and incremental. This class aims to synchronize and deliver messages to the Lib user.


![result in nanoseconds of receiving and processing messages](https://github.com/blogdarkspot/B3UMDF_SBE_Lib/assets/17806113/2f9b2956-b81b-4874-97f7-7c1fb58668d1)
Result in nanosecods to receiving and processing messages.
