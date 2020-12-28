#include "ThreadHandler.h"
#include"TftpManager.h"
#include "Logger.h"
#include "Ipacket.h"
#include "TftpPacket.h"
#include "Isocket.h"
#include "TcpSocket.h"
#include"UdpSocket.h"
#include <vector>
#include "Const.h"




int
TftpManager::Init(char* type, char* host, char* port, char* ip_type)
{
	//Initialize Winsock  
	std::stringstream msg;
	WSADATA wsdData;
	if (WSAStartup(MAKEWORD(2, 2), &wsdData) != 0)
	{
		Logger::Instance().WriteError(msg << "Failed to load Winsock!\n");
		return 1;
	}
	/*if server*/
	if (!strcmp(type, "-s"))
	{
		m_tcpServerSock = new TcpSocket;
		m_udpServerSock = new UdpSocket;

		Error_Barrier(m_tcpServerSock->Create());
		Error_Barrier(m_tcpServerSock->Bind(atoi(port)));

		Error_Barrier(m_udpServerSock->Create());
		Error_Barrier(m_udpServerSock->Bind(atoi(port)));
		m_tcpServerSock->Listen();
		return 1;
	}
	/*if client with TCP connection*/
	if ((!strcmp(type, "-c")) && (!strcmp(ip_type, "tcp")))
	{
		m_clientSock = new TcpSocket;

		Error_Barrier(m_clientSock->Create());
		Error_Barrier(m_clientSock->Connect(host, atoi(port)));
		return 1;
	}
	/*If client with UDP connection*/
	if ((!strcmp(type, "-c")) && (!strcmp(ip_type, "udp")))
	{
		m_clientSock = new UdpSocket ;

		Error_Barrier(m_clientSock->Create());
		Error_Barrier(m_clientSock->Connect(host, atoi(port)));
		return 1;
	}
	else
		return 0;

}



int
TftpManager::ReadRequestHandler(Isocket* sock, char* file)
{
	start:
	char buf[TFTP_PACKET_MAX_SIZE];
	char packet[TFTP_PACKET_MAX_SIZE];
	int opcode;
	int blockno;
	int file_pointer;
	char c;
	ifstream infile;
	opcode = 3;
	blockno = 1;
	file_pointer = 0;
	std::stringstream msg;
	Ipacket* Rpacket;
	Rpacket = new TftpPacket;



	infile.open(file, ios::binary);
	
	auto var = infile.good();
	if (!var)
	{
		Rpacket->EncodePacket(5, "1 FILE_NOT_FOUND", packet);
		sock->Send(packet);
	}
	infile.seekg(0, ios::beg);
	int i = 0;
	while (!infile.eof() && var)
	{
		c = infile.get();
		if (c != -1)
		{
			buf[file_pointer++] = c;
			i++;
		}
		if (infile.eof())
		{
			printf("eof\n");
		}
		if (file_pointer >= 511|| infile.eof())
		{
			/*Send 512 bytes of data except incase of EOF send less then 512bytes*/
			buf[file_pointer] = '\0';
			Rpacket->EncodePacket(opcode, buf, packet);
			Error_Barrier(sock->Send(packet));

			/*Receive  packet(ack expected)*/

			auto re = sock->Receive(buf);
			auto ackNum = -1;
			if (re!=-1)
			{
				ackNum = Rpacket->DecodePacket(buf);
			}
			while (re==-1||ackNum!=Rpacket->m_blockno)
			{
				Error_Barrier(sock->Send(packet));
				re = sock->Receive(buf);
				if (re != -1)
				{
					ackNum = Rpacket->DecodePacket(buf);
				}
			}
			
			if (Rpacket->GetopCode()==PACKET_READ)
			{
				goto start;
			}

			/*If not ack send error packet and close the connection*/
			if (Rpacket->GetopCode() != PACKET_ACK)
			{
				printf("%s", buf);
				//sprintf(buf, "%d wrongpacket %s", ERROR_WRONG_PACKET);
				Rpacket->EncodePacket(5, buf, packet);
				Error_Barrier(sock->Send(packet));
				break;
			}
			file_pointer = 0;
		}
	}
	infile.close();
	free(Rpacket);
	return 1;
}


int
TftpManager::WriteRequestHandler(Isocket* sock, char* file)
{
writeStart:
	printf("start write\n");
	char packet[TFTP_PACKET_MAX_SIZE];
	char buf[TFTP_PACKET_MAX_SIZE];
	int opcode;
	fstream outfile;
	int file_pointer;
	int size;
	file_pointer = 0;
	opcode = PACKET_ACK;
	Ipacket* m_packet;
	m_packet = new TftpPacket;
	std::stringstream msg;
	/*Encode and send request*/
	if (octet)
	{
		outfile.open(file, std::fstream::out | std::fstream::binary);

	}
	else
	{
		outfile.open(file, std::fstream::out);
	}
	//m_packet->EncodePacket(opcode, buf, packet);
	//sock->Send(packet);
		/*Send ACK */
	m_packet->EncodePacket(opcode, buf, packet);
	while (sock->Send(packet) == -1);
	opcode = PACKET_READ;
	int sliceCount = 0;



	while (true)
	{
	receive:
		/*Receive  packet */
		ZeroMemory(&buf, TFTP_PACKET_MAX_SIZE);
		size = sock->Receive(buf);
		m_packet->DecodePacket(buf);
		opcode = PACKET_ACK;
		auto op = m_packet->GetopCode();
		while (size == -1||op== PACKET_WRITE)
		{
			sock->Send(packet);
			size = sock->Receive(buf);
			m_packet->DecodePacket(buf);
			opcode = PACKET_ACK;
			op = m_packet->GetopCode();
		}

		auto len = 0;
		/*if data packet write into file*/
		if (op == PACKET_DATA)
		{
			if (++sliceCount != m_packet->m_blockno)
			{
				sliceCount--;
				m_packet->EncodePacket(opcode, "", packet);
				goto receive;
			}
			outfile.seekg(file_pointer, ios_base::beg);
			len = strlen(m_packet->m_data);
			outfile.write(m_packet->m_data, len);
			file_pointer += len;
			//size = strlen(&buf[5]);

			m_packet->EncodePacket(opcode, buf, packet);
			sock->Send(packet);

		}
		else
		{
			int errCode = 0;
			sscanf(&buf[3], "%d", &errCode);
			if (errCode == 2)
			{
				printf("rewrite\n");
				goto writeStart;
			}
			else
			{
				/*If not a data packet send error packet*/
				printf("Error! %s\n", buf);
				sprintf(buf, "%d wrongpacket ", ERROR_WRONG_PACKET);
				m_packet->EncodePacket(5, buf, packet);
				sock->Send(packet);
				outfile.close();
				remove(file);
				break;
			}
		}
		/*if last data packet then close*/
		if (len < 511)
		{
			printf("write end\n");
			break;
		}
			
	}
	outfile.close();
	free(m_packet);
	return 0;
}



DWORD
TftpManager::RequestHandler(void* socket)
{
	Isocket* sock = (Isocket*)socket;
	std::stringstream msg;
	while (1)
	{
		/*Accept the Connection from client*/
		sock->Accept();

		char buf[TFTP_PACKET_MAX_SIZE];
		char packet[TFTP_PACKET_MAX_SIZE];
		char file[FILE_NAME_MAX_SIZE];
		Ipacket* m_packet;
		m_packet = new TftpPacket;

		/*Receive the request*/
		ZeroMemory(&buf, sizeof(buf));
		Error_Barrier(sock->Receive(buf));
		m_packet->DecodePacket(buf);

		/*If read request call readrequesthandler*/
		if (m_packet->GetopCode() == PACKET_READ)
		{
			sscanf(&buf[2], "%s", file);
			Error_Barrier(ReadRequestHandler(sock, file));

		}

		/*If write request call wrirerequesthandler*/
		if (m_packet->GetopCode() == PACKET_WRITE)
		{
			char mode[10];
			printf("wrq handle\n");
			sscanf(&buf[2], "%s %s", file, mode);
			printf("%s\n", mode);
			if (!strcmp(mode, "octet"))
			{
				octet = true;
			}
			WriteRequestHandler(sock, file);

		}

		/*Send Wrong request received packet*/
		else
		{

			sprintf(buf, "%d wrongrequest ", ERROR_WRONG_REQUEST);
			m_packet->EncodePacket(5, buf, packet);
			Error_Barrier(sock->Send(packet));
		}
		free(m_packet);

	}
	sock->Close();

	return 0;
}




int
TftpManager::Run()
{
	/*this method Creates thread for UDP server and TCP server to RUN.*/



	std::stringstream msg;

	/*Initialise the threads*/
	ThreadHandler<TftpManager> thread(this, &TftpManager::RequestHandler, (void*)m_tcpServerSock);
	ThreadHandler<TftpManager> thread2(this, &TftpManager::RequestHandler, (void*)m_udpServerSock);

	/*Start the threads for TCP server*/
	Error_Barrier(thread.start());

	/*Start the threads for UDP server*/
	Error_Barrier(thread2.start());

	/*Wait for both threads to finish*/
	thread.join();
	thread2.join();

	return 0;
}

string Join(std::vector<std::string> x,
	const char* delim) {
	std::string y;

	switch (x.size())
	{
	case 0: y = "";   break;
	case 1: y = x[0]; break;
	default:
		std::ostringstream os;
		std::copy(x.begin(), x.end() - 1,
			std::ostream_iterator<std::string>(os, delim));
		os << *x.rbegin();
		y = os.str();
	}
	return y;
}
vector<string> split(const string& s, const string& seperator) {
	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;

	while (i != s.size()) {
		//找到字符串中首个不等于分隔符的字母；
		int flag = 0;
		while (i != s.size() && flag == 0) {
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x]) {
					++i;
					flag = 0;
					break;
				}
		}

		//找到又一个分隔符，将两个分隔符之间的字符串取出；
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0) {
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x]) {
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j) {
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}



int
TftpManager::Read(char* file)
{
	readStart:
	char packet[TFTP_PACKET_MAX_SIZE];
	char buf[TFTP_PACKET_MAX_SIZE];
	int opcode;
	fstream outfile;
	int file_pointer;
	int size;
	file_pointer = 0;
	opcode = PACKET_READ;
	Ipacket* m_packet;
	m_packet = new TftpPacket;
	std::stringstream msg;
	/*Encode and send request*/
	if (octet)
	{
		sprintf(buf, "%s octet", file);
		outfile.open(file, std::fstream::out | std::fstream::binary);

	}
	else
	{
		sprintf(buf, "%s netascii", file);
		outfile.open(file, std::fstream::out);
	}
	m_packet->EncodePacket(opcode, buf, packet);
	m_clientSock->Send(packet);
	int sliceCount = 0;

	

	while (true)
	{
		receive:
		/*Receive  packet */
		ZeroMemory(&buf, TFTP_PACKET_MAX_SIZE);
		size = m_clientSock->Receive(buf);
		while (size==-1)
		{
			m_clientSock->Send(packet);
			size = m_clientSock->Receive(buf);
		}
		
		m_packet->DecodePacket(buf);
		opcode = PACKET_ACK;
		auto op = m_packet->GetopCode();
		auto len = 0;
		/*if data packet write into file*/
		if (op == PACKET_DATA)
		{
			if (++sliceCount!=m_packet->m_blockno)
			{
				sliceCount--;
				m_packet->EncodePacket(opcode, "", packet);
				goto receive;
			}
			outfile.seekg(file_pointer, ios_base::beg);
			len = strlen(m_packet->m_data);
			outfile.write(m_packet->m_data, len);
			file_pointer += len;
			//size = strlen(&buf[5]);

			m_packet->EncodePacket(opcode, buf, packet);
			m_clientSock->Send(packet);

		}
		else
		{
			int errCode = 0;
			sscanf(&buf[3], "%d", &errCode);
			if (errCode == 2)
			{
				goto readStart;
			}
			else
			{
				/*If not a data packet send error packet*/
				printf("Error! %s\n", buf);
				sprintf(buf, "%d wrongpacket ", ERROR_WRONG_PACKET);
				m_packet->EncodePacket(5, buf, packet);
				m_clientSock->Send(packet);
				outfile.close();
				remove(file);
				break;
			}
		}
		/*if last data packet then close*/
		if (len < 511)
			break;
	}
	Error_Barrier(m_clientSock->Close());
	return 0;
}




int TftpManager::Write(char* file)
{
//start:
	char buf[TFTP_PACKET_MAX_SIZE];
	char packet[TFTP_PACKET_MAX_SIZE];
	int opcode;
	int blockno;
	int file_pointer;
	char c;
	ifstream infile;
	opcode = PACKET_WRITE;
	blockno = 1;
	file_pointer = 0;
	std::stringstream msg;
	Ipacket* Rpacket;
	Rpacket = new TftpPacket;


	/*Encode request and send*/
	sprintf(buf, "%s %s", file, octet?"octet":"netascii");
	Rpacket->EncodePacket(opcode, buf, packet);
	m_clientSock->Send(packet);

	/*Receive ACK*/

	int a = m_clientSock->Receive(buf);
	while (a==-1)
	{
		m_clientSock->Send(packet);

		/*Receive ACK*/

		a = m_clientSock->Receive(buf);
	}
	Rpacket->DecodePacket(buf);



	infile.open(file, ios::binary);

	infile.seekg(0, ios::beg);
	int i = 0;
	opcode = PACKET_DATA;
	while (!infile.eof())
	{
		c = infile.get();
		if (c != -1)
		{
			buf[file_pointer++] = c;
			i++;
		}
		if (infile.eof())
		{
			printf("eof\n");
		}
		if (file_pointer >= 511 || infile.eof())
		{
			/*Send 512 bytes of data except incase of EOF send less then 512bytes*/
			buf[file_pointer] = '\0';
			Rpacket->EncodePacket(opcode, buf, packet);
			Error_Barrier(m_clientSock->Send(packet));

			/*Receive  packet(ack expected)*/

			auto re = m_clientSock->Receive(buf);
			auto ackNum = -1;
			if (re != -1)
			{
				ackNum = Rpacket->DecodePacket(buf);
			}
			while (re == -1 || ackNum != Rpacket->m_blockno)
			{
				Error_Barrier(m_clientSock->Send(packet));
				re = m_clientSock->Receive(buf);
				if (re != -1)
				{
					ackNum = Rpacket->DecodePacket(buf);
				}
			}

			//if (Rpacket->GetopCode() == PACKET_READ)
			//{
			//	goto start;
			//}

			/*If not ack send error packet and close the connection*/
			if (Rpacket->GetopCode() != PACKET_ACK)
			{
				printf("%s", buf);
				//sprintf(buf, "%d wrongpacket %s", ERROR_WRONG_PACKET);
				Rpacket->EncodePacket(5, buf, packet);
				Error_Barrier(m_clientSock->Send(packet));
				break;
			}
			file_pointer = 0;
		}
	}
	infile.close();
	free(Rpacket);
	return 1;
}



int
TftpManager::Help()
{
	/*This method prints the Formate of the Commandline arguments for The server and clients */

	printf("For client :tftp -c  host (server_IP) port (Server_port) -ip (TCP/UDP) -read/-write (filename)\n ");
	printf("For server :tftp -s port (server_port)\n");
	printf("For Help   :-h\n");

	return 1;
}
