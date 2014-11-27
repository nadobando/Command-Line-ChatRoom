
#ifndef DISPATCHER_H_
#define DISPATCHER_H_
class Dispatcher;
#include "MultipleTCPSocketsListener.h"
#include "TCPMessengerProtocol.h"
#include "MThread.h"
#include "UserDetails.h"
#include "Broker.h"
#include "ChatRoom.h"
#include "Login.h"


#define SIZE 1024

class Dispatcher : public MThread
{
	friend class TCPMessengerServer;
	friend class Login;
private:
	vector < UserDetails* > users;
	vector < pair < UserDetails*,UserDetails*> > activeSessions;
	vector <ChatRoom*> activeRooms;
	Login * login;
	bool running;

public:

	MultipleTCPSocketsListener * listener;
	Dispatcher(Login * login);
	virtual ~Dispatcher();
	void addUser(UserDetails * user);
	void removeUser(TCPSocket * client);
	void listenToMessages();
	void startSession(TCPSocket * partnerSocket);
	void openBroker(pair<UserDetails*,UserDetails*> session);
	void createRoom(TCPSocket * userSocket);
	void joinRoom(TCPSocket * client);
	void startChatRoom(UserDetails * user);
	void closeChatRoom(string roomName);
	virtual void run();

};

#endif /* DISPATCHER_H_ */
