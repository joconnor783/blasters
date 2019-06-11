#include <iostream>
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
#include <sys/wait.h>
#include <time.h>

#include <Log.h>
#include <BlastersLoader.h>
#include <Packet.h>
#include <Socket.h>


class ClientLog : public Log {};




// Flag and signal handler which will be used for SIGINT
static volatile sig_atomic_t doneflag = 0;
static void setdoneflag(int signo) {
	doneflag = 1;
}

void logProxy(char *msg)
{
	ClientLog::getInstance().log("%s", msg);
}

int main(int argc, char **argv)
{

	BlastersLoader config("../../data/blastersConfig.xml");

	ClientLog::getInstance().init("blasters", config.getLogfilePath());

	ClientLog::getInstance().log("Hello blasters log!");

	Packet::setLogCallback(LogCallback(logProxy));


	/* nanosleep */
	struct timespec t, tremain; // won't ever look at tremain

	/* getaddressinfo */

	const string hostname = "127.0.0.1";

	int port = 45000;


	/* Set up signal handler for graceful exit on SIGINT */
	struct sigaction act;
	act.sa_handler = setdoneflag;
	act.sa_flags = 0;
	if ((sigemptyset(&act.sa_mask) == -1) || (sigaction(SIGINT, &act, NULL) == -1))
	{
		perror("Failed to set SIGINT handler");
		exit(1);
	}

	/* setup socket */
	Socket sock(hostname, port);

	bool success = sock.init();

	if (!success)
	{
		ClientLog::getInstance().log("ERROR: could not init blasters socket!");
		exit(1);
	}

	Packet packSyn(PacketType::pSyn);

	Packet packRecv(PacketType::pNack);

	ClientLog::getInstance().log("Blasters sending packet");
	success = sock.sendPacketToServer(packSyn);

	if (!success)
	{
		ClientLog::getInstance().log("ERROR: could not send packet to server!");
	}
	else
	{
		ClientLog::getInstance().log("Blasters waiting for packet...");
		packRecv = sock.receivePacket();

		if (packRecv.getType() == PacketType::pNack)
		{
			ClientLog::getInstance().log("ERROR: could not receive packet from server!");
		}
	}


	exit(0);

} // end main
