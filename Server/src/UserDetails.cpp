
#include "UserDetails.h"

UserDetails::UserDetails(string userName, string password, TCPSocket * userTcpSocket)
{
	this->userName = userName;
	this->password = password;
	this->userTcpSocket = userTcpSocket;
	this->status = CONNECTED;

}

TCPSocket * UserDetails::getTcpSocket()
{
	return this->userTcpSocket;
}

void UserDetails::setTcpSocket(TCPSocket * sock)
{
	this->userTcpSocket = sock;
}
string UserDetails::getUserName()
{
	return this->userName;
}

bool UserDetails::checkInfo(string userName, string password)
{
	if(this->userName.compare(userName) == 0 && this->password.compare(password) == 0)
		return true;
	return false;
}
bool UserDetails::checkUsers(UserDetails * user)
{
	if(this->userName.compare(user->getUserName()) == 0)
		return true;
	return false;
}

UserDetails::~UserDetails()
{
	
}

