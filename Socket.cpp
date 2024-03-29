/*
 * Socket.cpp
 *
 *  Created on: Jun 9, 2019
 *      Author: jim
 */

#include "Socket.h"

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
//#include <stdbool.h>
//#include <string.h>
//#include <signal.h>
//#include <sys/time.h>
#include <ctype.h>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <cerrno>


#include <iostream>

SocketLogCallback Socket::logCbk_ = nullptr;

Socket::Socket(string hostname, unsigned short port):
	hostname_(hostname),
	port_(port),
	fd_(-1),
	connected_(false),
	addr(nullptr),
	addrLen(0)
{
}

Socket::Socket(unsigned short port):
	hostname_(""),
	port_(port),
	fd_(-1),
	connected_(false),
	addr(nullptr),
	addrLen(0)
{
}

bool Socket::init()
{

	if (connected_)
	{
		return false;
	}

	string sPort = to_string(port_);

	// server
	if (hostname_.empty())
	{
		log("socket init for server");
		struct addrinfo hints, *servinfo, *p;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_flags = AI_PASSIVE;
		/* We use getaddrinfo instead of the deprecated gethostbyname() function
		   so that we can be agnostic about IPv4/v6.  */
		int status;
		if ((status = getaddrinfo(NULL, sPort.c_str(), &hints, &servinfo)) != 0)
		{
			fprintf(stderr, "Error: %s\n", gai_strerror(status));
			return false;
		}
		else
		{
			/*  getaddrinfo returns a linked list of addrinfo structs, each of which corresponds
			 *  to info about one of possibly many network interfaces on this host.
			 *  Thus we'll loop through each one and bind to the first one that works. */
			for (p = servinfo; p != NULL; p = p->ai_next)
			{
				// note that we directly pass info from addrinfo struct to socket()
				if ((fd_ = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
				{
					perror("blastee: socket");
					continue;
				}

				int yes = 1;

				if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
				{
					perror("setsockopt");
				}

				if (bind(fd_, p->ai_addr, p->ai_addrlen) == -1)
				{
					close(fd_);
					perror("blastee: bind");
					continue;
				}

				break; // if we get here, we either have a socket established or we
					   // couldn't find a suitable addrinfo
			}

			if (p == NULL)
			{ // if we didn't find one...
				log("blastee: failed to bind socket");

			}
			else
			{
				addr = p->ai_addr;
				addrLen = p->ai_addrlen;
				pfds_[0].fd = fd_;
				pfds_[0].events = POLLIN;
				connected_ = true;
			}

			freeaddrinfo(servinfo);

		}
	}
	else
	{
		struct addrinfo hints, *servinfo, *p;

		log("socket init for client");

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_flags = AI_PASSIVE;
		/* We use getaddrinfo instead of the deprecated gethostbyname() function
			   so that we can be agnostic about IPv4/v6.  */
		int status;
		if ((status = getaddrinfo(hostname_.c_str(), sPort.c_str(), &hints, &servinfo)) != 0)
		{
			printf("Error: %s\n", gai_strerror(status));
			return false;
		}
		/*  getaddrinfo returns a linked list of addrinfo structs, each of which corresponds
		 *  to info about one of possibly many network interfaces on this host.
		 *  Thus we'll loop through each one and bind to the first one that works. */
		for (p = servinfo; p != NULL; p = p->ai_next)
		{
			// note that we directly pass info from addrinfo struct to socket()
			if ((fd_ = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			{
				perror("blaster: socket");
				continue;
			}


			/* NO BINDING DONE since its not my socket */

			break; // if we get here, we either have a socket established or we
			// couldn't find a suitable addrinfo
		}

		if (p == NULL)
		{
			log("blaster: failed to bind socket");
		}
		else
		{
			addr = p->ai_addr;
			addrLen = p->ai_addrlen;
			pfds_[0].fd = fd_;
			pfds_[0].events = POLLIN;
			connected_ = true;
		}

		freeaddrinfo(servinfo);

	}

	return connected_;
}

bool Socket::sendPacketToServer(Packet packet)
{
	vector<char> data = packet.encodePacket();

	log("Sending packet to server of size: %d", data.size());

	if (sendto(fd_, &data[0], data.size(), 0, addr, addrLen) < 0)
	{
		fprintf(stderr, "sendto server error\n");
		return false;
	}

	return true;
}

bool Socket::sendPacketToClient(Packet packet)
{
	vector<char> data = packet.encodePacket();

	log("Sending packet to client of size: %d", data.size());

	if (sendto(fd_, &data[0], data.size(), 0, (struct sockaddr*) &recvFrom, sizeof(recvFrom)) < 0)
	{
		fprintf(stderr, "sendto client error\n");
		return false;
	}

	return true;
}

Packet Socket::receivePacket()
{
	vector<char> data(1024);
	unsigned int slen = sizeof(recvFrom);

	int len;

	Packet packet(PacketType::pNack);

    if ((len = recvfrom(fd_, &data[0], data.size(), 0, (struct sockaddr*) &recvFrom, &slen)) == -1)
	{
		return packet;
	}

    packet = Packet::decodePacket(vector<char>(data.begin(), data.begin() + len));


    log("Received %d bytes from %s:%d", len, inet_ntoa(recvFrom.sin_addr), ntohs(recvFrom.sin_port));

    return packet;

}

int Socket::hasData()
{
	int pRet = 0;

	pRet = poll(pfds_, 1, 100);

	if (pRet < 0)
	{
		log("poll error: %d", errno);
	}
	else if (pRet > 0)
	{
		if (pfds_[0].revents & POLLIN)
		{
			pRet = 1;
		}
		else
		{
			pRet = 0;
		}
	}

	return pRet;
}

Socket::~Socket()
{
	close(fd_);
}

void Socket::log(const char * format, ... )
{
  char buffer[2048];
  va_list args;

  if (logCbk_ == nullptr)
  {
	  return;
  }

  va_start(args, format);
  vsnprintf(buffer, 2048, format, args);

  logCbk_(buffer);

  va_end (args);
}

