
#include "TCPMessengerServer.h"

TCPMessengerServer::TCPMessengerServer()
{
	this->listener = new TCPSocket(MSNGR_PORT);
	this->running=true;
	this->login = NULL;
	this->start();
}
void TCPMessengerServer::run()
{
	TCPSocket * connectingClient;
	this->login = new Login();
	this->login->start();
	while(running)
	{
		connectingClient = this->listener->listenAndAccept();
		this->login->addUser(connectingClient);
	}
}
void TCPMessengerServer::listCommand(int command, string roomName)
{
	switch (command) {
		case GET_ALL_USERS:
			if(this->login->usersMap.size() == 0)
			{
				cout << "No Users" << endl;
				return;
			}
			cout << "Users: " << endl;

			for(map<string, UserDetails *>::iterator it = this->login->usersMap.begin();it !=this->login->usersMap.end();++it)
			{
				cout<< it->first<<endl;
			}
			break;
		case ONLINE_USERS:
			if(this->login->dispatcher->users.size() == 0)
			{
				cout << "No Users Online" << endl;
				return;
			}
			cout << "On-line Users:" << endl;
			for(unsigned int i=0;i<this->login->dispatcher->users.size();i++)
			{
				cout << this->login->dispatcher->users[i]->getUserName() << endl;
			}
			break;
		case ACTIVE_SESSIONS:
			if(this->login->dispatcher->activeSessions.size() == 0)
			{
				cout << "No Sessions" << endl;
				return;
			}
			cout << "Sessions:" << endl;
			for(unsigned int i=0;i<this->login->dispatcher->activeSessions.size();i++)
			{
				cout << this->login->dispatcher->activeSessions[i].first->getUserName() << " : " << this->login->dispatcher->activeSessions[i].second->getUserName() << endl;
			}
			break;
		case EXISTING_ROOMS:
			if(this->login->dispatcher->activeRooms.size() == 0)
			{
				cout << "No Rooms" << endl;
				return;
			}
			cout << "Rooms:" << endl;
			for(unsigned int i=0;i<this->login->dispatcher->activeRooms.size();i++)
			{
				cout << this->login->dispatcher->activeRooms[i]->roomName << endl;
			}
			break;
		case USERS_IN_ROOM:
		{
			bool exists = false;
			int userIndex;
			for(unsigned int i=0;i<this->login->dispatcher->activeRooms.size();i++)
			{
				if(this->login->dispatcher->activeRooms[i]->roomName.compare(roomName) == 0)
				{
					exists = true;
					userIndex = i;
					break;
				}
			}
			if(!exists)
			{
				cout << "Room Not Exists" << endl;
				return;
			}
			cout << "Users in Room: " << roomName << endl;
			for(unsigned int i=0;i<this->login->dispatcher->activeRooms[userIndex]->users.size();i++)
			{
				cout << this->login->dispatcher->activeRooms[userIndex]->users[i].first->getUserName() << endl;
			}
			break;
		}
		default:
			break;
	}


}
void TCPMessengerServer::close()
{
	running=true;
	sleep(1);
	this->listener->cclose();
}

