#pragma warning(disable:4996)
#ifndef __IPACKET__H
#define __IPACKET__H

/*An Interface class for framing and deframing of packets.*/
class Ipacket
{
public:

  /*This method Encode the  packet.
	*First parameter is opcode for the packet to be encoded.
	*Second parameter is a INPUT string containing information to be encoded as a packet.
	*third parameter is encoded packet.
	*/
	virtual int EncodePacket(int opcode,const char* Input,char* output)=0; 



	/*DecodePacket decode the  packets.
	 *INPUT parameter Contains The  packet to be decoded.*/
	 virtual  int DecodePacket(char* input)=0;
	
	 //returns opcode of the packet 
	 virtual  int GetopCode()=0;

	 
};

#endif 
