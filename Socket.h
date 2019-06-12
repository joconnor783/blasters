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
#include <poll.h>

using namespace std;

using SocketLogCallback = std::function<void(char *)>;

class Socket {
public:
	Socket(string hostname, unsigned short port);
	Socket(unsigned short port);
	virtual ~Socket();

	bool init();
	bool sendPacketToServer(Packet packet);
	bool sendPacketToClient(Packet packet);
	Packet receivePacket();
	int hasData();

	inline string getHostname() {return hostname_;};
	inline unsigned short getPort() {return port_;};

	static inline void setLogCallback(SocketLogCallback callback) {logCbk_ = callback;};

	inline int getFileDescriptor() {return fd_;};

private:
	string hostname_;
	unsigned short port_;
	int fd_;
	bool connected_;
	struct sockaddr *addr;
	socklen_t addrLen;
	struct sockaddr_in recvFrom;
	struct pollfd pfds_[1];

	static SocketLogCallback logCbk_;
	static void log( const char * format, ... );
};

#endif /* SOCKET_H_ */
