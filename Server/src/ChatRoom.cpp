

#include "ChatRoom.h"

ChatRoom::ChatRoom(UserDetails * user, Dispatcher *dispatcher, string roomName, int port)
{

	this->dispatcher = dispatcher;
	this->roomName = roomName;
	this->users.push_back(pair<UserDetails*,int>(user,port));
	this->MTSL.addSocket(user->getTcpSocket());
	this->running=true;

}

ChatRoom::~ChatRoom() {

}
void ChatRoom::run()
{
	TCPSocket * MTSListener;
	int command;
	while(running)
	{
		MTSListener = this->MTSL.listenToSocket();
		if(MTSListener == NULL)
			continue;
		if(MTSListener->recv((char*)&command,sizeof(int))<0)
			break;
		command = ntohs(command);
		switch(command)
		{
			case EXIT_ROOM:
				this->exitRoom(MTSListener);
				break;
		}
	}
}

int ChatRoom::numberOfClients()
{
	return this->users.size();
}

void ChatRoom::addUser(UserDetails * user, int port)
{
	
	int command = htons(ROOM_IN);
	int port1,port2;
	port1 = htons(port);
	

	for(unsigned int i=0;i<this->users.size();i++)
	{
		port2 = htons(users[i].second);
		
		if(user->getTcpSocket()->send(users[i].first->getTcpSocket()->fromAddr().c_str(),users[i].first->getTcpSocket()->fromAddr().length())<0)
		{
			perror("Error!!");
			return;
		}
		usleep(10000);
		if(user->getTcpSocket()->send((char*)&port2,sizeof(int))<0)
		{
			perror("Error!!");
			return;
		}
		usleep(10000);
		if(user->getTcpSocket()->send(users[i].first->getUserName().c_str(),users[i].first->getUserName().length())<0)
		{
			perror("Error!!");
			return;
		}


		if(users[i].first->getTcpSocket()->send((char*)&command,sizeof(int))<0)
		{
			perror("Error!!");
			return;
		}
		usleep(100000);

		if(users[i].first->getTcpSocket()->send(user->getTcpSocket()->fromAddr().c_str(),user->getTcpSocket()->fromAddr().length())<0)
		{
			perror("Error!!");
			return;
		}
		usleep(100000);
		if(users[i].first->getTcpSocket()->send((char*)&port1,sizeof(int))<0)
		{
			perror("Error!!");
			return;
		}
		usleep(100000);
		if(users[i].first->getTcpSocket()->send(user->getUserName().c_str(),user->getUserName().length())<0)
		{
			perror("Error!!");
			return;
		}
	}

	this->users.push_back(pair<UserDetails*,int>(user,port));
	this->MTSL.addSocket(user->getTcpSocket());

}

void ChatRoom::exitRoom(TCPSocket * userSocket)
{
	int command,userIndex;
	UserDetails * user;
	if(numberOfClients()==1)
	{
		user = users.at(0).first;
		this->dispatcher->startChatRoom(user);
		this->running = false;

		this->dispatcher->closeChatRoom(this->roomName);
	}
	else
	{
		command = htons(ROOM_OUT);
		for(unsigned int i=0;i<this->users.size();i++)
		{
			if(this->users[i].first->getTcpSocket()->getSocketFid() == userSocket->getSocketFid())
			{
				user = this->users[i].first;
				userIndex = i;
			}
			else
			{
				if(this->users[i].first->getTcpSocket()->send((char*)&command,sizeof(int))<0)
				{
					perror("Error!!");
					return;
				}
				usleep(1000);
				if(this->users[i].first->getTcpSocket()->send(userSocket->fromAddr().c_str(),userSocket->fromAddr().length())<0)
				{
					perror("Error!!");
					return;
				}
			}
		}
		this->users.erase(this->users.begin()+userIndex);
		this->MTSL.removeSocket(user->getTcpSocket());
		this->dispatcher->startChatRoom(user);


	}
}
