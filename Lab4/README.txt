WE ACCEPT THAT ALL WORK DONE IS OF OUR OWN.

This folder contains receiver.c and sender.c and README this file ...

############################################################################################################
ASSUMPTIONS:
RetransmissionTime is an integer, provided in seconds.
Loopback interface is ”lo”.
We print errors on the terminal and neccessary details on both terminal and file.  

#############################################################################################################
STEPS TO COMPILE AND RUN THE CODE:

1.Open two terminal windows side-by-side.
2.Go to the directory(the current folder) containing sender.c and receiver.c
3.Type "sudo tc qdisc add dev lo root netem delay <Delay in miliseconds>" in any terminal to emulate network delays between sender and receiver.
4.Type "gcc receiver.c -o receiver" 
in any one of two opened terminal windows and press enter(to compile receiver.c) and 
type "gcc sender.c -o sender" in other terminal  (to compile sender.c) and press enter.

5.First Type  "./receiver.c <ReceiverPort> <SenderPort> <PacketDropProbability>" 
and press enter to run the receiver code  

and then type "./sender.c <SenderPort> <ReceiverPort> <RetransmissionTimer> <NoOfPacketsToBeSent>"
 to run sender code in respective terminal windows 

#############################################################################################################	
Code Description::
**********************************
In UDP(User Datagram Protocol), the client does not form a connection with the server (in TCP it forms a conection with the server)
and instead just sends a datagram. 
Similarly, the server need not accept a connection and just waits for datagrams to arrive. Datagrams upon arrival contain the address of sender which the server uses to send data to the correct client.

In our code using socket  programming we have implemented UDP server-client implementation in C language.
The directory contains two .c files naming receiver.c and sender.c .

**********************************
UDP server follows following steps:
1.Creating the UDP socket.(Done using socket() function which returns socket file descriptor).

2.Bind the created socket to server address(IPv4)(Done using bind which assigns address to the unbound socket).

3.Wait until datagram packet arrives from client.(recvfrom() function)

4.Process the datagram packet (see if it receives what was expected or not). If expected packet is not received then send the acknowledgement for the expected packet. But if expected packet is received then generate a random number and compare it with packet_drop_probability and if probability is less then we send the right acknowlegment otherwise no acknowledgment was sent.

5.Go back to Step 3

**********************************
UDP client follows following steps:

1.Create UDP socket.(Done using socket() function which returns socket file descriptor)

2.Send message to server.(Done using sendto() function )

3.Wait until response from server is recieved else try again.(Using recvfrom() function check if acknoledgement is recieved or not). if acknowlegment is received as expected then continue step2, else send the packet number demanded by the server in its acknowlegment.

5.After sending all num_packets successfully close socket descriptor and exit.(using close() function).




