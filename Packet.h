/*
 * Packet.h
 *
 *  Created on: Jun 9, 2019
 *      Author: jim
 */

#ifndef PACKET_H_
#define PACKET_H_

#include <vector>

using namespace std;



enum PacketType
{
	pSyn   = 'S',  // client requests session
	pAck   = 'A',  // server replies with port for session
	pNack  = 'N',  // packet type or format does not match and default packet type
	pBusy  = 'B',  // server replies with no socket available
	pData  = 'D',  // send data to the server
	pClose = 'C',  // close session with the server
	pEcho  = 'E'   // server echos the data packet
};

class Packet {
public:
	Packet();
	Packet(PacketType type);
	Packet(PacketType type, unsigned int session, unsigned int seq);
	Packet(PacketType type, unsigned int session, unsigned int seq, vector<char> payload);
	virtual ~Packet();

	vector<char> encodePacket();

	static Packet decodePacket(vector<char> data);

	inline PacketType getType() { return type_;};
	inline unsigned int getSession() {return session_;};
	inline unsigned int getSequence() {return seq_;};
	inline vector<char> getPayload() {return payload_;};

private:
	PacketType type_;
	unsigned int session_;
	unsigned int seq_;
	vector<char> payload_;
};

#endif /* PACKET_H_ */
