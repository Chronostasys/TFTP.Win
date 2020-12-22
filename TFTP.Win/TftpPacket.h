
#ifndef __TFTPPACKET__H
#define __TFTPPACKET__H

#include "Ipacket.h"

/*including header files*/
#include <stdlib.h>
#include <stdio.h>
#include<iostream>
#include<string.h>
#include<string>

/*default length of buffer(string)*/
#define DEFAULT_BUFLEN 512


/*Macros for TFTP opcodes ,error,maximum size of packet and data. */
enum {PACKET_READ=1,PACKET_WRITE,PACKET_DATA,PACKET_ACK,PACKET_ERROR};

//Opcode for read request 
#define TFTP_OPCODE_READ     "01"
//Opcode for write request
#define TFTP_OPCODE_WRITE    "02"
//Opcode for data packet
#define TFTP_OPCODE_DATA     "03"
//Opcode for ACK packet
#define TFTP_OPCODE_ACK      "04"
//Opcode for Error packet
#define TFTP_OPCODE_ERROR    "05"

// mode of packets (can be ascii,octet etc)
#define TFTP_DEFAULT_TRANSFER_MODE "octet"

//maximum size of packets
#define TFTP_PACKET_MAX_SIZE 1024
//Maximum size od data in the packet
#define TFTP_PACKET_DATA_SIZE 512
//Maximum Size of file name
#define FILE_NAME_MAX_SIZE 1010

#define ERROR_WRONG_PACKET 1
#define ERROR_WRONG_REQUEST 2
using namespace std;

/*A class for framing and deframing of TFTP packets wich uses the interface class "Ipacket".
All packets framed will follow the TFTP prtotocol.*/

class TftpPacket:public Ipacket
{

public:

  /*This method Encode the  packet.
	*First parameter is opcode for the packet to be encoded.
	*Second parameter is a INPUT string in following formate.
	*1.For Write request- "filename mode"
	*2.For Read request - "filename mode"
	*3.For ACK          -  NULL 
	*4.For DATA         - "block_no buffer_containing_512bytes_of_data"
	*5.For ERROR        - "Error_Code Error_msg"
	*Second parameter is output string which will contain the TFTP packet formate of INPUT String.
	*/
	int EncodePacket(int opcode, const char* Input,char* output);

	/*Decodes the packet received and stores the value in 
	respective data members of TftpPacket class.*/
	int DecodePacket(char* input);
	
	/*returns the opcode of most recently received packet*/
	int GetopCode();

	TftpPacket(){m_blockno=0;}

protected:

	/*For creating request to write into a file .

	  2 bytes     string    1 byte     string   1 byte
      ------------------------------------------------
	  | Opcode |  Filename  |   0  |    Mode    |   0  |
      ------------------------------------------------	*/
	int CreateWrq();

	/*For creating  request to read a file .
	  2 bytes     string    1 byte     string   1 byte
      ------------------------------------------------
	  | Opcode |  Filename  |   0  |    Mode    |   0  |
      ------------------------------------------------	*/

	int CreateRrq();

	/*For creating a data packet that will contain 512 bytes of data.

	  2 bytes     2 bytes      n bytes
      ----------------------------------
      | Opcode |   Block #  |   Data     
	  ----------------------------------	*/
	int CreateData();
	
	/*For creating the aknowledgement responce packet

	 2 bytes     2 bytes
     ---------------------
     | Opcode |   Block #  |
     ---------------------*/
	int CreateAck();

	/*For creating a error packet.
	
	2 bytes     2 bytes      string    1 byte
    -----------------------------------------
    | Opcode |  ErrorCode |   ErrMsg   |   0  |
    -----------------------------------------		*/
	int CreateError();


	/*To cheack if the packet is a write request.*/
	bool IsRRQ(char* packet);

	/*To cheack if the packet is a read request.*/
	bool IsWRQ(char* packet);

	/*To cheack if the packet is an aknowledgement responce.*/
	bool IsACK(char* packet);

	/*To cheack if the packet is a DATA.*/
	bool IsData(char* packet);

	/*To cheack if the packet is a Error Responce.*/
	bool IsError(char* packet);

private:
	int	    m_opCode;								//opcode for the request.
	char	m_mode[10];								//mode of  the packet(ascii,octet etc).
	char	m_filename[FILE_NAME_MAX_SIZE];							//name of the file to be written of read
	int		m_blockno;								//block number of the packet
	int		m_errorCode;								//Error codes in the error packets.
	char	m_errorMsg[100];							//Error message in the error responce.
	char	m_data[TFTP_PACKET_DATA_SIZE];			//Data in the data packet.
	char	m_packet[TFTP_PACKET_MAX_SIZE];			//contains the final packet.
};
#endif
