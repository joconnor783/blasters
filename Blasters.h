/*
 * Blasters.h
 *
 *  Created on: Jun 11, 2019
 *      Author: jim
 */

#ifndef BLASTERS_H_
#define BLASTERS_H_

class Blasters {

public:
	Blasters();
	virtual ~Blasters();
	bool init();
	void run();

private:
	bool init_;


	static void signalHandler(int signum);
	static void logProxy(char *msg);
};

#endif /* BLASTERS_H_ */
