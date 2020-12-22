#pragma warning(disable:4996)
#ifndef __LOGGER__H
#define __LOGGER__H

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
#include<assert.h>

using namespace std;


/*Macro definition for logging*/
#define Error_Barrier(fun)\
  if(!fun)\
	{Logger::Instance().WriteError(msg<<":"<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<":");\
	assert(fun);}

/*Types of message to be written in log*/
#define INFO    "<Debug>:"
#define WARN   "<Warning>:"
#define ERR   "<Error>:"

/*singleton class for logging into the file */


class Logger
{
public:

	virtual ~Logger();						//distructor 
	static Logger& Instance();				//method to acces the private object of the Logger(single instance)
	void WriteLog(std::ostream& ss);		//for writing log info into the file using synchronisation
	void WriteError(std::ostream& ss);		//for writing errors info
	void WriteWarning(std::ostream& ss);	//for writing the warnings
	int GetSeqNo();							//for obtaining the sequence of the log information

private:

	HANDLE				m_Mutex;			//to use syncronisation in logging .
	char* GetTimeStamp();	//to get the timestamp of current execution
	ofstream			m_Stream;			//File stream will be used to write the log information
	int				m_seq;				//sequence of the log information

	Logger();								//private constructor



};

#endif
