

#include "Room.h"

Room::Room(string roomName,string clientName)
{
	this->roomName = roomName;
	this->clienName = clientName;
	this->socket = new UDPSocket();
	this->clientPort = this->socket->getSessionPort();

	this->running=true;

}

void Room::addClient(userInformation * user)
{
	this->users.push_back(user);
	cout << user->getUserName() <<" Enter Room" << endl;
}
void Room::removeClient(string ip)
{
	for(unsigned int i=0;i<this->users.size();i++)
	{
		if(this->users[i]->getIp().compare(ip) == 0)
		{
			cout << this->users[i]->getUserName()<< " Left Room" << endl;
			this->users.erase(this->users.begin()+i);
			break;
		}
	}
}
void Room::send(string msg)
{
	for(unsigned int i=0;i<this->users.size();i++)
	{

		if(this->socket->sendTo(string("--"+this->clienName+"--> " + msg),this->users[i]->getIp(),this->users[i]->getPort())<0)
			perror("Error Sending Msg");
	}
}

void Room::closeRoom()
{
	this->running = false;
	this->socket->cclose();
}

void Room::run()
{

	char * buffer;
	int size;
	while(this->running)
	{
		if(this->socket->recv((char*)&size,sizeof(int))<0)
		{
			perror("Error");
			break;
		}
		size = ntohs(size);
		buffer = new char[size];
		if(this->socket->recv(buffer,size)<0)
		{
			perror("Error");
			break;
		}
		buffer[size] = '\0';
		cout << buffer << endl;
		delete buffer;

	}
}

Room::~Room() {

}

