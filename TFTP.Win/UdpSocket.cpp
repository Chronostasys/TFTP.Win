
#include "UdpSocket.h"
#include "Logger.h"
#include "Ipacket.h"
#include "TftpPacket.h"
#include "Const.h"
std::stringstream logger;


using namespace std;

int 
UdpSocket::Create()
{  
	/*Creates the socket for UDP connections.and assigns the port to the m_server port m_Port.
	 *param port-IN 
	 *return 0 for success else 1
	 */
	struct timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	m_Listen = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	setsockopt(m_Listen, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout,
		sizeof(timeout));
	setsockopt(m_Listen, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout,
		sizeof(timeout));
    if (m_Listen == SOCKET_ERROR)
    {
        Logger::Instance().WriteError(logger<<"Error creading UDP socket"<<WSAGetLastError());
        return 0;
    }
	Logger::Instance().WriteLog(logger<<"UDP Socket creaded");
	return 1;
}



int 
UdpSocket::Bind(int Port)
{	
	/*Binds the socket. Returns 0 for success 1 else*/
	/*store the values in "m_local" sockaddr_in structure object and bind it*/
	int n;
	struct		sockaddr_in m_local;
   ZeroMemory(&m_local,sizeof(m_local));
   m_local.sin_family = AF_INET;
   m_local.sin_addr.s_addr=htonl(INADDR_ANY);
   m_local.sin_port=htons(Port);

   n=bind(m_Listen, (struct sockaddr *)&m_local, sizeof(m_local)) ;
    if (n==SOCKET_ERROR)
    {
        Logger::Instance().WriteError(logger<<"Error binding UDP socket"<<WSAGetLastError());
        return 0;
    }
	Logger::Instance().WriteLog(logger<<"Socket Binded");
	return 1;
}



int 
UdpSocket::Connect(char *szServer,int Port)
{
	/*Stores the Address of Server, IP and Port and assignes it to sockaddr_in Structure object m_server
	 *returns 0.
	 */
	
	ZeroMemory(& m_server,sizeof( m_server));
    m_server.sin_family = AF_INET;
    m_server.sin_addr.s_addr=inet_addr(szServer);
    m_server.sin_port=htons(Port);
    return 1;
}



int 
UdpSocket::Close()
{
	/*Closes the Socket of m_server and m_server.*/
	closesocket(m_Listen);
	Logger::Instance().WriteLog(logger<<"Closing the Socket");
	return 1;
}



int 
UdpSocket::Send(char* buff, int len)
 {	
	 /*Sent the data to the m_server
	 *param buff-IN
	 */	
	  char buf[1024];
	  int n;
	  strcpy(buf,buff);
	  n=  sendto(m_Listen,buf,len,0,(struct sockaddr *)&m_server,sizeof(m_server));
	  if(n==0)
		return 0;
	  else
		return 1;
 }

int
UdpSocket::Send(char* buff)
{
	/*Sent the data to the m_server
	*param buff-IN
	*/
	char buf[1024];
	int n;
	strcpy(buf, buff);

	auto len = strlen(buf);
	n = sendto(m_Listen, buf, strlen(buf), 0, (struct sockaddr*)&m_server, sizeof(m_server));
	if (n == 0)
		return 0;
	else
		return 1;
}



int 
UdpSocket::Receive(char* buff)
 {	
	/*Receive the data sent by m_server
	 *param buff-OUT
	 *returns data received as buff.
	 */
	 socklen_t len;
	 len = sizeof(m_server);
	 int n=recvfrom(m_Listen,buff,TFTP_PACKET_MAX_SIZE,0,(struct sockaddr *)&m_server,&len);
	 //if (n == -1&&!isServer)
	 //{
		// printf("error code: %d, error msg: %s\n", 0, "failed to receive responce");
	 //}
	 return n;
 
}






