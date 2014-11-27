/*
 * UDPSocket.cpp
 *
 *  Created on: Feb 8, 2013
 *      Author: Eliav Menachi
 */

#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include "UDPSocket.h"
#include <string>

UDPSocket::UDPSocket()
{
	// Creates a socket under IPv4 UDP
	if((udpSocket=socket(AF_INET,SOCK_DGRAM,0))<0)
	{
		throw "Socket Creation Failed";
	}

	bzero((void*)&clientInfo,sizeof(clientInfo));


	clientInfo.sin_family=(short)AF_INET;

	clientInfo.sin_port=0;//Random Port

	clientInfo.sin_addr.s_addr=htonl(INADDR_ANY);//listen to all addresses


	if((bind(udpSocket,(struct sockaddr*)&clientInfo,sizeof(clientInfo)))<0)
	{
		perror( "Error binding Socket");
	}

}

int UDPSocket::getSessionPort()
{
	struct sockaddr_in temp;//to get the fd properties
	unsigned int size = sizeof(temp);
	getsockname(udpSocket,(struct sockaddr*)&temp,&size);
	return ntohs(temp.sin_port);
}

int UDPSocket::recv(char* buffer, int length)
{

	unsigned int len=sizeof(partnerInfo);
	// Receives a message from someone and stores it in the buffer, also stores who it got the msg from in rcvinfo
	return recvfrom(udpSocket,buffer,length,0,(struct sockaddr*)&partnerInfo,&len);

}

int UDPSocket::sendTo(string msg, string ip, int port)
{
	// Creates the sender Info struct
	struct sockaddr_in senderInfo;

	unsigned int size = htons(msg.length());

	bzero((void*)&senderInfo,sizeof(senderInfo));

	senderInfo.sin_family=(short)AF_INET;
	senderInfo.sin_port=htons(port);
	senderInfo.sin_addr.s_addr=inet_addr(ip.c_str());

	// Sends the msg to the other side, send size first

	if(sendto(udpSocket,(char*)&size,sizeof(int),0,(struct sockaddr*)&senderInfo,sizeof(senderInfo))<0)
	{
		perror("error sending size");
		return -1;
	}
	return sendto(udpSocket,msg.c_str(),msg.length(),0,(struct sockaddr*)&senderInfo,sizeof(senderInfo));

}

int UDPSocket::reply(string msg)
{
	// Using rcvinfo, sends a replay to him

	 return sendto(udpSocket,msg.data(),msg.length(),0,(struct sockaddr*)&partnerInfo,sizeof(partnerInfo));

}

void UDPSocket::cclose()
{
	// Shuts down the socket and then closes it
	shutdown(udpSocket,SHUT_RDWR);

}

string UDPSocket::fromAddr()
{
	// Returns the string representation of the IP
	return inet_ntoa(partnerInfo.sin_addr);
}



