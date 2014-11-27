/*
 * CLIENT PROTOCOL
 * TCPMessengerProtocol.h
 *
 *  Created on: Feb 13, 2013
 *      Author: efi
 */

#ifndef TCPMESSENGERPROTOCOL_H_
#define TCPMESSENGERPROTOCOL_H_

/**
 * TCP Messenger protocol:
 * all messages are of the format [Command 4 byte int]
 * and optionally data follows in the format [Data length 4 byte int][ Data ]
 */

#define MSNGR_PORT 55555

#define EXIT						0
#define SERVER_DIED					1

#define CLOSE_SESSION_WITH_PEER 	100
#define OPEN_SESSION_WITH_PEER 		101

#define SESSION_REQUEST				200
#define SESSION_ACCEPTED 		    201
#define SESSION_REFUSED				202
#define SESSION_ESTABLISHED			203

#define CREATE_ROOM					300
#define OPEN_ROOM					301
#define EXIT_ROOM					302
#define ROOM_CREATED			    303
#define ROOM_REFUSED				304
#define ROOM_OUT					305
#define ROOM_CLOSED					306
#define ROOM_IN						307

#define LIST_USERS					400
#define LIST_CONNECTED_USERS		401
#define LIST_ROOMS					402
#define LIST_USERS_IN_ROOM			403

#define IN_SESSION 					500
#define IN_ROOM 					501
#define CONNECTED					502
#define DISCONNECTED 				503
#define LOGGED_IN					504
#define LISTENING		 			505
#define CLIENT_CONNECT	 			506

#define LOGIN_SUCCESS 				600
#define LOGIN_FAILED 				601
#define LOGIN						602
#define SIGN_UP					    603
#define SIGN_UP_SUCESS				604
#define SIGN_UP_FAILED				605

#define GET_ALL_USERS				700
#define ONLINE_USERS			    701
#define EXISTING_ROOMS				702
#define USERS_IN_ROOM		        703
#define LIST_REPLY					704
#define ROOM_NOT_FOUND				705
#define ACTIVE_SESSIONS				706





#endif /* TCPMESSENGERPROTOCOL_H_ */
