#include <iostream>
#include <fstream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <ctype.h>

#include <Log.h>
#include <BlasteeLoader.h>
#include <Packet.h>
#include <Socket.h>


// Flag and signal handler which will be used for SIGINT
static volatile sig_atomic_t doneflag = 0;
static void setdoneflag(int signo) {
	doneflag = 1;
}

void logProxy(char *msg)
{
	Log::getInstance().log("%s", msg);
}

int main(int argc, char **argv)
{
	BlasteeLoader config("../../data/blasteeConfig.xml");

	Log::getInstance().init("blastee", config.getLogfilePath());

	Log::getInstance().log("Hello blastee log!");

	Packet::setLogCallback(logProxy);
	Socket::setLogCallback(logProxy);


	/* Set up signal handler for graceful exit on SIGINT */
	struct sigaction act;
	act.sa_handler = setdoneflag;
	act.sa_flags = 0;
	if ((sigemptyset(&act.sa_mask) == -1) || (sigaction(SIGINT, &act, NULL) == -1)) {
		perror("Failed to set SIGINT handler");
		exit(1);
	}

	int port = 45000;



	/* setup socket */
	Socket sock(port);

	bool success = sock.init();

	if (!success)
	{
		Log::getInstance().log("ERROR: could not init blastee socket!");
		exit(1);
	}

	Packet packSyn(PacketType::pSyn);

	Packet packRecv(PacketType::pNack);

	printf("blastee: waiting to get blasted!\n");

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
	}


	return 0;
}

