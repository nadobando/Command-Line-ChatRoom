
#ifndef CHATROOM_H_
#define CHATROOM_H_
class ChatRoom;
#include "Dispatcher.h"


class ChatRoom : public MThread
{
	friend class TCPMessengerServer;
	friend class Dispatcher;
private:
	MultipleTCPSocketsListener MTSL;
	Dispatcher * dispatcher;
	vector <pair<UserDetails*,int > > users;
	bool running;


public:
	string roomName;
	ChatRoom(UserDetails * user, Dispatcher *dispatcher,string roomName, int port);
	void addUser(UserDetails * user,int port);
	void exitRoom(TCPSocket * userSocket);
	virtual void run();
	int numberOfClients();
	virtual ~ChatRoom();
};

#endif /* CHATROOM_H_ */
