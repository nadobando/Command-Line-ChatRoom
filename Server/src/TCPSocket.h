/*
 * TCPSocket.h
 *
 *  Created on: Feb 6, 2013
 *      Author: Eliav Menachi
 */

#ifndef TCPSOCKET_H_
#define TCPSOCKET_H_

#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "TCPMessengerProtocol.h"
using namespace std;

class TCPSocket{
	struct sockaddr_in serverAddr;
	int socket_fd;

private:
	/**
	 * private constructor to create a secondary server socket to communicate with a remote peer
	 */
	TCPSocket(int connected_sock,struct sockaddr_in serverAddr,struct sockaddr_in peerAddr);

public:
	struct sockaddr_in peerAddr;
	int status;
	/**
	 * Constructor create a TCP server socket
	 */
	TCPSocket(int port);

	/**
	 * Constructor creates TCP client socket
	 */
	TCPSocket(string peerIp, int port);
	/**
	 * Perform listen and accept on server socket
	 */
	TCPSocket* listenAndAccept();

	/**
	 * Read from socket into the given buffer up to the buffer given length.
	 * return the number of bytes read
	 */
	int recv(char* buffer, int length);

	/**
	 * send the given buffer to the socket
	 */
	int send(const char* msg, int len);

	/**
	 * close the socket and free all resources
	 */
	void cclose();

	/**
	 * return the address of the connected peer
	 */
	string fromAddr();

	/**
	 * returns the c socket fid
	 */
	int getSocketFid(){
		return socket_fd;
	}
	string addrPort();
};

#endif
