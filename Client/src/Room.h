

#ifndef UDPCHATROOMSESSION_H_
#define UDPCHATROOMSESSION_H_

#include <iostream>
#include <vector>
#include "UDPSocket.h"
#include "MThread.h"

class userInformation
{
private:
	string ip;
	int port;
	string userName;
public:
	const string& getIp() const {
		return ip;
	}

	void setIp(const string& ip) {
		this->ip = ip;
	}

	int getPort() const {
		return port;
	}

	void setPort(int port) {
		this->port = port;
	}

	const string& getUserName() const {
		return userName;
	}

	void setUserName(const string& userName) {
		this->userName = userName;
	}
};

class Room : public MThread
{
private:
	string clienName;
	vector<userInformation *> users;
	UDPSocket * socket;
	string roomName;
	bool running;

public:

	int clientPort;

	void addClient(userInformation * user);
	void removeClient(string ip);
	void send(string msg);
	void closeRoom();

	virtual void run();

	Room(string roomName,string clientName);
	virtual ~Room();
};

#endif /* Room_H_ */
