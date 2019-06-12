/*
 * Blastee.h
 *
 *  Created on: Jun 11, 2019
 *      Author: jim
 */

#ifndef BLASTEE_H_
#define BLASTEE_H_

class Blastee {

public:
	Blastee();
	virtual ~Blastee();
	bool init();
	void run();

private:
	bool init_;


	static void signalHandler(int signum);
	static void logProxy(char *msg);
};

#endif /* BLASTEE_H_ */
