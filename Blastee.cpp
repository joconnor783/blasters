/*
 * Blastee.cpp
 *
 *  Created on: Jun 11, 2019
 *      Author: jim
 */

#include "Blastee.h"

#include <Log.h>
#include <BlasteeLoader.h>
#include <Packet.h>
#include <Socket.h>
#include <poll.h>

#include <cerrno>
#include <csignal>
#include <atomic>

using namespace std;

static std::atomic_bool run_;

Blastee::Blastee():
		init_(false)
{}

Blastee::~Blastee() {
	// TODO Auto-generated destructor stub
}

void Blastee::signalHandler(int signum)
{
	run_ = false;
}

void Blastee::logProxy(char *msg)
{
	Log::getInstance().log("%s", msg);
}

bool Blastee::init()
{
	signal(SIGINT, signalHandler);

	run_ = true;

	BlasteeLoader config("../../data/blasteeConfig.xml");

	Log::getInstance().init("blastee", config.getLogfilePath());

	Log::getInstance().log("Hello blastee log!");

	Packet::setLogCallback(logProxy);
	Socket::setLogCallback(logProxy);


	init_ = true;

	return true;
}

void Blastee::run()
{

	int port = 45000;

	/* setup socket */
	Socket sock(port);

	bool success = sock.init();

	if (!success)
	{
		Log::getInstance().log("ERROR: could not init blastee socket!");
		return;
	}

	Packet packSyn(PacketType::pSyn);

	Packet packRecv(PacketType::pNack);

	Log::getInstance().log("blastee: waiting to get blasted!");

	struct pollfd pfds[1];

	pfds[0].fd = sock.getFileDescriptor();
	pfds[0].events = POLLIN;

	while (run_)
	{
		int pRet;
		pRet = poll(pfds, 1, 100);

		if (pRet < 0)
		{
			Log::getInstance().log("poll error: %d", errno);
		}
		else if (pRet > 0)
		{
			if (pfds[0].revents & POLLIN)
			{
				packRecv = sock.receivePacket();

				if (packRecv.getType() == PacketType::pNack)
				{
					Log::getInstance().log("ERROR: could not receive packet from client!");
				}
				else
				{
					success = sock.sendPacketToClient(packRecv);

					if (!success)
					{
						Log::getInstance().log("ERROR: could not send packet to client!");
					}
					else
					{
						return;
					}
				}
			}
		}
	}

}
