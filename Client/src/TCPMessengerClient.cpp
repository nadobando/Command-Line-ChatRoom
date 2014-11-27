
#include "TCPMessengerClient.h"
#include "TCPMessengerProtocol.h"


/**
 * initialize all properties
 */
TCPMessengerClient::TCPMessengerClient()
{
	clientChatRoom=NULL;
	clientSession=NULL;

	this->server = NULL;
	this->clientStatus = DISCONNECTED;

	running=true;
}

/**
 * connect to the given server ip (the port is defined in the protocol header file)
 */
void TCPMessengerClient::connect(string ip)
{
	this->server = new TCPSocket(ip,MSNGR_PORT);
	this->start();
	this->clientStatus = CONNECTED;
}

void TCPMessengerClient::login(int command,string username, string password)
{
//	int command;
	command = htons(command);
	//send command login or signup
	if(this->server->send((char*)&command,sizeof(int))<0)
	{
		perror("Error!!");
		return;
	}
	//send username
	if(this->server->send(username.c_str(),username.length())<0)
	{
		perror("Error!!");
		return;
	}
	usleep(100000);
	//send password
	if(this->server->send(password.c_str(),password.length())<0)
	{
		perror("Error!!");
		return;
	}
	this->userName = username;




}

/**
 * return true if connected to the server
 */
bool TCPMessengerClient::isConnected()
{
	if(this->clientStatus!=DISCONNECTED)
			return true;
	return false;
}
bool TCPMessengerClient::isLogged()
{
	if(this->clientStatus != DISCONNECTED && this->clientStatus != CONNECTED)
		return true;
	return false;
}
/**
 * disconnect from messenger server
 */
void TCPMessengerClient::disconnect()
{
	int command = htons(EXIT);
	int br = this->server->send((char*)&command,sizeof(int));
	if(br < 0)
	{
		perror("Error!!");
		return;
	}
	this->clientStatus=DISCONNECTED;
	this->server->cclose();
}

/**
 * open session with the given peer address (ip:port)
 */
void TCPMessengerClient::openSession(string userName)
{
	string IP;

	// checking if exists session
	if(this->clientStatus == IN_SESSION)
		return;
	int command = htons(OPEN_SESSION_WITH_PEER);

	//SEND Command
	if(this->server->send((char*)&command,sizeof(int))<0)
	{
		perror("Error!!");
		return;
	}
	usleep(100000);

	// Username to connect
	if(this->server->send(userName.c_str(),userName.length())<0)
	{
		perror("Error!!");
		return;
	}
	this->partnerUserName = userName;
}

/**
 * return true if a session is active
 */
bool TCPMessengerClient::inSession()
{
	if(this->clientStatus == IN_SESSION || this->clientStatus == IN_ROOM)
		return true;
	return false;
}

/**
 * close active session
 */
void TCPMessengerClient::closeSession()
{
	int command = htons(CLOSE_SESSION_WITH_PEER);
	if(this->server->send((char*)&command,sizeof(int))<0)
	{
		perror("Error!!");
		return;
	}
	this->clientSession->closeSession();
	delete this->clientSession;
	this->clientStatus = CONNECTED;
}

void TCPMessengerClient::send(string msg)
{
	if(this->clientStatus == IN_SESSION)
		this->clientSession->send(msg);
	if(this->clientStatus == IN_ROOM)
		this->clientChatRoom->send(msg);
}

void TCPMessengerClient::roomCommand(int command ,string roomName)
{
	command = htons(command);
	//SEND COMMAND
	if(this->server->send((char*)&command,sizeof(int))<0)
	{
		perror("Error!!");
		return;
	}
	usleep(1000);
	// SEND ROOM NAME
	if(this->server->send(roomName.c_str(),roomName.length())<0)
	{
		perror("Error!!");
		return;
	}
	this->roomName = roomName;
}

void TCPMessengerClient::closeRoom()
{

	if(this->clientStatus == IN_ROOM)
	{
		int command = htons(EXIT_ROOM);
		if(this->server->send((char*)&command,sizeof(int))<0)
		{
			perror("Error!!");
			return;
		}
		this->clientChatRoom->closeRoom();
		this->clientStatus = CONNECTED;
	}
}

void TCPMessengerClient::list(int command, string roomName)
{

	switch (command) {
		case USERS_IN_ROOM:
			command = htons(command);
			if(this->server->send((char*)&command,sizeof(int))<0)
			{
				perror("Error!!");
				return;
			}
			usleep(1000);
			if(this->server->send(roomName.c_str(),roomName.length())<0)
			{
				perror("Error!!");
				return;
			}
			break;

		default:
			command = htons(command);
			if(this->server->send((char*)&command,sizeof(int))<0)
			{
				perror("Error!!");
				return;
			}
			break;
	}

}

void TCPMessengerClient::run()
{
	int partnerPort, clientPort,port,size,command,bytes,clientsInRoom;
	char buffer[SIZE];
	string userName,ip,roomName,random;

	userInformation * userInfo;
	while(running)
	{
		if(this->server->recv((char*)&command,sizeof(int))<0)
			break;
		switch(ntohs(command))
		{
			case SESSION_REQUEST:


				bytes=this->server->recv(buffer,SIZE);
				if(bytes < 0)
					continue;
				buffer[bytes] = '\0';
				ip = string(buffer);
				usleep(100000);


				bytes=this->server->recv(buffer,SIZE);
				if(bytes < 0)
					continue;
				buffer[bytes] = '\0';
				userName=string(buffer);

				command = htons(SESSION_ACCEPTED);
				bytes = this->server->send((char*)&command,sizeof(int));
				if(bytes < 0)
					continue;


				this->clientSession = new Session(ip,userName,this->userName);


				clientPort = this->clientSession->port;


				bytes = this->server->send((char*)&clientPort,sizeof(int));
				if(bytes < 0)
					continue;


				bytes = this->server->recv((char*)&partnerPort,sizeof(int));
				if(bytes < 0)
					continue;
				this->clientSession->setPartnerPort(partnerPort);
				this->clientSession->start();
				this->clientStatus = IN_SESSION;
				break;
			case SESSION_ESTABLISHED:
				// Receive the IP
				usleep(10000);
				bytes = this->server->recv(buffer,SIZE);
				if(bytes < 0)
					continue;
				buffer[bytes] = '\0';
				ip=string(buffer);
				// Receive Port
				bytes = this->server->recv((char*)&partnerPort,sizeof(int));
				if(bytes < 0)
					continue;
				// Send My Port
				this->clientSession = new Session(ip,this->partnerUserName,this->userName);
				clientPort = this->clientSession->port;
				bytes = this->server->send((char*)&clientPort,sizeof(int));
				if(bytes < 0)
					continue;
				this->clientSession->setPartnerPort(partnerPort);
				this->clientSession->start();
				this->clientStatus = IN_SESSION;
				cout << "Starting Session" << endl;
				break;
			case SESSION_REFUSED:
				cout << "Session Failed" << endl;
				break;
			case CLOSE_SESSION_WITH_PEER:
				this->clientSession->closeSession();
				delete this->clientSession;
				this->clientStatus = LOGGED_IN;

				break;
			case ROOM_CREATED:

				if(this->server->recv((char*)&clientsInRoom,sizeof(int))<0)
					continue;
				clientsInRoom = ntohs(clientsInRoom);
				this->clientChatRoom = new Room(this->roomName, this->userName);

				bytes = this->server->send((char*)&this->clientChatRoom->clientPort,sizeof(int));
				if(bytes < 0)
					continue;
				for(int i=0;i<clientsInRoom;i++)
				{

					userInfo = new userInformation();
					bytes=this->server->recv((char*)&buffer,SIZE);
					if(bytes < 0)
						continue;
					buffer[bytes] = '\0';
					ip = string(buffer);
					bytes=this->server->recv((char*)&port,sizeof(int));
					if(bytes < 0)
						continue;
					port=ntohs(port);
					bytes=this->server->recv((char*)&buffer,SIZE);
					if(bytes < 0)
						continue;
					buffer[bytes]='\0';
					userName = string(buffer);
					userInfo->setIp(ip);
					userInfo->setPort(port);
					userInfo->setUserName(userName);

					this->clientChatRoom->addClient(userInfo);
				}
				this->clientChatRoom->start();
				this->clientStatus = IN_ROOM;
				cout << "Redirecting to Room" << endl;
				break;
			case ROOM_REFUSED:
				cout << "Room Connection Failed" << endl;
				break;
			case ROOM_IN:
				userInfo = new userInformation();
				bytes=this->server->recv((char*)&buffer,SIZE);
				if(bytes < 0)
					continue;
				buffer[bytes] = '\0';
				ip = string(buffer);
				bytes=this->server->recv((char*)&port,sizeof(int));
				if(bytes < 0)
					continue;
				port=ntohs(port);
				bytes=this->server->recv((char*)&buffer,SIZE);
				if(bytes < 0)
					continue;
				buffer[bytes]='\0';
				userName = string(buffer);
				userInfo->setIp(ip);
				userInfo->setPort(port);
				userInfo->setUserName(userName);
				this->clientChatRoom->addClient(userInfo);
				break;
			case ROOM_OUT:
				bytes=this->server->recv(buffer,SIZE);
				if(bytes < 0)
					continue;
				buffer[bytes] = '\0';
				ip = string(buffer);
				this->clientChatRoom->removeClient(ip);
				break;
			case ROOM_CLOSED:
				this->clientChatRoom->closeRoom();
				delete this->clientChatRoom;
				this->clientStatus = LOGGED_IN;
				break;
			case LOGIN_SUCCESS:
				this->clientStatus = LOGGED_IN;
				cout << "Login success" << endl;
				break;
			case LOGIN_FAILED:
				cout << "Login Failed" << endl;
				this->server->cclose();
				break;
			case SIGN_UP_SUCESS:
				this->clientStatus = LOGGED_IN;
				cout << "Register success" << endl;
				break;
			case SIGN_UP_FAILED:
				cout << "Register Failed" << endl;

				break;
			case LIST_REPLY:
				bytes = this->server->recv((char*)&size,sizeof(int));
				if(bytes < 0)
					continue;
				size=ntohs(size);
				if(size==0)
				{
					cout<<"No Results"<<endl;
					break;
				}
				for(int i=0;i<size;i++)
				{
					bytes = this->server->recv(buffer,SIZE);
					if(bytes < 0)
						continue;
					buffer[bytes] = '\0';
					random = string(buffer);
					cout << random << endl;
				}
				break;
			case ROOM_NOT_FOUND:
				cout << "Room Not Exists" << endl;
				break;
		}
	}
}
