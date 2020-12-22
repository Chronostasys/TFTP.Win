#ifndef __UDPSOCKET__H
#define __UDPSOCKET__H

/*including header files*/
#include <winsock2.h> 
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include<iostream>

#include "Isocket.h"
using namespace std;


/*A class for creating UDP sockets.It inherits the Isocket "Interface class" publicly.*/

class UdpSocket:public Isocket
{
public:
  int Create( );
	int Bind(int port);

	/*Stores the ip address and port of m_server*/
	int Connect(char* host,int port);

	int Close();
	int Send(char* packet);
	int Receive(char*  packet);


private:

	SOCKET   m_Listen;							    //Socket for listening the conection
	struct	 sockaddr_in m_server;					//Structure stores the address of m_server(ip,port,Socket etc).
	
};
#endif
