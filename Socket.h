/*
 * Socket.h
 *
 *  Created on: Jun 9, 2019
 *      Author: jim
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <string>

#include <Packet.h>

#include <arpa/inet.h>

using namespace std;

class Socket {
public:
	Socket(string hostname, unsigned short port);
	Socket(unsigned short port);
	virtual ~Socket();

	bool init();
	bool sendPacketToServer(Packet packet);
	bool sendPacketToClient(Packet packet);
	Packet receivePacket();

	inline string getHostname() {return hostname_;};
	inline unsigned short getPort() {return port_;};

private:
	string hostname_;
	unsigned short port_;
	int fd_;
	bool connected_;
	struct sockaddr *addr;
	socklen_t addrLen;
	struct sockaddr_in recvFrom;
};

#endif /* SOCKET_H_ */