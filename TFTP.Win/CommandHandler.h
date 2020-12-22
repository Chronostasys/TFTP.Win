#pragma warning(disable:4996)
#ifndef __COMMANDHANDLER__H
#define __COMMANDHANDLER__H

/*This class will handle the commands of the protocol.Each method identifies a command.*/

class CommandHandler
{

  	
public:

	/*This mathod performs the initialisation of Clients and the server.
	*first param- type(-c/-s)
	*second param- host
	*third param - port
	*fourth param- proto(-tcp/-udp)
	*/
	virtual int Init(char* type,char* host,char* port,char* proto)=0;

	/*this method will start the server for accepting the connections of clients.*/
	virtual int  Run()=0;
	
	/*All the three method bellow are for the client's use*/

	/*Downloading a file from server database .*/
	virtual int Read(char* filename)=0;

	/*Uploading a file to server database.*/
	virtual int Write(char* filename)=0;

	/* Help for the users.*/
	virtual int Help()=0;
};
#endif 
