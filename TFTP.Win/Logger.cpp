
#include"Logger.h"

/*initialise the instance of Logger with NULL*/


Logger& Logger::Instance () 
{
  /*Method returns the instance of Logger. if it does not exist it allocates 
	 *the memory to one instance and returns it.
	 */
	 static Logger instance;
	 return instance;
}



Logger::Logger ()
{
	/*Creates the  file in opens it for writing the logs
	 *File name is generated by using the current timestamp for 
	 *generating saperate log files each time.
	 *creates the mutex that will be used for synchonisation
	 */

	char file[30];
	 char*           tString = new char [80];
    time_t          t       = time (0);
    struct  tm*     today   = localtime (&t);
	strftime (file,30,"log__%d-%m-%Y_%H-%M-%S.txt",today);			//filename

	m_Mutex = CreateMutex( NULL,FALSE,NULL);				//create mutex

	m_seq=0;													//initialise the m_seq.

    m_Stream.open (file, fstream::app);						//open the file
}


Logger::~Logger ()
{	
	/*Close the log file and close the thread and mutex object,
	 *deallocate the instance of the Logger*/

    m_Stream.close ();
	CloseHandle(m_Mutex);

}



int  
Logger::GetSeqNo()
{	
	/*increase the current sequence and return it*/
	m_seq+=1;
	return m_seq;
}



void 
Logger::WriteLog (std::ostream& ss)
{
	/*Method locks the mutex and writes the Debug logs(in form of ioStream) into the log file
	 *and the releases the mutex
	 *param ss-IN
	 */
	WaitForSingleObject(m_Mutex, INFINITE);
	m_Stream<<"<LOG>"<< GetSeqNo()<<" : "<< GetTimeStamp()<<" " <<INFO << ss.rdbuf() << endl;
	ReleaseMutex(m_Mutex);
   
}



void 
Logger::WriteError (std::ostream& ss)
{
	/*Method locks the mutex and writes the Error logs(in form of ioStream) into the log file
	 *and the releases the mutex
	 *param ss-IN
	 */
  WaitForSingleObject(m_Mutex, INFINITE);
	m_Stream <<"<LOG>"<< GetSeqNo()<<" : "<< GetTimeStamp()<<" " << WARN << ss.rdbuf() << endl;
   ReleaseMutex(m_Mutex);
}



void 
Logger::WriteWarning(std::ostream& ss)
{
	/*Method locks the mutex and writes the Warning logs(in form of ioStream) into the log file
	 *and the releases the mutex
	 *param ss-IN
	 */
	WaitForSingleObject(m_Mutex, INFINITE);
	m_Stream<<"<LOG>" << GetSeqNo()<<" : "<< GetTimeStamp()<<" " <<ERROR << ss.rdbuf() << endl;
    ReleaseMutex(m_Mutex);

}



char*  
Logger::GetTimeStamp ()
{
	/*Returns the Current time as a string*/

    char*           tString = new char [80];
    time_t          t       = time (0);
    struct  tm*     today   = localtime (&t);

    strftime  (tString, 80,"%d/%m/%Y  %H:%M:%S",today);
    return tString;
}
