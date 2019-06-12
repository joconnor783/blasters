/*
 * Blasters.cpp
 *
 *  Created on: Jun 11, 2019
 *      Author: jim
 */

#include "Blasters.h"

#include <Log.h>
#include <BlastersLoader.h>
#include <Packet.h>
#include <Socket.h>

#include <csignal>
#include <atomic>

using namespace std;

static std::atomic_bool run_;

Blasters::Blasters():
		init_(false)
{}

Blasters::~Blasters() {
	// TODO Auto-generated destructor stub
}

void Blasters::signalHandler(int signum)
{
	run_ = false;
}

void Blasters::logProxy(char *msg)
{
	Log::getInstance().log("%s", msg);
}

bool Blasters::init()
{
	signal(SIGINT, signalHandler);

	run_ = true;

	BlastersLoader config("../../data/blastersConfig.xml");

	Log::getInstance().init("Blasters", config.getLogfilePath());

	Log::getInstance().log("Hello Blasters log!");

	Packet::setLogCallback(logProxy);
	Socket::setLogCallback(logProxy);


	init_ = true;

	return true;
}

void Blasters::run()
{

	const string hostname = "127.0.0.1";

	int port = 45000;

	/* setup socket */
	Socket sock(hostname, port);

	bool success = sock.init();

	if (!success)
	{
		Log::getInstance().log("ERROR: could not init blasters socket!");
		return;
	}

	Packet packSyn(PacketType::pSyn);

	Packet packRecv(PacketType::pNack);

	Log::getInstance().log("Blasters sending packet");
	success = sock.sendPacketToServer(packSyn);

	if (!success)
	{
		Log::getInstance().log("ERROR: could not send packet to server!");
	}
	else
	{
		Log::getInstance().log("Blasters waiting for packet...");
		packRecv = sock.receivePacket();

		if (packRecv.getType() == PacketType::pNack)
		{
			Log::getInstance().log("ERROR: could not receive packet from server!");
		}
	}
}
