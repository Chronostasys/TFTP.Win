#pragma warning(disable:4996)
#ifndef __TCPSOCKET__H
#define __TCPSOCKET__H
#include "Isocket.h"

/*including header files*/
#include <winsock2.h> 
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include<iostream>
using namespace std;

/*A class for handling TCP connections.*/

class TcpSocket:public Isocket
{  
public:
	int Create( );
	int Bind(int port);
	int Listen();
	int Accept();

	/*Stores the server address and connects the client to server*/
	int Connect(char* host,int port);

	int Send(char* buff, int len);
	int Send(char* buff);
	int Receive( char* packet);
	int Close();
	
	

private:
	
	SOCKET		m_Listen;								//Socket for listening the conection
	SOCKET		m_Client;								//socket for m_client and to accept the connection by m_server.
	struct		sockaddr_in m_server;					//Structure stores the address of m_server(ip,port,Socketetc).
						
	


};
#endif
