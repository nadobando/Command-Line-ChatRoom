

#ifndef LOGINHANDLER_H_
#define LOGINHANDLER_H_
class Login;
#include "MThread.h"
#include "MultipleTCPSocketsListener.h"
#include <iostream>

#include <map>
#include <stdexcept>
#include "TCPMessengerProtocol.h"
#include "Dispatcher.h"

class Login : public MThread
{
	friend class TCPMessengerServer;
	friend class Dispatcher;
private:
	MultipleTCPSocketsListener *MTSL;
	Dispatcher * dispatcher;
	bool running;
	map<string, UserDetails *> usersMap;
	vector < UserDetails * > users;

public:
	Login();
	virtual ~Login();
	virtual void run();
	void addUser(TCPSocket * user);
};

#endif /* LOGIN_H_ */
