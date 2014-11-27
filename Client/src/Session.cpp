

#include "Session.h"

Session::Session(string partnerIP, string partnerName, string clientName)
{
	this->partnerPort=0;
	this->partnerName = partnerName;
	this->partnerIp = partnerIP;
	this->clientName = clientName;
	this->socket = new UDPSocket();
	this->port = this->socket->getSessionPort();

	this->running=true;

}
void Session::setPartnerPort(int port)
{
	this->partnerPort = port;
}

void Session::send(string msg)
{
	this->socket->sendTo(msg,this->partnerIp,this->partnerPort);
}
void Session::run()
{
	char * buffer;
	int size;
	while(this->running)
	{
		if(this->socket->recv((char*)&size,sizeof(int))<0)
			break;

		size = ntohs(size);
		buffer = new char[size];
		if(this->socket->recv(buffer,size)<0)
			break;
		buffer[size] = '\0';
		cout << "--" << partnerName << "--> " <<  buffer << endl;
		delete buffer;


	}
}
void Session::closeSession()
{

	cout << "Session Finished" << endl;
	this->socket->cclose();

	this->running = false;
	delete this->socket;
}

Session::~Session() {

}

