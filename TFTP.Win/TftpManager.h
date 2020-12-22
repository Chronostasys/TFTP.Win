#ifndef __TFTPMANAGER__H
#define __TFTPMANAGER__H

/*including header files*/
#include <winsock2.h> 
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include<iostream>
#include<string.h>
#include<string>
#include <fstream>
#include<process.h>
#include <stdarg.h>
#include <sstream>
#include <time.h>
#include "CommandHandler.h"
#include "Isocket.h"

//Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

using namespace std;

/*This class implements the commands of TFTP.
Read,Write,Initialise,Help and Run.*/

class TftpManager:public CommandHandler
{

public:

  int Read(char* filename);
	int Write(char* filename);
	int Help();
	int Init(char* type,char* host,char* port,char* proto);
	int  Run();


protected:
	
	
		/*To handle a request read/write or wrong request*/
		DWORD  RequestHandler(void* socket);
		/*To handle a read request*/
		int	   ReadRequestHandler(Isocket* sock,char* file);
		/*to handle a write request*/
		int	   WriteRequestHandler(Isocket* sock,char* file);
		

		Isocket*	m_tcpServerSock;		//to create UDP socket for server		
		Isocket*	m_udpServerSock;		//to create TCP Socket for server
		Isocket*    m_clientSock;			//to create TCP/UDP socket for client
		
};
#endif
