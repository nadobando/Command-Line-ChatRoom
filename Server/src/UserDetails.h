
#ifndef USER_H_
#define USER_H_
#include "TCPSocket.h"
#include <iostream>
#include "TCPMessengerProtocol.h"

class UserDetails
{
private:
	string userName;
	string password;
	TCPSocket * userTcpSocket;
public:
	int status;
	UserDetails(string userName, string password, TCPSocket * userTcpSocket);
	virtual ~UserDetails();
	string getUserName();
	TCPSocket * getTcpSocket();
	void setTcpSocket(TCPSocket * sock = NULL);
	bool checkInfo(string userName, string password);
	bool checkUsers(UserDetails * user);

};

#endif /* USER_H_ */
