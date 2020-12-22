
#ifndef __THREAD_H__
#define __THREAD_H__

#define WIN32_LEAN_AND_MEAN

/*including header files*/
#include <winsock2.h> 
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include<iostream>
#include<string.h>
#include<string>

#include<process.h>

using namespace std;
template<class T>
class ThreadHandler
{
    typedef DWORD (T::* Method)(void*);  	//prototype of m_method to be passed while creating a ThreadHandler Object

public:

    explicit ThreadHandler(T* m_object, DWORD ( T::* m_method)(void*),void* m_arg)
    {
	  /*Creates a new Thread m_object. m_object: the one which m_method should be
		executed. m_method: pointer to the m_object's m_method. */

		m_Mutex = CreateMutex( NULL,FALSE,NULL);   //Create mutex m_object for synchronisation 

	  /*initialise the private members of Threadhandler*/
        this->m_Thread       = NULL;
        this->m_object        = m_object;
        this->m_method        = m_method;
        this->m_threadID      = 0;
		this->m_arg			=(T*)m_arg;
	 }

	
	~ThreadHandler(void)
    {
		if (m_Thread)
            CloseHandle(m_Thread);				//Close Current thread
		CloseHandle(m_Mutex);					//close the mutex m_object
    }

    
    inline bool start()
    {
	  /*Starts executing the objects m_method in a concurrent thread. True if the
		thread was started successfully; otherwise false. */
	  //lock it by mutex then create create the thread

		WaitForSingleObject(m_Mutex, INFINITE);

		m_Thread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadHandler<T>::run,this,0,&this->m_threadID);

        if (m_Thread)
            return true;
        return false;
        
    }

    
    inline void join()
    {
	  //Blocks the calling thread until this thread has stopped.
        WaitForSingleObject(m_Thread, INFINITE);				//wait for thread to execute
		ReleaseMutex(m_Mutex);								//release the mutex
    }

protected:
    HANDLE  m_Thread;      // unique handle to the thread
private:
	HANDLE  m_Mutex;		  //mutex m_object For synchronisation between threads
    DWORD   m_threadID;     // thread id - 0 until started
    T*      m_object;       // the m_object which owns the m_method
    Method  m_method;       // the m_method of the m_object
	T*		m_arg;
   
    static DWORD run(LPVOID thread_obj)
    {
        ThreadHandler<T>* thread = (ThreadHandler<T>*)thread_obj;
        return (thread->m_object->*thread->m_method) (thread->m_arg);
    }

};

#endif 
