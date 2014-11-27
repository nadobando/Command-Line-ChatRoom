

#include "Broker.h"

Broker::Broker(Dispatcher * dispatcher,UserDetails * firstClient, UserDetails * secClient)
{
	this->dispatcher = dispatcher;
	this->MTSL.addSocket(firstClient->getTcpSocket());
	this->MTSL.addSocket(secClient->getTcpSocket());
	this->users = pair<UserDetails*, UserDetails*>(firstClient,secClient);

	this->running = true;
}

void Broker::run()
{
	TCPSocket * connectedSocket;
	int command;
	while(running)
	{
		connectedSocket = this->MTSL.listenToSocket();
		if(connectedSocket == NULL)
			continue;
		if(connectedSocket->recv((char*)&command,sizeof(int))<0)
			break;
		command = ntohs(command);
		switch(command)
		{
			case CLOSE_SESSION_WITH_PEER:
				this->close(connectedSocket);
				break;
		}
	}
	delete this;
}


void Broker::close(TCPSocket * closingClient)
{
	int command = htons(CLOSE_SESSION_WITH_PEER);
	if(this->users.first->getTcpSocket()->getSocketFid() == closingClient->getSocketFid())
	{
		if(this->users.second->getTcpSocket()->send((char*)&command,sizeof(int))<0)
		{
			cerr << "Error" << endl;
			return;
		}
	}
	else
	{
		if(this->users.first->getTcpSocket()->send((char*)&command,sizeof(int))<0)
		{
			cerr << "Error" << endl;
			return;
		}
	}
	this->dispatcher->openBroker(this->users);
	this->running = false;
}

Broker::~Broker() {

}

