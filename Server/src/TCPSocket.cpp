/*
 * TCPSocket.cpp
 *
 *  Created on: Feb 6, 2013
 *      Author: Eliav Menachi
 */

#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include "TCPSocket.h"
using namespace std;

TCPSocket::TCPSocket(int connected_sock,struct sockaddr_in serverAddr,struct sockaddr_in peerAddr)
{
	this->status = DISCONNECTED;
	this->serverAddr =  serverAddr;
	this->peerAddr =  peerAddr;
	socket_fd = connected_sock;
	this->status = CLIENT_CONNECT;
}


TCPSocket::TCPSocket(int port){
	/**
	 * int socket(int domain, int type, int protocol);
	 * creates a TCP socket
	 * AF_INET - IPv4 Internet protocols
	 * SOCK_STREAM - TCP
	 * 0 - default protocol type
	 */
	this->status = DISCONNECTED;
	socket_fd = socket (AF_INET, SOCK_STREAM, 0);

	// clear the s_in struct
	bzero((char *) &serverAddr, sizeof(serverAddr));  /* They say you must do this    */

	//sets the sin address
	serverAddr.sin_family = (short)AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */
	serverAddr.sin_port = htons((u_short)port);

	//bind the socket on the specified address

	if (bind(socket_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		perror ("Error naming channel");
	}
	this->status = LISTENING;
}


TCPSocket::TCPSocket(string peerIp, int port)
{
	cout<<"openning new client socket"<<endl;

	/**
	 * int socket(int domain, int type, int protocol);
	 * creates a TCP socket
	 * AF_INET - IPv4 Internet protocols
	 * SOCK_STREAM - TCP
	 * 0 - default protocol type
	 */
	this->status = DISCONNECTED;
	socket_fd = socket (AF_INET, SOCK_STREAM, 0);

	// clear the s_in struct
	bzero((char *) &peerAddr, sizeof(peerAddr));  /* They say you must do this    */

	//sets the sin address
	peerAddr.sin_family = (short)AF_INET;
	peerAddr.sin_addr.s_addr = inet_addr(peerIp.data());
	peerAddr.sin_port = htons((u_short)port);

	if (connect(socket_fd, (struct sockaddr *)&peerAddr, sizeof(peerAddr)) < 0)
	{
		perror ("Error establishing communications");
		close(socket_fd);
	}
	this->status = CONNECTED;
}


TCPSocket* TCPSocket::listenAndAccept(){
	int rc = listen(socket_fd, 1);

	if (rc<0){
		return NULL;
	}
	socklen_t len = sizeof(peerAddr);
	bzero((char *) &peerAddr, sizeof(peerAddr));

	int connect_sock = accept(socket_fd, (struct sockaddr *)&peerAddr, &len);
	return new TCPSocket(connect_sock,serverAddr,peerAddr);
}


int TCPSocket::recv(char* buffer, int length)
{
	return read(socket_fd,buffer,length);
}


int TCPSocket::send(const char* msg, int len){
	return write(socket_fd,msg,len);
}


void TCPSocket::cclose(){

	shutdown(socket_fd,SHUT_RDWR);
	close(socket_fd);


}


string TCPSocket::fromAddr(){
	return inet_ntoa(peerAddr.sin_addr);
}

string TCPSocket::addrPort()
{
	char temp[256];
	snprintf(temp,256,"%s:%d",this->fromAddr().c_str(),htons(this->peerAddr.sin_port));
	return string(temp);
}

