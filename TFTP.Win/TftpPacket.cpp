#include "TftpPacket.h"
#include "Logger.h"
#include "Const.h"
std::stringstream msg;



int
TftpPacket::EncodePacket(int opcode, const char* argv, char* buf)
{
	/*Takes the string and parse it assigns the value to deffernet data members
	   *of TftpPacket class and the depending on type it incodes the packet.
	   *Param argv- IN(string to be encoded into tftp packet)
	   *param buf-  OUT(string in form of encoded packet)
	   *returns encoded packet.
	   */


	   //get logger instance for logging





	   //if Write request packet
	if (opcode == 2)
	{


		//assign values to data members
		sscanf(argv, "%s", m_filename);
		CreateWrq();
		strcpy(buf, m_packet);
		Logger::Instance().WriteLog(msg << "Write Request Packet" << buf);
		return 1;
	}

	//if read request packet
	if (opcode == 1)
	{

		sscanf(argv, "%s", m_filename);
		CreateRrq();
		strcpy(buf, m_packet);
		Logger::Instance().WriteLog(msg << "Read Request Packet" << buf);
		return 1;
	}

	//to encode ack packet
	if (opcode == 4)
	{
		CreateAck();
		strcpy(buf, m_packet);
		Logger::Instance().WriteLog(msg << "Acknowledgement Packet" << buf);
		return 1;
	}

	//to encode error packet
	if (opcode == 5)
	{

		sscanf(argv, "%d %s", &m_errorCode, m_errorMsg);
		CreateError();
		strcpy(buf, m_packet);

		return 1;

	}

	//to encode data packet
	if (opcode == 3)
	{


		strcpy(m_data, argv);
		CreateData();
		strcpy(buf, m_packet);
		Logger::Instance().WriteLog(msg << "Data Packet with block no:" << m_blockno);
		return 1;
	}
	else
		return 0;
}

int
TftpPacket::GetopCode()
{
	return m_opCode;
}


int
TftpPacket::CreateWrq()
{
	//creates the Write request packet
	char wrq[200];

	strcpy(wrq, TFTP_OPCODE_WRITE);
	strcat(wrq, m_filename);
	strcat(wrq, " ");
	strcat(wrq, TFTP_DEFAULT_TRANSFER_MODE);
	strcat(wrq, " ");
	strcpy(m_packet, wrq);

	return 0;
}



int
TftpPacket::CreateRrq()
{
	/*For creating  request to read a file .*/
	char rrq[800];

	strcpy(rrq, TFTP_OPCODE_READ);
	strcat(rrq, m_filename);
	strcat(rrq, " ");
	strcat(rrq, TFTP_DEFAULT_TRANSFER_MODE);
	strcat(rrq, " ");
	strcpy(m_packet, rrq);

	return 0;
}



int
TftpPacket::CreateData()
{
	/*For creating a data packet that will contain 512 bytes of data.*/

	char packet[TFTP_PACKET_MAX_SIZE], buffer[8];
	m_blockno++;
	strcpy(packet, TFTP_OPCODE_DATA);
	sprintf(buffer, " %d ", m_blockno);
	strcat(packet, buffer);
	strcat(packet, m_data);
	strcpy(m_packet, packet);

	return 0;
}



int
TftpPacket::CreateAck()
{
	/*For creating the aknowledgement responce packet*/
	char ack[10];
	char buffer[8];
	strcpy(ack, TFTP_OPCODE_ACK);
	sprintf(buffer, " %d ", m_blockno);
	strcat(ack, buffer);
	strcpy(m_packet, ack);
	return 0;
}



int
TftpPacket::CreateError()
{
	/*For creating a error packet.*/
	char error[100], buffer[8];
	strcpy(error, TFTP_OPCODE_ERROR);

	sprintf(buffer, " %d ", m_errorCode);
	strcat(error, buffer);
	strcat(error, m_errorMsg);
	strcat(error, " ");
	strcpy(m_packet, error);
	return 0;
}



int
TftpPacket::DecodePacket(char* buf)
{
	/*it decodes the packet received and assigns the values to data members
	 *Param buf-IN(string in form of tftp packet)
	 */




	if (IsRRQ(buf))
	{
		sscanf(buf, "%d %s %s", &m_opCode, m_filename, m_mode);
	}

	if (IsWRQ(buf))
	{
		sscanf(buf, "%d %s %s", &m_opCode, m_filename, m_mode);

	}

	if (IsACK(buf))
	{
		m_opCode = 4;
		int temp = 0;
		sscanf(&buf[3], "%d", &temp);
		return temp;
		Logger::Instance().WriteLog(msg << "ACK with block no:" << temp);
	}

	if (IsData(buf))
	{
		m_opCode = 3;
		sscanf(&buf[3], "%d %s", &m_blockno, m_data);
		Logger::Instance().WriteLog(msg << "Data Packet with block no:" << m_blockno);


	}

	if (IsError(buf))
	{
		m_opCode = 5;
		sscanf(&buf[3], "%d %s", &m_errorCode, m_errorMsg);
	}
	return m_opCode;

}




bool
TftpPacket::IsRRQ(char* packet)
{
	/*To cheack if the packet is a write request.
	 *param packet-IN
	 *return 1 for success else 0*/
	if (packet[1] == '1')
		return 1;
	else
		return 0;
}


bool
TftpPacket::IsWRQ(char* packet)
{
	/*To cheack if the packet is a read request.
	 *param packet-IN
	 *return 1 for success else 0*/
	if (packet[1] == '2')
		return 1;
	else
		return 0;
}



bool
TftpPacket::IsACK(char* packet)
{
	/*To cheack if the packet is an aknowledgement responce.
	 *param packet-IN
	 *return 1 for success else 0*/
	if (packet[1] == '4')
		return 1;
	else
		return 0;
}



bool
TftpPacket::IsData(char* packet)
{
	/*To cheack if the packet is a DATA.
	 *param packet-IN
	 *return 1 for success else 0*/
	if (packet[1] == '3')
		return 1;
	else
		return 0;
}



bool
TftpPacket::IsError(char* packet)
{
	/*To cheack if the packet is a Error Responce.
	 *param packet-IN
	 *return 1 for success else 0*/

	if (packet[1] == '5')
		return 1;
	else
		return 0;
}
