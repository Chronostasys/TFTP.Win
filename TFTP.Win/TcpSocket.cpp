
#include "TcpSocket.h"
#include "Logger.h"
#include "Ipacket.h"
#include "TftpPacket.h"
std::stringstream logS;


int 
TcpSocket::Create()
{
  /*Creates the socket for TCP connections..
	 *param port-IN 
	 *return 0 for success else 1
	 */
	
	m_Listen = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (m_Listen == SOCKET_ERROR)
    {
        Logger::Instance().WriteError(logS<<"Error creading TCP socket"<<WSAGetLastError());
        return 0;
    }
	Logger::Instance().WriteLog(logS<<"TCP Socket creaded");
	return 1;
}


int 
TcpSocket::Bind(int Port)
{
	/*Binds the socket. Returns 0 for success 1 else*/
	/*store the values in "m_local" sockaddr_in structure object and bind it*/
	struct		sockaddr_in m_local;
	
	m_local.sin_addr.s_addr = htonl(INADDR_ANY);
    m_local.sin_family = AF_INET;
    m_local.sin_port = htons(Port);

    if (bind(m_Listen, (struct sockaddr *)&m_local,
            sizeof(m_local)) == SOCKET_ERROR)
    {
        Logger::Instance().WriteError(logS<<"Error binding TCP socket"<<WSAGetLastError());
        return 0;
    }
	Logger::Instance().WriteLog(logS<<"Socket Binded");
	return 1;
}


int 
TcpSocket::Listen()
{	
	/*Listens to sockets for accepting the connection with max value 10
	 *returns 0 for success and 1 elsewise.
	 */
	
	m_Client=m_Listen;
	int iResult = listen(m_Client, 10);
    if (iResult == SOCKET_ERROR) {
        Logger::Instance().WriteError(logS<<"listen faild"<<WSAGetLastError());
       
        return 0;
    }
	Logger::Instance().WriteLog(logS<<"Listening to connection");
	return 1;

}
int 
TcpSocket::Accept()
{
	/*accepts the Connection and assigns the value to a differnt Socket m_Client
	 *returns 0 for success else 1.
	 */
	
	int iAddrSize = sizeof(m_server);
        m_Listen = accept(m_Client, (struct sockaddr *)&m_server,
                        &iAddrSize);
		 if (m_Listen== INVALID_SOCKET)
        {
		Logger::Instance().WriteError(logS<<"Accept failed:"<<WSAGetLastError());
			return 0;
        }
		 Logger::Instance().WriteLog(logS<<"New Connection Accepted:"<<m_Listen);
		 return 1;
}


int 
TcpSocket::Connect(char *szServer,int Port)
{	
	/*method first uses gethostbyname which returns a structure of type hostent for the given host name. 
	 *Here name is, an IPv4 address in standard dot notation.
	 *Copies the address into m_server structure and the connects to the Server.
	 *param szServer-IN, Ip address in dotted notation.
	 *returns 0 for success and 1 of failure.*/

	
   ZeroMemory(&m_server,sizeof( m_server));
   m_server.sin_family = AF_INET;
   m_server.sin_addr.s_addr=inet_addr(szServer);
   m_server.sin_port=htons(Port);
   
	struct hostent *host;
	
    if (m_server.sin_addr.s_addr == INADDR_NONE)
    {
        host = gethostbyname(szServer);
        if (host == NULL)
        {
            Logger::Instance().WriteError(logS<<"unable to resolve server");
            return 0;
        }
        CopyMemory(&m_server.sin_addr, host->h_addr_list[0],
            host->h_length);
    }
    if (connect(m_Listen, (struct sockaddr *)&m_server,
        sizeof(m_server)) == SOCKET_ERROR)
    {
	Logger::Instance().WriteError(logS<<"Error Connect failed:"<<WSAGetLastError());
        return 0;
    }
	Logger::Instance().WriteLog(logS<<"Connected to server");
	return 1;
}


int 
TcpSocket::Close()
{
	/*Closes the Socket of m_client.*/

	closesocket( m_Listen);
	Logger::Instance().WriteLog(logS<<"Closing the Socket");
	return 1;
}




int 
TcpSocket::Send(char* buff, int len)
 {	
	/*Sent the data to the m_server
	 *param buff-IN
	 */	
	 char buf[1024];
	 strcpy(buf,buff);
	 int n=send( m_Listen,buf, len, 0);
	 return 1;
 }
int
TcpSocket::Send(char* buff)
{
	/*Sent the data to the m_server
	 *param buff-IN
	 */
	char buf[1024];
	strcpy(buf, buff);
	int n = send(m_Listen, buf, strlen(buf), 0);
	return 1;
}

int
TcpSocket::Receive(char* buff)
 {	
	 /*Receive the data sent by m_server
	 *param buff-OUT
	 *returns data received as buff.
	 */
	 int n=recv( m_Listen, buff, TFTP_PACKET_MAX_SIZE, 0);

	return 1;
 
}



