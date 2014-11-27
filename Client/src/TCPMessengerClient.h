//============================================================================
// Name        : TCP Messenger Server
// Author      : Eliav Menachi
// Version     :
// Copyright   :
// Description : TCP Messenger application
//============================================================================
#include <strings.h>
#include <sstream>
#include "MThread.h"
#include "TCPSocket.h"
#include "UDPSocket.h"
#include "TCPMessengerProtocol.h"
#include "Session.h"
#include "Room.h"
#define SIZE 512

using namespace std;

/**
 * The TCP Messenger client class
 */
class TCPMessengerClient : public MThread
{
private:
	TCPSocket * server;
	Session * clientSession;
	Room * clientChatRoom;
	string userName,roomName,partnerUserName;
	bool running;

public:
	TCPMessengerClient();

	int clientStatus;

	bool isConnected();
	bool isLogged();
	bool inSession();


	void disconnect();
	void openSession(string username);
	void closeSession();
	void send(string msg);
	void roomCommand(int command, string roomName);
	void connect(string ip);
	void closeRoom();
	void login(int command,string username, string password);
	void list(int command, string roomName = "");
	virtual void run();

};

