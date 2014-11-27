

#ifndef UDPSESSION_H_
#define UDPSESSION_H_

#include "MThread.h"
#include <iostream>
#include "UDPSocket.h"


class Session : public MThread
{
private:
	UDPSocket * socket;
	string partnerIp,partnerName,clientName;
	int partnerPort;
	bool running;
public:
	int port;
	Session(string partnerIP, string partnerName, string clientName);
	virtual ~Session();
	virtual void run(); // Listener
	void closeSession();
	void send(string msg);
	void setPartnerPort(int port);


};

#endif /* UDPSESSION_H_ */
