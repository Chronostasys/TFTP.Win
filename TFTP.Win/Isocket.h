#pragma warning(disable:4996)
#ifndef __ISOCKET__H
#define __ISOCKET__H


/* An interface class for supporting the functionalities of transport layer protocols. */

class Isocket
{
	
public:
		/*This method  creates the socket .*/
		virtual int Create( ){return 0;}

		/*This method binds the socket .*/
		virtual int Bind(int port){return 0;}

		/*This method listens for the connections .*/
		virtual int Listen(){return 0;}

		/*This method accepts the connections.*/
		virtual int Accept(){return 0;}

		/*This method connects the client to the server.
		*takes server ip address and port as argument.*/
		virtual int Connect(char* host,int port){return 0;}

		/*This method closes the socket.*/
		virtual int Close(){return 0;}

		/*This method sends the data . Takes packet to be sent as arguments.*/
		virtual int Send(char* packet){return 0;}

		/*This method receives the data . returns the received packet as given parameter*/
		virtual int Receive(char* packet){return 0;}
	
};
#endif
