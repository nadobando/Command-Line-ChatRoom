//============================================================================
// Name        : TCPMessengerServer
// Author      : Eliav Menachi
// Version     :
// Copyright   :
// Description : TCP messenger server console
//============================================================================

#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include "TCPMessengerServer.h"

using namespace std;


void printInstructions()
{
	cout<<endl<<"******************************************************************"<<endl;
	cout<<"1. Registered Users: lu"<<endl;
	cout<<"2. On-line Users: lcu"<<endl;
	cout<<"3. Sessions: ls"<<endl;
	cout<<"4. Rooms: lr"<<endl;
	cout<<"5. Users in Room: lru <roomName>"<<endl;
	cout<<"6. Exit: x"<<endl;
	cout<<"******************************************************************"<<endl;
}

int main(){
	string choice="stam",roomName;


	cout<<endl<<endl<<"Welcome to TCP messenger Server"<<endl;
	printInstructions();
	TCPMessengerServer  server;
	while(choice!="x")
	{

		cin >> choice;

		if(choice == "lu")
		{
			server.listCommand(GET_ALL_USERS);
		}
		else if(choice == "lcu")
		{
			server.listCommand(ONLINE_USERS);
		}
		else if(choice == "ls")
		{
			server.listCommand(ACTIVE_SESSIONS);
		}
		else if(choice == "lr")
		{
			server.listCommand(EXISTING_ROOMS);
		}
		else if(choice == "lru")
		{
			cin >> roomName;
			server.listCommand(USERS_IN_ROOM,roomName);
		}
		if(choice!="x")
		{
			std::cout << "Press [ Enter ] to continue...";
			cin.clear(); // Make sure the buffers are clear, you may want to do cin.flush() as well but I am not sure if both are needed.

			cin.ignore();
			cin.get();
			printInstructions();
		}




	}
	server.close();
	cout<<"Bye Bye!!!"<<endl<<endl;
	return 0;
}

