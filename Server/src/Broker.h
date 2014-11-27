

#ifndef BROKER_H_
#define BROKER_H_
#include "Dispatcher.h"

class Broker : public MThread
{
private:
	MultipleTCPSocketsListener MTSL;
	Dispatcher * dispatcher;
	pair<UserDetails*,UserDetails*> users;
	bool running;
public:
	Broker(Dispatcher * dispatcher, UserDetails * firstClient, UserDetails * secClient);
	void close(TCPSocket * closingClient);
	virtual void run();
	virtual ~Broker();
};

#endif /* BROKER_H_ */
