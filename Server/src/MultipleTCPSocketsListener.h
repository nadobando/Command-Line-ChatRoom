

#ifndef MULTIPLETCPSOCKETSLISTENER_H_
#define MULTIPLETCPSOCKETSLISTENER_H_

#include <iostream>
#include <vector>
#include "TCPSocket.h"

using namespace std;


class MultipleTCPSocketsListener
{



public:
	vector<TCPSocket*> socketVector;
	/*
	 * Add the given socket to the socket list to be listen on
	 */
	void addSocket(TCPSocket* socket);

	/*
	 * Set the given sockets to the socket list to be listen on
	 */
	void addSockets(vector<TCPSocket*> socketVec);

	/*
	 * This method checks to see if any of the previous given sockets are ready for reading
	 * It returns one of the Sockets that are ready.
	 */
	void removeSocket(TCPSocket * sock);
	TCPSocket* listenToSocket();
	void removeAndCloseSocket(TCPSocket* socket);
};


#endif /* MULTIPLETCPSOCKETSLISTENER_H_ */
