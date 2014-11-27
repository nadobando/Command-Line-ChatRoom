//============================================================================
// Name        : TCP Messenger Server
// Author      : Eliav Menachi
// Version     :
// Copyright   :
// Description : TCP Messenger application
//============================================================================
#include <strings.h>
#include <map>
#include <vector>
#include "MThread.h"
#include "TCPSocket.h"
#include "MultipleTCPSocketsListener.h"
#include "TCPMessengerProtocol.h"
#include "Login.h"

using namespace std;

class TCPMessengerServer : public MThread
{
private:
	TCPSocket * listener;
	Login * login;
	bool running;
public:
	TCPMessengerServer();
	void run();
	void listCommand(int command, string roomName="");
	void close();
};

