/*
 * UDPSocket.h
 *
 *  Created on: Feb 6, 2013
 *      Author: Eliav Menachi
 */

#ifndef UDPSOCKET_H_
#define UDPSOCKET_H_

#include <netinet/in.h>
#include <inttypes.h>
#include <strings.h>
#include <stdio.h>
#include <iostream>

using namespace std;

class UDPSocket
{
private:
	int udpSocket;
	struct sockaddr_in clientInfo;
	struct sockaddr_in partnerInfo;


public:
	/**
	 * constructor: initialize a UDP socket and binds it on the given port.
	 * if no port is specified a default port 9999 is set.
	 * this UDP socket can be used both for server socket and client socket
	 */
	UDPSocket();
	/**
	 * reads an incoming message from the UDP socket.
	 * the message is copied into the given buffer up to the specified length
	 */
	int recv(char* buffer, int length);
	/**
	 * sends the given message as a UDP message to the given address specified by IP and port
	 */
	int sendTo(string msg, string ip, int port);
	/**
	 * reply to an incoming message, this method will send the given message
	 * as a UDP message to the peer from which the last message was received.
	 */
	int reply(string msg);
	/**
	 * close the UDP socket
	 */
	void cclose();
	/**
	 * return the sender IP of the last received message
	 */
	string fromAddr();

	int getSessionPort();
};

#endif /* UDPSOCKET_H_ */
