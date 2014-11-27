

#include "Login.h"

Login::Login()
{
	this->MTSL = new MultipleTCPSocketsListener();
	this->dispatcher = NULL;
	this->running=true;

}

Login::~Login() {

}

void Login::addUser(TCPSocket * user)
{
	this->MTSL->addSocket(user);
}

void Login::run()
{
	int command,bytes;
	bool isLoggedIn = false;
	char buffer[SIZE];
	this->dispatcher = new Dispatcher(this);
	this->dispatcher->start();
	TCPSocket * newUserSocket;
	UserDetails * newUser;
	string username;
	string password;
	while(running)
	{

		newUserSocket = this->MTSL->listenToSocket();
		if(newUserSocket == NULL)
			continue;


		if(newUserSocket->recv((char*)&command,sizeof(int))<0)
			break;
		command = ntohs(command);
		switch(command)
		{
			case LOGIN:
				//username
				bytes = newUserSocket->recv(buffer,SIZE);
				if(bytes<0)
					continue;
				buffer[bytes] = '\0';
				username = string(buffer);
				// password
				bytes = newUserSocket->recv(buffer,SIZE);
				if(bytes<0)
					continue;
				buffer[bytes] = '\0';
				password = string(buffer);

				if (this->usersMap.find(username) != this->usersMap.end())
				{
					if(this->usersMap.at(username)->checkInfo(username,password))
					{

						isLoggedIn = true;
						this->usersMap.at(username)->setTcpSocket(newUserSocket);
						this->usersMap.at(username)->status = LOGGED_IN;

						this->dispatcher->addUser(this->usersMap.at(username));
						command = htons(LOGIN_SUCCESS);
						if(newUserSocket->send((char*)&command,sizeof(int))<0)
							continue;
						this->MTSL->removeSocket(newUserSocket);
						break;
					}
				}
				if(!isLoggedIn)
				{
					command = htons(LOGIN_FAILED);
					bytes = newUserSocket->send((char*)&command,sizeof(int));
					if(bytes<0)
						continue;
					newUserSocket->cclose();
				}
				break;
			case SIGN_UP:
				bytes = newUserSocket->recv(buffer,SIZE);
				if(bytes<0)
					continue;
				buffer[bytes] = '\0';
				username = string(buffer);
				bytes = newUserSocket->recv(buffer,SIZE);
				if(bytes<0)
					continue;
				buffer[bytes] = '\0';
				password = string(buffer);
				if (this->usersMap.find(username) != this->usersMap.end())
				{
					command = htons(SIGN_UP_FAILED);

					if(newUserSocket->send((char*)&command,sizeof(int))<0)
						continue;
				}
				else
				{
					newUser = new UserDetails(username,password,newUserSocket);
					newUser->status = LOGGED_IN;
					this->usersMap.insert(pair<string, UserDetails*>(username, newUser));
					this->dispatcher->addUser(newUser);
					command = htons(SIGN_UP_SUCESS);
					if(newUserSocket->send((char*)&command,sizeof(int))<0)
						continue;
					this->MTSL->removeSocket(newUserSocket);
				}
				break;
		}
	}
}

