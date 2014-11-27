

#include "MultipleTCPSocketsListener.h"

using namespace std;


//MultipleTCPSocketsListener::MultipleTCPSocketsListener()
//{
//	this->SocketVector=new vector<TCPSocket*>();
//}

/*
 * Add the given socket to the socket list to be listen on
 */
void MultipleTCPSocketsListener::addSocket(TCPSocket* socket)
{
	this->socketVector.push_back(socket);
}

/*
 * Set the given sockets to the socket list to be listen on
 */
void MultipleTCPSocketsListener::addSockets(vector<TCPSocket*> socketVec)
{
	this->socketVector.clear();
	this->socketVector=socketVec;

}

/*
 * This method checks to see if any of the previous given sockets are ready for reading
 * It returns one of the Sockets that are ready.
 */
TCPSocket* MultipleTCPSocketsListener::listenToSocket()
{
	fd_set set;
	int check,maxfd=0;
	timeval timeout;
	FD_ZERO(&set);
	if(this->socketVector.size() == 0)
		return NULL;
	for(unsigned int i=0;i<this->socketVector.size();i++)
	{
		FD_SET(this->socketVector[i]->getSocketFid(),&set);
		if(this->socketVector[i]->getSocketFid()>maxfd)
			maxfd=this->socketVector[i]->getSocketFid();
	}
	timeout.tv_sec=2;
	timeout.tv_usec=0;
	check=select(maxfd+1,&set,NULL,NULL,&timeout);
	if(check==-1)
	{
		perror("Error!!");
	}
	else
	{
		for(unsigned int i=0;i<this->socketVector.size();i++)
		{
			if(FD_ISSET(this->socketVector[i]->getSocketFid(),&set))
			{
				FD_CLR(this->socketVector[i]->getSocketFid(),&set);
				return this->socketVector[i];
			}
		}
	}

	return NULL;
}
void MultipleTCPSocketsListener::removeSocket(TCPSocket * sock)
{
	for(unsigned int i=0;i<socketVector.size();i++)
		if(this->socketVector[i]->getSocketFid() == sock->getSocketFid())
			this->socketVector.erase(this->socketVector.begin()+i);
}
