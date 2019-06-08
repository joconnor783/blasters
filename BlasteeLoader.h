/*
 * BlasteeLoader.h
 *
 *  Created on: Jun 8, 2019
 *      Author: jim
 */

#ifndef BLASTEELOADER_H_
#define BLASTEELOADER_H_

#include "Loader.h"

class BlasteeLoader: public Loader {
public:
	BlasteeLoader(std::string configPath);
	virtual ~BlasteeLoader();

	std::string getLogfilePath();

private:
	std::string logfilePath_;
};

#endif /* BLASTEELOADER_H_ */
