/*
 * Packet.cpp
 *
 *  Created on: Jun 9, 2019
 *      Author: jim
 */

#include "Packet.h"

#include <arpa/inet.h>
#include <stdarg.h>
#include <stdio.h>

#include <algorithm>

#include <iostream>


const int kLengthHeader = 4; // First 4 bytes is the length of the packet



const vector<char> types {
	PacketType::pSyn,
	PacketType::pAck,
	PacketType::pNack,
	PacketType::pBusy,
	PacketType::pData,
	PacketType::pClose,
	PacketType::pEcho
};

Packet::Packet():
	type_(PacketType::pNack),
	session_(0),
	seq_(0),
	payload_()
{
}

Packet::Packet(PacketType type):
	type_(type),
	session_(0),
	seq_(0),
	payload_()
{
}

Packet::Packet(PacketType type, unsigned int session, unsigned int seq):
	type_(type),
	session_(session),
	seq_(seq),
	payload_()
{
}

Packet::Packet(PacketType type, unsigned int session, unsigned int seq, vector<char> payload):
	type_(type),
	session_(session),
	seq_(seq),
	payload_(payload)
{
}


Packet::~Packet() {
	// TODO Auto-generated destructor stub
}

vector<char> Packet::encodePacket()
{
	unsigned int length = kLengthHeader + sizeof(type_) + sizeof(session_) + sizeof(seq_) + payload_.size();
	vector<char> data(length);
	int offset;

	//data.reserve(length);

	*(reinterpret_cast<unsigned int *>(&data[0])) = htonl(length);
	data[kLengthHeader] = type_;
	offset = kLengthHeader + sizeof(type_);
	*(reinterpret_cast<unsigned int *>(&data[offset])) = htonl(session_);
	offset += sizeof(session_);
	*(reinterpret_cast<unsigned int *>(&data[offset])) = htonl(seq_);
	offset += sizeof(session_);
	data.insert(data.begin() + offset, payload_.begin(), payload_.end());

	log("encoded packet of size: %d", data.size());

	return data;
}

Packet Packet::decodePacket(vector<char> data)
{
	unsigned int minLength = kLengthHeader + sizeof(type_) + sizeof(session_) + sizeof(seq_);

	if (data.size() < minLength)
	{
		return Packet(PacketType::pNack);
	}

	unsigned int length = ntohl(*(reinterpret_cast<unsigned int *>(&data[0])));
	PacketType type;
	unsigned int session;
	unsigned int seq;
	unsigned int offset;

	log("decoded packet of size: %d", length);

	if (length < minLength)
	{
		return Packet(PacketType::pNack);
	}

	type = static_cast<PacketType>(data[kLengthHeader]);
	auto it = types.end();

	it = find(types.begin(), types.end(), type);

	if (it == types.end())
	{
		return Packet(PacketType::pNack);
	}

	offset = kLengthHeader + sizeof(type_);
	session = ntohl(*(reinterpret_cast<unsigned int *>(&data[offset])));
	offset += sizeof(session_);
	seq = ntohl(*(reinterpret_cast<unsigned int *>(&data[offset])));
	offset += sizeof(session_);

	vector<char> payload;

	if (length > minLength)
	{
		payload.insert(payload.begin(), data.begin() + offset, data.end());
	}

	Packet packet(type, session, seq, payload);

	return packet;
}

void Packet::log(const char * format, ... )
{
  char buffer[2048];
  va_list args;

  if (logCbk_ == nullptr)
  {
	  cout << "missing log callback\n";
	  return;
  }

  va_start(args, format);
  vsnprintf(buffer, 2048, format, args);

  cout << "packet calling callback\n";
  logCbk_(buffer);

  va_end (args);
}

