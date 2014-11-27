

#include "Dispatcher.h"



Dispatcher::Dispatcher(Login * login)
{
	this->listener = new MultipleTCPSocketsListener();
	this->login = login;
	this->running = true;
}

void Dispatcher::addUser(UserDetails * user)
{
	this->listener->addSocket(user->getTcpSocket());
	this->users.push_back(user);
}

void Dispatcher::removeUser(TCPSocket * client)
{
	for(unsigned int i=0;i<this->users.size();i++)
	{
		if(this->users[i]->getTcpSocket()->getSocketFid() == client->getSocketFid())
		{
			//get the current user index from the vector.
			this->users.erase(this->users.begin() + i);
			this->listener->removeSocket(client);
			client->cclose();
			break;
		}
	}
}

void Dispatcher::run()
{
	this->listenToMessages();
}

void Dispatcher::listenToMessages()
{
	TCPSocket * connectedSocket;
	int command,size,bytes,roomIndex;
	string roomName;
	bool roomExists=false;
	char buffer[SIZE];

	while(running)
	{
		connectedSocket = this->listener->listenToSocket();
		if(connectedSocket == NULL)
			continue;
		if(connectedSocket->recv((char*)&command,sizeof(int))<0)
			break;
		command = ntohs(command);
		switch(command)
		{
			case OPEN_SESSION_WITH_PEER:
				this->startSession(connectedSocket);
				break;
			case CREATE_ROOM:
				this->createRoom(connectedSocket);
				break;
			case OPEN_ROOM:
				this->joinRoom(connectedSocket);
				break;
			case GET_ALL_USERS:
				command = htons(LIST_REPLY);
				size = htons(this->login->usersMap.size());
				if(connectedSocket->send((char*)&command,sizeof(int))<0)
					continue;
				usleep(1000);
				if(connectedSocket->send((char*)&size,sizeof(int))<0)
					continue;
				for (std::map<string,UserDetails*>::iterator it=this->login->usersMap.begin(); it!=this->login->usersMap.end(); ++it)
				{
					if(connectedSocket->send(it->second->getUserName().c_str(),it->second->getUserName().length())<0)
						continue;


				}
				break;
			case ONLINE_USERS:
				command = htons(LIST_REPLY);
				size = htons(this->users.size());
				if(connectedSocket->send((char*)&command,sizeof(int))<0)
					continue;
				usleep(1000);
				if(connectedSocket->send((char*)&size,sizeof(int))<0)
					continue;
				cout << ntohs(size) << endl;

				for(int i=0;i<ntohs(size);i++)
				{
					if(connectedSocket->send(this->users[i]->getUserName().c_str(),this->users[i]->getUserName().length())<0)
						continue;
				}
				break;
			case EXISTING_ROOMS:
				command = htons(LIST_REPLY);
				size = htons(this->activeRooms.size());
				if(connectedSocket->send((char*)&command,sizeof(int))<0)
					continue;
				usleep(1000);
				if(connectedSocket->send((char*)&size,sizeof(int))<0)
					continue;
				for(int i=0;i<ntohs(size);i++)
				{
					if(connectedSocket->send(this->activeRooms[i]->roomName.c_str(),this->activeRooms[i]->roomName.length())<0)
						continue;
				}
				break;
			case USERS_IN_ROOM:
				bytes = connectedSocket->recv(buffer,SIZE);
				if(bytes<0)
					continue;
				buffer[bytes] = '\0';
				roomName = string(buffer);
				// find if room exists
				for(unsigned int i=0;i<this->activeRooms.size();i++)
				{
					if(this->activeRooms[i]->roomName.compare(roomName) == 0)
					{
						roomExists = true;
						roomIndex = i;
						break;
					}
				}
				if(!roomExists)
				{
					command = htons(ROOM_NOT_FOUND);
					if(connectedSocket->send((char*)&command,sizeof(int))<0)
						continue;
					break;
				}
				command = htons(LIST_REPLY);
				size = htons(this->activeRooms[roomIndex]->users.size());
				if(connectedSocket->send((char*)&command,sizeof(int))<0)
					continue;
				usleep(1000);
				if(connectedSocket->send((char*)&size,sizeof(int))<0)
					continue;
				for(int i=0;i<ntohs(size);i++)
				{
					if(connectedSocket->send(this->activeRooms[roomIndex]->users[i].first->getUserName().c_str(),this->activeRooms[roomIndex]->users[i].first->getUserName().length())<0)
						continue;
				}
				break;
		}
	}
}

void Dispatcher::createRoom(TCPSocket * userSocket)
{
	string roomName;
	char buffer[SIZE];
	int bytes,command;
	UserDetails * user;
	ChatRoom * chatRoom;
	for(unsigned int i=0;i<this->users.size();i++)
	{
		if(this->users[i]->getTcpSocket()->getSocketFid() == userSocket->getSocketFid())
		{
			user = this->users[i];
		}
	}
	bytes = userSocket->recv(buffer,SIZE);
	if(bytes<0)
	{
		perror("Error!!");
		return;
	}
	buffer[bytes] = '\0';
	roomName = string(buffer);

	for(unsigned int i=0;i<activeRooms.size();i++)
	{

		if(activeRooms[i]->roomName.compare(roomName) == 0)
		{
			command = htons(ROOM_REFUSED);
			if(userSocket->send((char*)&command,sizeof(int))<0)
			{
				perror("Error!!");
			}
			return;
		}
	}


	this->listener->removeSocket(userSocket);
	command = htons(ROOM_CREATED);
	if(userSocket->send((char*)&command,sizeof(int))<0)
	{
		perror("Error!!");
		return;
	}

	usleep(10000);
	command = htons(0);
	if(userSocket->send((char*)&command,sizeof(int))<0)
	{
		perror("Error!!");
		return;
	}

	int creatorsPort;
	if(userSocket->recv((char*)&creatorsPort,sizeof(int))<0)
	{
		perror("Error!!");
		return;
	}
	chatRoom = new ChatRoom(user,this,roomName,creatorsPort);
	this->activeRooms.push_back(chatRoom);
	user->status = IN_ROOM;
	chatRoom->start();
}

void Dispatcher::joinRoom(TCPSocket * client)
{
	string roomName;
	char buffer[SIZE];
	int bytes,command;
	bool existingRoom = false;
	UserDetails * clientDetails;
	ChatRoom * chatRoom;

	for(unsigned int i=0;i<this->users.size();i++)
	{
		if(this->users[i]->getTcpSocket()->getSocketFid() == client->getSocketFid())
		{
			clientDetails = this->users[i];
		}
	}

	bytes = client->recv(buffer,SIZE);
	if(bytes<0)
	{
		perror("Error!!");
		return;
	}
	buffer[bytes] = '\0';
	roomName = string(buffer);

	for(unsigned int i=0;i<activeRooms.size();i++)
	{
		if(activeRooms[i]->roomName.compare(roomName) == 0)
		{
			chatRoom = this->activeRooms[i];
			existingRoom = true;
			break;
		}
	}

	if(!existingRoom)
	{
		command = htons(ROOM_REFUSED);
		if(client->send((char*)&command,sizeof(int))<0)
		{
			perror("Error!!");
		}
		return;
	}

	command = htons(ROOM_CREATED);
	if(client->send((char*)&command,sizeof(int))<0)
	{
		perror("Error!!");
		return;
	}

	usleep(100000);
	command = htons(chatRoom->numberOfClients());
	if(client->send((char*)&command,sizeof(int))<0)
	{
		perror("Error!!");
		return;
	}

	int userPort;
	if(client->recv((char*)&userPort,sizeof(int))<0)
	{
		perror("Error!!");
		return;
	}

	chatRoom->addUser(clientDetails,userPort);
	this->listener->removeSocket(client);
	clientDetails->status = IN_ROOM;

}

void Dispatcher::startSession(TCPSocket * partnerSocket)
{
	char * buffer = new char[SIZE];
	int command,clientPort,partnerPort;
	UserDetails * clientDetails=NULL;
	UserDetails * partnerDetails=NULL;
	Broker * broker;
	int bytes;
	usleep(100000);
	bytes = partnerSocket->recv(buffer,SIZE);
	if(bytes<0)
	{
		perror("Error!!");
		return;
	}
	buffer[bytes] = '\0';
	string userName = string(buffer);

	for(unsigned int i=0;i<this->users.size();i++)
	{
		if(this->users[i]->getTcpSocket()->getSocketFid() == partnerSocket->getSocketFid())
		{
			clientDetails = this->users[i];
		}
	}

	for(unsigned int i=0;i<this->users.size();i++)
	{
		if(this->users[i]->getUserName().compare(userName) == 0)
		{
			partnerDetails = this->users[i];

			if(partnerDetails->status != LOGGED_IN)
			{
				command = htons(SESSION_REFUSED);
				if(partnerSocket->send((char*)&command,sizeof(int))<0)
					perror("Error!!");
				return;
			}
			else
				break;
		}
	}
	if(partnerDetails == NULL)
	{
		command = htons(SESSION_REFUSED);
		if(partnerSocket->send((char*)&command,sizeof(int))<0)
			perror("Error!!");
		return;
	}

	command = htons(SESSION_REQUEST);
	if(partnerDetails->getTcpSocket()->send((char*)&command,sizeof(int))<0)
	{
		perror("Error!!");
		return;
	}
	usleep(100000);

	if(partnerDetails->getTcpSocket()->send(clientDetails->getTcpSocket()->fromAddr().c_str(),clientDetails->getTcpSocket()->fromAddr().length())<0)
	{
		perror("Error!!");
		return;
	}
	usleep(100000);
	if(partnerDetails->getTcpSocket()->send(clientDetails->getUserName().c_str(),clientDetails->getUserName().length())<0)
	{
		perror("Error!!");
		return;
	}

	if(partnerDetails->getTcpSocket()->recv((char*)&command,sizeof(int))<0)
	{
		perror("Error!!");
		return;
	}
	command = ntohs(command);
	if(command == SESSION_ACCEPTED)
	{

		if(partnerDetails->getTcpSocket()->recv((char*)&partnerPort,sizeof(int))<0)
		{
			perror("Error!!");
			return;
		}

		command = htons(SESSION_ESTABLISHED);
		if(partnerSocket->send((char*)&command,sizeof(int))<0)
		{
			perror("Error!!");
			return;
		}

		usleep(100000);
		if(partnerSocket->send(partnerDetails->getTcpSocket()->fromAddr().c_str(),partnerDetails->getTcpSocket()->fromAddr().length())<0)
		{
			perror("Error!!");
			return;
		}

		usleep(100000);
		if(partnerSocket->send((char*)&partnerPort,sizeof(int))<0)
		{
			perror("Error!!");
			return;
		}

		if(partnerSocket->recv((char*)&clientPort,sizeof(int))<0)
		{
			perror("Error!!");
			return;
		}

		if(partnerDetails->getTcpSocket()->send((char*)&clientPort,sizeof(int))<0)
		{
			perror("Error!!");
			return;
		}

		clientDetails->status = IN_SESSION;
		partnerDetails->status = IN_SESSION;
		this->activeSessions.push_back(pair<UserDetails*, UserDetails*>(clientDetails,partnerDetails));
		broker = new Broker(this,clientDetails,partnerDetails);
		broker->start();
		this->listener->removeSocket(clientDetails->getTcpSocket());
		this->listener->removeSocket(partnerDetails->getTcpSocket());
	}
	else
	{
		command = htons(SESSION_REFUSED);
		if(clientDetails->getTcpSocket()->send((char*)&command,sizeof(int))<0)
		{
			perror("Error!!");
		}
	}

}
void Dispatcher::openBroker(pair<UserDetails*,UserDetails*> session)
{

	for(unsigned int i=0;i<this->activeSessions.size();i++)
	{

		if(activeSessions[i].first->checkUsers(session.first))
		{
			this->activeSessions.erase(this->activeSessions.begin()+i);
			break;
		}
	}

	session.first->status = LOGGED_IN;
	session.second->status = LOGGED_IN;
	this->listener->addSocket(session.first->getTcpSocket());
	this->listener->addSocket(session.second->getTcpSocket());
}
void Dispatcher::startChatRoom(UserDetails * user)
{
	this->listener->addSocket(user->getTcpSocket());
}
void Dispatcher::closeChatRoom(string roomName)
{
	ChatRoom * chatRoom;

	for(unsigned int i=0;i<this->activeRooms.size();i++)
	{
		if(this->activeRooms[i]->roomName == roomName)
		{
			chatRoom = this->activeRooms[i];
			this->activeRooms.erase(this->activeRooms.begin()+i);
			delete chatRoom;
			break;
		}
	}
}

Dispatcher::~Dispatcher()
{
	this->running = false;

}

