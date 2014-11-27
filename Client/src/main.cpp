/*
 * main.cpp
 *
 *  Created on: Feb 6, 2013
 *      Author: Eliav Menachi
 */

#ifndef MAIN_CPP_
#define MAIN_CPP_

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include "TCPMessengerClient.h"

using namespace std;
void pressEnter()
{
	std::cout << "Press [ Enter ] to continue...";
	cin.clear();
	cin.ignore();
	cin.get();
}
int exit(TCPMessengerClient * client)
{
	client->disconnect();
	delete client;
	cout<<"Bye Bye!!!"<<endl;
	exit(0);

}
void sessionEstablished(TCPMessengerClient * connectedClient)
{
	string selection,msg;
	if(connectedClient->inSession())
			{
				cout<<endl;
				cout << "1. Send Message: s <Message>>" << endl;
				cout << "2. Close Session: cs" << endl;

				while(selection!="cs")
				{
					cin >> selection;
					if(selection=="s")
					{
						getline(cin,msg);
						connectedClient->send(msg);
					}
					else if(selection=="cs")
					{
						if(connectedClient->clientStatus==IN_ROOM)
							connectedClient->closeRoom();
						else
							connectedClient->closeSession();

					}
				}
			}
}

void connectionEstablished(TCPMessengerClient * connectedClient)
{

	string selection;
	string username;
	string msg;
	string room;

	while(selection!="d")
	{
		cin.clear();
		cout.clear();
		cout<<endl<<endl;
		cout << "1. Private Chat: o <UserName>" << endl;
		cout << "2. Current Session: j" << endl;
		cout << "3. New Room: or <Room Name>" << endl;
		cout << "4. Enter Room: jr <Room Name>" << endl;
		cout << "5. List Users: lu" << endl;
		cout << "6. Online Users: lcu" << endl;
		cout << "7. Online Rooms: lr" << endl;
		cout << "8. Users in Room: lru <roomName>" << endl;
		cout << "9. Status: l" << endl;
		cout << "10. Logout: d" << endl;
		cout << "11. Exit: x" << endl;
		cin >> selection;


		if(selection == "o")//OPEN SESSION
		{

			if(connectedClient->inSession())
			{
				cout<<endl;
				cout << "Connection Exists, to connect to another session please close the existing one" << endl;
				sleep(3);
				continue;
			}

			cin >> username;
			connectedClient->openSession(username);
			sleep(1);
			sessionEstablished(connectedClient);

		}
		else if(selection == "j")//JOIN SESSION
		{
			sessionEstablished(connectedClient);
		}
		else if(selection == "or")//CREATE ROOM
		{

			if(connectedClient->inSession())
			{
				cout<<endl;
				cout << "Connection Exists, to connect to another session please close the existing one" << endl;
				sleep(3);
				continue;
			}
			cin >> room;
			// CREATE ROOM
			connectedClient->roomCommand(CREATE_ROOM,room);

			sleep(1);
			sessionEstablished(connectedClient);



		}
		else if(selection == "jr")//JOIN ROOM
		{

			if(connectedClient->inSession())
			{
				cout<<endl;
				cout << "Connection Exists, to connect to another session please close the existing one" << endl;
				sleep(3);
				continue;
			}



			cin >> room;
			connectedClient->roomCommand(OPEN_ROOM,room);
			sleep(1);
			sessionEstablished(connectedClient);

		}
		else if(selection == "lu")//LIST ALL USERS
		{

			if(connectedClient->inSession())
			{

				cout<<endl << "Please Close Active Session Before" << endl;

				continue;
			}
			cout<<"Users:"<<endl;
			connectedClient->list(GET_ALL_USERS);
			sleep(1);
			pressEnter();

		}
		else if(selection == "lcu")//LIST ONLINE USERS
		{
			if(connectedClient->inSession())
			{
				cout<<endl;
				cout << "Please Close Active Session Before" << endl;
				sleep(3);
				continue;
			}
			cout<<"On-line Users:"<<endl;
			connectedClient->list(ONLINE_USERS);
			sleep(1);
			pressEnter();


		}
		else if(selection == "lr")//LIST ROOMS
		{
			if(connectedClient->inSession())
			{
				cout<<endl;
				cout << "Please Close Active Session Before" << endl;
				sleep(3);
				continue;
			}
			cout<<"Rooms:"<<endl;
			connectedClient->list(EXISTING_ROOMS);
			sleep(1);
			pressEnter();

		}
		else if(selection == "lru")//LIST USERS IN ROOM
		{
			if(connectedClient->inSession())
			{
				cout<<endl;
				cout << "Please Close Active Session Before" << endl;
				sleep(3);
				continue;
			}

			cin >> room;
			cout<<"User In "<<room<<":"<<endl;
			connectedClient->list(USERS_IN_ROOM,room);
			sleep(1);
			pressEnter();

		}
		else if(selection == "l")//STATUS
		{
			if(connectedClient->inSession())
			{
				cout<<endl;
				cout << "Please Close Active Session Before" << endl;
				sleep(3);
				continue;
			}
			cout<<endl;
			switch (connectedClient->clientStatus) {
			case CONNECTED:
				cout << "Connected"<<endl;
				break;
			case IN_ROOM:
				cout << "In Room"<<endl;
				break;
			case IN_SESSION:
				cout << "In Session"<<endl;
				break;
				default:
					break;
			}

		}
		else if(selection=="x")
			exit(connectedClient);

	}
}

int main(){

	string selection,username,password,msg,ip;
	TCPMessengerClient* client = new TCPMessengerClient();

	cout<<"Welcome to Da Messenger"<<endl<<endl;

	while(selection!="x")
	{
		cout<<"1. Connect: c <ServerIp>"<<endl;
		cout<<"2. Exit: x"<<endl;
		
		cin >> selection;
		if(selection == "c")
		{
			//		CONNECTING CLIENT
			cin >> ip;
			client->connect(ip);
			usleep(10000);
			if(!client->isConnected())
			{
				cout << "Connection Failed... Try Again!!!" << endl;

				continue;
			}
			while(selection!="x")
			{
				cout<<endl<<"Registration Page"<<endl<<endl;
				cout << "1. Login: l <user name> <password>" << endl;
				cout << "2. Sign Up: s <user name> <password> "<< endl;
				cout<<"3. Exit: x"<<endl;
				cin >> selection;
				if(selection=="l")
				{
						cin >> username;
						cin >> password;
						client->login(LOGIN,username,password);
						sleep(2);
						if(client->isLogged())
							connectionEstablished(client);//connection  Established
						else
							cout << "Login Failed... Try Again!!!" << endl;



				}
				if(selection == "s")
				{
						cin >> username;
						cin >> password;
						client->login(SIGN_UP,username,password);
						sleep(2);
						if(client->isLogged())
							connectionEstablished(client);


				}
			}
		}
	}
	return exit(client);

}


#endif /* MAIN_CPP_ */
